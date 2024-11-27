#include <stdlib.h>
#include <stdio.h>

#include "rt.h"
#include "vp.h"
#include "light.h"
#include "plane.h"
#include "sphere.h"

Color trace(RAY_T ray, SCENE_T scene){
    //Getting list and light From Scene
    OBJ_T *list = scene.list;
    LIGHT_T light = scene.light;

    Color pixel_col; // Black Pixel
    pixel_col.R = 0;
    pixel_col.G = 0;
    pixel_col.B = 0;
    Color background_color; // Blue Color
    background_color.R = 0.3;
    background_color.G = 0.3;
    background_color.B = 0.5;

    double closest_t = -1; //Closest value of t
    VP_T closest_int_pt; //Int_pt for closest object
    VP_T closest_normal; //Normal vector for closest object
    int closest_object_index = -1;

    double t;
    VP_T int_pt; //Intersection Point for illuminate
    VP_T normal; //Normal for illuminate

    OBJ_T *curr; //Current object in linked list;
    int curr_index = -1;
    for(curr = list; curr!= NULL; curr = curr -> next){
        curr_index += 1;
        //printf("Checking object at index %d\n", curr_index);
        //printf("Current Object Type %c\n", curr -> type);
        if(curr -> intersect(ray, curr, &t, &int_pt, &normal) == 1) { //All items in scene
            //printf("Intersection found at t = %lf\n", t);
            if (closest_t < 0 || t < closest_t) { //Changes default value or the old closest value
                closest_t = t;
                closest_normal = normal;
                closest_int_pt = int_pt;
                closest_object_index = curr_index;
            }
        }
    }

    if (closest_object_index != -1) { //Illuminates as long as not default value
        OBJ_T *curr_obj = list;
        for(int i = 0; curr_obj != NULL && i < closest_object_index; i++){ //parses until closest object
            curr_obj = curr_obj -> next;
        }
        if(curr_obj != NULL){ //Checks if object is valid
            pixel_col = illuminate(ray, closest_int_pt, *curr_obj,closest_normal, light, list);
        }

    } else {
        pixel_col = background_color; //Sets to default color if no intersection
    }
    return pixel_col;
}

SCENE_T init(char filename[]){

    SCENE_T scene; //Scene to return to main

    LIGHT_T light; //Light Type for scene

    OBJ_T *list = NULL; //Linked list for scene
    FILE *input;
    input = fopen(filename, "r"); //Open input file to read

    char c; //Char to see type of object
    while(fscanf(input, " %c", &c) != EOF){
        if(c == 's'){
            printf("Initializing Sphere\n");
            double locx, locy, locz; //Location of Sphere
            double rad;              //Radius of Sphere
            double colR, colG, colB; //Color of Sphere

            fscanf(input, "%lf %lf %lf %lf %lf %lf %lf", &locx, &locy, &locz, &rad, &colR, &colG, &colB);

            printf("Sphere - Center: (%lf, %lf, %lf), Radius: %lf, Color: (%lf, %lf, %lf)\n",
                   locx, locy, locz, rad, colR, colG, colB);

            OBJ_T *node = (OBJ_T *) malloc(sizeof(OBJ_T)); //Creates node in memory
            //Setting Node to Parameters
            node -> type = 's'; //Sphere identifier
            node -> sphere.center.x = locx;
            node -> sphere.center.y = locy;
            node -> sphere.center.z = locz;
            node -> sphere.radius = rad;
            node -> color.R = colR;
            node -> color.G = colG;
            node -> color.B = colB;
            node -> intersect = &intersect_sphere;
            //Linked List Formatting
            node -> next = list;
            list = node;

        } else if(c == 'p'){
            printf("Initializing Plane\n");
            double normx, normy, normz; //Normal Vector of Plane
            double D;                   //D value of plane
            double col1R, col1G, col1B;
            double col2R, col2G, col2B;

            fscanf(input, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
                   &normx, &normy, &normz, &D, &col1R, &col1G, &col1B, &col2R, &col2G, &col2B);

            printf("Plane - Normal: (%lf, %lf, %lf), D: %lf, Color1: (%lf, %lf, %lf), Color2: (%lf, %lf, %lf)\n",
                   normx, normy, normz, D, col1R, col1G, col1B, col2R, col2G, col2B);

            OBJ_T *node = (OBJ_T *) malloc(sizeof(OBJ_T)); //Creates node in memory
            //Setting node plane parameters
            node -> type = 'p'; //Plane identifier
            node -> plane.normal.x = normx;
            node -> plane.normal.y = normy;
            node -> plane.normal.z = normz;
            node -> plane.D = D;
            node -> color.R = col1R;
            node -> color.G = col1G;
            node -> color.B = col1B;
            node -> color2.R = col2R;
            node -> color2.G = col2G;
            node -> color2.B = col2B;
            node -> checker = 1; //Assume checker is true for all planes
            node -> intersect = &intersect_plane;
            //Linked List Formatting
            node -> next = list;
            list = node;

        } else if(c == 'l'){
            printf("Initializing Light\n");
            double locx, locy, locz;
            fscanf(input, "%lf %lf %lf", &locx, &locy, &locz); //String to light parameters

            light.loc.x = locx;
            light.loc.y = locy;
            light.loc.z = locz;

            printf("Light - Location: (%lf, %lf, %lf)\n", light.loc.x, light.loc.y, light.loc.z);
        }
    }
    fclose(input); //Close File
    scene.list = list;
    scene.light = light;
    return scene;
}

int main(){
    SCENE_T scene = init("scene1.txt"); //Getting Scene from File

    FILE *fp;
    fp = fopen("img.ppm", "w"); //Opening or Creating img.ppm

    //Image Formatting
    int rows, cols;
    rows = 480;
    cols = 640;
    scene.pix_size = (double)1 / (double)rows;
    scene.start_y = 0.5;
    scene.start_x = -((double)cols / (double)rows) / 2;

    // PPM Formatting
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", cols, rows);
    fprintf(fp, "255\n");

    for(int y = 0; y < rows; y++){ //y loop
        for(int x = 0; x < cols; x++){ //x loop

            //Initializing Ray for Pixel
            struct RAY_T ray;
            ray.origin.x = 0;
            ray.origin.y = 0;
            ray.origin.z = 0;

            struct VP_T direction;
            direction.x = scene.start_x + x * scene.pix_size;
            direction.y = scene.start_y - y * scene.pix_size;
            direction.z = 1;

            ray.dir = normalize(direction); //Normalized direction of light through image plane

            Color pixel_col = trace(ray, scene);
            
            //PPM Output: WORKING
            fprintf(fp,"%c%c%c", (char)(pixel_col.R * 255.0),
                                               (char)(pixel_col.G * 255.0),
                                               (char)(pixel_col.B * 255.0));
        }
    }
    fclose(fp);//Close file
    return 0;
}