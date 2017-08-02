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
    camera.setNearClip(nearClip);
    camera.setFarClip(farClip);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // line animation whatever
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
    
    // update liss

    for (int l = 0; l < sizeof(lisses)/sizeof(lisses[0]); l++) {
        lisses[l].setRadius(lissRadius + (fftSmoothed[10] * 300));
        lisses[l].setHeight(lissHeight);
        lisses[l].setPoints(points);
    }
    
    // update rings
    int WINDOW_SIZE = nBandsToGet; // temporary
    
    for (int j = 0; j < NUMBER_OF_RINGS; j++){
        //if (RINGS[j]->status()) continue;
        
        float sonicRadius = 0.;
        int SOME_SIZE = (WINDOW_SIZE/2) / NUMBER_OF_RINGS;
        for (int i = j * SOME_SIZE; i < (j + 1) * SOME_SIZE; i++){
            sonicRadius += fftSmoothed[i];
        }
        sonicRadius /= SOME_SIZE;
        
        // make setter and class/static property ?
        rings[j].animationType = (ringAnimationType)ringAnimation;
        
        if (ringAnimation == tangental) {
            rings[j].setFinalRadius((1 * j) + (sonicRadius * 100.));
            
        } else {
            // why * 120 ?
            rings[j].setFinalRadius(ringRadius + (sonicRadius * 120.));
        }
        
        // why did I set z
        rings[j].setZ(ringZ);
        
    }
    
    // cout << "\n position " << camera.getPosition();
   
    // camera.pan(cameraRotate);
   
    
    if (lookAtCenter) {
        camera.lookAt(sceneCenter);
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
  
    // gui
    cameraPanel.draw();
    lissajousAttrsPanel.draw();
    ringAttrsPanel.draw();
    soundPanel.draw();
  
    
    camera.begin();
    
    //
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0); // now center is 0, 0, 0
    
    //cout << "\n camera position: " << camera.getPosition();

    if (showLissajous) {
        for (int l = 0; l < sizeof(lisses)/sizeof(lisses[0]); l++){
            lisses[l].draw();
        }
    }
    
    if (showRings) {
        
        for (int j = 0; j < NUMBER_OF_RINGS; j++){
            // rings[j].draw(1, 10.);
            rings[j].draw(.01);
        }
    }
    
    ofPopMatrix();
    camera.end();
    
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
    
    lissajousAttrsParams.add(lissRadius.set("radius", 10, 0, 300));
    lissajousAttrsParams.add(lissHeight.set("height", 10, 0, 300));
    lissajousAttrsParams.add(points.set("points", 700, 1, 1200));
    lissajousAttrsParams.add(showLissajous.set("lissajous", true));
    
    lissajousAttrsPanel.setup(lissajousAttrsParams, "paramSettings.xml", x, y);
    
    ringAttrsParams.setName("Rings");
    ringAttrsParams.add(ringRadius.set("fradius", 10, 0, 300));
    ringAttrsParams.add(ringZ.set("z", 0, 0, 1000));
    ringAttrsParams.add(ringBounce.set("bounce", false));
    ringAttrsParams.add(showRings.set("circles", false));
    
    //ringRadius.addListener(this, &ofApp::setRingRadius);
    
    
    ringAttrsPanel.setup(ringAttrsParams, "ring.xml", x, y += 100);
    ringAttrsPanel.add(ringAnimationButton.setup("anim type"));
   
   
    ringAnimationButton.addListener(this, &ofApp::ringAnimButtonPressed);
    
    
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
    zoom.addListener(this, &ofApp::handleCameraControls);
    nearClip.addListener(this, &ofApp::handleCameraControls);
    farClip.addListener(this, &ofApp::handleCameraControls);

    //lookAtCenter.addListener(this, &ofApp::handleCameraControls);
    
    cameraPanel.setup(cameraParams, "camera.xml", x, y += 120);
   
    soundParams.setName("Sound");
    
    soundParams.add(playSnd.set("playSnd", false));
    playSnd.addListener(this, &ofApp::sndTogglePressed);

    soundParams.add(playFennesz.set("playFennesz", true));
    playFennesz.addListener(this, &ofApp::sndTogglePressed);
    
    soundParams.add(volume.set("volume", 0, 0, 1));
    volume.addListener(this, &ofApp::handleVolume);
    soundPanel.setup(soundParams, "soundSettings.xml", x, y += 180);
  
}

void ofApp::setRingRadius(const void* sender, float &value) {}

void ofApp::handleCameraControls(const void* sender, float &value) {
    cout << "\n value " << value;
    
    //cout << "\n up dir " << camera.getUpDir();
    // camera.rotate(value, camera.getUpDir());
    
    ofParameter<float> s = *((ofParameter<float>*) sender);
    
    ofVec3f myCamGlobalPosition = camera.getPosition();
    ofQuaternion myCamRotation = camera.getGlobalOrientation();
    
    cout << "\n position " << myCamGlobalPosition;
    cout << "\n rotation " << myCamRotation;
    
    if (s == cameraRotate) {
        camera.rotateAround(value, camera.getUpDir(), sceneCenter);
        // camera.lookAt(sceneCenter);
    }
    
    if (s == cameraX) {
        camera.setPosition(cameraX, myCamGlobalPosition.y, myCamGlobalPosition.z);
        // camera.lookAt(sceneCenter);
    }
    
    if (s == cameraY) {
        camera.setPosition(myCamGlobalPosition.x, cameraY, myCamGlobalPosition.z);
        // camera.lookAt(sceneCenter);
    }
    
    if (s == nearClip) {
        camera.setNearClip(nearClip);
    }
    
    if (s == farClip) {
        camera.setFarClip(farClip);
    }
    
    if (s == zoom) {
        camera.setPosition(myCamGlobalPosition.x, myCamGlobalPosition.y, zoom);
    }
    
    // camera.rotate(value, 0, 0, 0);
}

void ofApp::ringAnimButtonPressed(const void* sender) {
    // int debug;
    (ringAnimation < 2) ? ringAnimation++ : ringAnimation = 0;
    if (ringAnimation == tangental) {
        for (int i = 0; i < NUMBER_OF_RINGS; i++) {
            rings[i].resetRadius();
        }
    }
    cout << "\n ring animation type " << ringAnimation;
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
    
    int liss_count = sizeof(lisses)/sizeof(lisses[0]);
    float sectionWidth = ofGetWidth()/liss_count;
    
    for (int l = 0; l < liss_count; l++) {
        
        // init lissajous
        lisses[l].setRadius(20);
        lisses[l].setHeight(2);
        lisses[l].setColor(256, 256, 0, 256);
        lisses[l].setAlive(true);

        float xPos = (-1 * ofGetWidth()/2) + (sectionWidth * l) + (sectionWidth/2 - 20/2);
        float zPos = 0; // abs((l++ % liss_count) - liss_count/2);
        lisses[l].setPosition(xPos, 0, zPos);
        lisses[l].setEndPosition(0., 0., 0);
    }
  
//    liss.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
//    liss.setEndPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
    
   
    
    // init rings
    for (int r = 0; r < sizeof(rings)/sizeof(rings[0]); r++) {
        rings[r] = Ring();
        rings[r].setPosition(0., -300 + r * 20, 0);
        rings[r].ident = r;
    }
}
