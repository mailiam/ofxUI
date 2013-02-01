/**********************************************************************************
 
 Copyright (C) 2012 Syed Reza Ali (www.syedrezaali.com)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
 **********************************************************************************/

#ifndef OFXUI_SCROLLABLE_CANVAS
#define OFXUI_SCROLLABLE_CANVAS

#include "ofxUICanvas.h"
#include "ofFbo.h"

class ofxUIScrollableCanvas : public ofxUICanvas
{
public:
    ~ofxUIScrollableCanvas()
    {
        delete sRect;
    }
    
    ofxUIScrollableCanvas(float x, float y, float w, float h) : ofxUICanvas(x,y,w,h)
    {
        initScrollable();
    }
    
    ofxUIScrollableCanvas(float x, float y, float w, float h, ofxUICanvas *sharedResources) : ofxUICanvas(x,y,w,h,sharedResources)
    {
        initScrollable();
    }
    
    ofxUIScrollableCanvas() : ofxUICanvas()
    {
        initScrollable();
    }
    
    ofxUIScrollableCanvas(ofxUICanvas *sharedResources) : ofxUICanvas(sharedResources)
    {
        initScrollable();
    }
	
    void initScrollable()
    {
        kind = OFX_UI_WIDGET_SCROLLABLECANVAS;
        sRect = new ofxUIRectangle(rect->x, rect->y, rect->getWidth(), rect->getHeight());
		sRect->setParent(rect);
        isScrolling = false;
        vel.set(0);
        pos.set(0);
        ppos.set(0);
        acc.set(0);
        damping = .90;
        scrollX = false;
        scrollY = true;
        
        nearTop = false;
        nearBot = false;
        nearRight = false;
        nearLeft = false;
        
        hitWidget = false;
        stickyDistance = 100;
        hit = false;
        snapping = true;
#ifdef TARGET_OPENGLES
        touchId = -1;
#endif
		//        initFbo(sRect->getWidth(),sRect->getHeight());
    }
	
	//    void initFbo(int w, int h)
	//    {
	//        fbo.allocate(w, h);
	//        fbo.begin();
	//        ofClear(0,0,0,0);
	//        fbo.end();
	//    }
    
    void setDamping(float _damping)
    {
        damping = _damping;
    }
    
    void setSnapping(bool _snapping)
    {
        snapping = _snapping;
    }
	
	void setStickyDistance(float _distance)
	{
		stickyDistance = _distance;
	}
    
    void setScrollArea(float x, float y, float w, float h)
    {
        sRect->x = x;
        sRect->y = y;
        sRect->setWidth(w);
        sRect->setHeight(h);
		//        initFbo(w,h);
    }
    
    void setScrollAreaToScreen()
    {
        sRect->x = 0;
        sRect->y = 0;
        sRect->setWidth(ofGetWidth());
        sRect->setHeight(ofGetHeight());
		//        initFbo(sRect->getWidth(),sRect->getHeight());
    }
    
    void setScrollAreaToScreenWidth()
    {
        sRect->x = 0;
        sRect->setWidth(ofGetWidth());
		//        initFbo(sRect->getWidth(),sRect->getHeight());
    }
	
    void setScrollAreaToScreenHeight()
    {
        sRect->y = 0;
        sRect->setHeight(ofGetHeight());
		//        initFbo(sRect->getWidth(),sRect->getHeight());
    }
    
    void setScrollableDirections(bool _scrollX, bool _scrollY)
    {
        scrollX = _scrollX;
        scrollY = _scrollY;
    }
	
	void setScrollPosX(float pct)
	{
		sRect->x = pct*(rect->width);
		rect->x = -sRect->x;
	}
	
	void setScrollPosY(float pct)
	{
		sRect->y = pct*(rect->height);
		rect->y = -sRect->y;
	}
	
	ofPoint getScrollPosMin()
	{
		ofPoint p(sRect->getMinX()/rect->width, sRect->getMinY()/rect->height);
		return scrollPosMin = p;
	}
	
	ofPoint getScrollPosMax()
	{
		//Scroll Position
		ofPoint p(sRect->getMaxX()/rect->height, sRect->getMaxY()/rect->height);
		return scrollPosMax = p;
;
	}
    
