#include "ofApp.h"

ofApp::ofApp( char *inputFile, char *outputFile, int mirror )
{
	// load svg file and save to xml file
	if(svgLoader.load( ofToDataPath(string(inputFile)), mirror ) )
    	{
		svgLoader.save( ofToDataPath(string(outputFile)) );
    	}else
	{
		cout << "could not load: " << inputFile << endl;
	}
}
//--------------------------------------------------------------
void ofApp::setup()
{
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw()
{
    // This code isn't run because the window isn't being drawn
    ofBackgroundGradient(ofColor(125),ofColor(255));
    ofPushMatrix();
    
    ofSetColor(0,255,0);
    
    svgLoader.draw();
    
    ofPopMatrix();
}
