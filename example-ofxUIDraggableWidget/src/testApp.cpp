#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    gui = new ofxUIDraggableCanvas(0,0,320,320);
    gui->addWidgetDown(new ofxUILabel("OFXUI TUTORIAL", OFX_UI_FONT_LARGE)); 
    gui->addSlider("BACKGROUND VALUE",0.0,255.0,100.0,304,16);
    gui->addWidgetDown(new ofxUIToggle(32, 32, false, "FULLSCREEN"));
    gui->setName("DEFUALT");
    
    ofxUIDraggableCanvas * draggable = (ofxUIDraggableCanvas *) gui->addWidgetDown(new ofxUIDraggableCanvas(0, 0, 100, 100, gui));
    draggable->addLabel("PARENT");
    draggable->setName("DRAGGABLE");
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    ofAddListener(draggable->newGUIEvent, this, &testApp::guiEvent); 
    gui->loadSettings("GUI/guiSettings.xml"); 
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
    else if(e.widget->getKind() == OFX_UI_WIDGET_DRAGGABLE_CANVAS)
    {
        ofxUIDraggableCanvas * canvas = (ofxUIDraggableCanvas *) e.widget;
        ofxUILabel * label = (ofxUILabel *)canvas->getWidget("PARENT");
        label->setLabel(canvas->getParent()->getName());
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