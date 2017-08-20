#include "ofApp.h"

//for ring cam

//double camTransitionTime = .3;
//double nextSpotWillHappenAt = 0;
//double startX = 0, startY = 0, startZ = 0;
//double endX = 0, endY = 0, endZ = 0;

////////from ringsApp
/*
 double camTransitionTime = 10.;
 double nextSpotWillHappenAt = 20.;
 
 double rotateAngleX = 0, rotateAngleY = 0, rotateAngleZ = 0;
 double startRotateX = 0, startRotateY = 0, startRotateZ = 0;
 double endRotateX = 0, endRotateY = 0, endRotateZ = 0;
 double startTranslateX = 0, startTranslateY = 0, startTranslateZ = 0;
 double endTranslateX = 0, endTranslateY = 0, endTranslateZ = 0;
 
 bool move = false;
 bool first = true;
 
 
 //what to show
 bool analysisView = false;
 bool rings = false;
 bool lines = true;
 bool plane = false;
 bool fullScreen = true;
 bool loopRestart = false;
 
 
 bool allLissOn = false;
 int ringSwitch = 0;
 int lineSwitch = 2;
 float coils = 1.;
 float lradius = .4;
 
 
 int lastWindow = 0;
 int windowsPassed = 0;
 int ticker = 0;
 int dir = 0;
 float beatCount = 0.;
 float lastSimTime = 0.;
 const float simTickLength = .0001;
 */

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0,0,0);
    
    setupGui();
    initSound();
    
    initAnimObjects();
    
    easyCam.setDistance(900);
    
}

//--------------------------------------------------------------
void ofApp::update(){

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

    // update rings
    int WINDOW_SIZE = nBandsToGet; // temporary, TODO write an extensice comment here on window size
    
    // and the smoothed code blow
    
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

    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // gui
    // cameraPanel.draw();
    ringAttrsPanel.draw();
    soundPanel.draw();
    
    easyCam.begin();
    
    ofTranslate(-ofGetWidth()/2, 0, 0);
    
    ofSetColor(255,0,0);
    ofFill();
    
    ofDrawBox(30);

    
    if (showRings) {
        for (int j = 0; j < NUMBER_OF_RINGS; j++){
            // rings[j].draw(1, 10.);
            rings[j].draw(.01);
        }
    }
    
    ofDisableAlphaBlending();
    easyCam.end();
    

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
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){}

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
    
        ringAttrsParams.setName("Rings");
    ringAttrsParams.add(ringRadius.set("fradius", 10, 0, 300));
    ringAttrsParams.add(ringZ.set("z", 0, 0, 1000));
    ringAttrsParams.add(ringBounce.set("bounce", false));
    ringAttrsParams.add(showRings.set("circles", false));
    
    //ringRadius.addListener(this, &ofApp::setRingRadius);
    
    
    ringAttrsPanel.setup(ringAttrsParams, "ring.xml", x, y += 100);
    ringAttrsPanel.add(ringAnimationButton.setup("anim type"));
    
    ringAnimationButton.addListener(this, &ofApp::ringAnimButtonPressed);
    
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

    float sectionWidth = ofGetWidth()/2 / floor(NUMBER_OF_LISS/2);
   
    // init rings
    for (int r = 0; r < sizeof(rings)/sizeof(rings[0]); r++) {
        rings[r] = Ring();
        rings[r].setPosition(0., -300 + r * 20, 0);
        rings[r].ident = r;
    }
}
