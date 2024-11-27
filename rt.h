#ifndef RT_H
#define RT_H
#define NUM_OBJS 3

#include "vp.h" //Include vector-point type for structs

typedef struct RAY_T{ //Defines ray type
    VP_T origin; //Point
    VP_T dir; //Vector
}RAY_T;

typedef struct Color{ //Defines color type
    double R; //Red
    double G; //Green
    double B; //Blue
}Color;

typedef struct Sphere{ //Defines sphere type
    VP_T center; //point
    double radius;
}Sphere;

typedef struct PLANE_T{ //Plane Type
    double D; //Distance along normal
    VP_T normal; //Normal vector for definition of plane
    Color plane_color; //Color of plane
}PLANE_T;

typedef struct OBJ_T{
    union{
        Sphere sphere;
        PLANE_T plane;
    };
    char type; //S or P
    Color color; //Primary color
    Color color2; //Secondary Color
    int checker; //Boolean for Checkerboard
    struct OBJ_T *next; //Next value in linked list
    int (*intersect) (RAY_T ray, struct OBJ_T *obj, double *t, VP_T *int_pt, VP_T *normal);
}OBJ_T;

typedef struct LIGHT_T{ //New Light Type
    struct VP_T loc;
}LIGHT_T;

typedef struct SCENE_T{
    //Image Formatting
    double start_x;
    double start_y;
    double pix_size;

    OBJ_T *list; //Linked list of OBJ_T
    LIGHT_T light;

}SCENE_T;

#endif