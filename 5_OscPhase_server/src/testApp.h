#pragma once

#include "ofMain.h"
#include "ofxSuperColliderServer.h"
#include "OscWave.h"
#include "MyRect.h"
#include "GlitchRect.h"
#include "Flicker.h"

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    int mode;
    ofxSCSynth *fx;
    deque<OscWave *> oscils;
    
    bool bMousePressed;
    ofVec2f drawPos;
    float pitchRatio[7];
    deque<MyRect *> rects;
    
    bool flickerMode;
    Flicker * flicker;
    
    bool glitchMode;
    GlitchRect * glitch;
};
