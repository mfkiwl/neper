/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_show_tesr_.h"

void
nev_show_tesr (char **argv, int *pi, struct PRINT *pPrint)
{
  (*pi)++;
  ut_string_int (argv[(*pi)], &((*pPrint).showtesr));
  if ((*pPrint).showtesr < 0 || (*pPrint).showtesr > 1)
  {
    ut_print_message (2, 0, "-showtesr: bad value.\n");
    abort ();
  }

  return;
}

void
nev_show_tesr_cell (char **argv, int *pi, struct TESR Tesr,
		    struct PRINT *pPrint)
{
  int i, j, status, varqty;
  double res;
  char **vars = NULL;
  double *vals = NULL;
  int **pshowarray = NULL;
  char *entity = NULL;

  (*pi)++;

  if (Tesr.Dim == 1)
  {
    pshowarray = &(*pPrint).showedge;
    ut_string_string ("edge", &entity);
  }
  else if (Tesr.Dim == 2)
  {
    pshowarray = &(*pPrint).showface;
    ut_string_string ("face", &entity);
  }
  else if (Tesr.Dim == 3)
  {
    pshowarray = &(*pPrint).showpoly;
    ut_string_string ("cell", &entity);
  }

  (*pshowarray) = ut_realloc_1d_int (*pshowarray, Tesr.CellQty + 1);
  ut_array_1d_int_zero ((*pshowarray) + 1, Tesr.CellQty);

  // checking for a general expression
  status = nev_show_genexpr (argv[(*pi)], *pshowarray, Tesr.CellQty);

  if (status != 0)
  {
    neut_tesr_var_list (entity, &vars, &varqty);
    vals = ut_alloc_1d (varqty);

    for (i = 1; i <= Tesr.CellQty; i++)
    {
      for (j = 0; j < varqty; j++)
	if (strstr (argv[(*pi)], vars[j]))
	  neut_tesr_var_val (Tesr, entity, i, vars[j], &(vals[j]), NULL);

      status = ut_math_eval (argv[(*pi)], varqty, vars, vals, &res);
      if (status == 0)
	(*pshowarray)[i] = res;
      else
	ut_print_message (2, 0, "Expression `%s' could not be processed.\n",
			  argv[(*pi)]);
    }
  }

  (*pshowarray)[0] = ut_array_1d_int_sum ((*pshowarray) + 1, Tesr.CellQty);

  ut_free_2d_char (vars, varqty);
  ut_free_1d (vals);
  ut_free_1d_char (entity);

  return;
}
