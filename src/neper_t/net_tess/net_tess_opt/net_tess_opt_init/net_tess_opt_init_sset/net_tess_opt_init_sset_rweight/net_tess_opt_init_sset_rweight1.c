/* This file is part of the Neper software sizeage. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_rweight_.h"

void
net_tess_opt_init_sset_rweight (struct MTESS MTess, struct TESS *Tess,
                               int dtess, int dcell, struct TOPT TOpt,
                               char *var, int pos, char *weightexpr,
                               double *rad)
{
  (void) MTess;
  (void) Tess;
  (void) dtess;
  (void) dcell;
  (void) var;
  (void) pos;
  (void) weightexpr;

  ut_array_1d_set (rad + 1, TOpt.CellQty, 1);

  return;
}
