/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_oset_.hpp"

void
neut_oset_kdtree (struct OL_SET *pOSet, struct QCLOUD *pqcloud,
                  my_kd_tree_t ** pqindex)
{
  neut_oset_kdtree_cloud (pOSet, pqcloud);
  neut_oset_kdtree_build (pqcloud, pqindex);

  return;
}

void
neut_oset_clustering (struct OL_SET OSet, struct ODF Odf, char *method, struct OL_SET *pOSet)
{
  int i, id;
  struct OL_SET OSet2;

  neut_odf_mesh_olset (Odf, &OSet2);

  if (!method || !strcmp (method, "kdtree"))
  {
    my_kd_tree_t *nano_index = nullptr;
    nanoflann::SearchParams params;
    QCLOUD nano_cloud;

    neut_oset_kdtree (&OSet2, &nano_cloud, &nano_index);

    size_t num_results = 1;
    std::vector<long unsigned int> ret_index(num_results);
    std::vector<double>    out_dist_sqr(num_results);

    for (i = 0; i < (int) OSet.size; i++)
    {
      nano_index->knnSearch (OSet.q[i], num_results, &ret_index[0], &out_dist_sqr[0]);

      id = ret_index[0] % Odf.Mesh[3].EltQty;

      OSet2.weight[id] += OSet.weight ? OSet.weight[i] : 1;
    }

    delete nano_index;
  }

  else if (!strcmp (method, "direct"))
  {
    int j;
    double mintheta, theta;

    for (i = 0; i < (int) OSet.size; i++)
    {
      id = 0;
      mintheta = DBL_MAX;
      for (j = 0; j < (int) OSet2.size; j++)
      {
        ol_q_q_disori (OSet.q[i], OSet2.q[j], OSet.crysym, &theta);
        if (theta < mintheta)
        {
          mintheta = theta;
          id = j;
        }
      }

      OSet2.weight[id] += OSet.weight ? OSet.weight[i] : 1;
    }
  }

  ol_set_free (pOSet);
  ut_string_string (OSet2.crysym, &(*pOSet).crysym);
  for (i = 0; i < (int) OSet2.size; i++)
    if (OSet2.weight[i] > 0)
    {
      (*pOSet).size++;
      (*pOSet).q = ut_realloc_2d_addline ((*pOSet).q, (*pOSet).size, 4);
      ol_q_memcpy (OSet2.q[i], (*pOSet).q[(*pOSet).size - 1]);
      (*pOSet).weight = ut_realloc_1d ((*pOSet).weight, (*pOSet).size);
      (*pOSet).weight[(*pOSet).size - 1] = OSet2.weight[i];
    }

  if ((*pOSet).size == 0)
    abort ();

  ol_set_free (&OSet2);

  return;
}
