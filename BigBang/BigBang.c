#include <stdio.h>
#include <stdlib.h>
#include "BigBang.h"
#include <math.h>

int main() {

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


    if(tga_read("empty.tga",&width,&height,&image)!=TGA_OK)
        goto error;

    char fileName[] = "frame0001.tga";

/***********************************************************/

    /* Initialize random particles in god */
    struct particles *god[NUMBER_OF_PARTICLES + 1];
    for (int i = 0; i < NUMBER_OF_PARTICLES; ++i){
        god[i] = malloc(sizeof(struct particles));
        randCircle(god[i], height, width);
    }
/*
    god[0]->speedX = -20;
    god[0]->speedY = -20;
    god[0]->x = width / 2 - 200;
    god[0]->y = height/ 2;
    god[0]->radius = 10;

    god[1]->speedX = 20;
    god[1]->speedY = 20;
    god[1]->x = width / 2 + 200;
    god[1]->y = height/ 2;
    god[1]->radius = 10;
*/
    /* End array with NULL pointer */
    god[NUMBER_OF_PARTICLES] = NULL;

    /* Short initialization loop, moving particles from center. */
    for(int t = 0; t < NUMBER_OF_INITIAL_ITERATIONS; ++t){
        for(int i = 0; i < NUMBER_OF_PARTICLES; ++i){
            god[i]->x += SCALING_OF_INITIAL_ITERATIONS * god[i]->speedX;
            god[i]->y += SCALING_OF_INITIAL_ITERATIONS * god[i]->speedY;
        }
        /* Draw circles */
        for(int i = 0; i < NUMBER_OF_PARTICLES; ++i){
                draw_circle(image, width, height, god[i]->x, god[i]->y, god[i]->radius, &black);
        }
    }

/********************************************************************/

    /* The Fat Loop */
    double tempForceX;
    double tempForceY;
    double tempAcceleration;


    for(int t = 0; t < NUMBER_OF_ITERATIONS; ++t){

        /* Calculate new speeds from gravitational influence */
        for(int i = 0; i < NUMBER_OF_PARTICLES; ++i){

            double tempForceX = 0;
            double tempForceY = 0;

            for(int j = 0; j < NUMBER_OF_PARTICLES; ++j){

                /* Prevent from checking a particle against itself */
                if (i == j)
                    continue;

                tempForceX += gravity( god[i], god[j], 'x' );
                tempForceY += gravity( god[i], god[j], 'y' );
            }

            god[i]->speedX += (SLOW_DOWN_FACTOR * (tempForceX / mass(god[i])));
            god[i]->speedY += (SLOW_DOWN_FACTOR * (tempForceY / mass(god[i])));
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

        // File name "frame0001.tga"
        int temp = t, j = 0;
        for(int i = 8; i >= 5; i--){
            j = 8 - i;
            fileName[i] = '0' + t % ((int)pow(10,j));
            temp = temp/10;

        }

        if(tga_write(fileName,width,height,image,24)!=TGA_OK) {
            goto error_free;
        }
        /* Reload blank image*/

    }



    exit:
    free(image); /* Free memory allocated by tga_read() */
    return 0;

    error_free:
    free(image);

    error:
    return -1;

}
