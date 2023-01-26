#include "ofApp.h"
#include "printQ.h"
#include "listenerThread.h"
//--------------------------------------------------------------
void ofApp::setup(){
	MyPrinterManager.setup();

	MyPrinterManager.img.loadImage("x.png");
	

	
	//MyPrinterManager.listenerObject.GetPrinters()[0].maxJobs
}

//--------------------------------------------------------------
void ofApp::update(){
	MyPrinterManager.update();

	
}

//--------------------------------------------------------------
void ofApp::draw(){
	MyPrinterManager.draw();
	//img.resize(grabbedImageSize.y, grabbedImageSize.x);

	


}

void ofApp::exit()
{
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	MyPrinterManager.mouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	MyPrinterManager.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

	MyPrinterManager.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	MyPrinterManager.mouseReleased(x, y,button);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
