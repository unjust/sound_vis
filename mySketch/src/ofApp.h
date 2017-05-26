#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Lissajous.hpp"

class ofApp : public ofBaseApp{
    
    public:
        void setup();
        void update();
        void draw();
        
        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
        
        void audioOut( float * output, int bufferSize, int nChannels );
        
        // gui
        ofxPanel gui;
        ofxFloatSlider radiusX;
        ofxFloatSlider radiusY;
        ofxToggle debug;
   
    
        // drawing
        ofPolyline line;
        Lissajous liss;
        
        float posX;
        float posY;
        double phase;
        
        // sound
        ofSoundPlayer noesysAudio;
        float * fftSmoothed;
        int nBandsToGet;

    private:
        void initSound();
        void fftDraw();
    
};
