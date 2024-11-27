#include "plane.h"
#include "vp.h"
#include "rt.h"

int intersect_plane(RAY_T ray, OBJ_T *obj, double *t, VP_T *int_pt, VP_T *normal){
    PLANE_T plane = obj -> plane;
    double dp = dot(plane.normal, ray.dir);
    if(dp == 0){ //No intersection
        return 0;
    } else {
        *t = -(dot(plane.normal, ray.origin) + plane.D) / dp;
        if(*t <= 0){ //No intersection, intersection behind point
            return 0;
        } else {
            //int_pt calculations
            (*int_pt).x = 0 + (*t * ray.dir.x); //int_pt x changed
            (*int_pt).y = 0 + (*t * ray.dir.y); //int_pt y changed
            (*int_pt).z = 0 + (*t * ray.dir.z); //int_pt z changed

            *normal = plane.normal; //Normal is plane normal
            return 1;
        }
    }
}