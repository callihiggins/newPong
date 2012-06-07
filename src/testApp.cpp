#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);
    ofTrueTypeFont::setGlobalDpi(72);
    ofSetFrameRate(60);
    ofHideCursor();
    loadSettings("settings.xml");
	receiver.setup( port );
    sender.setup( sendhost, sendport );
    whichuser = 0;
    loaduser = false;
    senduser1 = false;
    senduser2 = false;
    drawusers = false;
    user1load = false;
    user2load = false;
    instructions = false;
    instructionsmove = 1000;
    countdownnum = 400;
    countdownnumbool = false;
    players = false;
    newballcount = 0;
    alpha = 125;
    alphaincrement = 1;
    visitor82.loadFont("visitor1.ttf", 82, true, true);
	visitor82.setLineHeight(18.0f);
	visitor82.setLetterSpacing(1.037);
    visitor42.loadFont("visitor1.ttf", 38, true, true);
	visitor42.setLineHeight(18.0f);
	visitor42.setLetterSpacing(1.037);
    visitor150.loadFont("visitor1.ttf", 150, true, true);
	visitor150.setLineHeight(18.0f);
	visitor150.setLetterSpacing(1.037);
    visitor200.loadFont("visitor1.ttf", 200, true, true);
	visitor200.setLineHeight(18.0f);
	visitor200.setLetterSpacing(1.037);
    ofEnableAlphaBlending();
	box2d.init();
	box2d.setGravity(0, 10);
    bounds.set(773, 77, 1830, 925);
    box2d.createBounds(bounds);
	//box2d.createGround();
	box2d.setFPS(30.0);
    startScreen = true;
    startGameBool = false;
    score1 = 0;
    score2 = 0;
    counter = 0;
	// register the listener so that we get the events
	ofAddListener(box2d.contactStartEvents, this, &testApp::contactStart);
	ofAddListener(box2d.contactEndEvents, this, &testApp::contactEnd);
    
    vimeologo.loadImage("images/vimeo_logo.png");
    backgroundimg.loadImage("images/vimearcadeL.png");
    instructions1.loadImage("images/instructions1.png");
    instructions2.loadImage("images/instructions2.png");
    instructions3.loadImage("images/instructions3.png");
    
    // load the 8 sfx soundfile
	for (int i=0; i<6; i++) {
		sound[i].loadSound("sfx/" +ofToString(i) + ".wav");
		sound[i].setMultiPlay(true);
		sound[i].setLoop(false);
	}

    logo.loadImage("images/logo.png");
    ofxBox2dRect paddle1;
    paddle1.setPhysics(0.1, 1.0, 0.0);
    paddle1.setup(box2d.getWorld(), bounds.x + 40, bounds.y, 20, bounds.height/8, b2_kinematicBody);
    paddle1.setData(new Data());
    //  paddle1.body->SetUserData(paddle1);
    Data * sd1 = (Data*)paddle1.getData();
    sd1->hit	= false;		
    sd1->type = 1;
    paddles.push_back(paddle1);	
    
    ofxBox2dRect paddle2;
    paddle2.setPhysics(0.1, 1.0, 0.0);
    paddle2.setup(box2d.getWorld(), bounds.x + bounds.width - 30, bounds.y, 20, bounds.height/8, b2_kinematicBody);
    //   paddle2.body->SetUserData(paddle2);
    paddle2.setData(new Data());
    Data * sd2 = (Data*)paddle2.getData();
    sd2->hit	= false;	
	sd2->type = 1;
    paddles.push_back(paddle2);	
    
}


