#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    gui.setup();
    gui.add(radiusX.setup("radius x", 10, 10, 300));
    gui.add(radiusY.setup("radius y", 10, 10, 300));
    gui.add(debug.setup("debug", false, 20, 20));
    
    initSound();
    
    // ofSoundStreamSetup(2, 0, 44100, 256, 4);
    // phase = 0;
    
    ofBackground(0,0,0);
 
    // lissajous setup
    liss = Lissajous();
    
    liss.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
    liss.setEndPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
    liss.setRadius(20, 1.2);
    liss.setColor(256, 256, 0, 256);
    liss.setAlive(true);
}

//--------------------------------------------------------------
void ofApp::update(){
    for (auto &vert : line.getVertices()){
        vert.x += ofRandom(-1,1);
        vert.y += ofRandom(-1,1);
    }

    //printf("update called %f", radius);
    //liss.setRadius(radiusX, radiusY);
    liss.setRadius(fftSmoothed[10] * 300, radiusY);
    
    
    
    //sound
    
    // update the sound playing system:
    ofSoundUpdate();
  
    
    // grab the fft and put in smoothed array like example project
    float *val = ofSoundGetSpectrum(nBandsToGet); // request 128 for FFT
    
    for (int i = 0; i < nBandsToGet; i++) {
        // let the smoothed value sink to zero:
        fftSmoothed[i] *= 0.96f;
        // take the max, either the smoothed or the incoming:
        if (fftSmoothed[i] < val[i]) {
            fftSmoothed[i] = val[i];
        }
        //printf("%f \n", fftSmoothed[i]);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    gui.draw();
    
    line.draw();
    
    liss.draw();
    
    // draw fft
    if (debug) {
        fftDraw();
    }
}

//----------------------------------------------------------------
void ofApp::audioOut( float * output, int bufferSize, int nChannels ){
    for (int i = 0; i < bufferSize * nChannels; i += 2) {
        float sample = sin(phase);
        output[i] = sample;
        output[i+1] = sample;
        phase += 0.05;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    posX = x;
    posY = y;
    
    ofPoint pt;
    pt.set(x,y);
    line.addVertex(pt);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    line.clear();
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

void ofApp::initSound() {
    noesysAudio.load("sounds/theMix.mp3");
    noesysAudio.play();
    
    // the fft needs to be smoothed out, so we create an array of floats
    // for that purpose:
    fftSmoothed = new float[8192];
    for (int i = 0; i < 8192; i++){
        fftSmoothed[i] = 0;
    }
    nBandsToGet = 128;
}

void ofApp::fftDraw() {
    ofSetColor(255,255,255,100);
    
    float bandwidth = ofGetWidth()/nBandsToGet;
    for (int i = 0; i < nBandsToGet; i++) {
        ofDrawRectangle(100 + bandwidth * i, ofGetHeight()/2, bandwidth, -(fftSmoothed[i]*200));
    }
}
