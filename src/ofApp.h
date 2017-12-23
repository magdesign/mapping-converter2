#pragma once

#include "ofMain.h"
#include "ofxSvg.h"
#include "ofxXmlSettings.h"
#include "SVGLoader.h"

class ofApp : public ofBaseApp{

	public:
		ofApp( char *inputFile, char *outputFile );

		void setup();
		void update();
		void draw();

    		SVGLoader svgLoader;

};
