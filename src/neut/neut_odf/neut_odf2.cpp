/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_odf_.h"
#include "neut/neut_oset/neut_oset.hpp"
#include "neut_qcloud_struct.hpp"

extern void neut_odf_comp_elts_all (struct OL_SET *pOSet, double *q, struct ODF *pOdf, int id);
extern void neut_odf_comp_elts_neigh (double *q, double cut_fact, QCLOUD qcloud, my_kd_tree_t *nano_index, struct ODF *pOdf, int id);
extern void neut_odf_comp_nodes_all (struct OL_SET *pOSet, double *q, struct ODF *pOdf, int id);
extern void neut_odf_comp_nodes_neigh (double *q, double cut_fact, QCLOUD qcloud, my_kd_tree_t *nano_index,
                           struct ODF *pOdf, int id);

double
neut_odf_comp_elts (char *neigh, struct OL_SET *pOSet, QCLOUD nano_cloud,
                    my_kd_tree_t *nano_index, struct ODF *pOdf, int verbosity)
{
  int i, dim = neut_mesh_array_dim ((*pOdf).Mesh);
  double avradeq;
  char *prev = ut_alloc_1d_char (100);

  neut_ori_n_avradeq (NULL, (*pOSet).size, (*pOSet).crysym, &avradeq);

  if (!(*pOdf).EltWeight)
    (*pOdf).EltWeight = ut_alloc_1d ((*pOdf).odfqty);

  ut_array_1d_zero ((*pOdf).odf, (*pOdf).odfqty);

#pragma omp parallel for schedule(dynamic)
  for (i = 0; i < (*pOdf).Mesh[dim].EltQty; i++)
  {
    double *q = ol_q_alloc ();
    double *coo = ut_alloc_1d (3);

#pragma omp critical
    if (verbosity > 0)
      ut_print_progress (stdout, i, (*pOdf).Mesh[dim].EltQty - 1, "%3.0f%%", prev);

    if (dim == 3)
      neut_mesh_elt_volume ((*pOdf).Nodes, (*pOdf).Mesh[dim], i + 1, (*pOdf).EltWeight + i);
    else
      neut_mesh_elt_area ((*pOdf).Nodes, (*pOdf).Mesh[dim], i + 1, (*pOdf).EltWeight + i);
    neut_mesh_elt_centre ((*pOdf).Nodes, (*pOdf).Mesh[dim], i + 1, coo);

    if (!strncmp ((*pOdf).gridtype, "rodrigues", 9))
    {
      (*pOdf).EltWeight[i] *= pow (M_PI * (1 + ut_vector_scalprod (coo, coo)), -2);
      ol_R_q (coo, q);
    }
    else if (!strncmp ((*pOdf).gridtype, "euler-bunge", 11))
    {
      if (!strcmp ((*pOdf).gridunit, "degree"))
      {
        (*pOdf).EltWeight[i] *= sin (coo[1] * M_PI / 180);
        ol_e_q (coo, q);
      }
      else
      {
        (*pOdf).EltWeight[i] *= sin (coo[1]);
        ol_e_q_rad (coo, q);
      }
    }
    else if (!strcmp ((*pOdf).gridtype, "homochoric"))
      ol_homochoric_q (&((*pOdf).hfct), coo, q);
    else
      ut_print_message (2, 2,
                        "Unkown grid type `%s' (option -odfgridtype).\n",
                        (*pOdf).gridtype);

    if (!strcmp (neigh, "all"))
      neut_odf_comp_elts_all (pOSet, q, pOdf, i);
    else
    {
      double cut_fact = atof (neigh);
      neut_odf_comp_elts_neigh (q, cut_fact, nano_cloud, nano_index, pOdf, i);
    }

    ol_q_free (q);
    ut_free_1d (&coo);
  }

  ut_array_1d_normalize ((*pOdf).EltWeight, (*pOdf).odfqty);

  (*pOdf).odfmean = ut_array_1d_wmean ((*pOdf).odf, (*pOdf).EltWeight, (*pOdf).odfqty);
  ut_array_1d_scale ((*pOdf).odf, (*pOdf).odfqty, 1. / (*pOdf).odfmean);
  (*pOdf).odfmean = 1;
  (*pOdf).odfsig = ut_array_1d_wstddev ((*pOdf).odf, (*pOdf).EltWeight, 1, (*pOdf).odfqty);
  (*pOdf).odfmin = ut_array_1d_min ((*pOdf).odf, (*pOdf).odfqty);
  (*pOdf).odfmax = ut_array_1d_max ((*pOdf).odf, (*pOdf).odfqty);

  ut_free_1d_char (&prev);

  return 1;
}

void
neut_odf_comp_nodes (char *neigh, struct OL_SET *pOSet, QCLOUD nano_cloud,
                     my_kd_tree_t *nano_index, struct ODF *pOdf, int verbosity)
{
  int i;
  double mean;
  char *prev = ut_alloc_1d_char (1000);

  ut_array_1d_zero ((*pOdf).odfn, (*pOdf).odfnqty);

  mean = 0;
#pragma omp parallel for schedule(dynamic)
  for (i = 0; i < (*pOdf).odfnqty; i++)
  {
    double *q = ol_q_alloc ();
    double *qs = ol_q_alloc ();

#pragma omp critical
    if (verbosity > 0)
      ut_print_progress (stdout, i, (*pOdf).odfnqty - 1, "%3.0f%%", prev);

    if (!strncmp ((*pOdf).gridtype, "rodrigues", 9))
      ol_R_q ((*pOdf).Nodes.NodeCoo[i + 1], q);
    else if (!strncmp ((*pOdf).gridtype, "euler-bunge", 11))
    {
      if (!strcmp ((*pOdf).gridunit, "degree"))
        ol_e_q ((*pOdf).Nodes.NodeCoo[i + 1], q);
      else
        ol_e_q_rad ((*pOdf).Nodes.NodeCoo[i + 1], q);
    }
    else if (strcmp ((*pOdf).gridtype, "homochoric"))
      ol_homochoric_q (&((*pOdf).hfct), (*pOdf).Nodes.NodeCoo[i + 1], q);
    else
      ut_print_message (2, 2,
                        "Unkown grid type `%s' (option -odfgridtype).\n",
                        (*pOdf).gridtype);

    if (!strcmp (neigh, "all"))
      neut_odf_comp_nodes_all (pOSet, q, pOdf, i);
    else
    {
      double cut_fact;
      sscanf (neigh, "%lf", &cut_fact);
      neut_odf_comp_nodes_neigh (q, cut_fact, nano_cloud, nano_index, pOdf, i);
    }

#pragma omp atomic
    mean += (*pOdf).odfn[i];

    ol_q_free (q);
    ol_q_free (qs);
  }

  mean /= (*pOdf).odfnqty;
#pragma omp parallel for
  for (i = 0; i < (*pOdf).odfnqty; i++)
    (*pOdf).odfn[i] /= mean;

  ut_free_1d_char (&prev);

  return;
}
