#include "PrinterManager.h"
#include "printQ.h"
#include "listenerThread.h"

//--------------------------------------------------------------
void PrinterManager::setup() {

	//myImg.load("C:/Users/f/Documents/of_v0.11.2_vs2017_release/apps/myApps/X/src/x.png");
	
	
	//ofFbo::Settings fboSettings;
	//fboSettings.width = 200;
	//fboSettings.height = 200;
	//fboSettings.internalformat = GL_RGBA;
	//fboSettings.textureTarget = GL_TEXTURE_2D;
	
	//fbo.attachTexture(myImg.getTexture(), GL_RGBA, 0);
	//fbo.allocate(fboSettings);

	ofLoadImage(pixelsButtonSource, "y.png");				//obraz do skalowania
	pixelsButtonID = gui.loadPixels(pixelsButtonSource); 
	

	ofSetLogLevel(OF_LOG_VERBOSE);

	
	listenerObject.x.getPrinters(loadedPrinters);

	

	gui.setup(nullptr, true, ImGuiConfigFlags_ViewportsEnable);
	backgroundColor = ofColor(114, 144, 154);

	listenerObject.setup();
	listenerObject.lock();
	listenerObject.printerVar = "1"; //przekazanie do watku nazwy drukarki ktorej kolejke chcemy nasluchiwac

	for (auto& item : loadedPrinters) {
		printers.insert(std::pair<std::string, int>(item, 4));
		PrinterStats TempPrinterStats;
		listenerObject.GetPrinters().insert(std::pair<std::string, PrinterStats>(item, TempPrinterStats));
	}

	//for (const auto& item : loadedPrinters){
	//	PrinterStats TempPrinterStats;
	//	listenerObject.GetPrinters().insert(std::pair<std::string, PrinterStats>(item, TempPrinterStats));
	//}
	listenerObject.unlock();

	loadSettings();


}

//--------------------------------------------------------------
void PrinterManager::update() {
	if (updateFrequency != previousUpdateFrequency){
		listenerObject.lock();
		listenerObject.setUpdateFrequency(updateFrequency);
		listenerObject.unlock();
	}

	previousUpdateFrequency = updateFrequency;
	
	if (bManualMode) {
		topLeftGrabIndicator.setPosition(imagePosition.x, imagePosition.y);
		//topLeftGrabIndicator.set(imagePosition.x - rectSize/2, imagePosition.y - rectSize/2, rectSize, rectSize);
		topRightGrabIndicator.setPosition(imagePosition.x - topRightGrabIndicator.width + grabbedImageSize.x, imagePosition.y);
		bottomLeftGrabIndicator.setPosition(imagePosition.x, imagePosition.y + grabbedImageSize.y - bottomLeftGrabIndicator.width);
		bottomRightGrabIndicator.setPosition(imagePosition.x - bottomRightGrabIndicator.width + grabbedImageSize.x, imagePosition.y + grabbedImageSize.y - bottomRightGrabIndicator.width);
	}
//odrazu zrobic tu jak¹œ flage zeby te 4 badziewia sie wykonywaly tylko gdy uzytkownik tego potrzebuje
	//rectangle maj¹ sie nie pojawiac tylko sie statycznie wlaczac gdy jest uruchomiony 
}

//--------------------------------------------------------------
void PrinterManager::draw() {
	
	drawGui();

	img.draw(imagePosition, grabbedImageSize.x, grabbedImageSize.y);

	if (bManualMode) {
		drawRects();
	}



	//ImGui::GetWindowPos();
	//ImVec2 x = ImGui::GetWindowPos();
	

	//bool open = false;
	//ImGui::SetNextWindowSize(ImVec2(m_initialWidth, m_initialHeight), ImGuiCond_Once);
	//ImGui::SetNextWindowPos(x, ImGuiCond_Once);
	//::Begin("resizable image");
	
	//ImGui::Image((ImTextureID)(uintptr_t)(myImg.getTexture().getTextureData().textureID), ImVec2(480, 480), ImVec2(1, 1), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
	//ImGui::GetWindowDrawList()->AddImage((void *)texColorBuffer, ImVec2(ImGui::GetCursorScreenPos()), ImVec2(ImGui::GetCursorScreenPos().x + 800 , ImGui::GetCursorScreenPos().y + 600 ), ImVec2(0, 1), ImVec2(1, 0));
	//ImVec2 availableSize = ImGui::GetContentRegionAvail();

	//if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) || m_width == 0) {
	//	// std::cout << availableSize.x << " " << availableSize.y << std::endl;
	//	// should I resize my FBO here
	//	if (availableSize.x != m_width || availableSize.y != m_height)
	//	{
	//		m_width = availableSize.x;
	//		m_height = availableSize.y;
	//		//m_height = m_width * 0.5625;
	//		//m_isFrameBufferDirty = true;
	//	}
	//}
	////draw FBO
	//ImGui::Image(GetImTextureID(pixelsButtonID), ImVec2(m_width, m_height)); //obraz
	////ImGui::EndChild();

	////ImGui::EndChild();

	////ImGui::BeginChild("asdf");
	////std::cout << ImGui::GetWindowPos();
	//ImGui::End();
	//ImGui::End();
	//ImGui::EndChild();

}

