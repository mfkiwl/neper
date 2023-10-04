/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_ori_.h"

void
net_tess_opt_init_sset_ori (int TessId, struct SEEDSET *SSet, double *rad, struct TOPT *pTOpt)
{
  int i;

  neut_seedset_memcpy (SSet[TessId], &(*pTOpt).SSet);

  (*pTOpt).SSet.SeedOriR = ut_alloc_2d ((*pTOpt).SSet.N + 1, 3);
  for (i = 1; i <= (*pTOpt).SSet.N; i++)
    ol_q_R ((*pTOpt).SSet.SeedOri[i], (*pTOpt).SSet.SeedOriR[i]);

  if (ut_list_testelt ((*pTOpt).dof, NEUT_SEP_NODEP, "rt"))
    (*pTOpt).SSet.SeedOriTheta = ut_alloc_1d ((*pTOpt).SSet.N + 1);

  ut_array_1d_memcpy (rad + 1, (*pTOpt).SSet.N, (*pTOpt).SSet.SeedWeight + 1);

  return;
}
