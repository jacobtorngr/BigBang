//
// Created by Jacob on 2020-10-20.
//

#include "tga.h"
#include <math.h>

void draw_circle(PIXEL_RGB24 *image, int width, int height,
                 int x0, int y0, int radius, const PIXEL_RGB24 *color) {

    double alpha; /* angle */
    int x,y,index;

    for(alpha=0; alpha<=2*M_PI; alpha+=0.5/radius) {

        x = x0 + radius*cos(alpha); /* x coordinate */
        y = y0 + radius*sin(alpha); /* y coordinate */

        if(x>=0 && x<width && y>=0 && y<height ) {

            index = x + width*y; /* index of pixel in buffer */

            copy_pixel(&image[index], color);
        }
    }
}
