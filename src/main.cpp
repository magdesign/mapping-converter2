#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( int argc, char *argv[] )
{
	if(argc > 2)
	{
		//ofSetupOpenGL(1920,1080, OF_WINDOW);			// <-------- setup the GL context

		// this kicks off the running of my app
		// can be OF_WINDOW or OF_FULLSCREEN
		// pass in width and height too:

		int mirror = 1;
		if(argc > 3)
		{
			mirror = ofToInt(argv[3]);
		}

		ofRunApp( new ofApp( argv[1], argv[2], mirror ));

	}else
	{
		cout << "ERROR! please specify an svg file to parse and output filename" << endl;
		cout << "Usage: MapperSVGConverter inputFile outputFile" << endl;
	}	

}
