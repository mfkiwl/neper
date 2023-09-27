/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_gen_odf_.h"

void
net_tess_opt_comp_objective_fval_gen_odf (struct TOPT *pTOpt, int var)
{
  if ((*pTOpt).cvlsig[var] > 0)
  {
    /*
    if ((*pTOpt).iter == (*pTOpt).loop_start[(*pTOpt).loop - 1]
        || (*pTOpt).iter % 1000 == 1)
        */
    {
      if (!strcmp ((*pTOpt).cvlmethod, "numerical"))
        net_tess_opt_comp_objective_fval_gen_odf_smoothed (pTOpt, var);
      else
        ut_print_neperbug ();
    }
    /*

    else
    {
      if (!strcmp ((*pTOpt).cvlmethod, "numerical"))
        net_tess_opt_comp_objective_fval_gen_odf_smoothed_update (pTOpt, var);
      else
        ut_print_neperbug ();
    }
    */
  }

  net_tess_opt_comp_objective_fval_gen_odf_evaluate (pTOpt, var);

  return;
}
