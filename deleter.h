#pragma once
//g³ówna klasa deletera z funkcj¹ do usuwania tasków z kolejki 
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <vector>
#include <cstring>
#include <map>
#include <Windows.h>

class Deleter {
public:
	
	void taskDeleter(const std::string& printername) {
		
		//std::cout << commandWriter.exec("powershell Get-Printer -Name \"1\" |powershell Get-PrintJob |powershell Remove-PrintJob");
		system(("powershell \"Get-Printer -Name '" + printername + "' | Get-PrintJob | Remove-PrintJob\"").c_str());
	}

};