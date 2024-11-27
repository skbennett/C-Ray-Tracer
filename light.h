#ifndef LIGHT_H
#define LIGHT_H

#include "vp.h" //VP_T for light location
#include "rt.h" //Color type for illuminate

struct Color illuminate(RAY_T ray, VP_T int_pt, OBJ_T obj, VP_T normal, LIGHT_T light, OBJ_T *list);

#endif
