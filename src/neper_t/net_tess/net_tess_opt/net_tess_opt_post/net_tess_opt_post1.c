/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_post_.h"

void
net_tess_opt_post (struct MTESS *pMTess, struct TESS *Tess, int dtess,
                   int dcell, int tessid, struct TOPT TOpt,
                   struct SEEDSET *SSet)
{
  if (!strcmp (TOpt.optitype, "morpho"))
    net_tess_opt_post_tess (pMTess, Tess, dtess, dcell, tessid, TOpt, SSet);

  else if (!strcmp (TOpt.optitype, "ori"))
  {
    neut_seedset_memcpy (TOpt.SSet, SSet + tessid);

    Tess[tessid].CellQty = SSet[tessid].N;
    Tess[tessid].CellOri = ut_alloc_2d (Tess[tessid].CellQty + 1, 4);
    ut_array_2d_memcpy (SSet[tessid].SeedOri + 1, Tess[tessid].CellQty, 4,
                        Tess[tessid].CellOri + 1);

    if (ut_list_testelt (TOpt.dof, NEUT_SEP_NODEP, "rw"))
    {
      Tess[tessid].CellWeight = ut_alloc_1d (Tess[tessid].CellQty + 1);
      ut_array_1d_memcpy (SSet[tessid].SeedWeight + 1, Tess[tessid].CellQty,
                          Tess[tessid].CellWeight + 1);
      ut_array_1d_scale (Tess[tessid].CellWeight + 1, Tess[tessid].CellQty,
                         1. / ut_array_1d_mean (Tess[tessid].CellWeight + 1, Tess[tessid].CellQty));
    }

    if (ut_list_testelt (TOpt.dof, NEUT_SEP_NODEP, "rt"))
    {
      Tess[tessid].CellTheta = ut_alloc_1d (Tess[tessid].CellQty + 1);
      ut_array_1d_memcpy (SSet[tessid].SeedOriTheta + 1, Tess[tessid].CellQty,
                          Tess[tessid].CellTheta + 1);
      ut_array_1d_scale (Tess[tessid].CellTheta + 1, Tess[tessid].CellQty, 180. / M_PI);
    }
  }

  return;
}
