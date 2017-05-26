//
//  Lissajous.cpp
//  mySketch
//
//  Created by Ivy Feraco on 5/23/17.
//
//

#include "Lissajous.hpp"

Lissajous::Lissajous() {
    printf("lissajous constructor called\n");

    alive = false;
    expanding = false;

    radiusX = 100.;
    radiusY = 100.;
    radiusZ = 100.;
    
    centerX = 0.0,
    centerY = 0.0,
    centerZ = 0.0;
    centerX_end = 0.0,
    centerY_end = 0.0,
    centerZ_end = 0.0;
    
    phaserA = 0.0,
    phaserB = 0.0,
    counter = 0.0,
   
    setColor(256, 256, 256, 1.);
    
    nPoints = 200;
    coils = 0.;
    stretch = 0.;
    
    //setId(0);
}


void Lissajous::setRadius(float rx, float ry) {
    radiusX = rx;
    radiusY = ry;
}

void Lissajous::setColor(float r, float g, float b, float a) {
    red = r;
    green = g;
    blue = b;
    alpha = a;
}

void Lissajous::setAlpha(float a) {
    alpha = a;
}

void Lissajous::setPosition(float x, float y, float z) {
    centerX = x;
    centerY = y;
    centerZ = z;
}

void Lissajous::setEndPosition(float x, float y, float z) {
    centerX_end = x;
    centerY_end = y;
    centerZ_end = z;
}


bool Lissajous::isInPosition() {
    return (centerX == centerX_end && centerY == centerY_end && centerZ == centerZ_end);
}

void Lissajous::setAlive(bool istatus) {
    alive = istatus;
    printf("am I alive %d\n\n", alive);
}

void Lissajous::setId(int id) {
    id = id;
}

void Lissajous::move() {
    if (isInPosition()) return;
    
    // acceleration += .05;
    acceleration = 1.;
    centerX += acceleration * (.001 * (centerX_end - centerX));
    centerY += acceleration * (.001 * (centerY_end - centerY));
    centerZ += acceleration * (.001 * (centerZ_end - centerZ));
}

void Lissajous::draw() {
    
    ofSetColor(red, green, blue, alpha);
    
    move();
    
    // this is some fade??
    if (alive && alpha < 1. && !expanding) {
        alpha += .001;
    }
    
    // not sure what phasers do
    // phaserA += .008;
    // phaserB += .003;
    
    counter += .1;
    
    float lissLength = radiusY * 6.; // how vertically stretched it is / phase point
    // float lissLength = ofGetHeight();
    
    // draw dots
    for (int point = 1; point < nPoints; point++)
    {
        float position = ((360) * (float)point/nPoints) * DEG_TO_RAD;
        
        
        // turning in one place, static pattern
        float x = centerX + (cos(position + counter) * radiusX);
        float y = centerY + sin(position + counter)  - (lissLength/2.) + fmod(y + (lissLength/2.), lissLength);
        float z = centerZ;
        
        
        /*
        // changing patterns when position of liss moves
        float x = centerX + (cos(position) * radiusX);
        float y = centerY - (lissLength/2.) + fmod(y + (lissLength/2.0), lissLength);
        float z = centerZ;
        */
        
        /*
        // changing patterns with counter
        float x = (centerX + counter) + (cos(position) * radiusX);
        float y = (centerY + counter) - (lissLength/2.) + fmod(y + (lissLength/2.0), lissLength);
        float z = centerZ;
        */

        //printf("draw %f %f %f \n", x, y, z);
        
        ofDrawCircle(x, y, z, 3);
    }
}


void Lissajous::expand() {
    radiusX += expansionFactor * xacceleration;
    xacceleration += .07;
    setAlpha(alpha -= .001);
}

//
//void Lissajous::coil(float ixRadius, float icoils) {
//    if (expanding) {
//        expand();
//        if (A < 0.1) alive = false;
//        return;
//    }
//    if (selected && radiusX > threshhold) {
//        expanding = true;
//        return;
//    }
//
//    float startCoils = coils;
//    float endCoils   = icoils;
//
//    coils += (.001 * (endCoils - startCoils));
//    printf("diff = %f, coils = %f\n", startCoils - endCoils, coils);
//
//    float startXRadius = radiusX;
//    float endXRadius = ixRadius;
//
//    radiusX += (.001 * (endXRadius - startXRadius));
//
//    //float startYRadius = yRadius;
//    //float endYRadius   = iyRadius;
//
//    //yRadius += (.001 * (endYRadius - startYRadius));
//}
//

//
//bool Lissajous::getAlive() {
//    return alive;
//}



