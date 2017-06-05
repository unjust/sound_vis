#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Lissajous.hpp"
#include "Ring.h"

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
        
        void sndTogglePressed(const void* sender, bool &value);
        void animButtonPressed(const void* sender, bool &value);
    
        void audioOut( float * output, int bufferSize, int nChannels );
        void playSound(bool snd);
    
    private:
    
        // gui
    
    
        ofxPanel soundPanel;
        ofParameterGroup soundParams;
    
        ofxPanel animationsPanel;
        ofParameterGroup animationParams;
    
        ofParameterGroup drawingParams;
        ofxPanel drawingParamsPanel;
    
        ofParameter <float>radiusX;
        ofParameter <float>radiusY;
    
        ofParameter <bool>playSnd;
        ofParameter <bool>playFennesz;
    
        ofParameter <bool>showRings;
        ofParameter <bool>showLissajous;
        ofParameter <bool>showDebug;
    
        // drawing
    
        ofPolyline line;
    
        Lissajous liss;
    
        Ring ring;
        Ring rings[128];
        
        float posX;
        float posY;
        double phase;
    
        // sound
    
        ofSoundPlayer noesysAudio;
        float *fftSmoothed;
        int nBandsToGet;
        bool playingSnd;
    
        void initSound();
        void fftDraw();
        void setupGui();
        void initAnimObjects();

};
