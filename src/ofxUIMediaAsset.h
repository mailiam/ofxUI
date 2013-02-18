//
//  ofxUIMediaAsset.h
//  ofxUIDirList
//
//  Created by SeJun Jeong on 13. 1. 30..
//
//

#ifndef ofxUIDirList_ofxUIMediaAsset_h
#define ofxUIDirList_ofxUIMediaAsset_h

#include "ofxUIWidgetWithLabel.h"

class ofxUIMediaAsset : public ofxUIToggle
{
public:
    ofxUIMediaAsset(string path, float w = 0, float h = 0, float x = 0, float y = 0, int _size = OFX_UI_FONT_MEDIUM) : ofxUIToggle()
    {
		ofFile _file(path);
		useReference = false;
        init(&_file, w, h, x, y, _size);
    }
	
	ofxUIMediaAsset(ofFile _file, float w = 0, float h = 0, float x = 0, float y = 0, int _size = OFX_UI_FONT_MEDIUM) : ofxUIToggle()
    {
		useReference = false;
        init(&_file, w, h, x, y, _size);
    }
	
	ofxUIMediaAsset(ofFile *_file, float w = 0, float h = 0, float x = 0, float y = 0, int _size = OFX_UI_FONT_MEDIUM) : ofxUIToggle()
    {
		useReference = true;
        init(_file, w, h, x, y, _size);
    }
	
	virtual void init(ofFile *_file, float w, float h, float x = 0, float y = 0, int _size = OFX_UI_FONT_MEDIUM)
    {
        rect = new ofxUIRectangle(x,y,w,h);
        autoSize = w == 0 ? true : false;
		thumbnailSize = w*0.25;
        name = _file->getBaseName();
        
        label = new ofxUILabel(0,0,(name+" LABEL"), name, _size);
		label->setParent(label);
		label->setRectParent(rect);
        label->setEmbedded(true);

		
		kind = OFX_UI_WIDGET_MEDIAASSET;
        paddedRect = new ofxUIRectangle(-padding, -padding, padding*2.0, padding*2.0);
		paddedRect->setParent(rect);
        
		if(useReference)
        {
            file = _file;
        }
        else
        {
            file = new ofFile();
            *file = *_file;
        }
		
		font = new ofTrueTypeFont();
		font -> loadFont(string(OFX_UI_FONT_NAME), OFX_UI_FONT_MEDIUM_SIZE);
		font -> setLineHeight(OFX_UI_FONT_MEDIUM_SIZE*1.5);
		
		
		ofxUIToggle::value = new bool(false);
		setFile(*file);
	}
	
	virtual void draw()
    {
        ofPushStyle();
        
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        
        drawPadded();
        drawPaddedOutline();
        
        drawBack();
        
        drawOutline();
        drawOutlineHighlight();
        
        drawFill();
        drawFillHighlight();
        
		
		if(rect->height>thumbnailSize*0.75) {
			ofFill();
			ofSetColor(255);
			thumbnail.draw(rect->getX()+padding,rect->getY()+padding,thumbnailSize,thumbnailSize*0.75);
			font->drawString(description, label->getRect()->getX(), label->getRect()->getY()+label->getRect()->getHeight()*2);
		}
		
        ofPopStyle();
    }
	
    void setLabelText(string labeltext)
    {
        label->setLabel(labeltext);
        if(!autoSize)
        {
            ofxUIRectangle *labelrect = label->getRect();
            float h = labelrect->getHeight();
            float ph = rect->getHeight();
            float w = labelrect->getWidth();
            float pw = rect->getWidth();
            labelrect->y = padding;//(int)(ph*.5 - h*.5);
            labelrect->x = pw-w-padding;//(int)(pw*.5 - w*.5-padding*.5);
        }
    }
    
	void setParent(ofxUIWidget *_parent)
	{
		parent = _parent;
        if(rect->height == 0)
        {
            rect->height = label->getPaddingRect()->height+padding;
        }

		if(rect->height < thumbnailSize*0.75 && *value && autoSize)
		{
			rect->height = thumbnailSize*0.75+padding*2;
		}
		
		ofxUIRectangle *labelrect = label->getRect();
        if(autoSize)
        {
            //rect->width = label->getPaddingRect()->width+padding*2.0;
        }
        else
        {
            while(labelrect->width > rect->width)
            {
                string labelstring = label->getLabel();
                string::iterator it;
                it=labelstring.end();
                it--;
                labelstring.erase (it);
                label->setLabel(labelstring);
            }
        }
		
		float h = labelrect->getHeight();
		float ph = rect->getHeight();
        
        float w = labelrect->getWidth();
        float pw = rect->getWidth();
        
		labelrect->y = padding;//(int)(ph*.5 - h*.5);
		if(rect->height>thumbnailSize*0.75)
			labelrect->x = padding*2+thumbnailSize;//pw-w-padding;//(int)(pw*.5 - w*.5-padding*.5);
		else
			labelrect->x = padding;
		
		paddedRect->height = rect->height+padding*2.0;
        paddedRect->width = rect->width+padding*2.0;
	}
    
    virtual void setFile(ofFile _file)
	{
		*file = _file;
        draw_fill = *file;
        label->setDrawBack((*file));
		string ext = file->getExtension();
		
		description = "";
		
		if(file->exists()){			
			if(ext == "png" || ext == "jpg" || ext == "jpeg"){
				thumbnail.loadImage(*file);
				description += "image/"+ext+"\n";
				description += ofToString(thumbnail.getWidth()) + " X " + ofToString(thumbnail.getHeight())+"\n";
			}else if(file->isDirectory()){
				ofDirectory *dir = new ofDirectory();
				dir->listDir(file->getAbsolutePath());
				description += (string)"folder"+"\n";
				description += ofToString(dir->numFiles())+" files"+"\n";
				delete dir;
				thumbnail.allocate(thumbnailSize, thumbnailSize*0.75, OF_IMAGE_COLOR);
			}else{
				description += "etc/"+ext+"\n";
				thumbnail.allocate(thumbnailSize, thumbnailSize*0.75, OF_IMAGE_COLOR);
			}
		}else{
			ofLogError("ofxUIMediaAsset")<<"file "<<  file->getAbsolutePath() << " does not exist";
			description = "file does not exist";
			thumbnail.allocate(thumbnailSize, thumbnailSize*0.75, OF_IMAGE_COLOR);
		}
	}
	
	ofFile * getFile(){
		return file;
	}
    
	static ofImage getTypeIcon(string type)
	{
		static ofImage *icon;
		static ofImage imageIcon;
		static ofImage videoIcon;
		static ofImage swfIcon;
		
		if (type=="image") {
			icon = &imageIcon;
		}
		
		return *icon;
	}
	
	void setAutoSize(bool autoSizing){
		autoSize = autoSizing;
	}
	
protected:    //inherited: ofxUIRectangle *rect; ofxUIWidget *parent;
    bool autoSize;
	bool useReference; 
	ofFile *file;
	ofImage thumbnail;
	float thumbnailSize;
	string description;
};


#endif
