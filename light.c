#include "light.h"
#include "rt.h"
#include "vp.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "sphere.h"
#include "plane.h"

int shadow_test(LIGHT_T light, VP_T int_pt, OBJ_T *list, VP_T normal) {
    //Shadow Ray Init
    RAY_T shadow_ray;
    shadow_ray.origin = int_pt;
    shadow_ray.dir = vp_subtract(light.loc, int_pt);
    shadow_ray.dir = normalize(shadow_ray.dir);
    //Offset
    shadow_ray.origin.x += normal.x * 0.001;
    shadow_ray.origin.y += normal.y * 0.001;
    shadow_ray.origin.z += normal.z * 0.001;
    //Intersect check
    double dummy_t;
    VP_T dummy_int_pt, dummy_normal;

    OBJ_T *curr;
    for (curr = list; curr != NULL; curr = curr->next) {
        if (curr->intersect(shadow_ray, curr, &dummy_t, &dummy_int_pt, &dummy_normal) == 1) {
            // Ensure that the intersection occurs between the point and the light
            if (dummy_t < distance(int_pt, light.loc)) {
                return 1;//Intersection
            }
        }
    }
    return 0;//No Intersection
}

Color illuminate(RAY_T ray, VP_T int_pt, OBJ_T obj, VP_T normal, LIGHT_T light, OBJ_T *list){
    VP_T light_loc = light.loc; //Fix for function with new light in function call

    Color col;//Return Color
    col.R = 0;
    col.G = 0;
    col.B = 0;
    Color obj_col;

    //light attenuation
    double atten = 1/(.002 * (distance(int_pt, light_loc) * distance(int_pt, light_loc)) +
            (0.02 * distance(int_pt, light_loc)) + 0.2);

    //Checker Functionality
    if(obj.checker && (((int)floor(int_pt.x) + (int)floor(int_pt.y) + (int)floor(int_pt.z)) & 1)){
        obj_col = obj.color2;
    } else {
        obj_col = obj.color;
    }

    //Ambient Light
    col.R += (.1 * obj_col.R);//Red Ambient
    col.G += (.1 * obj_col.G);//Green Ambient
    col.B += (.1 * obj_col.B);//Blue Ambient

    if(shadow_test(light, int_pt, list, normal) == 0){ //If not in shadow
        //Diffuse Light
        VP_T light_vec;//Vector from light_loc to int_pt
        light_vec.x = light_loc.x - int_pt.x;
        light_vec.y = light_loc.y - int_pt.y;
        light_vec.z = light_loc.z - int_pt.z;

        light_vec = normalize(light_vec);//Normalized for dot product
        double dp = dot(light_vec, normal);
        if(dp > 0){
            col.R += (dp * obj_col.R * atten);//Red Diffuse
            col.G += (dp * obj_col.G * atten);//Green Diffuse
            col.B += (dp * obj_col.B * atten);//Blue Diffuse

            //Specular Light
            //VP_T ref_vec = light_vec - normal * 2 * dp
            VP_T ref_vec = {.x = light_vec.x - normal.x * 2 * dp, //Reflective Vector
                    .y = light_vec.y - normal.y * 2 * dp,
                    .z = light_vec.z - normal.z * 2 * dp};
            ref_vec = normalize(ref_vec);
            double dp2 = dot(ref_vec, normalize(ray.dir));
            if(dp2 > 0){
                col.R += pow(dp2, 200) * atten;
                col.G += pow(dp2, 200) * atten;
                col.B += pow(dp2, 200) * atten;
            }
        }
    }

    //Light Correction
    if(col.R > 1){ //Capping R at 1
        col.R = 1;
    }
    if(col.G > 1){ //Capping G at 1
        col.G = 1;
    }
    if(col.B > 1){ //Capping B at 1
        col.B = 1;
    }
    return col;
}