//--------------------------------------------------------------
void testApp::contactStart(ofxBox2dContactArgs &e) {
	if(e.a != NULL && e.b != NULL) { 
		
		// if we collide with the ground we do not
		// want to play a sound. this is how you do that
		if((e.a->GetType() == b2Shape::e_circle && e.b->GetType() == b2Shape::e_polygon) || (e.a->GetType() == b2Shape::e_polygon && e.b->GetType() == b2Shape::e_circle )) {
			
			Data * aData = (Data*)e.a->GetBody()->GetUserData();
			Data * bData = (Data*)e.b->GetBody()->GetUserData();
            
            if(aData){
                if((aData->type == 0 && bData->type == 1)|| (bData->type == 0 && aData->type == 1)) {
                    sound[1].play();
                    //apply a force back at the ball when it hits a paddle
                    b2Vec2 veloc = e.b->GetBody()->GetLinearVelocity();
                    veloc.operator*=(1.5);
                    veloc.operator-();
                    e.b->GetBody()->SetLinearVelocity(veloc);
                }
                
                if(aData->type == 0 && bData->type == 2){
                    aData->hit = true;
                    bData->hit = true;
                    printf("collision!");
                }
                
                if(bData->type == 0 && aData->type == 2){
                    aData->hit = true;  
                    bData->hit = true;
                    printf("collision!");
                    
                    
                }
            }
            
 		}
	}
}

//--------------------------------------------------------------
void testApp::contactEnd(ofxBox2dContactArgs &e) {
}



