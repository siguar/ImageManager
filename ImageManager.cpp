#include "ImageManager.h"



ImageManager::ImageManager(ofImage& imgTempVal)
{
	imgVal = &imgTempVal;
}
void ImageManager::draw()
{
	

	imgVal->draw(imagePosition, grabbedImageSize.x, grabbedImageSize.y);

	if (bManualMode) {
		drawRects();
	}
}

void ImageManager::update()
{
	
	if (bManualMode) {
		topLeftGrabIndicator.setPosition(imagePosition.x, imagePosition.y);
		//topLeftGrabIndicator.set(imagePosition.x - rectSize/2, imagePosition.y - rectSize/2, rectSize, rectSize);
		topRightGrabIndicator.setPosition(imagePosition.x - topRightGrabIndicator.width + grabbedImageSize.x, imagePosition.y);
		bottomLeftGrabIndicator.setPosition(imagePosition.x, imagePosition.y + grabbedImageSize.y - bottomLeftGrabIndicator.width);
		bottomRightGrabIndicator.setPosition(imagePosition.x - bottomRightGrabIndicator.width + grabbedImageSize.x, imagePosition.y + grabbedImageSize.y - bottomRightGrabIndicator.width);
	}
}

void ImageManager::drawRects()
{
	ofSetColor(255, 0, 0);
	ofNoFill();
	ofDrawRectangle(topLeftGrabIndicator);
	ofDrawRectangle(topRightGrabIndicator);
	ofDrawRectangle(bottomLeftGrabIndicator);
	ofDrawRectangle(bottomRightGrabIndicator);

	//ofDrawCircle(topLeftGrabIndicator.x, topLeftGrabIndicator.y, 50);

	ofSetColor(255, 255, 255);
}

void ImageManager::mouseReleased(const int& x, const int& y, const int& button)
{
}
void ImageManager::mousePressed(const int& x, const int& y, const int& button)
{
	startGrabPos = ofVec2f(x, y);
	centralGrabIndicator = ofRectangle(imagePosition.x ,imagePosition.y, grabbedImageSize.x, grabbedImageSize.y);

	if (topLeftGrabIndicator.inside(x, y)) {
		cornerGrabb = TOP_LEFT;
		
	}
	else if (topRightGrabIndicator.inside(x, y)) {
		cornerGrabb = TOP_RIGHT;
		
	}
	else if (bottomLeftGrabIndicator.inside(x, y)) {
		cornerGrabb = BOTTOM_LEFT;
		
	}
	else if (bottomRightGrabIndicator.inside(x, y)) {
		cornerGrabb = BOTTOM_RIGHT;
		
	}
	else if (centralGrabIndicator.inside(x, y)) {
		cornerGrabb = CENTRAL;

		

	}
	else {
		cornerGrabb = NOTHING;
	}
	
}
void ImageManager::mouseDragged(const int& x, const int& y, const int& button)
{
	if (bManualMode) {
		switch (cornerGrabb)
		{
		case NOTHING:
		{

		}
		break;

		case CENTRAL:
		{
			ofVec2f grabDistance = startGrabPos - ofVec2f(x, y);
			imagePosition -= grabDistance;
			startGrabPos -= grabDistance;
		}
		break;

		case TOP_LEFT:
		{
			ofVec2f grabDistance = startGrabPos - ofVec2f(x, y);
			grabbedImageSize += grabDistance;
			startGrabPos -= grabDistance;

			if (keepProportion16to9) {
				imagePosition.x -= grabDistance.x;
				imagePosition.y -= grabDistance.x * 9 / 16;
			}
			else if (keepProportion9to16)
			{
				imagePosition.x -= grabDistance.y * 9 / 16;
				imagePosition.y -= grabDistance.y;
			}
			else {
				imagePosition -= grabDistance;
			}
		}
		break;
		case TOP_RIGHT:
		{
			ofVec2f grabDistance = startGrabPos - ofVec2f(x, y);
			grabbedImageSize.x -= grabDistance.x;
			grabbedImageSize.y += grabDistance.y;
			startGrabPos -= grabDistance;


			if (keepProportion16to9) {
				imagePosition.y += grabDistance.x * 9 / 16;
			}
			else if (keepProportion9to16)
			{
				imagePosition.y -= grabDistance.y;
			}
			else {
				imagePosition.y -= grabDistance.y;
			}
		}
		break;
		case BOTTOM_LEFT:
		{

			ofVec2f grabDistance = startGrabPos - ofVec2f(x, y);
			grabbedImageSize.x += grabDistance.x;
			grabbedImageSize.y -= grabDistance.y;
			startGrabPos -= grabDistance;


			if (keepProportion9to16)
			{
				imagePosition.x += grabDistance.y * 9 / 16;
			}
			else
			{
				imagePosition.x -= grabDistance.x;
			}
		}
		break;
		case BOTTOM_RIGHT:
		{
			ofVec2f grabDistance = startGrabPos - ofVec2f(x, y);
			grabbedImageSize -= grabDistance;
			startGrabPos -= grabDistance;
		}
		break;
		default:

			break;
		}

		if (keepProportion16to9)
		{
			grabbedImageSize.y = grabbedImageSize.x * 9 / 16;
		}
		else if (keepProportion9to16)
		{
			grabbedImageSize.x = grabbedImageSize.y * 9 / 16;
		}
	}
}
void ImageManager::mouseMoved(const int& x, const int& y)
{
	overGrabPoint = NOTHING;

	if (topLeftGrabIndicator.inside(x, y)) {
		overGrabPoint = TOP_LEFT;
	}
	else if (topRightGrabIndicator.inside(x, y)) {
		overGrabPoint = TOP_RIGHT;
	}
	else if (bottomLeftGrabIndicator.inside(x, y)) {
		overGrabPoint = BOTTOM_LEFT;
	}
	else if (bottomRightGrabIndicator.inside(x, y)) {
		overGrabPoint = BOTTOM_RIGHT;
	}
}
;