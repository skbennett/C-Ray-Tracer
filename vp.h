#ifndef VP_H
#define VP_H

typedef struct VP_T{ //Defines Vector/point type
    double x;
    double y;
    double z;
}VP_T;

double vec_len(VP_T vec); //Magnitude of vector

double dot(VP_T vec1, VP_T vec2); //Dot Produce prototype

double cosine(VP_T vec1, VP_T vec2); //Cosine using dot product and magnitude

VP_T vp_subtract(VP_T vp1, VP_T vp2); //Subtract two vectors or points

double distance(VP_T p1, VP_T p2); //Distance between two points

VP_T normalize(VP_T vec); //Normalize Vector prototype

#endif