#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(255, 255, 255);
	ofSetVerticalSync(true);
	//ofSetFrameRate(60);
	//ofEnableAlphaBlending();
	
	shader.setGeometryInputType(GL_LINE_STRIP_ADJACENCY);
	shader.setGeometryOutputType(GL_TRIANGLE_STRIP);
	shader.setGeometryOutputCount(4);
	shader.load("shaders/vert.glsl", "shaders/frag.glsl", "shaders/geom.glsl");
	
	ofLog() << "Maximum number of output vertices support is: " << shader.getGeometryMaxOutputCount();
	mesh.setMode(OF_PRIMITIVE_LINE_STRIP_ADJACENCY);
	mesh.enableColors();
    mesh.enableNormals();
    
    
    for(int i = 0; i < 1000;i++){
        ofMesh tempmesh;
        tempmesh.setMode(OF_PRIMITIVE_LINE_STRIP_ADJACENCY);
        tempmesh.enableColors();
        tempmesh.enableNormals();
        meshies.push_back(tempmesh);
        
    }
	
	doShader = true;
	ofDisableDepthTest();
	play = true;
    
    fbo.allocate(1400, 768, GL_RGBA, 8);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    fbo.begin();
    ofClear(255,255,255, 0);
    fbo.end();
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofPushMatrix();
	
	//mesh.clear();
    //mesh2.clear();
    
    /*
	int nPts = 100;
	ofPoint center = ofPoint(ofGetWidth()/2, ofGetHeight()/2);
	static float T = 0;
	if( play ) T += ofGetLastFrameTime();

	float alpha = fmodf(T/10,2);
	if( alpha > 1 ) alpha = 2 - alpha;
	
	for( int i = 0; i < nPts; i++ ){
		float t = i/(float)nPts;
		
		ofPoint f1 = ofPoint( cosf(TWO_PI*t), sinf(TWO_PI*t));
		ofPoint f2 = ofPoint( cosf(TWO_PI*t), sinf(TWO_PI*t*2));
		ofPoint pt;
		if( t > alpha ){
			pt = f1;
		}
		else{
			ofPoint p1 = ofPoint( cosf(TWO_PI*alpha), sinf(TWO_PI*alpha));
			ofPoint p2 = ofPoint( cosf(TWO_PI*alpha), sinf(TWO_PI*alpha*2));
			ofPoint d1 = ofPoint( -TWO_PI*sinf(TWO_PI*alpha), TWO_PI*cosf(TWO_PI*alpha));
			ofPoint d2 = ofPoint( -TWO_PI*sinf(TWO_PI*alpha), TWO_PI*cosf(TWO_PI*alpha*2)*2);
			ofMatrix4x4 mat;
			mat.rotate(-RAD_TO_DEG*(atan2(d1.y, d1.x) - atan2(d2.y, d2.x)), 0, 0, 1);
			pt = p1 + mat*(f2-p2);
		}
		
		mesh.addVertex(center + center.x*pt/6);
		mesh.addColor(ofColor(
							  127+127*cos(i/50.0+T),
							  127+127*cos(i/50.0+T*2),
							  127+127*cos(i/50.0+T*3)
		));
        
	}*/
    
    /*
    float time = ofGetFrameNum() *0.017389;
    float x = (ofNoise(time + 100.38798))* ofGetWidth();
    float y = ofNoise((time * .939) + 26.8789) * ofGetHeight();
    float colorr = ofNoise((time) + 771.9) * 255;

    float xN = (ofNoise(time*100.1))* 4.098230498;
    ofPoint pusher = ofPoint(x,y,0);
    mesh.addVertex(pusher);
    mesh.addNormal(ofPoint(xN,xN,xN));
    mesh.addColor(ofColor(colorr,colorr,colorr));
     */
    
	
    for (int i = 0; i < 1000; i++) {
        float time = ofGetFrameNum() *0.017389 + (i * 400.987234);
        float x = (ofNoise(time + 100.38798))* mouseX;
        float y = ofNoise((time * .939) + 26.8789) * mouseY;
        float colorr = ofNoise((time) + 771.9) * 4;
        
        float xN = (ofNoise(time*100.1))* 4.098230498;
        ofPoint pusher = ofPoint(x,y,0);
        /*
        mesh.addVertex(pusher);
        mesh.addNormal(ofPoint(xN,xN,xN));
        mesh.addColor(ofColor(colorr,colorr,colorr));
        */
        
        meshies[i].addVertex(pusher);
        meshies[i].addNormal(ofPoint(xN,xN,xN));
        meshies[i].addColor(ofColor(colorr,colorr,colorr));
        
        
        
    }
	
    fbo.begin();
	if(doShader) {
		shader.begin();
		ofMatrix4x4 mat;
		
		
		ofMatrix4x4 modelView = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW);
		modelView = ofGetCurrentViewMatrix();
		modelView = modelView.getInverse();
		ofVec4f zero = modelView*ofVec4f(0,0,0,1);
		ofVec4f one = modelView*ofVec4f(1,1,0,1);
		shader.setUniformMatrix4f("modelViewMatrix",modelView);
		// set thickness of ribbons
		shader.setUniform1f("thickness", fmaxf(1,(ofGetMouseX()/100.0))-10);
		
		// make light direction slowly rotate
		//shader.setUniform3f("lightDir", sin(ofGetElapsedTimef()/10), cos(ofGetElapsedTimef()/10), 0);
	}
	
	
	
	mesh.draw();
    
    
    
    for (int i = 0; i < 1000; i++) {
        meshies[i].draw();
    }
    
	
	//ofDrawLine( 0,0, 500, 500);
	
	if(doShader) shader.end();
	
    fbo.end();
    
    fbo.draw(0,0);
    ofSetColor(0);
	ofPopMatrix();
	
	ofDrawBitmapString("fps: " + ofToString((int)ofGetFrameRate()) + "\nPress 's' to toggle shader: " + (doShader ? "ON" : "OFF"), 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
	if( key == 's' ){
		doShader = !doShader;
	}
	if( key == 'r' ){
		shader = ofShader();
		shader.setGeometryInputType(GL_LINE_STRIP_ADJACENCY);
		shader.setGeometryOutputType(GL_TRIANGLE_STRIP);
		shader.setGeometryOutputCount(4);
		shader.load("shaders/vert.glsl", "shaders/frag.glsl", "shaders/geom.glsl");
	}
	if( key == ' ' ){
		play ^= true;
	}
    if( key == 't' ){
        for (int i = 0; i < 1000; i++) {
        meshies[i].clear();
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
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

