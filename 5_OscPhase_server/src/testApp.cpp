#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetLogLevel(OF_LOG_FATAL_ERROR);
    ofSetCircleResolution(32);
	ofBackgroundHex(0x000000);
    
    ofxSuperColliderServer::init();
    
    fx = new ofxSCSynth("fx");
    fx->create();
    
    pitchRatio[0]= 1.0;
    pitchRatio[1]= 9.0/8.0;
    pitchRatio[2]= 5.0/4.0;
    pitchRatio[3]= 4.0/3.0;
    pitchRatio[4]= 3.0/2.0;
    pitchRatio[5]= 5.0/3.0;
    pitchRatio[6]= 15.0/8.0;
	bMousePressed = false;
    
    mode = 1;
    glitchMode = false;
    flickerMode = false;
}

//--------------------------------------------------------------
void testApp::update(){
    for (int i = 0; i < oscils.size(); i++) {
        oscils[i]->update();
    }
    
    for (int i=0; i<rects.size(); i++) {
        rects[i]->update();
    }

    if (glitchMode) {
        glitch->update();
    }
    
    if (flickerMode) {
        flicker->update();
    }
}

//--------------------------------------------------------------
void testApp::draw()
{
    //draw OscWave
    for (int i = 0; i < oscils.size(); i++) {
        oscils[i]->draw();
    }
    
    for (int i=0; i<rects.size(); i++) {
        rects[i]->draw();
    }
    
    if (glitchMode) {
        glitch->draw();
    }
    
    if (flickerMode) {
        flicker->draw();
    }
    
    ofSetLineWidth(1);
    
    // draw OscWave guide & log
    if (mode == 1) {
        ofVec2f currentPos;
        ofSetHexColor(0x666666);
        ofNoFill();
        if (bMousePressed) {
            float dist = drawPos.distance(ofVec2f(mouseX, mouseY));
            if (dist > 100.0) {
                dist = 100.0;
            }
            
            ofCircle(drawPos.x, drawPos.y, dist);
            ofLine(drawPos.x, drawPos.y, mouseX, mouseY);
            currentPos = drawPos;
        } else {
            currentPos = ofVec2f(mouseX, mouseY);
        }
        
        int note  = int(ofMap(currentPos.y, 0, ofGetHeight(), 85, 1));
        float freq = 20 + 20 * pow((13.0/12.0), note);
        float pan = ofMap(currentPos.x, 0, ofGetWidth(), -1.0, 1.0);
        ofLine(0, currentPos.y, ofGetWidth(), currentPos.y);
        ofLine(currentPos.x, 0, currentPos.x, ofGetWidth());

        ofSetHexColor(0xffffff);
        ofDrawBitmapString(ofToString(freq,2), 5, currentPos.y - 10);
        ofDrawBitmapString(ofToString(pan,2), currentPos.x + 5, 10);
        ofDrawBitmapString("nOsc: " + ofToString(oscils.size()), 10, 22);
        ofFill();
    }
    
    // draw MyRect or GlitchRect guide & log
    if (mode == 2 && bMousePressed) {
        ofNoFill();
        ofSetColor(255, 255, 255, 63);
        ofRect(drawPos.x, drawPos.y, mouseX - drawPos.x, mouseY-drawPos.y);
        ofFill();
    }
    
    // draw Glitch guide & log
    if (mode == 3) {
        ofSetHexColor(0xffffff);
        ofDrawBitmapString("amp: " + ofToString(glitch->amp, 4), 10, 22);
    }
    
    // draw Flickr guide & log
    if (mode == 4) {
        ofSetHexColor(0xffffff);
        ofDrawBitmapString("amp: " + ofToString(flicker->level, 4), 10, 22);
    }
    
    // log
    ofSetHexColor(0xffffff);
    ofDrawBitmapString("mode: " + ofToString(mode) + ", frameRate: " + ofToString(ofGetFrameRate()), 10, 10);
}

//--------------------------------------------------------------
void testApp::exit(){
    for (int i = 0; i < oscils.size(); i++) {
        oscils[i]->synth->free();
    }
    if (glitchMode) {
        glitch->synth->free();
    }
    if (flickerMode) {
        flicker->synth->free();
    }

    fx->free();
}


//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    switch (key) {
        case 'd':
            if (mode == 1 && oscils.size()>0) {
                oscils[0]->synth->set("gate", 0);
                oscils.pop_front();
            }
            if (mode == 2 && rects.size() > 0) {
                rects.pop_front();
            }
            if (mode == 3) {
                if (glitchMode) {
                    glitchMode = false;
                    //glitch->synth->free();
                    glitch->synth->set("gate", 0);
                } else {
                    glitchMode = true;
                }
            }
            if (mode == 4) {
                if (flickerMode) {
                    flickerMode = false;
                    flicker->synth->free();
                } else {
                    flickerMode = true;
                }
            }
            break;
            
        case 'f':
            ofToggleFullscreen();
            break;
            
        case '1':
            // OscWave
            mode = 1;
            break;
            
        case '2':
            // Rect Sequencer
            mode = 2;
            break;

        case '3':
            // Glitch
            mode = 3;
            if (!glitchMode) {
                glitch = new GlitchRect(ofPoint(0, 0), ofGetWidth(), ofGetHeight());
                glitchMode = true;
            }
            break;

        case '4':
            // Flicker
            if (!flickerMode) {
                flicker = new Flicker(0.1);
                flickerMode = true;
            }
            mode = 4;
            break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    bMousePressed = true;
    drawPos.x = x;
    drawPos.y = y;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    bMousePressed = false;
    
    if (mode == 1) {
        int note  = int(ofMap(mouseY, 0, ofGetHeight(), 85, 1));
        float freq = 20 + 20 * pow((13.0/12.0), note);
        float pan = ofMap(mouseX, 0, ofGetWidth(), -1.0, 1.0);
        float dist = drawPos.distance(ofVec2f(mouseX, mouseY));
        if (dist > 100.0) {
            dist = 100.0;
        }
        float amp = ofMap(dist, 0.0, 100.0, 0.0, 0.5);
        OscWave *o = new OscWave(oscils.size()+1, freq, pan, ofRandom(0.1, 1.0), amp);
        oscils.push_back(o);
    }
    
    if (mode == 2) {
        int zone = (ofGetHeight() - drawPos.y)/(ofGetHeight()/9);
        float thisFreq = 20 * pow(2.0, zone) * pitchRatio[int(ofRandom(0, 6))];
        if (thisFreq > 18000) {
            thisFreq = 18000;
        }
        MyRect *r = new MyRect(drawPos, mouseX - drawPos.x, mouseY - drawPos.y, ofRandom(1.0, 2.0), thisFreq, ofRandom(-1, 1), (mouseY - drawPos.y)/1000.0);
        rects.push_back(r);
    }

    if (mode == 3) {
        glitch->coin += (mouseX - drawPos.x) / float(ofGetWidth()) * 40.0;
        glitch->amp += (drawPos.y - mouseY) / float(ofGetHeight()) * 0.2;
    }
    
    if (mode == 4) {
        if (!flickerMode) {
            flicker = new Flicker(0.1);
            flickerMode = true;
        } else {
            flicker->freq += (mouseX - drawPos.x) / float(ofGetWidth()) * 0.02;
            flicker->level += (drawPos.y - mouseY) / float(ofGetHeight()) * 0.5;
        }
    }
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}