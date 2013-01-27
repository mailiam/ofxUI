#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    ofSetLogLevel(OF_LOG_VERBOSE);
    dir.listDir("~/desktop/");
    //dirList = new ofxUIDirList("DIRECTORY", dir, 300, 300,100,100);
    //dirList ->setScrollAreaToScreen();
    //dirList ->setScrollableDirections(false, true);
    //dirList ->setDrawBack(false);
    //dirList ->autoSizeToFitWidgets();
    
    gui = new ofxUICanvas(0,0,320,320);
    gui->addWidgetDown(new ofxUILabel("OFXUI TUTORIAL", OFX_UI_FONT_LARGE)); 
    gui->addSlider("BACKGROUND VALUE",0.0,255.0,100.0,304,16);
    gui->addWidgetDown(new ofxUIToggle(32, 32, false, "FULLSCREEN"));
    //gui->addWidgetDown(new ofxUIScrollableCanvas(0, 0, 300, 300));
    dirList = (ofxUIDirList* )gui->addWidgetDown(new ofxUIDirList("DIRECTORY", &dir, 300, 300));
    ofRectangle * rect = dirList->getRect();
    dirList -> setScrollArea(rect->x, rect->y, rect->width, rect->height);
    dirList ->autoSizeToFitWidgets();
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    //gui->loadSettings("GUI/guiSettings.xml");
    gui->autoSizeToFitWidgets();
    
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

}
//--------------------------------------------------------------
void testApp::exit()
{
    //gui->saveSettings("GUI/guiSettings.xml");
    delete gui; 
}
//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
    if(e.widget->getName() == "BACKGROUND VALUE")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;    
        ofBackground(slider->getScaledValue());
    }
    else if(e.widget->getName() == "FULLSCREEN")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        ofSetFullscreen(toggle->getValue());   
    }
}
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    switch (key) {
        case 'p':
            gui->setDrawWidgetPadding(true);
            break;
        case 'P':
            gui->setDrawWidgetPadding(false);
            break;

            
        default:
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}