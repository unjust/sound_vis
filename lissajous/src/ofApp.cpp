#include "ofApp.h"

//global transforms + camera
float translateX = 0, translateY = 0, translateZ = -2.;
float translateAX = 0, translateAY = 0, translateAZ = -2.;
float scaleX = 1, scaleY = 1, scaleZ = 1.;
float rotateAngle = 0, rotateX = 0, rotateY = 0, rotateZ = 0.;

float xRight = .9;
float xLeft = xRight * -1.;
float yTop = .9;
float yBottom = yTop * -1.;
float xFactor = 0.;

bool allLissOn = false;

int silence = 0;


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0,0,0);
    
    setupGui();
    initSound();
    
    initAnimObjects();
    
    sceneCenter = ofVec3f(ofGetWidth()/2, ofGetHeight()/2, 0);
    camera.setupPerspective();
    camera.setFov(60);
    camera.setNearClip(nearClip);
    camera.setFarClip(farClip);
    
    easyCam.setDistance(900);
    
    LISS_SELECTOR = 0;
    
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
    
    updateLissajous(fftSmoothed);
    
    
    // update rings
    int WINDOW_SIZE = nBandsToGet; // temporary, TODO write an extensice comment here on window size
    
    // and the smoothed code blow

    // cout << "\n position " << camera.getPosition();
    
    // camera.pan(cameraRotate);
    
    /*
     if (lookAtCenter) {
     easyCam.lookAt(sceneCenter);
     camera.lookAt(sceneCenter);
     }
     */
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // gui
    // cameraPanel.draw();
    lissajousAttrsPanel.draw(); 
    soundPanel.draw();
    
    easyCam.begin();
    
    ofTranslate(-ofGetWidth()/2, 0, 0);
    
    ofSetColor(255,0,0);
    ofFill();
    
    ofDrawBox(30);
    
    if (showLissajous) {
        for (int l = 0; l < sizeof(lisses)/sizeof(lisses[0]); l++){
            lisses[l].draw();
        }
    }
    
    
    ofDisableAlphaBlending();
    easyCam.end();
    
    // draw fft
    if (showDebug) {
        line.draw();
        fftDraw();
    }
}

