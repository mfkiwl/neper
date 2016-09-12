/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#ifndef OL_SSET
#define OL_SSET

#include "ol_des.h"
#include "ol_cal.h"
#include "ol_gen.h"

struct OL_SET
{
  size_t size;
  double **q;
  double *weight;
  int* id;
  char* crysym;
};
typedef struct OL_SET OL_SET;

extern void ol_set_mean (struct OL_SET, double *);
extern void ol_set_mean_ref (struct OL_SET, double *, double *);
extern int ol_set_mean_iter (struct OL_SET, double *);
extern int ol_set_mean_iter_ref (struct OL_SET, double *, double *);

extern struct OL_SET ol_set_alloc (size_t, char*);
extern void ol_set_free (struct OL_SET);
extern int ol_set_fscanf (FILE *, struct OL_SET *, char*);
extern int ol_set_fscanf_sample (FILE *, double, struct OL_SET *, char*);
extern int ol_set_fscanf_sample_nb (FILE *, int, struct OL_SET *, char*);
extern int ol_set_fprintf (FILE *, struct OL_SET, char*);
extern void ol_set_subset (struct OL_SET, double, struct OL_SET *);
extern void ol_set_orthotropic (struct OL_SET, struct OL_SET *);
extern void ol_set_memcpy (struct OL_SET, struct OL_SET*);

extern void ol_set_aniso (struct OL_SET, double **, double *);
extern void ol_q_aniso_theta (double *, double **, double *);
extern void ol_set_aniso_thetadistrib (struct OL_SET, double **, double,
				       double **, double *, int *);

extern void ol_set_sample (struct OL_SET, double, struct OL_SET *);
extern void ol_set_sample_nb (struct OL_SET, int, struct OL_SET *);

extern void ol_set_disori (struct OL_SET, double *, double *);
extern int ol_set_filter (struct OL_SET, double, struct OL_SET *, double*, double*);
extern int ol_set_filter_iter (struct OL_SET, double, struct OL_SET *, double*, double*);
extern int ol_set_anifilter (struct OL_SET, double, struct OL_SET *);
extern int ol_set_anifilter_iter (struct OL_SET, double, struct OL_SET *, double*, double**, double*);
extern int ol_set_anifilter_iter_itermax (struct OL_SET, double, int, struct OL_SET *, double*, double**, double*);
extern int ol_set_mean_aniso_anifilter_iter_itermax (struct OL_SET, double*, double**, double*, double, int, struct OL_SET *, double*, double**, double*);
extern int ol_set_mean_anifilter (struct OL_SET, double*, double, struct OL_SET*);
extern int ol_set_mean_aniso_anifilter (struct OL_SET, double*, double**, double*, double, struct OL_SET*);

extern void ol_set_disoriset (struct OL_SET, struct OL_SET*);
extern void ol_set_disoriset_cur (struct OL_SET, struct OL_SET*);
extern void ol_set_mean_disoriset (struct OL_SET, double*, struct OL_SET*);
extern void ol_set_mean_disoriset_cur (struct OL_SET, double*, struct OL_SET*);

extern int ol_set_mean_aniso_anifilter_iter (struct OL_SET, double*,
    double**, double*, double, struct OL_SET *, double*,
    double**, double*);

extern void ol_set_aniso_modes (struct OL_SET, double*, struct OL_SET*, struct OL_SET*);
extern void ol_set_aniso_v_modes (struct OL_SET, double*, double**, struct OL_SET*, struct OL_SET*);
extern void ol_set_aniso_v_thetatr_modes (struct OL_SET, double*, double**,
	        double, struct OL_SET*, struct OL_SET*);
extern void ol_set_aniso_v_thetatr_modes_id (struct OL_SET, double*,
					     double**, double, int*);

#endif /* OL_SSET */
#ifndef OL_SETx
#define OL_SETx

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"ut.h"
#define isNaN(x) ((x) != (x))
#endif /* OL_SETx */
#ifndef OL_SETx
#define OL_SETx

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"ut.h"
#define isNaN(x) ((x) != (x))
#include <string.h>
#include "ol_glo.h"
#include "ol_des.h"
#include "ol_gen.h"
#endif /* OL_SETx */