void PrinterManager::drawGui() {
	gui.begin();
	int test = 0;
	ImGuiWindowFlags yvalue = ImGuiWindowFlags_NoBringToFrontOnFocus;
	bool xvalue;

	ImGui::Begin("Scrolling", &xvalue, yvalue);

	ImGui::Text("Update Frequency: ");
	ImGui::InputInt("updateFrequency", &updateFrequency);
	ImGui::Separator();

	for (auto& item : printers) {
		if (ImGui::Button(item.first.c_str())) {
			selectedPrinter = item.first;
			selectedPrinterMaxJobs = listenerObject.GetPrinters()[selectedPrinter].maxJobs;
			std::cout << "ABC" << std::endl;
		}
	}

	if (selectedPrinter != "") {
		ImGui::OpenPopup("ManagePrinter");
		if (ImGui::BeginPopupModal("ManagePrinter", NULL)) {
			if (listenerObject.GetPrinters().count(selectedPrinter)) {
				ImGui::InputInt((selectedPrinter).c_str(), &selectedPrinterMaxJobs);
			}
			else
				ImGui::Text(("Printer " + selectedPrinter + " not found").c_str());
			if (ImGui::Button("Ok")) {
				if (listenerObject.GetPrinters().count(selectedPrinter))
					listenerObject.GetPrinters()[selectedPrinter].maxJobs = selectedPrinterMaxJobs;

				selectedPrinter = "";

				ImGui::CloseCurrentPopup();
			}

			if (!automaticJobDelete && listenerObject.GetPrinters()[selectedPrinter].pendingJobs >= selectedPrinterMaxJobs)
				if (ImGui::Button("Clear Printer Queue")) {
					listenerObject.PrinterJobDeleter.taskDeleter(selectedPrinter);
					listenerObject.GetPrinters()[selectedPrinter].pendingJobs = 0;
				}

			ImGui::EndPopup();
		}
	}

	ImGui::Checkbox("Automatic Job Deletetion", &listenerObject.automaticJobDelete);

	if (!listenerObject.automaticJobDelete) {
		for (auto const& item : listenerObject.GetPrinters()) {
			if (item.second.pendingJobs >= listenerObject.GetPrinters()[item.first].maxJobs)
				ImGui::Text(("Printer " + item.first + " exceeded jobs limit. Pending jobs %i").c_str(), item.second.pendingJobs);
		}
	}

	if (ImGui::Button("Save Settings")) {
		for (const auto& item : listenerObject.GetPrinters()) {
			printersSettings.setValue("settings:printers:d" + removeSpaces(item.first) + ":maxJobs", item.second.maxJobs);
		}

		printersSettings.setValue("settings:deleteJobsAutomatically", listenerObject.automaticJobDelete);
		printersSettings.setValue("settings:updateFrequency", listenerObject.getUpdateFrequency());

		printersSettings.setValue("settings:scallableImageSettings:sizeX", grabbedImageSize.x);
		printersSettings.setValue("settings:scallableImageSettings:sizeY", grabbedImageSize.y);//ustawienia rozdzielczosci obrazu
		printersSettings.setValue("settings:scallableImageSettings:posX", imagePosition.x);
		printersSettings.setValue("settings:scallableImageSettings:posY", imagePosition.y);//ustawienia pozycji obrazu w oknie OF
		printersSettings.saveFile("printersSettings.xml");
	}

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

		imagePosition.x = (appSize.x/2 - grabbedImageSize.x/2);
		imagePosGui[0] = imagePosition.x;
	}
	ImGui::SameLine();
	if (ImGui::Button("center y")) {

		imagePosition.y = (appSize.y/ 2 - grabbedImageSize.y / 2);
		imagePosGui[1] = imagePosition.y;
	}
	ImGui::SameLine();
	if (ImGui::Button("center all")) {
		imagePosition.x = (appSize.x / 2 - grabbedImageSize.x / 2);
		imagePosGui[0] = imagePosition.x;
		imagePosition.y = (appSize.y / 2 - grabbedImageSize.y / 2);
		imagePosGui[1] = imagePosition.y;
	}
	gui.end();
}

