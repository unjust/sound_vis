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
        
        void setRadius(float radiusX, float radiusY);
        void setColor(float r, float g, float b, float a);

        void setPosition(float x, float y, float z);
        void setEndPosition(float x, float y, float z);
    
        bool getAlive();
        void setAlive(bool alive);
    
        void move();
        void draw();
        void expand();
        void coil(float ixRadius, float icoils);
    
        Lissajous();
    
        
    private:
        int id;
        int nPoints;
        
        float centerX, centerX_end;
        float centerY, centerY_end;
        float centerZ, centerZ_end;
        float radiusX, radiusY, radiusZ;
        
        float red, green, blue, alpha;
        
        float coils;
        float stretch;
        
        float counter, phaserA, phaserB, expansionFactor;
        
        float threshhold,  xacceleration;
        bool alive;
        bool expanding;
    
        bool isInPosition();
        void setAlpha(float a);
        void setId(int id);
            
};

#endif /* Lissajous_hpp */