//--------------------------------------------------------------
void testApp::update() {
	box2d.update();
    
    //OSC STUFF
    while( receiver.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage( &m );
		
		if ( m.getAddress() == "/joysticks" )
		{
			// both the arguments are int32's
			joystick1 = m.getArgAsInt32( 0 );
            joystick2 = m.getArgAsInt32( 1 );
        }
        if ( m.getAddress() == "/user" && players && !startScreen)
		{
            user = m.getArgAsString( 0 );
            username =  m.getArgAsString( 1 );
            printf("got a user!");
            loaduser = true;
            sound[0].play();
        }
        
        if ( m.getAddress() == "/leftshoot")
		{
            if(!startScreen && players){
                sound[0].play();
                user = "images/guest.jpg";
                username = "Guest";
                loaduser = true;
            }
            if(startScreen){
                startScreen = false;
                instructions = true;
                break;
            }
            if(instructions){
                instructions = false; 
                players = true;
                instructionsmove = 1000;
            }
            
        }
        if ( m.getAddress() == "/rightshoot") {
            if(!startScreen && players){
                sound[0].play();
                user = "images/guest.jpg";
                username = "Guest";
                loaduser = true;
            }
            if(startScreen){
                startScreen = false;
                instructions = true;
                break;
            }
            if(instructions){
                instructions = false; 
                players = true;
                instructionsmove = 1000;
            }
            
        }
        
    }
    if(loaduser && players && whichuser == 0){
        user1photo = user;
        user1.loadImage(user);
        username1 = username;
        whichuser = 1;
        user1load = true;
        loaduser = false;

          
    }
    
    if(loaduser && players && whichuser == 1){
        user2photo = user;
        user2.loadImage(user);
        username2 = username;
        whichuser = 0;
        user2load = true;
        loaduser = false;
          }
   
    if(senduser1){
        ofxOscMessage m;
        m.setAddress( "/user" );
        m.addStringArg( user1photo );
        sender.sendMessage( m );
        senduser1 = false;            
    }
    if(senduser2){
        ofxOscMessage m;
        m.setAddress( "/user" );
        m.addStringArg( user2photo );
        sender.sendMessage( m );
        senduser2 = false;            
    }

    
    alpha  = alpha + alphaincrement;
    if(alpha > 200 || alpha < 100){
        alphaincrement = alphaincrement * -1;
    }
    
    if(countdownnumbool){
        printf("counting down: %d \n", countdownnum);
        countdownnum = countdownnum -1;
    }
    
    
    
    
    if(startGameBool){
        
        for(int i=0; i<wvideos.size(); i++){ 
            wvideos[i]->update();
        }
        
        for(int i=0; i<nvideos.size(); i++){ 
            nvideos[i]->update();
        }
        
        //MOVE THE PADDLES
        mapped_joystick1 = int(ofMap(joystick1, 0, 360, bounds.y + 77 + paddles[0].getHeight()/2 , bounds.y+ bounds.height - 77 - paddles[0].getHeight()/2)); 
        mapped_joystick2 = int(ofMap(joystick2, 0, 360, bounds.y + 77 + paddles[1].getHeight()/2, bounds.y+ bounds.height - 77 - paddles[1].getHeight()/2));
        
        b2Vec2 pos1 =  paddles[0].body->GetPosition();
        b2Vec2 target1 = b2Vec2(pos1.x, mapped_joystick1/OFX_BOX2D_SCALE);
        //     b2Vec2 target1 = b2Vec2(pos1.x, mouseY/OFX_BOX2D_SCALE);
        
        target1.y = ofClamp(target1.y, bounds.y - paddles[0].getHeight(), bounds.y + bounds.height - paddles[0].getHeight());
        
        b2Vec2 diff1 = b2Vec2(target1.x-pos1.x,target1.y-pos1.y);
        diff1.operator*=(1.5);
        paddles[0].body->SetLinearVelocity(diff1);
        
        b2Vec2 pos2 =  paddles[1].body->GetPosition();
        //   b2Vec2 target2 = b2Vec2(pos2.x, mouseY/OFX_BOX2D_SCALE);
        b2Vec2 target2 = b2Vec2(pos2.x, mapped_joystick2/OFX_BOX2D_SCALE);
        target2.y = ofClamp(target2.y, bounds.y - paddles[1].getHeight(), bounds.y + bounds.height - paddles[1].getHeight());

        b2Vec2 diff2 = b2Vec2(target2.x-pos2.x,target2.y-pos2.y);
        diff2.operator*=(1.5);
        paddles[1].body->SetLinearVelocity(diff2);
        
        
        //FOR WHEN THE BALL GETS TOO FAST OR SLOW
        for(int i=0; i<circles.size(); i++) {
            int maxSpeed = 50;
            int minSpeed = 10;
            b2Vec2 velocity = circles[i].body->GetLinearVelocity();
            float32 speed = velocity.Length();
            if (speed > maxSpeed) {
                circles[i].body->SetLinearDamping(0.5);
            } else if (speed < maxSpeed) {
                circles[i].body->SetLinearDamping(0.0);
            }
            if (speed < 10){
                b2Vec2 veloc = circles[i].body->GetLinearVelocity();
                veloc.operator*=(1.5);
                circles[i].body->SetLinearVelocity(veloc);
            }
        }
        for(int i=0; i<circles.size(); i++) {
            b2Vec2 p = circles[i].body->GetLocalCenter();
            //KEEP THE BALL GOING UP
            circles[i].body->ApplyForce(b2Vec2( 0, -3 ), p);
            //IF THE CIRCLE GOES OFFSCREEN, UP THE PLAYER SCORE 
            if(circles[i].getPosition().x > bounds.x + bounds.width - 40){
                score1 ++;
                sound[5].play();
                box2d.getWorld()->DestroyBody(circles[i].body);
                circles.erase(circles.begin() + i);
                if (score1 < 5 && score2 < 5){
                    newballbool = true;
                }
            }
            if(circles[i].getPosition().x < bounds.x + 40){
                score2 ++;
                sound[5].play();
                box2d.getWorld()->DestroyBody(circles[i].body);
                circles.erase(circles.begin() + i);
                if (score1 < 5 && score2 < 5){
                    newballbool = true;
                }
            }
        }
        if(newballbool){
            newballcount++;
            if (newballcount > 100){
                newBall();
                newballbool = false;
                newballcount = 0;
            }
            
        }
        for(int i=0; i<winningvideos.size(); i++) {
            Data * theData = (Data*)winningvideos[i].getData();
            int movieid = theData->id;
            if(theData->hit == true){
                sound[2].play();
                for(int j=0; j< 4; j++){
                    ofxThreadedVideo * vid = new ofxThreadedVideo();
                    vid->loadMovie("movies/finalist" + ofToString(i) + ofToString(j) +".mov");
                    ofAddListener(vid->threadedVideoEvent, this, &testApp::threadedVideoEvent);
                    vid->play();
                    nvideos.push_back(vid);
                    NominatedVideo v;
                    v.setPhysics(1.0, 0.0, 0.5);
                    v.setup(box2d.getWorld(), winningvideos[i].getPosition().x, winningvideos[i].getPosition().y, 30, 30, b2_dynamicBody);            
                    v.setupTheCustomData();
                    v.setVelocity(int(ofRandom(0, 5)), int(ofRandom(5, 20)));
                    v.movie = vid;
                    nomvideos.push_back(v);
                }
                
                
                
                //delete winning videos after they've been hit 
                theData->hit = false;  
                winningvideos[i].movie->stop();
                delete winningvideos[i].movie;
                box2d.getWorld()->DestroyBody(winningvideos[i].body);
                wvideos.erase(wvideos.begin()+i);
                winningvideos.erase(winningvideos.begin()+i);   //HOW TO DELETE FROM VECTOR LIST?
                
            }
        }
        
        //delete nominated videos after their lifespan ends
        for(int i=0; i<nomvideos.size(); i++) {
            Data * theData = (Data*)nomvideos[i].getData();
            if(theData->lifespan < 0){
                box2d.world->DestroyBody(nomvideos[i].body);
                nomvideos[i].movie->stop();
                delete nomvideos[i].movie;
                nvideos.erase(nvideos.begin()+i);
                nomvideos.erase(nomvideos.begin()+i);
            }
        }
        
        //GAME OVER- RESET
        
        if (counter == 199) {
            score1 = 0;
            score2 = 0;
            counter = 0;
            
            for(int i=0; i<nomvideos.size(); i++) {
                box2d.world->DestroyBody(nomvideos[i].body);
                delete nomvideos[i].movie;
            }  
            for(int i=0; i<winningvideos.size(); i++) {
                box2d.world->DestroyBody(winningvideos[i].body);
                delete winningvideos[i].movie;
                
            }  
            nomvideos.clear();
            winningvideos.clear();
            wvideos.clear();
            nvideos.clear();
            
            vector <ofxBox2dCircle>::iterator iter3 = circles.begin();  
            while (iter3 != circles.end()) {  
                iter3->draw();  
                box2d.world->DestroyBody(iter3->body);  
                iter3 = circles.erase(iter3);  
            }  
            startGameBool = false;
            startScreen = true;    
            drawusers = false;
        }   
    }
}