/*


	ratioMode
	std::vectorK

	16:9
	9:16 ? 
	3:2


*/

void PrinterManager::drawRects() {

	
	//if (overGrabPoint == TOP_LEFT)
	//	ofDrawRectangle(topLeftGrabIndicator);
	//else if (overGrabPoint == TOP_RIGHT)
	//	ofDrawRectangle(topRightGrabIndicator);
	//else if (overGrabPoint == BOTTOM_LEFT)
	//	ofDrawRectangle(bottomLeftGrabIndicator);
	//else if (overGrabPoint == BOTTOM_RIGHT)
	//	ofDrawRectangle(bottomRightGrabIndicator);
	ofSetColor(255, 0, 0);
	ofNoFill();
	ofDrawRectangle(topLeftGrabIndicator);
	ofDrawRectangle(topRightGrabIndicator);
	ofDrawRectangle(bottomLeftGrabIndicator);
	ofDrawRectangle(bottomRightGrabIndicator);

	ofDrawCircle(topLeftGrabIndicator.x, topLeftGrabIndicator.y, 50);

	ofSetColor(255, 255, 255);
}

void PrinterManager::loadSettings() {
	if (printersSettings.loadFile("printersSettings.xml")) {

		for (const auto& item : loadedPrinters) {
			if (listenerObject.GetPrinters().count(item))
				listenerObject.GetPrinters()[item].maxJobs = printersSettings.getValue("settings:printers:d" + removeSpaces(item) + ":maxJobs", 4);
		}

		listenerObject.setUpdateFrequency(printersSettings.getValue("settings:updateFrequency", 10000));

		grabbedImageSize.x = printersSettings.getValue("settings:scallableImageSettings:sizeX", 680);
		grabbedImageSize.y = printersSettings.getValue("settings:scallableImageSettings:sizeY", 680);
		imagePosition.x = printersSettings.getValue("settings:scallableImageSettings:posX", 680);
		imagePosition.y = printersSettings.getValue("settings:scallableImageSettings:posY", 680);


		imagePosGui[0] = imagePosition.x;
		imagePosGui[1] = imagePosition.y;
		imagePosGui[2] = grabbedImageSize.x;
		imagePosGui[3] = grabbedImageSize.y;
		// imgX, imgY, imgW, imgH
		

		updateFrequency = listenerObject.getUpdateFrequency();
		previousUpdateFrequency = listenerObject.getUpdateFrequency();
		listenerObject.automaticJobDelete = printersSettings.getValue("settings:deleteJobsAutomatically", false);
	}
}

void PrinterManager::mouseReleased(const int& x, const int& y, const int& button) {
	
	
}

void PrinterManager::mousePressed(const int& x, const int& y, const int& button) {
	startGrabPos = ofVec2f(x, y);

	//=------------------------ funkcja inside
	if (topLeftGrabIndicator.inside(x, y)) {
		cornerGrabb = TOP_LEFT;
		startGrabPos = ofVec2f(x, y);
	}
	else if (topRightGrabIndicator.inside(x, y)) {
		cornerGrabb = TOP_RIGHT;
		startGrabPos = ofVec2f(x, y);
	}
	else if (bottomLeftGrabIndicator.inside(x, y)) {
		cornerGrabb = BOTTOM_LEFT;
		startGrabPos = ofVec2f(x, y);
	}
	else if (bottomRightGrabIndicator.inside(x, y)) {
		cornerGrabb = BOTTOM_RIGHT;
		startGrabPos = ofVec2f(x, y);
	}
	else {
		cornerGrabb = NOTHING;
	}
}

void PrinterManager::mouseDragged(const int& x, const int& y, const int& button) {

	switch (cornerGrabb)
	{
	case NOTHING:
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
		else if(keepProportion9to16)
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

void PrinterManager::mouseMoved(const int& x, const int& y) {
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


std::string PrinterManager::removeSpaces(std::string givenText){
	givenText.erase(remove(givenText.begin(), givenText.end(), ' '), givenText.end());
	return givenText;
}



/// VIEWPORT      OKNO

