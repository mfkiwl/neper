/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTIN_T_H
#define STRUCTIN_T_H

  struct IN_T
  {
    // global options (level-independent)
    int dim;				// dimension
    char *domain;			// domain
    char *periodicstring;		// periodicity
    char *tesrsizestring;		// size of the tesr

    char *nstring;			// number of seeds
    char **n;				//
    char *idstring;			// id of the seed distribution
    char **id;				//

    char *morphostring;		        // morphology input string
    char **morpho;			//
    char *optialgostring;		// opti algorithm
    char **optialgo;		//
    char *optiinistring;		// opti algorithm
    char **optiini;		//
    char *optialgomaxiterstring;  // max number of iterations without
    					// decreasing the objective function
    char **optialgomaxiter;	//
    char *optiobjectivestring;	// opti objective function
    char **optiobjective;		//
    char *optigridstring;		// grid definition
    char **optigrid;		//
    char *optismoothstring;	// smoothing parameter
    char **optismooth;		//
    char *optistopstring;		// opti stop criterion
    char **optistop;		//
    char *optidofstring;		// opti dofs
    char **optidof;		//
    char *optideltamaxstring;		// opti limit distance
    char **optideltamax;		//
    char *optiinistepstring;	// opti ini step
    char **optiinistep;		//
    char *optilogtimestring;      // time log
    char **optilogtime;		//
    char *optilogvarstring;       // variable log
    char **optilogvar;		//
    char *optilogdisstring;       // distribution log
    char **optilogdis;		//
    char *optilogtesrstring;      // tesr control points log
    char **optilogtesr;		//
    char *optilogvalstring;       // value log
    char **optilogval;		//
    char *optimultiseedstring;	// opti multiseed
    char **optimultiseed;		//

    char *oristring;		        // ori input string
    char **ori;                         // orientation distribution
    char *orioptistopstring;            // orientation opti / termination criterion
    char **orioptistop;                 //
    char *orioptineighstring;           // orientation opti / neighbor string
    char **orioptineigh;                //
    char *orioptiinistring;             // orientation opti initial orientations
    char **orioptiini;                  //
    char *orioptifixstring;             // orientation opti fixed orientations
    char **orioptifix;                  //
    char *orioptilogvarstring;           // variable log
    char **orioptilogvar;		//

    char *orispreadstring;		// orientation spread string
    char **orispread;                   // orientation spreads

    char *crysymstring;		        // crystal symmetry
    char **crysym;			//

    // post-tessellation options
    int reg;				// regularization
    double fmax;			// regularization fmax
    char *seltype;			// regularization sel type
    char *selstring;			// regularization sel
    int mloop;				// regularization loop
    int maxedgedelqty;			// regularization max del
    char *sortstring;			// cell sorting
    int checktess;			// checking tess?

    char *transform;			// transformation

    char *group;                      // cell sets

    // internal variables
    int *periodic;			// periodicity
    int levelqty;			// number of levels
    char *mode;				// running mode: tess or tesr
    char *input;			// NULL (=none), n, tess, tesr

    char *loadpoint;			// loadpoint file
    char *load;				// loadtess or loadtesr file

    // output options
    char *format;			// output format
    char *tesrformat;			// output tesr format

    char *orides;			// orientation descriptor
    char *oriformat;			// orientation file format

    // output files
    char *body;				// file name without extension
    char *tess;				// tess file
    char *tesr;				// tesr file
    char *vtk;				// AMITEX-FFT file
    char *raw;				// raw data of tesr file
    char *geo;				// Gmsh geo file
    char *ply;				// Ply file
    char *stl;				// stl file
    char *dec;				// 3dec file
    char *fe;				// fe file
    char *sttess;			// tess stat file
    char *sttesr;			// tesr stat file
    char *stc;				// cell stat file
    char *stv;				// ver stat file
    char *ste;				// edge stat file
    char *stf;				// face stat file
    char *stp;				// poly stat file
    char *sts;				// seed stat file
    char *stpt;				// point stat file
    char *stvox;		        // vox stat file
    char *stgroup;		        // group stat file
    char *obj;				// Wavefront obj file
    char *orif;				// orientation file
    char *sim;				// simulation file

    // output files for development
    char *debug;			// tess debug file
    char *mtess;			// multiscale tess file
    char *mgeo;				// multiscale Gmsh geo file
  };
  typedef struct IN_T IN_T;

#endif				/* STRUCTIN_T_H */

#ifdef __cplusplus
}
#endif
