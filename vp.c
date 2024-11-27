#include <math.h>

#include "vp.h" //Vector-Point struct and prototypes

double vec_len(VP_T vec){ //magnitude of vector
    return sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
}

double dot(VP_T vec1, VP_T vec2){
    return (vec1.x * vec2.x) + (vec1.y * vec2.y) + (vec1.z * vec2.z); //Dot Product Formula
}

double cosine(VP_T vec1, VP_T vec2){
    double dot_p = dot(vec1, vec2); //Dot product of Vectors
    double mag1 = vec_len(vec1); //Magnitude of Vector 1
    double mag2 = vec_len(vec2); //Magnitude of Vector 2

    return dot_p / (mag1 * mag2); //Definition of Cosine by Dot Product Formula
}

VP_T vp_subtract(VP_T vp1, VP_T vp2){ //Subtract two vectors or points
    VP_T new_vpt;
    new_vpt.x = vp1.x - vp2.x;
    new_vpt.y = vp1.y - vp2.y;
    new_vpt.z = vp1.z - vp2.z;
    return new_vpt;
}

double distance(VP_T p1, VP_T p2){ //Distance between two points
    return sqrt((p2.x - p1.x) * (p2.x - p1.x) +
                   (p2.y - p1.y) * (p2.y - p1.y) +
                   (p2.z - p1.z) * (p2.z - p1.z));
}

VP_T normalize(VP_T vec){ //Normalize vector
    VP_T new_vec;
    double mag = vec_len(vec);
    new_vec.x = vec.x / mag;
    new_vec.y = vec.y / mag;
    new_vec.z = vec.z / mag;

    return new_vec; //Normalized Vector
}