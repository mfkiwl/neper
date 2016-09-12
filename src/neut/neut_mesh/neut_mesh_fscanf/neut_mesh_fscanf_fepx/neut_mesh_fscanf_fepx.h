/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_MESH_FSCANF_FEPX

  extern void neut_mesh_fscanf_fepx (FILE * parms, FILE * mesh, FILE * elsets,
				     struct NODES *pNodes,
				     struct MESH *pMesh);

#endif				// NEUT_MESH_FSCANF_FEPX

#ifdef __cplusplus
}
#endif
