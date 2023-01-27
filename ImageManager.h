#pragma once
#include "ofxXmlSettings.h"
#include "ofxImGui.h"


//ENUM G£ÓWNY
enum CornerGrabb
{
	NOTHING,
	CENTRAL,
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT

};


//=================================================================================================================
//DEKLARACJA KLASY MANAGERA
class ImageManager
{
public:
	ImageManager() {};
	ImageManager(ofImage &imgTempVal);


	ofImage *imgVal;



	bool keepProportion16to9 = false;
	bool keepProportion9to16 = false;

//==============================================================================================================
	void draw();
	void update();
	void drawRects();
	void mouseReleased(const int& x, const int& y, const int& button);
	void mousePressed(const int& x, const int& y, const int& button);

	void mouseDragged(const int& x, const int& y, const int& button);
	void mouseMoved(const int& x, const int& y);
	void drawGui();

	void imageSettingsSaver();
	void imageSettingsLoader();


//===============================================================================================================
public:
	ofVec2f startGrabPos;
	ofVec2f grabbedImageSize = ofVec2f(500, 500);
	ofVec2f imagePosition = ofVec2f(100, 100);
	int rectSize = 50;
	
	ofRectangle topLeftGrabIndicator = ofRectangle(0.0f, 0.0f, rectSize, rectSize);
	ofRectangle topRightGrabIndicator = ofRectangle(0.0f, 0.0f, rectSize, rectSize);
	ofRectangle bottomLeftGrabIndicator = ofRectangle(0.0f, 0.0f, rectSize, rectSize);
	ofRectangle bottomRightGrabIndicator = ofRectangle(0.0f, 0.0f, rectSize, rectSize);
	ofRectangle centralGrabIndicator = ofRectangle();

	CornerGrabb cornerGrabb = NOTHING;
	CornerGrabb overGrabPoint = NOTHING;

	ofxXmlSettings imgSettingsSaver;


	bool bManualMode = false;

	int scaleX = 680;
	int scaleY = 470;

	int imagePosGui[4];
	std::string settingsID;
	ofVec2f appSize = ofVec2f(1024, 768);
};

