#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    ofSetFrameRate(60);

    ofSoundStreamListDevices();
    ofSoundStreamSetup(2, 0, this, 44100, 2048, 4);

    /*
    inputStream.setDeviceID(3);
    inputStream.setup(0, 2, 44100, 512, 4);
    outputStream.setDeviceID(1);
    outputStream.setup(2, 0, 44100, 512, 4);
    */

    pd.init(2, 2, 44100);
    pd.openPatch("osc.pd");
    pd.stop();
}

//--------------------------------------------------------------
void testApp::update() {

}

//--------------------------------------------------------------
void testApp::draw() {

}

//--------------------------------------------------------------
void testApp::exit() {

}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
    pd.start();
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
    pd.stop();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void testApp::audioReceived(float * input, int bufferSize, int nChannels) {
}

//--------------------------------------------------------------
void testApp::audioRequested(float * output, int bufferSize, int nChannels) {
    pd.audioOut(output, bufferSize, nChannels);
}
