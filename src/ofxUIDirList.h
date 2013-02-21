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
		listUI->setScrollableDirections(false, false);
		listUI->setStickyDistance(5);
		listUI->setWidgetSpacing(0);
		listUI->setPadding(padding);
		listUI->setDrawBack(false);
		
		scrollbar = (ofxUIRangeSlider *) addWidgetRight(new ofxUIRangeSlider("SCROLLBAR", dir->numFiles(), 1, 1, 0, SCROLL_WIDTH, rect->getHeight()));
		scrollbar->setLabelVisible(false);
		scrollbar->setLabelPrecision(0);
		
		scrollbar->setValueHigh(1);
		scrollbar->setValueLow(10);
		
		updateList();
		
		ofAddListener(newGUIEvent, this, &ofxUIDirList::guiEvent);
		ofAddListener(listUI->newGUIEvent, this, &ofxUIDirList::guiEvent);
    }
	
	void update(){

		//scrollbar->setValueHigh(ofMap(listUI->getScrollPosMin().y, 0, 1, 1, dir->numFiles(),true));
		//scrollbar->setValueLow(ofMap(listUI->getScrollPosMax().y, 0, 1, 1, dir->numFiles(),true));
		
		ofxUICanvas::update();
	}
	
	void guiEvent(ofxUIEventArgs &e)
	{
		if(e.widget->getName() == "SCROLLBAR")
		{
			refreshView();
		}
		else if(e.widget->getKind() == OFX_UI_WIDGET_MEDIAASSET)
		{
			ofxUIMediaAsset *asset = (ofxUIMediaAsset*) e.widget;
			vector <ofxUIWidget *> listedWidgets = listUI->getWidgetsOfType(OFX_UI_WIDGET_MEDIAASSET);
			for(int i=0; i<listedWidgets.size(); i++){
				ofxUIMediaAsset * widget = (ofxUIMediaAsset*)listedWidgets[i];
				if(widget != e.widget)
					widget->setValue(false);
			}
			if(asset->getValue()) selectedAsset = asset;
			else selectedAsset = NULL;
			
			refreshView();
			triggerEvent(this);
		}else{
			
		}
	}
	
	void updateList(){
		listUI->removeWidgets();
		selectedAsset = NULL;
		
		for(int i=0; i<dir->numFiles(); i++){
			ofxUIMediaAsset * asset = new ofxUIMediaAsset(dir->getFile(i),listUI->getSRect()->width,max(listUI->getSRect()->height/dir->numFiles(), 30.f));
			asset->setAutoSize(false); //disabling highlighting
            listUI->addWidgetDown(asset);
        }
		
		scrollbar->setMaxAndMin(1, dir->numFiles());
		scrollbar->setValueHigh(1);
		scrollbar->setValueLow(10);
		
		if(dir->numFiles()<2) {
			scrollbar->setVisible(false);
		}
		
		refreshView();
	}
	
	void refreshView(){
		if(listUI->getWidgetsOfType(OFX_UI_WIDGET_MEDIAASSET).size()<2) return;
		float diff = (scrollbar->getPercentValueHigh()-scrollbar->getPercentValueLow());
		float listHeight = listUI->getSRect()->getHeight();
		if(selectedAsset&&false) { //disabling highlighting
			float wh = selectedAsset->getRect()->height + selectedAsset->getPadding();
			listHeight -= wh;
			diff -= wh/listUI->getRect()->height;
		}
		float diffValue = round(diff*dir->numFiles());
		float h = (listHeight/max(diffValue, 1.0f));
		
		
		float yy =0;
		
		vector <ofxUIWidget *> listedWidgets = listUI->getWidgetsOfType(OFX_UI_WIDGET_MEDIAASSET);
		for(int i=0; i<listedWidgets.size(); i++){
			ofxUIMediaAsset * widget = (ofxUIMediaAsset*)listedWidgets[i];
			widget->getRect()->height = h-widget->getPadding();
			widget->getRect()->y = yy; //(h*i);
			widget->setParent(listUI);
			yy+=widget->getRect()->height + widget->getPadding();
		}
		listUI->autoSizeToFitWidgets();
		listUI->setScrollPosY(1-scrollbar->getPercentValueHigh());
	}
	
	ofxUIMediaAsset * getSelected(){
		return selectedAsset;
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
	
	ofxUIMediaAsset * selectedAsset;
	
	ofxUILabel *label;
};


#endif
