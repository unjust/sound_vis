#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0,0,0);
   
    setupGui();
    initSound();
    
    sceneCenter = ofVec3f(ofGetWidth()/2, ofGetHeight()/2, 0);
    initAnimObjects();
    
    camera.setupPerspective();
    camera.setFov(60);
    
    // camera.setPosition(sceneCenter);
    // sceneCenter = ofVec3f(0, 0, 0);
    
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
    
    liss.setRadius(radiusX + (fftSmoothed[10] * 300), radiusY);
    liss.setPoints(points);
    
    int WINDOW_SIZE = nBandsToGet; // temporary
    
    for (int j = 0; j < NUMBER_OF_RINGS; j++){
        //if (RINGS[j]->status()) continue;
        
        float radius = 0.;
        for (int i = j * (WINDOW_SIZE/2)/NUMBER_OF_RINGS; i < (j + 1) * ((WINDOW_SIZE/2)/NUMBER_OF_RINGS); i ++){
            radius += fftSmoothed[i];
        }
        radius /= (WINDOW_SIZE/2)/NUMBER_OF_RINGS;
        
        //float heightR = theWindows[windowToDraw].magnitudeR;
        rings[j].setFinalRadius(ringRadius + (radius * 120.));
        rings[j].setZ(ringZ);
        
        // if (theWindows[currentWindow].subBeats[j] == 1)
        //{
            // RINGS[j]->ringBounce();
            //ringSwitch = 1;
            //printf("SUBBEAT FOUND \n");
        //}
        //else ringSwitch = 0;
        
    }
    
    camera.setNearClip(nearClip);
    camera.setFarClip(farClip);
    
    // change this for one update to constant
    if (lookAtCenter) {
        camera.lookAt(sceneCenter);
    }
    
    // cout << "\n position " << camera.getPosition();

    
    // camera.setPosition(ofVec3f(ofGetWidth()/2, ofGetHeight()/2, zoom));
    
    //camera.pan(cameraRotate);
}

//--------------------------------------------------------------
void ofApp::draw(){
  
    // gui
    cameraPanel.draw();
    lissajousAttrsPanel.draw();
    ringAttrsPanel.draw();
    soundPanel.draw();
  
    // ofPushMatrix();
    // ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0); // now center is 0, 0, 0
    
    camera.begin();
    cout << "\n position " << camera.getPosition();

    if (showLissajous) {
        liss.draw();
    }
    
    if (showRings) {
        for (int j = 0; j < NUMBER_OF_RINGS; j++){
            rings[j].draw(1, 10.);
        }
    }
    
    camera.end();
    // ofPopMatrix();
    
    // draw fft
    if (showDebug) {
        line.draw();
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
    
    float x = 10,
        y = 10;
    
    lissajousAttrsParams.setName("Lissajous");
    
    lissajousAttrsParams.add(radiusX.set("radius x", 10, 10, 300));
    lissajousAttrsParams.add(radiusY.set("radius y", 10, 10, 300));
    lissajousAttrsParams.add(points.set("points", 700, 1, 1200));
    lissajousAttrsParams.add(showLissajous.set("lissajous", true));
    
    showLissajous.addListener(this, &ofApp::animButtonPressed);
    
    lissajousAttrsPanel.setup(lissajousAttrsParams, "paramSettings.xml", x, y);
    
    ringAttrsParams.setName("Rings");
    ringAttrsParams.add(ringRadius.set("radius", 10, 0, 300));
    ringAttrsParams.add(ringZ.set("z", 0, 0, 1000));
    ringAttrsParams.add(showRings.set("circles", false));
    
    showRings.addListener(this, &ofApp::animButtonPressed);
    
    ringAttrsPanel.setup(ringAttrsParams, "ring.xml", x, y += 100);
    
    cameraParams.setName("Camera");

    cameraParams.add(nearClip.set("near", .1, .1, 3000));
    cameraParams.add(farClip.set("far", 700, 1, 3000));
    
    cameraParams.add(cameraX.set("x", ofGetWidth()/2, 1, 3000));
    cameraParams.add(cameraY.set("y", ofGetHeight()/2, 1, 3000));
    cameraParams.add(zoom.set("zoom", 665., 0, 1000.0));
    cameraParams.add(cameraRotate.set("rotate", 0, 0, 360));
    cameraParams.add(lookAtCenter.set("point", true));
    cameraParams.add(showDebug.set("debug", false));
    
    cameraX.addListener(this, &ofApp::handleCameraControls);
    cameraY.addListener(this, &ofApp::handleCameraControls);
    cameraRotate.addListener(this, &ofApp::handleCameraControls);
    //lookAtCenter.addListener(this, &ofApp::handleCameraControls);
    
    cameraPanel.setup(cameraParams, "camera.xml", x, y += 100);
   
    soundParams.setName("Sound");
    
    soundParams.add(playSnd.set("playSnd", false));
    playSnd.addListener(this, &ofApp::sndTogglePressed);

    soundParams.add(playFennesz.set("playFennesz", true));
    playFennesz.addListener(this, &ofApp::sndTogglePressed);
    
    soundParams.add(volume.set("volume", 0, 0, 1));
    volume.addListener(this, &ofApp::handleVolume);
    soundPanel.setup(soundParams, "soundSettings.xml", x, y += 100);
  
}

void ofApp::handleCameraControls(const void* sender, float &value) {
    cout << "\n value " << value;
    
    //cout << "\n up dir " << camera.getUpDir();
    // camera.rotate(value, camera.getUpDir());
    ofParameter<float> s = *((ofParameter<float>*) sender);
    
    if (s == cameraRotate) {
        camera.rotateAround(value, camera.getUpDir(), sceneCenter);
        // camera.lookAt(sceneCenter);
    }
    
    ofVec3f myCamGlobalPosition = camera.getGlobalPosition();
    ofQuaternion myCamRotation = camera.getGlobalOrientation();
    
    if (s == cameraX) {
        camera.setPosition(cameraX, myCamGlobalPosition.y, myCamGlobalPosition.z);
        // camera.lookAt(sceneCenter);
    }
    if (s == cameraY) {
        camera.setPosition(myCamGlobalPosition.x, cameraY, myCamGlobalPosition.z);
        // camera.lookAt(sceneCenter);
    }
   
    
    cout << "\n position " << myCamGlobalPosition;
    cout << "\n rotation " << myCamRotation;
    // camera.rotate(value, 0, 0, 0);
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

void ofApp::handleVolume(const void* sender, float &value) {
    noesysAudio.setVolume(value);
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
    
    // for debugging
    noesysAudio.setVolume(volume);
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
    
    liss.setPosition(sceneCenter.x, sceneCenter.y, sceneCenter.z);
    liss.setEndPosition(sceneCenter.x, sceneCenter.y, sceneCenter.z);
    liss.setRadius(20, 1.2);
    liss.setColor(256, 256, 0, 256);
    liss.setAlive(true);
    
    // init rings
    for (int r = 0; r < 128; r++) {
        rings[r] = Ring();
        rings[r].setPosition(sceneCenter.x, 200 + r * 20, 0);
    }
}
