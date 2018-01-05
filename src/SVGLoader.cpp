//
//  SVGLoader.cpp
//  MapperSVG
//
//  Created by Jayson Haebich on 20/12/2017.
//


#include "SVGLoader.h"

// load xml file
bool SVGLoader::load( string filepath, int mirror )
{
    mMirror = mirror;

    ofxXmlSettings xml;

    cout << "loading xml file: " << filepath << endl;
    
    if(xml.load( filepath ))
    {
    
	    string viewBox = xml.getAttribute("svg", "viewBox", "");
	    vector <string> viewBoxAtts = ofSplitString(viewBox, " ");

	    // Get the dimensions of the svg canvas
	    mViewBox = ofRectangle( ofToFloat(viewBoxAtts[0]), 
  				    ofToFloat(viewBoxAtts[1]),
				    ofToFloat(viewBoxAtts[2]), 
                                    ofToFloat(viewBoxAtts[3]) );

	    if( xml.pushTag("svg") )
	    {

	        string outputProgram = xml.getValue("title","default value");
	        
	        // Check which program this xml was exported from
	        if( outputProgram.compare("MadMapper SVG export") == 0 )
	            mOutputProgram = MAD_MAPPER;
	        else if( outputProgram.compare("SVG export from MAPIO") ==  0)
	            mOutputProgram = MAPIO;
	        else
	            mOutputProgram = NONE;
	        
	        // Load surfaces
	        if(xml.pushTag("g"))
	        {
	            for(int i = 0; i < xml.getNumTags("g");i++)
	            {
	                string transform = xml.getAttribute("g", "transform","",i);
	                
	                xml.pushTag("g", i);
	                
			// Get list of drawing commands
	                string commandList =xml.getAttribute("path", "d", "") ;
	                
	                if(commandList.size() > 0)
	                {
	                     SVGSurface surface = loadSurface( transform, commandList );
	                     mSurfaces.push_back( surface );
	                }
	                xml.popTag();
	            }
	            xml.popTag();
	        }
	        
    	}
	return true;
    }else
    {
	cout << "Cannot load file svg file: " << filepath << endl;
	return false;
    }
}

vector <ofVec2f> SVGLoader::getTexCoords( SurfaceType surfaceType, OutputProgram program)
{
	vector <ofVec2f> texCoords;

	if(program == MAD_MAPPER)
	{
		texCoords.push_back(ofVec2f(1.0, 0.0));
		texCoords.push_back(ofVec2f(0.0, 0.0));
		texCoords.push_back(ofVec2f(0.0, 1.0));
		texCoords.push_back(ofVec2f(1.0, 1.0));
		
	}else if(program == MAPIO)
	{
		
		texCoords.push_back(ofVec2f(1.0, 1.0));
		texCoords.push_back(ofVec2f(0.0, 1.0));
		texCoords.push_back(ofVec2f(0.0, 0.0));
		texCoords.push_back(ofVec2f(1.0, 0.0));
		
	}else
	{
		cout << "Error invalid program type when exporting tex coords" << endl;
	}

	return texCoords;
}

// Save svg to an xml file
void SVGLoader::save(string filepath)
{
	cout << "saving output to: " << filepath <<endl;
	ofFile outputFile;

	outputFile.open( ofToDataPath(filepath), ofFile::WriteOnly );

	outputFile << "<surfaces>" << endl;

	for( int i = 0; i < mSurfaces.size(); i++)
	{
		// Mad mapper always includes screen dimensions as a vector object first, so do not add it
		if( (mOutputProgram == MAD_MAPPER && i >= 1) || mOutputProgram == MAPIO)
		{
			outputFile << "<surface type=\"" << (int)mSurfaces[i].getType() << "\">" << endl;
	
			vector <ofVec2f> verts = mSurfaces[i].getVerts();
			outputFile << "<vertices>" <<endl;
	
			for(int j = 0; j < verts.size(); j++ )
			{
				outputFile << "<vertex>" << endl;
				outputFile << "<x>" << verts[j].x << "</x>" << endl;
				outputFile << "<y>" << verts[j].y << "</y>" << endl;
				outputFile << "</vertex>" << endl;
			}
	
			outputFile << "</vertices>" << endl;
			
			
			outputFile << "<texCoords>" << endl;
			
			vector <ofVec2f> texCoords = getTexCoords(mSurfaces[i].getType(), mOutputProgram);
			
			for(int j = 0; j < texCoords.size(); j++)
			{
				outputFile << "<texCoord>" << endl;
				outputFile << "<x>" << texCoords[j].x << "</x>" << endl;
				outputFile << "<y>" << texCoords[j].y << "</y>" << endl;
				outputFile << "</texCoord>" << endl;
			}
			outputFile << "</texCoords>" << endl;
			
			outputFile << "<source>" << endl;
			outputFile << "<source-type>none</source-type>" << endl;
			outputFile << "<source-name>none</source-name>" << endl;
			outputFile << "</source>" << endl;
			
			outputFile << "<properties>" << endl;
			outputFile << "<perspectiveWarping>1</perspectiveWarping>"<<endl;
			outputFile << "</properties>" <<endl;
	
		outputFile << "</surface>" << endl;
		}
		
	}
	
	outputFile << "</surfaces>" << endl;

	outputFile.close();
}

