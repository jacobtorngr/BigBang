#include <stdio.h>
#include <stdlib.h>
#include "BigBang.h"
#include <math.h>
#include <python3.10/Python.h>

int main() {
    srand(SEED);

    // Load blank tga file
    int width;
    int height;
    PIXEL_RGB24 *image,red,green,blue, gray, orange, black;

    red.R=255; red.G=0;     red.B=0;
    green.R=0; green.G=255; green.B=0;
    blue.R=0;  blue.G=0;    blue.B=255;
    gray.R=100;  gray.G=100;    gray.B=100;
    orange.R=0xFF;  orange.G=0x88;    orange.B=0;
    black.R=0;  black.G=0;    black.B=0;

    char fileName[] = "frame0000.tga";

    /* Load image before initial iterations */
    if(tga_read("../empty.tga",&width,&height,&image)!=TGA_OK){
        goto error;
    }


/***********************************************************/

    /* Initialize random particles in god */
    struct particles *god[NUMBER_OF_PARTICLES + 1];
    for (int i = 0; i < NUMBER_OF_PARTICLES; ++i){
        god[i] = malloc(sizeof(struct particles));
        randCircle(god[i], height, width);
    }
/*
    god[0]->speedX = -5;
    god[0]->speedY = -5;
    god[0]->x = width / 2 - 200;
    god[0]->y = height/ 2;
    god[0]->radius = 20;

    god[1]->speedX = 5;
    god[1]->speedY = 5;
    god[1]->x = width / 2 + 200;
    god[1]->y = height/ 2;
    god[1]->radius = 20;
*/
    /* End array with NULL pointer */
    god[NUMBER_OF_PARTICLES] = NULL;

    /* Short initialization loop, moving particles from center. */
    for(int t = 0; t < NUMBER_OF_INITIAL_ITERATIONS; ++t){
        for(int i = 0; i < NUMBER_OF_PARTICLES; ++i){
            god[i]->x += SCALING_OF_INITIAL_ITERATIONS * god[i]->speedX;
            god[i]->y += SCALING_OF_INITIAL_ITERATIONS * god[i]->speedY;
        }



        /* Change file name "frameXXXX.tga" */
        FileName(fileName, t);


        /* Draw circles */
        for(int i = 0; i < NUMBER_OF_PARTICLES; ++i){
            draw_circle(image, width, height, god[i]->x, god[i]->y, god[i]->radius, &black);
        }

        /* Write to file */
        if(tga_write(fileName,width,height,image,24)!=TGA_OK) {
            goto error_free;
        }

        /* Reload blank image*/
        free(image);
        if(tga_read("../empty.tga",&width,&height,&image)!=TGA_OK)
            goto error;

    }

/********************************************************************/

    /* The Fat Loop */
    double tempForce, forcesX, forcesY, tempAngle;
    Py_Initialize();

    for(int t = 0; t < NUMBER_OF_ITERATIONS; ++t){

        /* Reload blank image for current iteration*/
        free(image);
        if(tga_read("../empty.tga",&width,&height,&image)!=TGA_OK)
            goto error;


        /* Calculate new speeds from gravitational influence */
        for(int i = 0; i < NUMBER_OF_PARTICLES; ++i){

            forcesX = 0;
            forcesY = 0;

            for(int j = 0; j < NUMBER_OF_PARTICLES; ++j){

                /* Prevent from checking a particle against itself */
                if (i == j)
                    continue;

                tempForce = gravity( god[i], god[j] );

                tempAngle = atan2(god[j]->y - god[i]->y,god[j]->x - god[i]->x);

                forcesX += tempForce * cos(tempAngle);
                forcesY += tempForce * sin(tempAngle);


            }


            god[i]->speedX += (SLOW_DOWN_FACTOR * (forcesX / mass(god[i])));
            god[i]->speedY += (SLOW_DOWN_FACTOR * (forcesY / mass(god[i])));
        }

        /* Move particles according to velocity */
        for(int i = 0; i < NUMBER_OF_PARTICLES; ++i){
            god[i]->x += SLOW_DOWN_FACTOR * god[i]->speedX;
            god[i]->y += SLOW_DOWN_FACTOR * god[i]->speedY;
        }

        /* Draw circles */
        for(int i = 0; i < NUMBER_OF_PARTICLES; ++i){
            draw_circle(image, width, height, god[i]->x, god[i]->y, god[i]->radius, &black);
        }

        /* Change file name "frameXXXX.tga" */
        FileName(fileName, t + NUMBER_OF_INITIAL_ITERATIONS);

        /* Write to file */
        if(tga_write(fileName,width,height,image,24)!=TGA_OK) {
            goto error_free;
        }

        // Convert to png
        PyObject *obj = Py_BuildValue("s", "../FromTgaToPng.py");
        FILE *fp = _Py_fopen_obj(obj, "r+");

        if (fp != NULL){
            PyRun_SimpleFile(fp, "FromTgaToPng.py");
        }
        int its = NUMBER_OF_ITERATIONS + NUMBER_OF_INITIAL_ITERATIONS;
        printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
        printf("Generated frame %i/%i",(t + NUMBER_OF_INITIAL_ITERATIONS +1),its);
        fflush(stdout);
    }

    

    exit:
    printf("\nFrames generated successfully.\n");
    free(image); /* Free memory allocated by tga_read() */

    /* free particles */
    for(int i = 0; i < NUMBER_OF_PARTICLES; ++i){
        free(god[i]);
    }

    // PyRun_SimpleString("print('Hello World')");
    PyObject *obj = Py_BuildValue("s", "../CreateVideo.py");
    FILE *fp = _Py_fopen_obj(obj, "r+");

    if (fp != NULL){
        PyRun_SimpleFile(fp, "CreateVideo.py");
    }
    
    Py_Finalize();
    return 0;

    error_free:
    printf("failed to read empty.tga\n");
    free(image);
    return -1;

    error:
    printf("failed to read empty.tga\n");
    return -1;

}
