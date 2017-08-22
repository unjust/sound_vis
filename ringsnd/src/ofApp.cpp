#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(255,255,255);
    
    setupGui();
    initSound();
    initAnimObjects();
    
    easyCam.setDistance(900);
    
}

//--------------------------------------------------------------
void ofApp::update(){

    // update the sound playing system:
    ofSoundUpdate();
    
    float* fftData = getFFTData(NUMBER_OF_BANDS);
    
    // update rings
    
    for (int j = 0; j < NUMBER_OF_RINGS; j++){
        //if (RINGS[j]->status()) continue;
        
        float sonicRadius = 0.;
        int BANDS_DATA_PER_ITEM = (NUMBER_OF_BANDS / 2) / NUMBER_OF_RINGS;
        
        // currently (128 / 2) / 28;
        // is it that half the window size is usable? or L and R data interleaved or repeated?
        // then this is sum of energy for a few bins
        
        int fftDataIndexBegin = j * BANDS_DATA_PER_ITEM;
        int fftDataIndexEnd = (j + 1) * BANDS_DATA_PER_ITEM;
        
        for (int i = fftDataIndexBegin; i < fftDataIndexEnd; i++){
            sonicRadius += fftData[i];
        }
        
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
    ringAttrsPanel.draw();
    soundPanel.draw();
    if (showFFT) {
        fftDraw();
    }
    
    easyCam.begin();
    
    ofSetColor(255,0,0);
    ofFill();
    
    ofDrawBox(30);
    
    if (showRings) {
        for (int j = 0; j < NUMBER_OF_RINGS; j++){
            rings[j].draw(.01);
        }
    }
    
    ofDisableAlphaBlending();
    easyCam.end();
}


//------------------------------------------------------------
void ofApp::setupGui(){
    
    float   x = 10,
            y = 10;
    
    ringAttrsParams.setName("Rings");
    ringAttrsParams.add(ringRadius.set("base radius", 10, 0, 300));
    ringAttrsParams.add(ringOrbitRadius.set("orbit", 10, 0, 300));
    ringAttrsParams.add(ringZ.set("z", 0, 0, 1000));
    ringAttrsParams.add(ringBounce.set("bounce", false));
    ringAttrsParams.add(showRings.set("circles", true));
    
    ringRadius.addListener(this, &ofApp::setRingRadius);
    ringOrbitRadius.addListener(this, &ofApp::setRingRadius);
    
    ringAttrsPanel.setup(ringAttrsParams, "ring.xml", x, y += 100);
    ringAttrsPanel.add(ringAnimationButton.setup("anim type"));
    
    ringAnimationButton.addListener(this, &ofApp::ringAnimButtonPressed);
    
    soundParams.setName("Sound");
    
    soundParams.add(volume.set("volume", 0, 0, 1));
    soundParams.add(showFFT.set("draw fft", false));
    volume.addListener(this, &ofApp::handleVolume);
    soundPanel.setup(soundParams, "soundSettings.xml", x, y += 180);
    
}

void ofApp::setRingRadius(const void* sender, float &value) {
    ofParameter<float> s = *((ofParameter<float>*) sender);
    if (s == ringRadius) {
        Ring::setBaseRadius(value);
        
    } else if (s == ringOrbitRadius) {
        Ring::setOrbitRadius(value);
    }
}

void ofApp::ringAnimButtonPressed(const void* sender) {
    int ra = (int)ringAnimation;
    (ra < 2) ? ringAnimation = (ringAnimationType)++ra : ringAnimation = (ringAnimationType)0;
    
    if ((ringAnimationType)ringAnimation == tangental) {
        for (int i = 0; i < NUMBER_OF_RINGS; i++) {
            rings[i].resetRadius();
        }
    }
    cout << "\n ring animation type " << ringAnimation;
}


void ofApp::handleVolume(const void* sender, float &value) {
    noesysAudio.setVolume(value);
}

//--------------------------------------------------------------
void ofApp::initSound(){
    
    playSound();
    
    // the fft needs to be smoothed out, so we create an array of floats
    // for that purpose:
    // fftSmoothed = new float[8192];  // 8192 / 128 = 64... what is the signigicance of 8192
    fftSmoothed = new float[NUMBER_OF_BANDS];
    for (int i = 0; i < NUMBER_OF_BANDS; i++){
        fftSmoothed[i] = 0;
    }
    
}

//----------------------------------------------------------------
void ofApp::audioOut( ofSoundBuffer &outBuffer ) {
    for(int i = 0; i < outBuffer.size(); i += 2) {
        float sample = sin(phase); // generating a sine wave sample
        outBuffer[i] = sample; // writing to the left channel
        outBuffer[i + 1] = sample; // writing to the right channel
        phase += 0.05;
    }
}

//--------------------------------------------------------------
float* ofApp::getFFTData(int numBands) {
    float *val = ofSoundGetSpectrum(numBands); // request 128 for FFT

    for (int i = 0; i < numBands; i++) {
        // let the smoothed value sink to zero:
        fftSmoothed[i] *= 0.96f;
        
        // take the max, either the smoothed or the incoming:
        if (fftSmoothed[i] < val[i]) {
            fftSmoothed[i] = val[i];
        }
    }
    return fftSmoothed;
}

//----------------------------------------------------------------
void ofApp::playSound() {
    string filename = "sounds/snd.aiff" ;
    noesysAudio.load(filename);
    noesysAudio.play();
    
    // for debugging
    noesysAudio.setVolume(volume);
}

//--------------------------------------------------------------
void ofApp::fftDraw(){
    ofSetColor(0,255,255,80);
    
    float bandwidth = ofGetWidth()/NUMBER_OF_BANDS;
    for (int i = 0; i < NUMBER_OF_BANDS; i++) {
        ofDrawRectangle(100 + bandwidth * i, ofGetHeight()/2, bandwidth, -(fftSmoothed[i]*200));
    }
}

//--------------------------------------------------------------
void ofApp::initAnimObjects(){

    // init rings
    for (int r = 0; r < NUMBER_OF_RINGS; r++) {
        rings[r] = Ring();
        rings[r].setPosition(0., -300 + r * 20, 0);
        rings[r].ident = r;
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    posX = x;
    posY = y;
    
    ofPoint pt;
    pt.set(x,y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){}
