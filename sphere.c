#include "sphere.h"
#include "rt.h"
#include <math.h>

int intersect_sphere(RAY_T ray, OBJ_T *obj, double *t, VP_T *int_pt, VP_T *normal){
    Sphere sphere = obj -> sphere;
    double A = 1;
    double B = 2 * ((ray.dir.x * (ray.origin.x - sphere.center.x)) +
                    (ray.dir.y * (ray.origin.y - sphere.center.y)) +
                    (ray.dir.z * (ray.origin.z - sphere.center.z)));
    double C = ((ray.origin.x - sphere.center.x) * (ray.origin.x - sphere.center.x)) +
               ((ray.origin.y - sphere.center.y) * (ray.origin.y - sphere.center.y)) +
               ((ray.origin.z - sphere.center.z) * (ray.origin.z - sphere.center.z)) -
               ((sphere.radius) * (sphere.radius));

    if(((B * B) - (4 * A * C)) <= 0){ //Discriminant <= 0, no intersection
        return 0;

    } else { //Discriminant > 0, intersection

        double t1 = (-B + sqrt((B * B) - (4 * A * C))) / (2 * A); //1st value of t
        double t2 = (-B - sqrt((B * B) - (4 * A * C))) / (2 * A); //2nd value of t

        if(t1 > 0 && t2 > 0){ //both positive
            if(t1 < t2){ //t1 smaller
                *t = t1;
                //return 1;
            } else { //t2 smaller
                *t = t2;
                //return 1;
            }
        } else if(t1 > 0 && t2 < 0){ //only t1 positive
            *t = t1;
            //return 1;
        } else if(t2 > 0 && t1 < 0){ //only t2 positive
            *t = t2;
            //return 1;
        } else {
            return 0;
        }

        //int_pt calculations
        (*int_pt).x = 0 + (*t * ray.dir.x); //int_pt x changed
        (*int_pt).y = 0 + (*t * ray.dir.y); //int_pt y changed
        (*int_pt).z = 0 + (*t * ray.dir.z); //int_pt z changed

        //normal vector calculations
        VP_T norm;
        norm.x = (*int_pt).x - sphere.center.x; //x for normal vector
        norm.y = (*int_pt).y - sphere.center.y; //y for normal vector
        norm.z = (*int_pt).z - sphere.center.z; //z for normal vector
        *normal = normalize(norm); //sets normal vector in memory to normalized normal vector

        return 1; //Intersection Occurs
    }
}