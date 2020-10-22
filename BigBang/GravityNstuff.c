//
// Created by Jacob on 2020-10-20.
//
#include <stdio.h>
#include <stdlib.h>
#include "BigBang.h"
#include <math.h>

void randCircle(struct particles *particle, int height, int width){
    int sign1 = 0;
    int sign2 = 0;
    int coinFlip1 = rand();
    int coinFlip2 = rand();

    if(coinFlip1 > (RAND_MAX/2))
        sign1 = 1;
    else
        sign1 = -1;

    if(coinFlip2 > (RAND_MAX/2))
        sign2 = 1;
    else
        sign2 = -1;

    particle->speedX = sign1 * ORDER_OF_MAGNITUDE * rand();
    particle->speedY = sign2 * ORDER_OF_MAGNITUDE * rand();
    particle->x = width / 2;
    particle->y = height / 2;
    particle->radius = 5;
}

/*****************************************************************************/


double mass(struct particles *particle) {
    double m = PI * particle->radius * particle->radius;
    return m;
}

/*****************************************************************************/

double gravity(struct particles *particleN, struct particles *particleP){

    double massN = mass(particleN);
    double massP = mass(particleP);

    double dist = distance(particleN, particleP);


    /* Calculate gravitational pull composant */
    double force = -GRAVITY_CONSTANT * massN * massP / (dist * dist);

    return force;

}

/***************************************************************************/

double distance(struct particles *particleN, struct particles *particleP){
    double dist, tempx, tempy;

    tempx = sqrt( (particleP->x - particleN->x) * (particleP->x - particleN->x) );
    tempy = sqrt( (particleP->y - particleN->y) * (particleP->y - particleN->y) );
    dist = sqrt(tempx + tempy);
}

/*****************************************************************************/

double slope(struct particles *particleN, struct particles *particleP){

    /*  */
    double temp = particleN->y;
    if(particleP->x == particleN->x)
        temp += 0.01;



}