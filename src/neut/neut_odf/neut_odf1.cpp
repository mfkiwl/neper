/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_odf_.h"
#include "neut/neut_oset/neut_oset.hpp"

extern double neut_odf_comp_elts (char *neigh, struct OL_SET *pOSet, QCLOUD nanocloud, my_kd_tree_t *nano_index, struct ODF *pOdf, int verbosity);
extern double neut_odf_comp_nodes (char *neigh, struct OL_SET *pOSet, QCLOUD nano_cloud, my_kd_tree_t *nano_index, struct ODF *pOdf, int verbosity);

void
neut_odf_set_zero (struct ODF *pOdf)
{
  int i;

  (*pOdf).gridtype = NULL;
  (*pOdf).gridunit = NULL;

  neut_nodes_set_zero (&((*pOdf).Nodes));
  (*pOdf).Mesh = (struct MESH*) calloc (4, sizeof (struct MESH));
  for (i = 0; i < 4; i++)
    neut_mesh_set_zero ((*pOdf).Mesh + i);

  (*pOdf).odfqty = 0;
  (*pOdf).odf = NULL;
  (*pOdf).odfnqty = 0;
  (*pOdf).odfn = NULL;

  (*pOdf).EltWeight = NULL;

  (*pOdf).odfmin = 0;
  (*pOdf).odfmax = 0;
  (*pOdf).odfmean = 0;
  (*pOdf).odfsig = 0;

  (*pOdf).sigma = 0;

  ut_fct_set_zero (&((*pOdf).hfct));
  ol_homochoric_thetafct (&((*pOdf).hfct));

  return;
}

void
neut_odf_space_fnscanf (char *filename, struct ODF *pOdf, char *mode)
{
  int i, dim;

  neut_odf_set_zero (pOdf);

  neut_mesh_fnscanf_msh (filename, &((*pOdf).Nodes),
                         (*pOdf).Mesh, (*pOdf).Mesh + 1,
                         (*pOdf).Mesh + 2, (*pOdf).Mesh + 3, NULL, NULL, mode);

  dim = neut_mesh_array_dim ((*pOdf).Mesh);

  if (!(*pOdf).Mesh[dim].Domain)
    ut_print_message (2, 2, "Mesh domain not defined\n");
  ut_string_string ((*pOdf).Mesh[dim].Domain, &((*pOdf).gridtype));

  if (!strncmp ((*pOdf).Mesh[dim].Domain, "euler-bunge", 11))
  {
    ut_string_string ("radian", &(*pOdf).gridunit);
    if (!strncmp ((*pOdf).gridtype, "euler-bunge", 11))
    {
      double **bbox = ut_alloc_2d (3, 2);
      neut_nodes_bbox ((*pOdf).Nodes, bbox);
      for (i = 0; i < 3; i++)
        if (bbox[i][1] > 10) // meaning we are in degrees and not radians
        {
          ut_string_string ("degree", &(*pOdf).gridunit);
          break;
        }
      ut_free_2d (&bbox, 3);
    }
  }

  (*pOdf).odfqty = (*pOdf).Mesh[dim].EltQty;
  (*pOdf).odf = ut_alloc_1d ((*pOdf).odfqty);
  (*pOdf).odfnqty = (*pOdf).Nodes.NodeQty;
  (*pOdf).odfn = ut_alloc_1d ((*pOdf).odfnqty);

  return;
}

void
neut_odf_setsigma (struct ODF *pOdf, char *expr, int qty, char *crysym)
{
  int status, varqty = 1;
  double *vals = ut_alloc_1d (varqty);
  char **vars = ut_alloc_1d_pchar (varqty);

  ut_string_string ("avthetaeq", vars);
  neut_ori_n_avthetaeq (NULL, qty, crysym, vals);
  ol_theta_rad2deg (vals[0], vals);

  status = ut_math_eval (expr, varqty, vars, vals, &((*pOdf).sigma));
  if (status == -1)
    ut_print_message (2, 2, "Failed to parse expression `%s'.\n", expr);

  ol_theta_deg2rad ((*pOdf).sigma, &((*pOdf).sigma));

  ut_free_2d_char (&vars, varqty);
  ut_free_1d (&vals);

  return;
}

