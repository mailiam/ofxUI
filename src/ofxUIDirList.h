//
//  ofxUIDirList.h
//  ofxUIDirList
//
//  Created by SeJun Jeong on 13. 1. 29..
//
//

#ifndef ofxUIDirList_ofxUIDirList_h
#define ofxUIDirList_ofxUIDirList_h

#define SCROLL_WIDTH 15

#include "ofxUIMediaAsset.h"

class ofxUIDirList : public ofxUICanvas
{
public:
    ofxUIDirList() : ofxUICanvas()
    {
        
    }
    ofxUIDirList(string _name, ofDirectory directory, float w, float h, float x = 0, float y = 0, int _size = OFX_UI_FONT_SMALL) : ofxUICanvas(x, y, w, h){
        useReference = false;
        initDirList(_name, &directory);
    }
    ofxUIDirList(string _name, ofDirectory *directory, float w, float h, float x = 0, float y = 0, int _size = OFX_UI_FONT_SMALL) : ofxUICanvas(x, y, w, h){
        useReference = true;
        initDirList(_name, directory);
    }
    ofxUIDirList(string _name, string path, float w, float h, float x = 0, float y = 0, int _size = OFX_UI_FONT_SMALL) : ofxUICanvas(x, y, w, h){
        useReference = false;
        ofDirectory directory;
        directory.listDir(path);
        initDirList(_name, &directory);
    }
    
    void initDirList(string _name, ofDirectory *directory)
    {
        kind = OFX_UI_WIDGET_DIRLIST;
        setWidgetSpacing(0);
		
		name = _name;
		
        if(useReference)
        {
            dir = directory;
        }
        else
        {
            dir = new ofDirectory();
            *dir = *directory;
        }
		
		label = new ofxUILabel(0,0,(name+" LABEL"), name, OFX_UI_FONT_MEDIUM);
		addEmbeddedWidget(label);
		
		listUI = (ofxUIScrollableCanvas* )addWidgetLeft(new ofxUIScrollableCanvas(0, 0, rect->width - SCROLL_WIDTH-padding, rect->height));
        listUI->setDamping(0.1);
		listUI->setStickyDistance(5);
		listUI->setWidgetSpacing(0);
		listUI->setPadding(padding);
		listUI->setDrawBack(false);

        for(int i=0; i<dir->numFiles(); i++){
            listUI->addWidgetDown(new ofxUIMediaAsset(dir->getFile(i),300,100));
        }
		listUI->autoSizeToFitWidgets();
		
		scrollbar = (ofxUIRangeSlider *) addWidgetRight(new ofxUIRangeSlider("scrollbar", dir->numFiles(), 1, 1, 0, SCROLL_WIDTH, rect->getHeight()));
		scrollbar->setLabelVisible(false);
		scrollbar->setLabelPrecision(0);
		
    }
	
	void update(){
		if(scrollbar->isHit(ofGetMouseX(), ofGetMouseY())){
			float diff = (scrollbar->getPercentValueHigh()-scrollbar->getPercentValueLow());
			float h = listUI->getSRect()->getHeight()/round(diff*dir->numFiles());
			
			vector <ofxUIWidget *> listedWidgets = listUI->getWidgetsOfType(OFX_UI_WIDGET_MEDIAASSET);			
			for(int i=0; i<listedWidgets.size(); i++){
				ofxUIMediaAsset * widget = (ofxUIMediaAsset*)listedWidgets[i];
				widget->getRect()->height = h-widget->getPadding();
				widget->getRect()->y = (h*i);
				widget->setParent(listUI);
			}
			listUI->autoSizeToFitWidgets();
			listUI->setScrollPosY(1-scrollbar->getPercentValueHigh());
		}else{
			scrollbar->setValueHigh(ofMap(listUI->getScrollPosMin().y, 0, 1, 1, dir->numFiles(),true));
			scrollbar->setValueLow(ofMap(listUI->getScrollPosMax().y, 0, 1, 1, dir->numFiles(),true));
		}
		
		ofxUICanvas::update();
	}
	
	
	bool isDraggable()
    {
        return true;
    }
	
protected:
	ofxUIScrollableCanvas *listUI;
	ofxUIRangeSlider* scrollbar;
	
    ofDirectory *dir;
    bool useReference;
    bool drawLabel;
	
	ofxUILabel *label;
};


#endif
