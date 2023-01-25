#pragma once

#include "ofMain.h"
#include "listenerThread.h"
//#include "deleterThread.h"
#include "ofxXmlSettings.h"
#include "ofxImGui.h"



enum CornerGrabb
{
	NOTHING,
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT
};

class PrinterManager {
public:
	void setup();
	void update();
	void draw();

	void drawGui();
	void drawRects();
	void loadSettings();
	void saveSettings();

	std::string removeSpaces(std::string givenText);

	bool HandleWindowResize();

	inline ListenerThreadObject& getListener() {
		return listenerObject;
	}

	inline ofxImGui::Gui& GetGui() { //zwraca gui
		return gui;
	}


	void mouseReleased(const int& x, const int& y, const int& button);
	void mousePressed(const int& x, const int& y, const int& button);

	void mouseDragged(const int& x, const int& y, const int& button);
	void mouseMoved(const int& x, const int& y);

protected:
	ListenerThreadObject listenerObject;
	//DeleterThreadObject deleteTaskFunction;
	ofxImGui::Gui gui;
	ImVec4 backgroundColor;
	std::vector<std::string> loadedPrinters;
	int updateFrequency = 1000;
	int previousUpdateFrequency = 1000;

	std::map<std::string, int> printers;

	bool automaticJobDelete = false;

	ofxXmlSettings printersSettings;

	std::string selectedPrinter = "";
	int selectedPrinterMaxJobs = 4;

	float m_initialWidth;
	float m_initialHeight;
	float m_width;
	float m_height;

	bool keepProportion16to9 = false;
	bool keepProportion9to16 = false;

	int scaleX = 680;
	int scaleY = 470;

	ofFbo fbo;
	ofImage myImg;
	ofPixels pixelsButtonSource;
	GLuint pixelsButtonID;

	bool bManualMode = false;
	int imagePosGui[4];
	ofVec2f appSize = ofVec2f(1024, 768);
	//Image scaling
public:
	ofImage img;
protected:
	ofVec2f startGrabPos;
	ofVec2f grabbedImageSize = ofVec2f(500, 500);
	ofVec2f imagePosition = ofVec2f(100, 100);

	int rectSize = 50;
	ofRectangle topLeftGrabIndicator = ofRectangle(0.0f, 0.0f, rectSize, rectSize);
	ofRectangle topRightGrabIndicator = ofRectangle(0.0f, 0.0f, rectSize, rectSize);
	ofRectangle bottomLeftGrabIndicator = ofRectangle(0.0f, 0.0f, rectSize, rectSize);
	ofRectangle bottomRightGrabIndicator = ofRectangle(0.0f, 0.0f, rectSize, rectSize);

	CornerGrabb cornerGrabb = NOTHING;
	CornerGrabb overGrabPoint = NOTHING;
};

