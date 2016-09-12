/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_fscanf_.h"

void
neut_tess_fscanf (FILE * file, struct TESS *pTess)
{
  neut_tess_fscanf_verbosity (file, pTess, 0);
}

void
neut_tess_fscanf_verbosity (FILE * file, struct TESS *pTess, int verbosity)
{
  char *version = ut_alloc_1d_char (10);

  verbosity = verbosity;

  if ((*pTess).PolyQty > 0)
    neut_tess_free (pTess);

  neut_tess_fscanf_version (file, version);

  if (!strcmp (version, "1.9.2"))
  {
    neut_tess_fscanf_1p9 (file, pTess);
    (*pTess).CellQty = (*pTess).PolyQty;
    (*pTess).DomType = ut_alloc_1d_char (5);
    strcpy ((*pTess).DomType, "cube");
    neut_tess_init_domain (pTess);
  }

  else if (!strcmp (version, "1.10"))
  {
    neut_tess_fscanf_1p10 (file, pTess);
    neut_tess_init_domain_label (pTess);
  }

  else if (!strcmp (version, "2.0"))
  {
    char *tmp = ut_alloc_1d_char (100);
    neut_tess_fscanf_head (pTess, file);
    neut_tess_fscanf_cell (pTess, file);
    if ((*pTess).Dim >= 0)
      neut_tess_fscanf_ver (pTess, file);
    if ((*pTess).Dim >= 1)
    {
      neut_tess_fscanf_edge (pTess, file);
      neut_tess_init_veredge (pTess);
    }
    if ((*pTess).Dim >= 2)
    {
      neut_tess_fscanf_face (pTess, file);
      neut_tess_init_edgeface (pTess);
    }
    if ((*pTess).Dim >= 3)
    {
      neut_tess_fscanf_poly (pTess, file);
      neut_tess_init_facepoly (pTess);
    }

    ut_file_nextstring (file, tmp);
    if (!strcmp (tmp, "**domain"))
      neut_tess_fscanf_domain (pTess, file);

    ut_file_nextstring (file, tmp);
    if (!strcmp (tmp, "**periodicity"))
      neut_tess_fscanf_per (pTess, file);

    ut_file_nextstring (file, tmp);
    if (!strcmp (tmp, "**scale"))
      neut_tess_fscanf_scale (pTess, file);

    neut_tess_fscanf_foot (file);
    neut_tess_init_edgelength (pTess);

    ut_free_1d_char (tmp);
  }

  else
  {
    ut_print_message (2, 2, "Unsupported tess file version `%s'.\n", version);
    abort ();
  }

  /*
     if (neut_tess_test (*pTess, verbosity) != 0)
     {
     ut_print_message (2, 0,
     "Tessellation checking failed - tessellation is not valid.\n");
     if (!verbosity)
     ut_print_message (2, 0, "Debug with option -checktess of neper -T.\n");

     abort ();
     }
     else if (verbosity)
     ut_print_message (2, 0, "Tessellation checking succeeded.\n");
   */

  neut_tess_init_alldom_fromdomtess (pTess);

  neut_tess_init_celltrue (pTess);
  neut_tess_init_cellbody (pTess);

  ut_free_1d_char (version);

  return;
}

void
neut_tess_name_fscanf (char *name, struct TESS *pTess)
{
  FILE *file = NULL;
  char **list = NULL;
  int i, j, qty, qty2;
  char **string = ut_alloc_2d_char (6, 1000);
  int *bounds = NULL;
  double *scale = NULL;

  ut_string_separate (name, NEUT_SEP_DEP, &list, &qty);

  file = ut_file_open (list[0], "r");
  neut_tess_fscanf (file, pTess);
  ut_file_close (file, list[0], "r");

  for (i = 1; i < qty; i++)
  {
    if (!strncmp (list[i], "scale", 5))
    {
      ut_string_fnr (list[i], ',', ' ');
      list[i][strlen (list[i]) - 1] = '\0';

      scale = ut_alloc_1d (3);
      qty2 = sscanf (list[i], "scale(%s%s%s", string[0], string[1], string[2]);

      if (qty2 == 1)
      {
	ut_math_eval (string[0], 0, NULL, NULL, &(scale[0]));
	ut_array_1d_set (scale, 3, scale[0]);
      }
      else
      {
	for (j = 0; j < qty2; j++)
	  ut_math_eval (string[j], 0, NULL, NULL, &(scale[j]));
	for (j = qty2; j < 3; j++)
	  scale[j] = 1;
      }

      neut_tess_scale (pTess, scale[0], scale[1], scale[2]);
    }
    else
      ut_print_message (2, 0, "Failed to process expression `%s'.\n", name);
  }

  ut_free_1d_int (bounds);
  ut_free_1d (scale);
  ut_free_2d_char (string, 6);
  ut_free_2d_char (list, qty);

  return;
}
