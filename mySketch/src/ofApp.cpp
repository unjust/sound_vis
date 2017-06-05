#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0,0,0);
    
    setupGui();
    initSound();
    initAnimObjects();
}

//--------------------------------------------------------------
void ofApp::update(){
    for (auto &vert : line.getVertices()){
        vert.x += ofRandom(-1,1);
        vert.y += ofRandom(-1,1);
    }

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
    
    liss.setRadius(fftSmoothed[10] * 300, radiusY);
    
    int WINDOW_SIZE = nBandsToGet; // temporary
    
    for (int j = 0; j < NUMBER_OF_RINGS; j++){
        //if (RINGS[j]->status()) continue;
        
        float radius = 0.;
        for (int i = j * (WINDOW_SIZE/2)/NUMBER_OF_RINGS; i < (j + 1) * ((WINDOW_SIZE/2)/NUMBER_OF_RINGS); i ++){
            radius += fftSmoothed[i];
        }
        radius /= (WINDOW_SIZE/2)/NUMBER_OF_RINGS;
        //float heightR = theWindows[windowToDraw].magnitudeR;
        rings[j].setFinalRadius(radius * 120.);
        
        // if (theWindows[currentWindow].subBeats[j] == 1)
        //{
            // RINGS[j]->ringBounce();
            //ringSwitch = 1;
            //printf("SUBBEAT FOUND \n");
        //}
        //else ringSwitch = 0;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
  
    drawingParamsPanel.draw();
    animationsPanel.draw();
    soundPanel.draw();
    
    line.draw();
    
    if (showLissajous) {
        liss.draw();
    }
    
    if (showRings) {
        for (int j = 0; j < NUMBER_OF_RINGS; j++){
            rings[j].draw(1, 10.);
        }
    }
    
    // draw fft
    if (showDebug) {
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

// gui ------------------------------------------------------------

void ofApp::setupGui(){
    
    drawingParams.setName("Drawing params");
    
    drawingParams.add(radiusX.set("radius x", 10, 10, 300));
    drawingParams.add(radiusY.set("radius y", 10, 10, 300));

    drawingParamsPanel.setup(drawingParams, "paramSettings.xml", 10, 10);
    
    soundParams.setName("Sound");
    
    soundParams.add(playSnd.set("playSnd", false));
    playSnd.addListener(this, &ofApp::sndTogglePressed);

    soundParams.add(playFennesz.set("playFennesz", true));
    playFennesz.addListener(this, &ofApp::sndTogglePressed);
    
    soundPanel.setup(soundParams, "soundSettings.xml", 10, 100);
    
    animationParams.setName("Animations");
    
    animationParams.add(showRings.set("circles", false));
    showRings.addListener(this, &ofApp::animButtonPressed);
    
    animationParams.add(showLissajous.set("lissajous", true));
    showLissajous.addListener(this, &ofApp::animButtonPressed);
    
    animationParams.add(showDebug.set("debug", false));
    
    animationsPanel.setup(animationParams, "animationsSettings.xml", 10, 200);
}

void ofApp::animButtonPressed(const void* sender, bool &value) {
    // int debug;
}

void ofApp::sndTogglePressed(const void* sender, bool &value) {

    //ofParameter<bool>* x = (ofParameter<bool>*) sender;
    ofParameter<bool> s = *((ofParameter<bool>*) sender);
    
    cout << "\n toggle " << s.getName();
    // cout << "\n toggle : " << x->getName();
    
    // radio-like behavior, if clicking something already true
    if (!s.get()) {
        s.set(true);
        return;
    }
    
    if (s.getName() == "playSnd") {
        playFennesz.setWithoutEventNotifications(false);
        playSound(true);
        ofBackground(255,255,255);
    } else {
        playSnd.setWithoutEventNotifications(false);
        playSound(false);
        ofBackground(0,0,0);
    }
  
}

//--------------------------------------------------------------
void ofApp::initSound(){
   
    playSound(false);
    // the fft needs to be smoothed out, so we create an array of floats
    // for that purpose:
    fftSmoothed = new float[8192];
    for (int i = 0; i < 8192; i++){
        fftSmoothed[i] = 0;
    }
    nBandsToGet = 128;
}

void ofApp::playSound(bool snd) {
    string filename = (snd) ? "sounds/snd.aiff" : "sounds/fennesz.aiff";
    noesysAudio.load(filename);
    noesysAudio.play();
    
}

//--------------------------------------------------------------
void ofApp::fftDraw(){
    ofSetColor(255,255,255,100);
    
    float bandwidth = ofGetWidth()/nBandsToGet;
    for (int i = 0; i < nBandsToGet; i++) {
        ofDrawRectangle(100 + bandwidth * i, ofGetHeight()/2, bandwidth, -(fftSmoothed[i]*200));
    }
}

//--------------------------------------------------------------
void ofApp::initAnimObjects(){
    // lissajous setup
    liss = Lissajous();
    
    liss.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
    liss.setEndPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
    liss.setRadius(20, 1.2);
    liss.setColor(256, 256, 0, 256);
    liss.setAlive(true);
    
    // init rings
    for (int r = 0; r < 128; r++) {
        rings[r] = Ring();
        rings[r].setPosition(ofGetWidth()/2, 100 + r * 10);
    }
}
