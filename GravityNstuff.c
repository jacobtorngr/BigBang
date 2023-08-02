//
// Created by Jacob on 2020-10-20.
//
#include <stdio.h>
#include <stdlib.h>
#include "BigBang.h"
#include <math.h>

void randCircle(struct particles *particle, int height, int width){

    double angle = (double) rand() * 2 * PI / RAND_MAX;
    double speed = (double) rand() / RAND_MAX * ORDER_OF_MAGNITUDE;

    particle->speedX = cos(angle) * speed;
    particle->speedY = sin(angle) * speed;
    particle->x = width / 2;
    particle->y = height / 2;
    particle->radius = STANDARD_RADIUS;
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

    if(dist < MINIMUM_DISTANCE)
        dist = MINIMUM_DISTANCE;


    /* Calculate gravitational pull composant */
    double force = GRAVITY_CONSTANT * massN * massP / (dist * dist);

    return force;

}

/***************************************************************************/

double distance(struct particles *particleN, struct particles *particleP){
    double dist, tempx, tempy;

    tempx = (particleP->x - particleN->x) * (particleP->x - particleN->x);
    tempy = (particleP->y - particleN->y) * (particleP->y - particleN->y);
    dist = sqrt(tempx + tempy);
    return dist;
}

/*****************************************************************************/

