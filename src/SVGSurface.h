//
//  SVGSurface.h
//  MapperSVG
//
//  Created by Jayson Haebich on 20/12/2017.
//

#ifndef SVGSurface_h
#define SVGSurface_h

#include "ofMain.h"
enum SurfaceType { TRIANGLE, QUAD };

class SVGSurface
{
public:
    
    SVGSurface( ofPolyline path )
    {
	// Simplify the shape, will remove extra vertices
	path.simplify();

	// Save ofPolyline points to our vector list
	vector <ofPoint> verts = path.getVertices();

	for(int i = 0; i < verts.size(); i++)
	{
		mPoints.push_back( ofVec2f(verts[i].x, verts[i].y) );
	}

	// if first and last points are the same then remove one of them
	// happens sometimes when loading mapio shapes
	if( ofDist(mPoints[0].x, mPoints[0].y,
                   mPoints[mPoints.size()-1].x, mPoints[mPoints.size()-1].y ) < 0.0001 )
	{
		mPoints.pop_back();
	}
        
	// Set shape type
        if(mPoints.size() == 3)
            mType = TRIANGLE;
        else if(mPoints.size() == 4)
        {
            mType = QUAD;
        }
    }
    
    SurfaceType getType()
    {
        return mType;
    }
    
    vector <ofVec2f> getVerts()
    {
        return mPoints;
    }
    
private:
    
    SurfaceType      mType;
    vector <ofVec2f> mPoints;
    
};
#endif /* SVGSurface_h */