//--------------------------------------------------------------
void testApp::draw() {
    ofBackground(0); 
    ofSetColor(255);
    ofSetRectMode(OF_RECTMODE_CORNER);
    backgroundimg.draw(0,0, ofGetWidth(), ofGetHeight());
    ofSetColor(0, 173, 238, 125);
    
    if(startScreen){
        ofSetColor(255);
        ofSetRectMode(OF_RECTMODE_CENTER);
        logo.draw(bounds.x + bounds.width/2, bounds.y + bounds.height/2);
        ofSetColor(236, 28, 36);
        
        printf("startscreen \n");
    }
    
    if(instructions){
        instructionsmove--;
        ofSetColor(255);
        if (instructionsmove > 800) {
            ofSetRectMode(OF_RECTMODE_CENTER);
            instructions1.draw(bounds.x + bounds.width/2, bounds.height/2 + 100);
        }
        if (instructionsmove < 800 && instructionsmove > 400) {
            ofSetRectMode(OF_RECTMODE_CENTER);
            instructions2.draw(bounds.x + bounds.width/2, bounds.height/2 + 100);
        }
        if (instructionsmove < 400 && instructionsmove > 100) {
            ofSetRectMode(OF_RECTMODE_CENTER);
            instructions3.draw(bounds.x + bounds.width/2, bounds.height/2 + 100);
        }
        if (instructionsmove < 100 ) {
            players = true; 
            instructions = false;
            instructionsmove = 1000;
        }
    }
    
    
    if(!user1load && players && !startScreen){
        //    printf("alpha: %d: \n", alpha);
        ofSetColor(0, 173, 238, alpha);
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofRect(bounds.x,bounds.y, bounds.width/2, bounds.height);
        ofSetColor(255);
        visitor82.drawString("Left Player", bounds.x + bounds.width/4 - visitor82.stringWidth("Left Player")/2, bounds.y + bounds.height/2 - 150);
        visitor82.drawString("Tap Your Pass", bounds.x + bounds.width/4 - visitor82.stringWidth("Tap Your Pass!")/2, bounds.y + bounds.height/2 -75);
        
        visitor42.drawString("Or press the button to play as a guest", bounds.x + 30, bounds.y + bounds.height/2 + 50);
    }
    
    if(user1load && !user2load && players){
        ofSetColor(0, 173, 238, alpha);
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofRect(bounds.x + bounds.width/2, bounds.y, bounds.width/2, bounds.height);
        ofSetColor(255);
        user1.draw(bounds.x + bounds.width/4 - 50, bounds.height/2, 100, 100);
        visitor82.drawString(username1, bounds.x + bounds.width/4 - visitor82.stringWidth(username1)/2, bounds.y + bounds.height/2 + 120);
        ofSetColor(255);
        visitor82.drawString("Right Player", bounds.x + bounds.width/2 + bounds.width/4 - visitor82.stringWidth("Right Player")/2, bounds.y + bounds.height/2 - 150);
        visitor82.drawString("Tap Your Pass", bounds.x + bounds.width/2 + bounds.width/4 - visitor82.stringWidth("Tap Your Pass")/2, bounds.y + bounds.height/2 - 75);
        visitor42.drawString("Or press the button to play as a guest", bounds.x + 30 + bounds.width/2, bounds.y + bounds.height/2 + 50);
        
    }
    
    
       
    if(user1load &&  user2load){
        countdownnumbool = true;
        ofSetColor(255);
        user1.draw(bounds.x + bounds.width/4 - 50, bounds.height/2, 100, 100);
        user2.draw(bounds.x + bounds.width/4 - 50 + bounds.width/2, bounds.height/2, 100, 100);
        visitor82.drawString(username1, bounds.x + bounds.width/4 - visitor82.stringWidth(username1)/2, bounds.y + bounds.height/2 + 120);
        visitor82.drawString(username2, bounds.x + bounds.width/4 - visitor82.stringWidth(username2)/2 + bounds.width/2, bounds.y + bounds.height/2 + 120);
        if(countdownnum == 375)
            sound[4].play();
        if(countdownnum == 300)
            sound[4].play();
        if(countdownnum > 300){
            visitor200.drawString("3", bounds.x + 10 + bounds.width/2- visitor200.stringWidth("3")/2, bounds.y + bounds.height/2 + visitor200.stringHeight("3")/2 );
        }
        if(countdownnum == 200)
            sound[4].play();
        if(countdownnum > 200 &&countdownnum < 300 ){
            visitor200.drawString("2", bounds.x + 10 + bounds.width/2 - visitor200.stringWidth("2")/2, bounds.y + bounds.height/2 + visitor200.stringHeight("2")/2 );
        }
        if(countdownnum == 100)
            sound[4].play();
        if(countdownnum > 100 && countdownnum < 200){
            visitor200.drawString("1", bounds.x + + 25 + bounds.width/2 - visitor200.stringWidth("1!")/2, bounds.y + bounds.height/2 + visitor200.stringHeight("1")/2 );
        }
        if(countdownnum > 0 && countdownnum < 100){
            visitor200.drawString("GO!", bounds.x + 15 + bounds.width/2 - visitor200.stringWidth("GO!")/2, bounds.y + bounds.height/2 + visitor200.stringHeight("GO!")/2 );
        }
        if (countdownnum == 0 && countdownnumbool){
            countdownnumbool = false;
            countdownnum = 400;
            startGame();
            drawusers = true;
            players = false;
            user1load = false;
            user2load = false;
            newBall();
        }
        
    }    if(startGameBool){
        
        for(int i=0; i<circles.size(); i++) {
            Data * data = (Data*)circles[i].getData();
            ofSetColor(255);
            ofEnableAlphaBlending();
            //  ofRectMode(OF_RECTMODE_CORNER);
            vimeologo.draw(circles[i].getPosition().x, circles[i].getPosition().y, circles[i].getRadius() * 2, circles[i].getRadius() * 2); 
            
        }
        
        
        for(int i=0; i<paddles.size(); i++) {
            ofFill();
            Data * data = (Data*)paddles[i].getData();
            ofSetColor(236, 28, 36);
            paddles[i].draw();
        }
        
        for(int i=0; i<winningvideos.size(); i++) {

            
            winningvideos[i].draw();
       	}
        
        for(int i=0; i<nomvideos.size(); i++) {
            nomvideos[i].draw();
        }
        
        if (score1 > 4 && counter == 10){
             sound[3].play();
            senduser1 = true;
        }
        if (score2 > 4 && counter == 10){
             sound[3].play();
            senduser2 = true;
        }
        
        if (score1 > 4 && counter < 200) {
           
            ofSetColor(255,221,21);
            visitor150.drawString("YOU WIN!", bounds.x + bounds.width/4 - visitor150.stringWidth("YOU WIN!")/2, bounds.y + bounds.height/2 - 120);
            visitor150.drawString("YOU LOSE!", bounds.x + bounds.width/4 - visitor150.stringWidth("YOU LOSE!")/2 + bounds.width/2, bounds.y + bounds.height/2 - 120);        
            ofSetColor(255);
            ofSetRectMode(OF_RECTMODE_CORNER);
            user1.draw(bounds.x + bounds.width/4 - 50,  bounds.height/2 + 20, 100, 100);
            user2.draw(bounds.x + bounds.width/4 - 50 + bounds.width/2,  bounds.height/2 + 20, 100, 100);
            ofSetColor(255,221,21);
            visitor82.drawString(username1, bounds.x + bounds.width/4 - visitor82.stringWidth(username1)/2, bounds.y + bounds.height/2 + 120);
            visitor82.drawString(username2, bounds.x + bounds.width/4 - visitor82.stringWidth(username2)/2 + bounds.width/2, bounds.y + bounds.height/2 + 120);
            
            counter++;
       
            
        }
        if (score2 > 4 && counter < 200) {
            ofSetColor(255,221,21);
            visitor150.drawString("YOU LOSE!", bounds.x + bounds.width/4 - visitor150.stringWidth("YOU LOSE!")/2, bounds.y + bounds.height/2 - 120);
            visitor150.drawString("YOU WIN!", bounds.x + bounds.width/4 - visitor150.stringWidth("YOU WIN!")/2 + bounds.width/2, bounds.y + bounds.height/2 - 120);        
            ofSetColor(255);
            ofSetRectMode(OF_RECTMODE_CORNER);
            user1.draw(bounds.x + bounds.width/4 - 50,  bounds.height/2 + 20, 100, 100);
            user2.draw(bounds.x + bounds.width/4 - 50 + bounds.width/2,  bounds.height/2 + 20, 100, 100);
            ofSetColor(255,221,21);
            visitor82.drawString(username1, bounds.x + bounds.width/4 - visitor82.stringWidth(username1)/2, bounds.y + bounds.height/2 + 120);
            visitor82.drawString(username2, bounds.x + bounds.width/4 - visitor82.stringWidth(username2)/2 + bounds.width/2, bounds.y + bounds.height/2 + 120);
            
            counter++;
                 }
               
        ofSetColor(0,173,238);
        
        if (drawusers){
            visitor82.drawString("SCORE " + ofToString(score1, 1), bounds.x+120, bounds.y +65);
            visitor82.drawString("SCORE " + ofToString(score2, 1), bounds.x + bounds.width/2 + 120, bounds.y +65);
            ofSetColor(255);
            ofSetRectMode(OF_RECTMODE_CORNER);
            user1.draw(bounds.x+ 30, bounds.y+15, 80,80);
            user2.draw(bounds.x+ 30 + bounds.width/2, bounds.y+15, 80,80);
        }
    }
    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	if(key == 't') ofToggleFullscreen();
    
    if(key == 's' && startScreen == true){
        startScreen = false;
        players = true;
        
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
    ofxOscMessage m;
    m.setAddress( "/user" );
    m.addStringArg( "http://b.vimeocdn.com/ps/215/215646_100.jpg" );
    sender.sendMessage( m );

  	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
}

void testApp::startGame(){
    printf("starting new game");
    startGameBool = true;
    
    for(int i=0; i< 12; i++){
        ofVideoPlayer * v = new ofVideoPlayer();
        v->loadMovie("movies/winner" + ofToString(i) +".mov");
        v->play();
        wvideos.push_back(v);
    }
    
    
    for(int i=0; i < 4; i++){
        for(int j=0; j < 3; j++){
            WinningVideo v;
            v.setPhysics(1.0, 0.5, 0.3);
            v.setup(box2d.getWorld(), float((bounds.x + 300 + ((bounds.width - 100)/4)*i)), float((bounds.y + 200 + ((bounds.height - 200)/3)*j)), 60,60, b2_staticBody);
            v.setupTheCustomData();
            winningvideos.push_back(v);
        }
    }
    for(int i=0; i<winningvideos.size(); i++){
        winningvideos[i].movie = wvideos[i];
    }
                

    
    
}


void testApp::newBall(){
    ofxBox2dCircle  c;
    c.setPhysics(0.1, 1.0, 0.0);
    c.setup(box2d.getWorld(), bounds.x + bounds.width/2, bounds.y + bounds.height/2, 30);
    float sgn = ofRandom(-1, 1);
    float vx = copysign(20,sgn);
    c.setVelocity(vx, 0);
    c.setData(new Data());
    Data * sd = (Data*)c.getData();
    sd->hit	= false;		
    sd->type = 0;
    circles.push_back(c);	
}

void testApp::threadedVideoEvent(ofxThreadedVideoEvent & event){
    ofLogVerbose() << "VideoEvent:" << event.eventTypeAsString << "for" << event.path;
}




void testApp::loadSettings(string fileString){
	string host_address;
    string send_host_address;
	string host_address1;
	string host_address2;
	string filename;
	
	//--------------------------------------------- get configs
    ofxXmlSettings xmlReader;
	bool result = xmlReader.loadFile(fileString);
	if(!result) printf("error loading xml file\n");
	
	
	host_address = xmlReader.getValue("settings:master:address","test",0);
	port = xmlReader.getValue("settings:master:port",5204,0);
	host = (char *) malloc(sizeof(char)*host_address.length());
    strcpy(host, host_address.c_str());
    
    send_host_address = xmlReader.getValue("settings:send:address","test",0);
    sendport = xmlReader.getValue("settings:send:port",5204,0);
	sendhost = (char *) malloc(sizeof(char)*host_address.length());
	strcpy(sendhost, send_host_address.c_str());
    
    
	filename = xmlReader.getValue("settings:movie:","test",0);
	
	
    int w = xmlReader.getValue("settings:dimensions:width", 640, 0);
	int h = xmlReader.getValue("settings:dimensions:height", 480, 0);
	
    ofSetWindowShape(w, h);
    
	
	if(xmlReader.getValue("settings:go_fullscreen", "false", 0).compare("true") == 0) {
		fullscreen = true;
		ofSetFullscreen(true);
	}
}

