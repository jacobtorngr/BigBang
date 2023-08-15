//
// Created by Jacob on 2020-10-20.
//

#include "tga.h"

#ifndef BIGBANG_BIGBANG_H
#define BIGBANG_BIGBANG_H

#define NUMBER_OF_PARTICLES 3000
#define STANDARD_RADIUS 3
#define NUMBER_OF_ITERATIONS 1000
#define GRAVITY_CONSTANT 20
#define NUMBER_OF_INITIAL_ITERATIONS 10
#define SCALING_OF_INITIAL_ITERATIONS 3
#define SEED 1

#define MAXIMUM_SPEED 40.0
#define MINIMUM_SPEED 10.0
#define MINIMUM_DISTANCE 10
#define PI 3.14159265359
#define SLOW_DOWN_FACTOR .2

struct particles{
    double speedX;
    double speedY;
    double x;
    double y;
    double radius;
};

void randCircle(struct particles *particle, int height, int width);


double mass(struct particles *particle);
double distance(struct particles *particleN, struct particles *particleP);
double gravity(struct particles *particleN, struct particles *particleP);

void draw_circle(PIXEL_RGB24 *image, int width, int height, int x0, int y0, int radius, const PIXEL_RGB24 *color);
void FileName(char * string,int number);


#endif //BIGBANG_BIGBANG_H