// Draw converted file to the screen
void SVGLoader::draw()
{
	ofNoFill();
	ofSetColor(255,255,255);

	for(int i = 0; i < mSurfaces.size(); i++)
	{
	    int indx = i;

	    vector <ofVec2f> verts = mSurfaces[indx].getVerts();
	   
	    ofBeginShape();
            
            for(int j = 0; j < verts.size();j++)
            {
                ofVertex(verts[j].x, verts[j].y);
            }

            ofEndShape(true);
        }
}

// Load a surface from a list of SVG draw commands
SVGSurface SVGLoader::loadSurface( string transform, string commandList )
{
    
    float transMat[9];
    
    vector<string> coords = ofSplitString(transform, ",");
    
    transMat[0] = ofToFloat( coords[0].substr( 7, coords[0].size()-7 )) ;
    transMat[1] = ofToFloat( coords[2]);
    transMat[2] = ofToFloat( coords[4]);
    transMat[3] = ofToFloat( coords[1]);
    transMat[4] = ofToFloat( coords[3]);
    transMat[5] = ofToFloat( coords[5].substr( 0, coords[5].size()-1 )) ;
    transMat[6] = 0.0;
    transMat[7] = 0.0;
    transMat[8] = 1.0;

    vector<string> commands = ofSplitString(commandList, " ");
    
    ofPolyline path;
    //vector<ofVec2f> shape;
    
    for(int j = 0; j < commands.size();j++)
    {
        char commandType = commands[j].at(0);
        
        // Move command
        if(commandType == 'M')
        {
            vector<string> tmp = ofSplitString(commands[j], "M");
            vector<string>coordsStr = ofSplitString(tmp[1],",");
            
            ofVec2f pos = ofVec2f( ofToFloat(coordsStr[0]), ofToFloat(coordsStr[1]));
            
            ofVec2f transformedPos = applyMatrix( transMat,         pos            );
	    if(mMirror == 1)
	    {
            	transformedPos         = ofVec2f( mViewBox.width - transformedPos.x, mViewBox.height - transformedPos.y);
	    }

            path.addVertex(transformedPos);
        }
	// Line command
	else if(commandType == 'L')
        {
            vector<string> tmp = ofSplitString(commands[j], "L");
            vector<string>coordsStr = ofSplitString(tmp[1],",");
            
            ofVec2f pos = ofVec2f( ofToFloat(coordsStr[0]), ofToFloat(coordsStr[1]));
            
            ofVec2f transformedPos = applyMatrix( transMat,         pos            );
            if(mMirror == 1)
	    {
            	transformedPos         = ofVec2f( mViewBox.width - transformedPos.x, mViewBox.height - transformedPos.y);//applyMatrix( reflectionMatrix, transformedPos );
	    }

            path.addVertex(transformedPos);

        }
    }

    

    // Load ofPolyline into an SVGSurface object and return it from the function
    return SVGSurface(path);
}

// Apply a 3x3 matrix transformation to a 2d point
ofVec2f SVGLoader::applyMatrix( float * matrix, ofVec2f p )
{
    
    ofVec2f result;
    float cw = matrix[6] * p.x + matrix[7] * p.y + matrix[8];
    
    result.x = (matrix[0] * p.x + matrix[1] * p.y + matrix[2]) / cw;
    result.y = (matrix[3] * p.x + matrix[4] * p.y + matrix[5]) / cw;
    
    return result;
}

