#pragma once

#include "ofMain.h"
#include "ofxUI.h"

class ofxUIDirList : public ofxUIScrollableCanvas
{
public:
    ofxUIDirList() : ofxUIScrollableCanvas()
    {
        
    }
    ofxUIDirList(string _name, ofDirectory directory, float w, float h, float x = 0, float y = 0, int _size = OFX_UI_FONT_SMALL) : ofxUIScrollableCanvas(x, y, w, h){
        useReference = false;
        initDirList(_name, &directory);
    }
    ofxUIDirList(string _name, ofDirectory *directory, float w, float h, float x = 0, float y = 0, int _size = OFX_UI_FONT_SMALL) : ofxUIScrollableCanvas(x, y, w, h){
        useReference = true;
        initDirList(_name, directory);
    }
    ofxUIDirList(string _name, string path, float w, float h, float x = 0, float y = 0, int _size = OFX_UI_FONT_SMALL) : ofxUIScrollableCanvas(x, y, w, h){
        useReference = false;
        ofDirectory directory;
        directory.listDir(path);
        initDirList(_name, &directory);
    }
    
    void initDirList(string _name, ofDirectory *directory)
    {
        kind = OFX_UI_WIDGET_DIRLIST;
        
        if(useReference)
        {
            dir = directory;
        }
        else
        {
            dir = new ofDirectory();
            *dir = *directory;
        }
        
        for(int i=0; i<dir->numFiles(); i++){
            addWidgetDown(new ofxUILabel(dir->getName(i),OFX_UI_FONT_SMALL));
        }
        
        damping = 0.1;
        stickyDistance = 5;
    }
    
    void drawBack()
    {
        if(draw_back)
        {
            ofFill();
            ofSetColor(color_back);
            sRect->draw();
        }
    }
    
    virtual void draw()
    {
        ofPushStyle();
        
        //set up to draw stencil
        ///////////////////////////////////////////////////////
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glClear(GL_STENCIL_BUFFER_BIT);
        
        glEnable(GL_STENCIL_TEST); //Enable using the stencil buffer
        glColorMask(0, 0, 0, 0); //Disable drawing colors to the screen
        glDisable(GL_DEPTH_TEST); //Disable depth testing
        glStencilFunc(GL_ALWAYS, 1, 1); //Make the stencil test always pass
        //Make pixels in the stencil buffer be set to 1 when the stencil test passes
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        
        //Set all of the pixels below to be 1 in the stencil buffer...
        drawScrollableRect();
        
        //switch from drawing stencil to scene to be masked  
        glColorMask(1, 1, 1, 1); //Enable drawing colors to the screen
        //Make the stencil test pass only when the pixel is 1 in the stencil buffer
        glStencilFunc(GL_EQUAL, 1, 1);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //Make the stencil buffer not change
        
        //Draw all pixels where the stencil buffer is 1...
        
        ///////////////////////////////////////////////////
        
        
		glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofSetLineWidth(1.0);
        
        drawPadded();
        
        drawPaddedOutline();
        
        drawBack();
        
        drawFill();
        
        drawFillHighlight();
        
        drawOutline();
        
        drawOutlineHighlight();
        
		for(int i = widgets.size()-1; i >= 0; i--)
		{
            if(widgets[i]->isVisible() && widgets[i]->getRect()->rIntersects(*sRect))
            {
                widgets[i]->draw();
            }
		}
		
		glDisable(GL_DEPTH_TEST);
        
        glDisable(GL_STENCIL_TEST); //Disable using the stencil buffer  
        ofPopStyle();
    }
    
    
protected:    
    ofDirectory *dir;
    bool useReference;
    bool drawLabel;
};

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();


    
		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

    ofxUICanvas *gui;
    ofxUIDirList *dirList;
    ofDirectory dir;
    void exit(); 
    void guiEvent(ofxUIEventArgs &e);
    
    
    
};
