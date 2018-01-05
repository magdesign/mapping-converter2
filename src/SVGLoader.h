//
//  SVGLoader.h
//  MapperSVG
//
//  Created by Jayson Haebich on 20/12/2017.
//

#ifndef SVGLoader_h
#define SVGLoader_h

#include "ofMain.h"
#include "SVGSurface.h"
#include "ofxXmlSettings.h"

enum OutputProgram { MAD_MAPPER, MAPIO, NONE };
class SVGLoader
{
public:
    
    bool load( string filepath, int mirror );
    void save( string filepath );
    void draw();

private:
    
vector <ofVec2f> getTexCoords( SurfaceType surfaceType, OutputProgram program);

    // Private functions
    SVGSurface loadSurface( string transform, string commandList );
    ofVec2f    applyMatrix( float * matrix, ofVec2f p );
    
    // Private variabes
    OutputProgram       mOutputProgram;
    vector <SVGSurface> mSurfaces;
    ofRectangle         mViewBox;
    int 		mMirror;
};

#endif /* SVGLoader_h */
