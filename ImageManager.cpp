#include "ImageManager.h"



ImageManager::ImageManager(ofImage& imgTempVal)
{
	imgVal = &imgTempVal;
}
void ImageManager::draw()
{
	
	if (imgVal) {
		imgVal->draw(imagePosition, grabbedImageSize.x, grabbedImageSize.y);
	}
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
void ImageManager::drawGui()
{
	ImGui::BeginChild("imageManagerGui");

	if (ImGui::Checkbox("Keep Proportion 16:9", &keepProportion16to9))
	{
		keepProportion9to16 = false;
	}
	if (ImGui::Checkbox("Keep Proportion 9:16", &keepProportion9to16))
	{
		keepProportion16to9 = false;
	}
	if (ImGui::Checkbox("Keep Proportion 3:2", &keepProportion9to16))
	{
		keepProportion9to16 = false;
		keepProportion16to9 = false;
	}

	ImGui::BeginChild("EnterScale");
	ImGui::InputInt("Scale X", &scaleX);
	ImGui::InputInt("Scale Y", &scaleY);

	if (ImGui::Button("ApplyScale"))
	{
		keepProportion16to9 = false;
		keepProportion9to16 = false;

		grabbedImageSize.x = scaleX;
		grabbedImageSize.y = scaleY;
	}
	ImGui::EndChild();

	ImGui::Checkbox("manual mode", &bManualMode);
	ImGui::DragInt("rect size", &rectSize, 1, 1, 100);
	if (ImGui::DragInt4("img pos & size", imagePosGui, 1, 1, 10000)) {

		imagePosition.x = imagePosGui[0];
		imagePosition.y = imagePosGui[1];
		grabbedImageSize.x = imagePosGui[2];
		grabbedImageSize.y = imagePosGui[3];
	}
	if (ImGui::Button("center x")) {
		// jaki jest rozmiar monitora/

		imagePosition.x = (appSize.x / 2 - grabbedImageSize.x / 2);
		imagePosGui[0] = imagePosition.x;
	}
	ImGui::SameLine();
	if (ImGui::Button("center y")) {

		imagePosition.y = (appSize.y / 2 - grabbedImageSize.y / 2);
		imagePosGui[1] = imagePosition.y;
	}
	ImGui::SameLine();
	if (ImGui::Button("center all")) {
		imagePosition.x = (appSize.x / 2 - grabbedImageSize.x / 2);
		imagePosGui[0] = imagePosition.x;

		imagePosition.y = (appSize.y / 2 - grabbedImageSize.y / 2);
		imagePosGui[1] = imagePosition.y;
	}
	ImGui::EndChild();
}
void ImageManager::imageSettingsSaver()
{	//POMYSL JAK TO ZROBIC !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
	//zapis ustawien do xml ktore sie spodobaly uzytkownikowi tak by pozniej wyswietlaly sie w rozwijanej liscie do wyboru przez uzytkownika
	//=====================================================================================================================
	
	std::string settingsVal = settingsID + ".xml";

	imgSettingsSaver.setValue("settings:scallableImageSettings:sizeX", grabbedImageSize.x);
	imgSettingsSaver.setValue("settings:scallableImageSettings:sizeY", grabbedImageSize.y);//ustawienia rozdzielczosci obrazu
	imgSettingsSaver.setValue("settings:scallableImageSettings:posX", imagePosition.x);
	imgSettingsSaver.setValue("settings:scallableImageSettings:posY", imagePosition.y);//ustawienia pozycji obrazu w oknie OF
	imgSettingsSaver.setValue("settings:scallableImageSettings:ID", settingsID);

	imgSettingsSaver.saveFile(settingsVal); //jak sie chce podac miejsce inne niz "data" miejsce zapisu to nalezy podac nazwe wraz ze sciezka
}
void ImageManager::imageSettingsLoader()
{	
	std::string settingsVal = settingsID + ".xml";
	if (imgSettingsSaver.loadFile(settingsVal)) 
	{
		grabbedImageSize.x = imgSettingsSaver.getValue("settings:scallableImageSettings:sizeX", 680);
			grabbedImageSize.y = imgSettingsSaver.getValue("settings:scallableImageSettings:sizeY", 680);
			imagePosition.x = imgSettingsSaver.getValue("settings:scallableImageSettings:posX", 680);
			imagePosition.y = imgSettingsSaver.getValue("settings:scallableImageSettings:posY", 680);
			settingsID = imgSettingsSaver.getValue("settings:scallableImageSettings:ID", 0);


			imagePosGui[0] = imagePosition.x;
			imagePosGui[1] = imagePosition.y;
			imagePosGui[2] = grabbedImageSize.x;
			imagePosGui[3] = grabbedImageSize.y;
	}
}
;