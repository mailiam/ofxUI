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

#ifndef OFXUI_DRAGGABLE_CANVAS
#define OFXUI_DRAGGABLE_CANVAS

#include "ofxUICanvas.h"
#include "ofFbo.h"

class ofxUIDraggableCanvas : public ofxUICanvas
{    
public:    
    ~ofxUIDraggableCanvas()
    {
        delete sRect;
    }
    
    ofxUIDraggableCanvas(float x, float y, float w, float h) : ofxUICanvas(x,y,w,h)
    {
        initDraggable();
    }
    
    ofxUIDraggableCanvas(float x, float y, float w, float h, ofxUICanvas *sharedResources) : ofxUICanvas(x,y,w,h,sharedResources)
    {
        initDraggable();        
    }
    
    ofxUIDraggableCanvas() : ofxUICanvas()
    {
        initDraggable();
    }
    
    ofxUIDraggableCanvas(ofxUICanvas *sharedResources) : ofxUICanvas(sharedResources)
    {
        initDraggable();
    }

    void initDraggable()
    {
        kind = OFX_UI_WIDGET_DRAGGABLE_CANVAS;
        sRect = new ofxUIRectangle(rect->x, rect->y, rect->getWidth(), rect->getHeight());
        isDragging = false; 
        vel.set(0);
        pos.set(0); 
        ppos.set(0); 
        acc.set(0); 
        damping = 0.0;
        dragX = true;
        dragY = true; 
        
        nearTop = false; 
        nearBot = false;        
        nearRight = false; 
        nearLeft = false;
        
        hitWidget = false; 
        stickyDistance = 100;
        hit = false; 
        snapping = false;
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
    
    void setDragArea(float x, float y, float w, float h)
    {
        sRect->x = x; 
        sRect->y = y; 
        sRect->setWidth(w);
        sRect->setHeight(h);
//        initFbo(w,h);
    }
    
    void setDragAreaToScreen()
    {
        sRect->x = 0; 
        sRect->y = 0; 
        sRect->setWidth(ofGetWidth());
        sRect->setHeight(ofGetHeight());
//        initFbo(sRect->getWidth(),sRect->getHeight());
    }
    
    void setDragAreaToScreenWidth()
    {
        sRect->x = 0; 
        sRect->setWidth(ofGetWidth());
//        initFbo(sRect->getWidth(),sRect->getHeight());        
    }

    void setDragAreaToScreenHeight()
    {
        sRect->y = 0;
        sRect->setHeight(ofGetHeight());
//        initFbo(sRect->getWidth(),sRect->getHeight());        
    }
    
    void setDraggableDirections(bool _dragX, bool _dragY)
    {
        dragX = _dragX;
        dragY = _dragY;
    }
    
    void update()
    {		     
        if(!isDragging)
        {
            if(dragX && snapping)
            {
                float dxLeft = rect->x - sRect->x; 
                float dxRight = (sRect->x+sRect->getWidth()) - (rect->x+rect->getWidth()); 
                
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
            
            if(dragY && snapping)
            {
                float dyTop = rect->y - sRect->y; 
                float dyBot = (sRect->y+sRect->getHeight()) - (rect->y+rect->getHeight()); 
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
            if(dragX && fabs(vel.x) > 1.0) rect->x += floor(vel.x);
            if(dragY && fabs(vel.y) > 1.0) rect->y += floor(vel.y);
            
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
//        fbo.begin(); 
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        ofClearAlpha();
        
        ofPushStyle();
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
            //if(widgets[i]->isVisible() && widgets[i]->getRect()->rIntersects(*sRect))
            {
                widgets[i]->draw();
            }
		}
        
		
		glDisable(GL_DEPTH_TEST);
        ofPopStyle();
//        fbo.end();
//        
//        ofSetColor(255);
//        fbo.draw(sRect->getX(), sRect->getY());x
    }
    
    virtual void setPosition(int x, int y)
    {
        rect->x = x;
        rect->y = y;
        sRect->x = x;
        sRect->y = y; 
    }

    void drawDraggableRect()
    {
        sRect->draw();
    }
    	
#ifdef TARGET_OPENGLES
    void touchDown(ofTouchEventArgs& touch)
    {        
        if(rect->inside(touch.x, touch.y))
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
            isDragging = false; 
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
                if(isDragging != true)
                {
                    isDragging = true; 
                    ppos = ofPoint(touch.x,touch.y);
                    vel.set(0); 
                }
                else
                {
                    pos = ofPoint(touch.x, touch.y);             
                    vel = pos-ppos; 
                    if(dragX) rect->x +=vel.x; 
                    if(dragY) rect->y +=vel.y;             
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
        if(isDragging)
        {
            isDragging = false; 
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
        if(isDragging)
        {
            isDragging = false; 
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
                if(isDragging != true)
                {
                    isDragging = true; 
                    ppos = ofPoint(x,y);
                    vel.set(0,0);
                    
                }
                else
                {
                    pos = ofPoint(x, y);             
                    vel = pos-ppos; 
                    if(dragX) rect->x +=vel.x; 
                    if(dragY) rect->y +=vel.y;             
                    ppos = pos; 
                }
            }
        }
    }
    
    void mousePressed(int x, int y, int button) 
    {
        if(rect->inside(x, y))
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
        
        isDragging = false; 
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
        if(isDragging)
        {
            isDragging = false; 
            pos = ofPoint(x,y);
            triggerEvent(this);
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
    bool isDragging;
    bool snapping; 
    bool dragX, dragY; 
    bool nearTop, nearBot, nearRight, nearLeft;
    bool hitWidget; 
    ofPoint pos; 
    ofPoint ppos; 
    ofPoint vel; 
    ofPoint acc; 
    float damping;
    float stickyDistance;     
};
    
    
    
#endif
