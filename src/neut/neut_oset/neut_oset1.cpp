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

// Could be much more efficient by 1/ making sure that the orientations are in
// the FR and 2/ looking for the closest point using a kD tree approach.
void
neut_oset_clustering (struct OL_SET OSet, struct ODF Odf, struct OL_SET *pOSet)
{
  int i, j, id;
  struct OL_SET OSet2;
  double mintheta, theta;

  neut_odf_mesh_olset (Odf, &OSet2);

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
