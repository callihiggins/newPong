#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxOsc.h"
#include "ofxVectorMath.h"
#include "ofxXmlSettings.h"


#define OF_ADDON_USING_OFXOSC
#define N_SOUNDS 5
// listen on port 12345
//port is 9002 at nyu
#define PORT 9001 
#define NUM_MSG_STRINGS 20
#include <stdio.h>
#include <stdlib.h>

//128.122.151.183 - nyu
//#define HOST "localhost"
//"192.168.130.241"
//"localhost"
//define PORT 12345


class Data {
public:
	int	 soundID;
	bool hit;
    ofColor color;
	int		id;
    int type;
    int lifespan;

};

class NominatedVideo : public ofxBox2dRect {
	
public:
    ofVideoPlayer * movie;
    
    void setupTheCustomData() {
		static int i;
		static int colors[] = {0xcae72b, 0xe63b8f, 0x2bb0e7, 0x00FFFF, 0xFF00FF, 0x99FF66, 0xFFCCFF, 0x9900FF, 0x33CCCC, 0x00FFCC, 0x666633, 0xCC0066};
        // we are using a Data pointer because 
		// box2d needs to have a pointer not 
		// a referance
		setData(new Data());
		Data * theData = (Data*)getData();
        theData->color.setHex(colors[i]);
        theData->id = i;
        theData->soundID = ofRandom(0, N_SOUNDS);
        theData->hit	= false;		
		theData->type= 2;
        theData->lifespan = 255;
        printf("setting the custom data!\n");
		i ++;
        i = i% 13;
	}
	
	void draw() {
        
		Data* theData = (Data*)getData();
		if(theData) {
			
			// Evan though we know the data object lets just 
			// see how we can get the data out from box2d
			// you would use this when using a contact listener
			// or tapping into box2d's solver.
			
			float width = getWidth()*2;
			ofPushMatrix();
            ofSetRectMode(OF_RECTMODE_CENTER);
			ofTranslate(getPosition());
			ofRotateZ(getRotation());
            ofSetColor(255, 255, 255, theData->lifespan);
            movie->draw(0,0,width,width);
			ofDrawBitmapString(ofToString(theData->id), -5, 5);
			ofPopMatrix();
            theData->lifespan -= 5;
		}
    }
    
};




// A Custom Particle extedning the box2d circle
class WinningVideo : public ofxBox2dRect {
	
public:
        ofVideoPlayer * movie;
		
    void setupTheCustomData() {
		static int i;
		static int colors[] = {0xcae72b, 0xe63b8f, 0x2bb0e7, 0x00FFFF, 0xFF00FF, 0x99FF66, 0xFFCCFF, 0x9900FF, 0x33CCCC, 0x00FFCC, 0x666633, 0xCC0066};
				// we are using a Data pointer because 
		// box2d needs to have a pointer not 
		// a referance
		setData(new Data());
		Data * theData = (Data*)getData();
        theData->color.setHex(colors[i]);
        theData->id = i;
        theData->soundID = ofRandom(0, N_SOUNDS);
        theData->hit	= false;		
		theData->type= 2;
				printf("setting the custom data!\n");
		i ++;
        i = i% 13;
	}
	
	void draw() {
        
		Data* theData = (Data*)getData();
		if(theData) {
			
			// Evan though we know the data object lets just 
			// see how we can get the data out from box2d
			// you would use this when using a contact listener
			// or tapping into box2d's solver.
			
			float width = getWidth()*2;
			ofPushMatrix();
            ofSetRectMode(OF_RECTMODE_CENTER);
			ofTranslate(getPosition());
			ofRotateZ(getRotation());
            ofSetColor(255);
            movie->draw(0,0,width,width);
			ofDrawBitmapString(ofToString(theData->id), -5, 5);
			ofPopMatrix();
		}
    }
        
};


// -------------------------------------------------
class testApp : public ofBaseApp {
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void resized(int w, int h);
	void loadSettings(string fileString);
	// this is the function for contacts
	void contactStart(ofxBox2dContactArgs &e);
	void contactEnd(ofxBox2dContactArgs &e);

	// when the ball hits we play this sound
	ofSoundPlayer  sound[N_SOUNDS];
	ofxBox2d						box2d;			  //	the box2d world
	vector		<ofxBox2dCircle>	circles;		  //	default box2d circles
	vector		<ofxBox2dRect>	paddles;
	vector		<WinningVideo>	winningvideos;
    vector		<NominatedVideo>	nomvideos;
    vector		<ofVideoPlayer*>	wvideos;
    vector		<ofVideoPlayer*>	nvideos;
    ofTrueTypeFont	verdana22;
    ofImage vimeologo;
    int score1, score2, counter, joystick1, joystick2, mapped_joystick1, mapped_joystick2 ;
    ofImage user1, user2;
    ofxOscSender sender1, sender2;
    ofxOscReceiver	receiver;
    int port;
    char * host;
    string user;
    bool loaduser, user1load, user2load;
    int whichuser;
    int paddleattraction;
    b2Vec2 pveloc;
    bool fullscreen;
};

