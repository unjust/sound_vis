/* ring.h *  PA4 *  Created by ivy on 4/05/06. */

#ifndef RINGS_
#define RINGS_


#define NUMBER_OF_RINGS 28 // SUBBANDS
//#define RINGS_POINTS WINDOW_SIZE/2

#include "ofMain.h"
#include <stdio.h>

class Ring {
	
	private:
		float x, y, z;
		// float centerX, centerY, centerZ;
		float startX, startY, startZ;
		float endX, endY, endZ;
		float R, G, B, A;
		
		float orbitRadius;
		float currentRadius; 
		float finalRadius;
	
		int ident;
		int positionType;
		float step;
		float counter;
		float acceleration;
		
		int switcher;
		bool subBeat;
		bool alive;
		bool ready;
	
		
	public:
		Ring();
		
        void setColor(float r, float g, float b, float a);
    
        void setRadius(float r);
        void setFinalRadius(float r);

		void setPosition(float ix, float iy);
		void position();
	
		void bounce();
		void move();
		void draw(int ipositionNum, float iacceleration);
	
		bool isReady();
		bool status();
		//~Ring();
	
};


//we'll be getting half as many frequencies as the window

#endif
