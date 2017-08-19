//
//  Lissajous.cpp
//  mySketch
//
//  Created by Ivy Feraco on 5/23/17.
//
//

#include "Lissajous.hpp"

Lissajous::Lissajous() {
    
    alive = false;
    expanding = false;

    radius = 100.,
    height = 100.;
    
    x = 0.0,
    y = 0.0,
    z = 0.0;

    endX = 0.0,
    endY = 0.0,
    endZ = 0.0;
    
    phaserA = 0.0,
    phaserB = 0.0,
    counter = 0.0;
   
    setColor(256, 256, 256, 1.);
    
    nPoints = 800;
    coils = 0.;
    stretch = 0.;
    
    //setId(0);
}

void Lissajous::setPoints(int p) {
    nPoints = p;
}

void Lissajous::setRadius(float r) {
    radius = r;
}

void Lissajous::setHeight(float h) {
    height = h;
}


void Lissajous::setColor(float r, float g, float b, float a) {
    R = r;
    G = g;
    B = b;
    A = a;
}

void Lissajous::setAlpha(float a) {
    A = a;
    
    printf("SET ALPHA MY ALPHA IS %d \n", A);
}

int Lissajous::getAlpha() {
    return A;
}

void Lissajous::setPosition(float ix, float iy, float iz) {
    x = ix;
    y = iy;
    z = iz;
}

void Lissajous::setEndPosition(float ix, float iy, float iz) {
    endX = ix;
    endY = iy;
    endZ = iz;
}


bool Lissajous::isInPosition() {
    return (x == endX && y == endY && z == endZ);
}

void Lissajous::setAlive(bool istatus) {
    alive = istatus;
    printf("am I alive %d\n\n", alive);
}

void Lissajous::setId(int i) {
    id = i;
}

void Lissajous::move() {
    if (isInPosition()) {
        return;
    }
    
    // acceleration += .05;
    acceleration = 1.;
    x += acceleration * (.001 * (endX - x));
    y += acceleration * (.001 * (endY - y));
    z += acceleration * (.001 * (endZ - z));
}

void Lissajous::draw() {
    
    // printf("about to ofSetColor in draw, alpha is %d \n", A);
    
    ofSetColor(R, G, B, A);
    // ofSetColor(100, 1, 25, 80);
    
    // move();
    
    // this is some fade??
    /*
    if (alive && A < 1. && !expanding) {
        A += .001;
    }
    */
    
    // not sure what phasers do
    // phaserA += .008;
    // phaserB += .003;
    
    counter += .1;
    
    // float lissLength = ofGetHeight();
    
    for (int point = 1; point < nPoints; point++) {
        
        float pointRatio = (float)point/nPoints;
        float radPosition = 360 * pointRatio * DEG_TO_RAD;
        
        float lissX = cos(radPosition) * radius;
        
        // playing around with this, fmod seems to be crucial for a good effect,
        // other params seem a little arbitrary but it works
        // not much understanding behind the math, but this is a cool effect
        
        float lissY = sin(radPosition) * radius + (height * pointRatio * counter);
        lissY = fmod(lissY, height);
        
        float lissZ = (sin(radPosition) * radius);

        /*
         weird effect when you change the radius
         
         pointX = x + cos(radPosition) * radius;
         //pointY = (sin(radPosition) * radius + pointRatio * counter * height);
         pointY = (y - height/2.) + fmod(sin(radPosition * height) * radius + counter, height);
         pointZ = z;
         
        */

        ofDrawCircle(x + lissX, (y - height/2.) + lissY, z + lissZ, 2);
    }
}


void Lissajous::expand() {
    radius += expansionFactor * xacceleration;
    xacceleration += .07;
    setAlpha(A -= .001);
}


void Lissajous::coil(float ixRadius, float icoils) {
    if (expanding) {
        expand();
        if (A < 0.1) alive = false;
        return;
    }
    
    /*
    if (selected && radius > threshhold) {
        expanding = true;
        return;
    }
    */

    float startCoils = coils;
    float endCoils   = icoils;

    coils += (.001 * (endCoils - startCoils));
    printf("diff = %f, coils = %f\n", startCoils - endCoils, coils);

    float startXRadius = radius;
    float endXRadius = ixRadius;

    radius += (.001 * (endXRadius - startXRadius));

    //float startYRadius = yRadius;
    //float endYRadius   = iyRadius;

    //yRadius += (.001 * (endYRadius - startYRadius));
}


bool Lissajous::getAlive() {
    return alive;
}