	void autoSizeToFitWidgets() //Setting padding size accordung to sRect allows proper widget arrangement
	{
		ofxUICanvas::autoSizeToFitWidgets();
		paddedRect->width = sRect->width+padding*2;
        paddedRect->height = sRect->height+padding*2;
	}
	
    void update()
    {
        if(!isScrolling)
        {
            if(scrollX && snapping)
            {
                float dxLeft = rect->getX() - sRect->getX();
                float dxRight = (sRect->getX()+sRect->getWidth()) - (rect->getX()+rect->getWidth());
                
                if(fabs(dxLeft) < stickyDistance)
                {
                    nearLeft = true;
                }
                else if(fabs(dxRight) < stickyDistance)
                {
                    nearRight = true;
                }
                else
                {
                    nearLeft = false;
                    nearRight = false;
                }
                
                if(dxLeft > 0)
                {
                    acc.x += (-dxLeft)/10.0;
                    acc.x -=vel.x*(1.0-damping);
                }
                else if(nearLeft)
                {
                    acc.x += (-dxLeft)/10.0;
                    acc.x -=vel.x*(1.0-damping);
                }
                else if(dxRight > 0)
                {
                    acc.x += (dxRight)/10.0;
                    acc.x -=vel.x*(1.0-damping);
                }
                else if(nearRight)
                {
                    acc.x += (dxRight)/10.0;
                    acc.x -=vel.x*(1.0-damping);
                }
            }
            
            if(scrollY && snapping)
            {
                float dyTop = rect->getY() - sRect->getY();
                float dyBot = (sRect->getY()+sRect->getHeight()) - (rect->getY()+rect->getHeight());
                if(fabs(dyBot) < stickyDistance)
                {
                    nearTop = false;
                    nearBot = true;
                }
                if(fabs(dyTop) < stickyDistance)
                {
                    nearTop = true;
                    nearBot = false;
                }
				
                if(dyTop > 0)
                {
                    acc.y += (-dyTop)/10.0;
                    acc.y -=vel.y*(1.0-damping);
                }
                else if(nearTop)
                {
                    acc.y += (-dyTop)/10.0;
                    acc.y -=vel.y*(1.0-damping);
                }
                
                if(dyBot > 0)
                {
                    acc.y += (dyBot)/10.0;
                    acc.y -=vel.y*(1.0-damping);
                }
                else if(nearBot)
                {
                    acc.y += (dyBot)/10.0;
                    acc.y -=vel.y*(1.0-damping);
                }
                
                nearTop = false;
                nearBot = false;
            }
            
            acc.limit(50);
            vel +=acc;
            vel.limit(100);
            if(scrollX && fabs(vel.x) > 1.0) rect->x += floor(vel.x);
            if(scrollY && fabs(vel.y) > 1.0) rect->y += floor(vel.y);
			sRect->x = -rect->x; //reset sRect position
			sRect->y = -rect->y;
            
            vel *=damping;
            acc.set(0);
        }
		
		for(int i = 0; i < widgets.size(); i++)
		{
			widgets[i]->update();
		}
    }
    
    virtual void draw()
    {
        ofPushStyle();
        
		//Cleaner Masking out with stencil//
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
        
		ofRectangle asRect(sRect->getX(),sRect->getY(),sRect->getWidth(),sRect->getHeight()); //absolute sRect
		
		for(int i = widgets.size()-1; i >= 0; i--)
		{
            if(widgets[i]->isVisible() && widgets[i]->getRect()->rIntersects(asRect))
            {
                widgets[i]->draw();
            }
		}
		
		glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST); //Disable using the stencil buffer
		
        ofPopStyle();
		//        fbo.end();
		//
		//        ofSetColor(255);
		//        fbo.draw(sRect->getX(), sRect->getY());
    }
    
    virtual void setPosition(int x, int y)
    {
        rect->x = x;
        rect->y = y;
        sRect->x = x;
        sRect->y = y;
    }
	
    void drawScrollableRect()
    {
        sRect->draw();
    }
	