void ofApp::updateLissajous(float *soundData) {
    
    // set size of them
    
    for (int l = 0; l < sizeof(lisses)/sizeof(lisses[0]); l++) {
        lisses[l].setRadius(lissRadius + (fftSmoothed[10] * 300));
        lisses[l].setHeight(lissHeight);
        lisses[l].setPoints(points);
    }
    
    // reduce opacity fn
    
    // int a = (lisses[LISS_SELECTOR]).getAlpha();
    // why didnt this work???
    // Lissajous selected = lisses[LISS_SELECTOR];
    
    //    if (fadingIn) {
    //        for (int l = 0; l < sizeof(lisses)/sizeof(lisses[0]); l++) {
    //            int a = lisses[l].getAlpha();
    //            printf("Number %d alpha is %d \n", l, a);
    //
    //            lisses[l].setAlpha(a+1);
    //
    //            printf("Number %d alpha is NOW %d \n", l, lisses[l].getAlpha());
    //
    //
    //            // all are faded up completely
    //            if (l == NUMBER_OF_LISS - 1 && a == 256) {
    //                fadingIn = false;
    //            }
    //        }
    //    }
    //    } else {
    //        if (a < 0) {
    //            LISS_SELECTOR++;
    //        } else {
    //            int newAlpha = a - 1;
    //            (lisses[LISS_SELECTOR]).setAlpha(newAlpha);
    //        }
    //    }
    
    
    //for Lissajous motion
    
    float totalMagnitude = 0.;
    int WINDOW_SIZE = nBandsToGet; // temporary, TODO write an extensice comment here on window size
    
    for (int i = 0; i < WINDOW_SIZE/2; i++) {
        totalMagnitude += soundData[i];
        // totalMagnitude += theWindows[currentWindow].magnitudeL[i];
    }
  
    //totalMagnitude /= (WINDOW_SIZE/2);
    int ok = (int)((WINDOW_SIZE/2.)/NUMBER_OF_LISS);
    bool selectedLiss = false;
    int theLeader = 0;
    
    //go through Liss and select a Leader | a new Liss must be selected if the previous dies
    for (int k = 0; k < NUMBER_OF_LISS; k++) {
        float subMagnitude = 0.;
        
        if (!lisses[k].getAlive()) {
            continue; //if its dead carry on
        }
        
        int coils = k + 3;
        
        for (int y = k * ok; y < (k + 1) * ok; y++) {
            //printf("y = %d\n", y);
            // subMagnitude += theWindows[currentWindow].magnitudeL[k];
            subMagnitude += soundData[k];
        }
        
        //subMagnitude /= ok;
        if (!selectedLiss) {
            lisses[k].selected = true;
            lisses[k].setPosition(0., 0., .4);
            //lissajousPosition(float ixCenter, float iyCenter, float izCenter);
            selectedLiss = true;
        }
        
        float xCenterPos = lisses[k].x;
        if (lisses[k].selected == true && (xCenterPos < 0.01 && xCenterPos > -0.01)) {
            lisses[k].coil((totalMagnitude * .3) + .1, (totalMagnitude + coils) * 1.5);
            theLeader = k;
        }
        else {
            lisses[k].coil(.1, subMagnitude/2 + coils);
            
        }
        if (!lisses[k].selected && theLeader != 0 && k > 2 && (theLeader - k) % 2 == 0) {
            float newXPosition = 0., newYPosition = 0., newZPosition = 0.;
            newZPosition = lisses[k - 1].z;
            newYPosition = lisses[k - 1].y;
            if (k % 2 == 0) {
                newXPosition = (lisses[k - 2].x) -.45 - ( 1.25 * (xRight - xLeft)/NUMBER_OF_LISS);
            }
            else  newXPosition = (lisses[k - 2].x) + .45 + ( 1.25  * (xRight - xLeft)/NUMBER_OF_LISS);
            lisses[k].setPosition(newXPosition, newYPosition, newZPosition);
        }
        //LISS[k]->lissajousCoiling((totalMagnitude * .2) + .1, subMagnitude/2 + coils);
        //lissajousCoiling(float ixRadius, float icoils)
    }
    
    //when the first Lissajous is ready and together, turn the rest on
    if (lisses[0].getAlive() && lisses[0].x < .5 && allLissOn == false) {
        
        for (int i = 1; i < NUMBER_OF_LISS; i++)  {
            lisses[i].setAlive(true);
        }
        allLissOn = true;
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
    

    cameraParams.setName("Camera");
    
    cameraParams.add(nearClip.set("near", .1, .1, 3000));
    cameraParams.add(farClip.set("far", 700, 1, 3000));
    
    cameraParams.add(cameraX.set("x", ofGetWidth()/2, 1, 3000));
    cameraParams.add(cameraY.set("y", ofGetHeight()/2, 1, 3000));
    cameraParams.add(zoom.set("zoom", 665., 0, 1000.0));
    cameraParams.add(cameraRotate.set("rotate", 0, 0, 360));
    cameraParams.add(lookAtCenter.set("point", true));
    cameraParams.add(resetCamera.set("reset cam", true));
    cameraParams.add(showDebug.set("debug", false));
    
    cameraX.addListener(this, &ofApp::handleCameraControls);
    cameraY.addListener(this, &ofApp::handleCameraControls);
    cameraRotate.addListener(this, &ofApp::handleCameraControls);
    zoom.addListener(this, &ofApp::handleCameraControls);
    nearClip.addListener(this, &ofApp::handleCameraControls);
    farClip.addListener(this, &ofApp::handleCameraControls);
    resetCamera.addListener(this, &ofApp::handleCameraReset);
    
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

void ofApp::handleCameraReset(const void* sender, bool &value) {
    
    if (value) {
        camera.lookAt(sceneCenter);
    }
}

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
        
    }
    
    if (s == resetCamera) {
        camera.lookAt(sceneCenter);
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
    
    // int liss_count = sizeof(lisses)/sizeof(lisses[0]);
    
    float sectionWidth = ofGetWidth()/2 / floor(NUMBER_OF_LISS/2);
    
    fadingIn = true;
    
    for (int l = 0; l < NUMBER_OF_LISS; l++) {
        
        // init lissajous
        lisses[l].setRadius(20);
        lisses[l].setHeight(2);
        
        if (l == 0) {
            lisses[l].setColor(256, 256, 256, 0);
        } else if (l == 3) {
            lisses[l].setColor(0, 256, 256, 0);
        } else if (l == NUMBER_OF_LISS -1) {
            lisses[l].setColor(0, 256, 0, 0);
        } else {
            lisses[l].setColor(256, 256, 0, 0);
        }
        
        
        lisses[l].setAlpha(256);
        lisses[l].setAlive(true);
        
        
        
        float xPos = (sectionWidth * l);
        float zPos = -100 * (abs(l - floor(NUMBER_OF_LISS/2)));
        
        lisses[l].setPosition(xPos, 0, 0);
        //lisses[l].setEndPosition(0., 0., 0);
    }
 
}
