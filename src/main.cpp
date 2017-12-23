#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( int argc, char *argv[] )
{
	if(argc > 2)
	{
		cout << argv[1] <<endl;
		//ofSetupOpenGL(1920,1080, OF_WINDOW);			// <-------- setup the GL context

		// this kicks off the running of my app
		// can be OF_WINDOW or OF_FULLSCREEN
		// pass in width and height too:
		ofRunApp( new ofApp( argv[1], argv[2] ));

	}else
	{
		cout << "ERROR! please specify an svg file to parse and output filename" << endl;
		cout << "Usage: MapperSVGConverter inputFile outputFile" << endl;
	}	

}