#ifdef TARGET_OPENGLES
    void touchDown(ofTouchEventArgs& touch)
    {
        if(sRect->inside(touch.x, touch.y))
        {
			for(int i = 0; i < widgets.size(); i++)
			{
                if(widgets[i]->isHit(touch.x, touch.y))
                {
                    if(widgets[i]->isDraggable())
                    {
                        hitWidget = true;
                    }
                    widgets[i]->touchDown(touch);
                }
			}
		}
        
        if(sRect->inside(touch.x, touch.y) && touch.id == 0)
        {
            hit = true;
            isScrolling = false;
            vel.set(0);
        }
    }
    
    void touchMoved(ofTouchEventArgs& touch)
    {
        for(int i = 0; i < widgets.size(); i++)
        {
            if(widgets[i]->isVisible())	widgets[i]->touchMoved(touch);
        }
        
        if(hit && touch.id == 0)
        {
            if(!hitWidget)
            {
                if(isScrolling != true)
                {
                    isScrolling = true;
                    ppos = ofPoint(touch.x,touch.y);
                    vel.set(0);
                }
                else
                {
                    pos = ofPoint(touch.x, touch.y);
                    vel = pos-ppos;
                    if(scrollX) rect->x +=vel.x;
                    if(scrollY) rect->y +=vel.y;
					sRect->x = -rect->x;
					sRect->y = -rect->y;
                    ppos = pos;
                }
            }
        }
    }
    
    void touchUp(ofTouchEventArgs& touch)
    {
        for(int i = 0; i < widgets.size(); i++)
        {
            if(widgets[i]->isVisible())	widgets[i]->touchUp(touch);
        }
        
        hit = false;
        hitWidget = false;
        if(isScrolling)
        {
            isScrolling = false;
            pos = ofPoint(touch.x,touch.y);
        }
    }
    
    void touchCancelled(ofTouchEventArgs& touch)
    {
        for(int i = 0; i < widgets.size(); i++)
        {
            if(widgets[i]->isVisible())	widgets[i]->touchUp(touch);
        }
        
        hit = false;
        hitWidget = false;
        if(isScrolling)
        {
            isScrolling = false;
            pos = ofPoint(touch.x,touch.y);
        }
    }
    
#endif
	
    void mouseDragged(int x, int y, int button)
    {
        for(int i = 0; i < widgets.size(); i++)
        {
            if(widgets[i]->isVisible())	widgets[i]->mouseDragged(x, y, button);
        }
        
        if(hit)
        {
            if(!hitWidget)
            {
                if(isScrolling != true)
                {
                    isScrolling = true;
                    ppos = ofPoint(x,y);
                    vel.set(0,0);
                }
                else
                {
                    pos = ofPoint(x, y);
                    vel = pos-ppos;
                    if(scrollX) rect->x +=vel.x;
                    if(scrollY) rect->y +=vel.y;
					sRect->x = -rect->x;
					sRect->y = -rect->y;
                    ppos = pos;
                }
            }
        }
    }
    
    void mousePressed(int x, int y, int button)
    {
        if(sRect->inside(x, y))
        {
            hit = true;
            for(int i = 0; i < widgets.size(); i++)
            {
                if(widgets[i]->isVisible())
                {
                    if(widgets[i]->isHit(x, y))
                    {
                        if(widgets[i]->isDraggable())
                        {
                            hitWidget = true;
                        }
                        widgets[i]->mousePressed(x, y, button);
                    }
                }
            }
        }
        
        isScrolling = false;
        vel.set(0,0);
    }
    
    void mouseReleased(int x, int y, int button)
    {
        for(int i = 0; i < widgets.size(); i++)
        {
            if(widgets[i]->isVisible()) widgets[i]->mouseReleased(x, y, button);
        }
        
        hit = false;
        hitWidget = false;
        if(isScrolling)
        {
            isScrolling = false;
            pos = ofPoint(x,y);
        }
    }
	
    ofxUIRectangle *getSRect()
    {
        return sRect;
    }
	
	bool isDraggable()
    {
        return true;
    }
	
protected:
	//    ofFbo fbo;    //experimental
    ofxUIRectangle *sRect;
    bool isScrolling;
    bool snapping;
    bool scrollX, scrollY;
    bool nearTop, nearBot, nearRight, nearLeft;
    bool hitWidget;
	ofPoint scrollPosMin, scrollPosMax;
    ofPoint pos;
    ofPoint ppos;
    ofPoint vel;
    ofPoint acc;
    float damping;
    float stickyDistance;
};



#endif
