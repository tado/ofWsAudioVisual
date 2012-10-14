#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackgroundHex(0x000000);
	ofSetCircleResolution(32);
	ofSetLogLevel(OF_LOG_NOTICE);
	
	box2d.init();
	box2d.setGravity(0, 10.0);
	box2d.setFPS(30.0);
	box2d.createBounds();
	box2d.registerGrabbing();
	
	// register the listener so that we get the events
	ofAddListener(box2d.contactStartEvents, this, &testApp::contactStart);
	ofAddListener(box2d.contactEndEvents, this, &testApp::contactEnd);
	
	reverb = new ofxSCSynth("fx");
    reverb->create();
	
	mouseDowned = false;
}

void testApp::exit() {
	reverb->free();
}

void testApp::contactStart(ofxBox2dContactArgs &e) {
	if(e.a != NULL && e.b != NULL) { 
		float pan = (e.b->GetBody()->GetPosition().x * OFX_BOX2D_SCALE / ofGetWidth() * 2.0 - 1.0);
		float mass2 = e.b->GetBody()->GetMass();
		float freq = log(12.0/mass2)*2000.0;
		if(freq < 120){
			freq = 120;
		}
		float decay = mass2*0.2;
		if (decay>1.0) {
			decay = 1.0;
		}
		float amp = 0.1;
		
		perc = new ofxSCSynth("perc");
		perc->set("amp", amp);
		perc->set("freq", freq);
		perc->set("decay", decay);
		perc->set("pan", pan);
		perc->create();
	}
}

//--------------------------------------------------------------
void testApp::contactEnd(ofxBox2dContactArgs &e) {

}



//--------------------------------------------------------------
void testApp::update() {
	
	box2d.update();	

}


//--------------------------------------------------------------
void testApp::draw() {
	//circlesに格納された全ての図形を描画
    for(int i=0; i<circles.size(); i++) {
		ofSetHexColor(0x3399ff);
		ofFill();
		circles[i]->draw();
    }
	
	//円を描く際のガイド表示
    if (mouseDowned) {
        ofNoFill();
        ofSetLineWidth(1);
        ofSetHexColor(0xcc0000);
        ofCircle(mouseDownLoc.x, mouseDownLoc.y, ofDist(mouseDownLoc.x, mouseDownLoc.y, mouseX, mouseY));
        //ofLine(mouseX, mouseY, mouseDownLoc.x, mouseDownLoc.y);
    }
	
	string info = "";
	info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
	info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 30, 30);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	if(key == 'f') ofToggleFullscreen();
	if (key == 'd') {
		if (circles.size() > 0) {
			circles[0]->destroy();
			circles.pop_front();
		}
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
    mouseDownLoc.set(x, y);
    mouseDowned = true;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
	//マウスをクリックすると新規にパーティクルを追加
	ofVec2f mouse (x, y);
    float r = mouseDownLoc.distance(mouse);

    if (r<4.0) {
        r = 4.0+ofRandom(0.0, 12.0);
    }

    ofxBox2dCircle *c =new ofxBox2dCircle();
    c->setPhysics(1.0, 0.7, 0.5); //物理パラメータを設定
    c->setup(box2d.getWorld(), mouseDownLoc.x, mouseDownLoc.y, r); //マウスの位置に円を設定
    circles.push_back(c); //生成した円をcirclesベクターに追加
    mouseDowned = false;
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
}