void
neut_odf_comp (char *mode, char *neigh, struct OL_SET *pOSet, struct ODF *pOdf, int verbosity)
{
  my_kd_tree_t *nano_index = nullptr;
  nanoflann::SearchParams params;
  QCLOUD nano_cloud;

  neut_oset_kdtree (pOSet, &nano_cloud, &nano_index);

  if (strstr (mode, "m") || strstr (mode, "n"))
    neut_odf_comp_elts (neigh, pOSet, nano_cloud, nano_index, pOdf, verbosity);

  if (strstr (mode, "n"))
    neut_odf_comp_nodes (neigh, pOSet, nano_cloud, nano_index, pOdf, verbosity);

  delete nano_index;

  return;
}

void
neut_odf_orides (struct ODF Odf, char **porides)
{
  ut_string_function (Odf.gridtype, porides, NULL, NULL, NULL);

  return;
}

void
neut_odf_fnscanf (char *filename, struct ODF *pOdf, char *mode)
{
  int i, qty, tmp;
  char *fct = NULL, **vars = NULL, **vals = NULL;

  ut_string_function (filename, &fct, &vars, &vals, &qty);

  neut_odf_set_zero (pOdf);

  for (i = 0; i < qty; i++)
  {
    if (!strcmp (vars[i], "mesh"))
      neut_odf_space_fnscanf (vals[i], pOdf, mode);
    else if (!strcmp (vars[i], "val"))
    {
      (*pOdf).odfqty = (*pOdf).Mesh[3].EltQty;

      tmp = ut_file_nbwords (vals[i]);
      if (tmp != (*pOdf).odfqty)
        ut_print_message (2, 0, "Number of data and elements do not match (%d != %d, file = %s).\n", tmp, (*pOdf).Mesh[3].EltQty, vals[i]);

      (*pOdf).odf = ut_alloc_1d ((*pOdf).odfqty);
      ut_array_1d_fnscanf (vals[i], (*pOdf).odf, (*pOdf).odfqty, mode);
    }
    else if (!strcmp (vars[i], "valn"))
    {
      (*pOdf).odfnqty = (*pOdf).Nodes.NodeQty;

      tmp = ut_file_nbwords (vals[i]);
      if (tmp != (*pOdf).odfnqty)
        ut_print_message (2, 0, "Number of data and elements do not match (%d != %d, file = %s).\n", tmp, (*pOdf).Mesh[3].EltQty, vals[i]);

      (*pOdf).odfn = ut_alloc_1d ((*pOdf).odfnqty);
      ut_array_1d_fnscanf (vals[i], (*pOdf).odfn, (*pOdf).odfnqty, mode);
    }
    else if (!strcmp (vars[i], "theta") || !strcmp (vars[i], "sigma"))
    {
      sscanf (vals[i], "%lf", &(*pOdf).sigma);
      (*pOdf).sigma *= M_PI / 180;
    }
    else
      ut_print_message (2, 0, "Failed to process `%s'.\n", vars[i]);
  }

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vars, qty);
  ut_free_2d_char (&vals, qty);

  return;
}

void
neut_odf_mesh_olset (struct ODF Odf, struct OL_SET *pOSet)
{
  int i, varqty;
  double *coo = ut_alloc_1d (3);
  char *fct = NULL, **vars = NULL, **vals = NULL;

  ut_string_function (Odf.gridtype, &fct, &vars, &vals, &varqty);

  (*pOSet) = ol_set_alloc (Odf.Mesh[3].EltQty, vals[0]);
  ut_array_1d_zero ((*pOSet).weight, (*pOSet).size);

  for (i = 0; i < (int) (*pOSet).size; i++)
  {
    neut_mesh_elt_centre (Odf.Nodes, Odf.Mesh[3], i + 1, coo);
    ol_R_q (coo, (*pOSet).q[i]);
  }

  ut_free_1d (&coo);
  ut_free_1d_char (&fct);
  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);

  return;
}
