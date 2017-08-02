/*  Ring.cpp * PA4 */

#include "Ring.h"

Ring * RINGS[NUMBER_OF_RINGS];


Ring::Ring(){
	printf("default ring constructor called\n");

	alive = false;
    currentRadius = 0.;
    finalRadius = 100.;
	orbitRadius = 100.;
    
    x = 0;
    y = 0;
    z = 0;
    
    endX = 0;
    endY = 0;
    endZ = 0;
    
    setFinalRadius(100);
    setColor(0, .529, .878, 1);
    
    acceleration = 1.;
}


void Ring::setRadius(float r){
    currentRadius = r;
}

void Ring::setFinalRadius(float r){
    finalRadius = r;
    alive = true; //turned on
}

void Ring::setPosition(float ix, float iy, float iz){
    x = ix;
    y = iy;
    z = iz;
}

void Ring::setZ(float iz){
    z = iz;
}

void Ring::setColor(float r, float g, float b, float a){
    R = r;
    G = g;
    B = b;
    A = a;
}

// void Ring::bounce(){
//      subBeat = true;
//      //printf("BOUNCEDDDDDDDD\n");
// }

void Ring::move(){
    
	float startX = x;
	float startY = y;
	float startZ = z;
	
	// if (fabs(endZ - startZ) < .1) {
    //		ready = false;
    // }
    //	else {
    //    ready = true;
    // }

     x += (.01 + acceleration) * (endX - startX);
     y += (.01 + acceleration) * (endY - startY);
	//z += (.001 + acceleration/10.) * (endZ - startZ);

    //    printf("x = %f y = %f \n", x, y );
}

void Ring::position(){
	
	//x = ix * 2.6/NUMBER_OF_RINGS * 6.;
	
	if (animationType == line) {
        
        endX = 0.;
		endY = -300 + ident * 20; // copy initial position set by App for now
        // endY = -.1 + (.6/NUMBER_OF_RINGS) * ident;
		//endZ = ident * .01;
        
    } else if (animationType == orbital) {
        
        float input = TWO_PI * ident/NUMBER_OF_RINGS + counter;
        endX = orbitRadius * sin(input);
        endY = orbitRadius * cos(input);
        endZ = 0.;
        
    } else {
        endX = 0;
        endY = 0 - finalRadius;
    }
}

void Ring::resetRadius() {
    currentRadius = 0;
    finalRadius = 0;
}

void Ring::draw(float iacceleration){
    
    acceleration = iacceleration;
    GLint circle_points = 100;


    if (currentRadius < finalRadius) {
        position();
        move();
        
        //if (switcher)
//        if (bounce) {
//            glColor4f(R, G, B, A);
//            
//            glBegin(GL_POLYGON);
//            for (int i = 0; i < circle_points; i++) {
//                float angle = 2 * PI * i/circle_points;
//                //glVertex3f(-1.6 + x + cos(angle) * radius, sin(angle) *  radius, 0.0);
//                glVertex3f(x + cos(angle) * currentRadius, y + sin(angle) * currentRadius, z);
//            }
//            glEnd();
//        }
        
        glColor4f(R, G, B, A);
        glBegin(GL_LINE_LOOP);
        
        for (int i = 0; i < circle_points; i++) {
//            float angle = TWO_PI * i/circle_points;
//            float aX = x + sin(angle) * currentRadius;
//            float aY = y + cos(angle) * currentRadius;
//            float aZ = z;
//            glVertex3f(aX, aY, aZ);
            float angle = 2 * PI * i/circle_points;
            //glVertex3f(-1.6 + x + cos(angle) * radius, sin(angle) *  radius, 0.0);
            glVertex3f(x + cos(angle) * currentRadius, y + sin(angle) * currentRadius, 0);

            
        }
        
        // printf("%f, %f, %f, %f < %f \n", x, y, z, currentRadius, finalRadius);
        glEnd();
        
        currentRadius += .1;
        
        counter += .001;
        
        // alpha was causing problems
        // if (A > 0.) A -= .001;
        
        
    } else {
        bounce = false;
        alive = false; //turned off ready for new value
        currentRadius = 0.0;
        A = 1.0;
        //counter = 0.;
    }
}

//void Ring::draw(int iswitcher, float iacceleration){
//	
//	switcher = iswitcher;
//	acceleration = .2;
//	GLint circle_points = 100; 
//	
//	if (currentRadius < finalRadius) {
//		//printf("RUNNING acceleration %f\n", iacceleration);
//		//position();
//		//move();
//		
//        if (subBeat) {
//			glColor4f(R, G, B, A);
//			
//			glBegin(GL_POLYGON); 
//			for (int i = 0; i < circle_points; i++) {
//				float angle = 2 * PI * i/circle_points; 
//				float aX = x + sin(angle) * currentRadius;
//				float aY = y + cos(angle) * currentRadius;
//				float aZ = z;
//				glVertex3f(aX, aY, aZ);
//				
//			} 
//			glEnd();
//		}		
//		
//		glColor4f(R, G, B, A);
//		glBegin(GL_POLYGON);
//		
//		for (int i = 0; i < circle_points; i++) {
//			float angle = TWO_PI * i/circle_points;
//			//glVertex3f(-1.6 + x + cos(angle) * radius, sin(angle) *  radius, 0.0); 
//			float aX = x + sin(angle) * currentRadius;
//			float aY = y + cos(angle) * currentRadius;
//			float aZ = z;
//			glVertex3f(aX, aY, aZ);
//            // printf("currentRadius %f \n", currentRadius);
//			// printf("hello %f %f %f \n", aX, aY, aZ);
//		} 
//		
//		glEnd();
//		currentRadius += .1;
//		counter += .001;
//        if (A > 0.) {
//            A -= .001;
//        }
//	}
//	else  {
//		subBeat = false;
//		alive = false; //turned off ready for new value
//		currentRadius = 0.0;
//		A = 1.0;
//		//counter = 0.;
//	}
//	
//}

bool Ring::status(){
	return alive;
}

bool Ring::isReady(){
	return ready;
}

//
//Ring::~Ring() {
//	printf("goodbye\n");
//}

/*
 Ring::Ring(int iident, float ix, float iy, float iz, float iR, float iG, float iB, float iA, float ifinalRadius) {
	orbitRadius = .2;
	currentRadius = 0.;
	finalRadius =  .2;
	
	R = iR, G = iG, B = iB;
	A = iA;
	
	ident = iident;
	x = ix, y = iy, z = iz;
	
	startX = x, startY = y, startZ = z;
	endX = x, endY = y, endZ = z;
	//finalRadius =  rand() % 255;									//is this working?
	
	step = 1/10000.;
	
	alive = true;
	ready = false, subBeat = false;
	
	counter = 0;
	acceleration = 0.;
	
	switcher = 0;
	//startX = orbitRadius * sin(ident * PI_2/NUMBER_OF_RINGS);
	//startY = orbitRadius * cos(ident * PI_2/NUMBER_OF_RINGS);
 }
 */
