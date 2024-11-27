#ifndef PLANE_H
#define PLANE_H

#include "vp.h" //Vector for normal
#include "rt.h" //Ray Type for ray, Plane Type

//Intersect Function, returns boolean
int intersect_plane(RAY_T ray, OBJ_T *obj, double *t, VP_T *int_pt, VP_T *normal);

#endif
