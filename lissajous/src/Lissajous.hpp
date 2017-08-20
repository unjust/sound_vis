//
//  Lissajous.hpp
//  mySketch
//
//  Created by Ivy Feraco on 5/23/17.
//
//

#ifndef Lissajous_hpp
#define Lissajous_hpp

#include "ofMain.h"
#include <stdio.h>

class Lissajous {

    public:
        bool selected;
        float acceleration;
        
        void setRadius(float radius);
        void setHeight(float height);
        void setColor(int r, int g, int b, int a);
    
        int  getAlpha();
        void setAlpha(float a);

        void setPosition(float ix, float iy, float iz);
        void setEndPosition(float x, float y, float z);
    
        bool getAlive();
        void setAlive(bool alive);
    
        void move();
        void draw();
        void expand();
        void coil(float ixRadius, float icoils);
    
        Lissajous();
        void setPoints(int p);
    
        // todo proper get and seet
        float x, endX;
        float y, endY;
        float z, endZ;
    private:
        int id;
        int nPoints;
    
        float radius;
        float height;
    
        int R, G, B, A;
        
        float coils;
        float stretch;
        
        float counter, phaserA, phaserB, expansionFactor;
        
        float threshhold, xacceleration;
        bool alive;
        bool expanding;
    
        bool isInPosition();
        void setId(int i);
            
};

#endif /* Lissajous_hpp */
