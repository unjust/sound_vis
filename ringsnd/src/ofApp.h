#pragma once


#include "ofMain.h"
#include "ofxGui.h"
#include "Ring.h"

#define NUMBER_OF_LISS 7

class ofApp : public ofBaseApp{
    
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void sndTogglePressed(const void* sender, bool &value);
    void ringAnimButtonPressed(const void* sender);
    void handleVolume(const void* sender, float &value);
    void setRingRadius(const void* sender, float &value);
    
    void handleCameraControls(const void* sender, float &value);
    void handleCameraReset(const void* sender, bool &value);
    
    void audioOut( float * output, int bufferSize, int nChannels );
    void playSound(bool snd);
    
private:
    
    ofCamera camera;
    ofEasyCam easyCam;
    
    ofVec3f sceneCenter;
    
    // gui
    
    ofxPanel soundPanel;
    ofParameterGroup soundParams;
    
    ofParameter <float>volume;
    ofParameter <bool>playSnd;
    ofParameter <bool>playFennesz;
    
    ofxPanel ringAttrsPanel;
    ofParameterGroup ringAttrsParams;
    
    ofParameter <bool>ringBounce;
    ofParameter <bool>showRings;
    
    ofParameter <float>ringRadius;
    ofParameter <float>ringZ;
    
    int ringAnimation; // ringAnimationType ?
    ofxButton ringAnimationButton;
    
    ofxPanel cameraPanel;
    ofParameterGroup cameraParams;
    
  
    // drawing
    
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
    
    // methods
    
    void initSound();
    void fftDraw();
    void setupGui();
    void initAnimObjects();
    
};
