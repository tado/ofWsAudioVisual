#pragma once

#include "ofMain.h"
#include "ofxSuperCollider.h"
#include "ofxSuperColliderServer.h"
#include "SawSynth.h"

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
    
    ofxSCSynth *fx;
    ofxSCSynth *saw;
    int synthNum;
    deque<SawSynth *> saws;    
};
