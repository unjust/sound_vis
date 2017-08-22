/* ring.h *  PA4 *  Created by ivy on 4/05/06. */

#ifndef RINGS_
#define RINGS_


#define NUMBER_OF_RINGS 28 // SUBBANDS
//#define RINGS_POINTS WINDOW_SIZE/2

#include "ofMain.h"
#include <stdio.h>

enum ringAnimationType {
    line = 0,
    orbital,
    tangental
};

class Ring {
	
	private:
		float x, y, z;
		// float centerX, centerY, centerZ;
		float endX, endY, endZ;
		
		float currentRadius;
        float finalRadius; // TODO deltaRadius instead
    
        static float baseRadius;  // added to currentRadius
        static float orbitRadius;
    
        float R, G, B, A;
    
		float step;
		float counter;
		float acceleration;
		
		bool subBeat;
		bool alive;
		bool ready;
	
	public:
    
        int ident;
        bool bounce;
        ringAnimationType animationType;
    
		Ring();
		
        void setColor(float r, float g, float b, float a);
    
        void setRadius(float r);
        void setFinalRadius(float r);
    
        static void setBaseRadius(float r);
        static void setOrbitRadius(float r);

		void setPosition(float ix, float iy, float iz);
        void setZ(float iz);
        void setAnimationType(ringAnimationType type);
	
		// void bounce();
		void move();
        void position();
		void draw(float iacceleration);
	
		bool isReady();
		bool status();
    
        void resetRadius();
		//~Ring();
	
};


//we'll be getting half as many frequencies as the window

#endif
