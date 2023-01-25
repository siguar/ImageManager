#pragma once
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <vector>
#include <cstring>
#include <map>
#include <Windows.h>


class printQ {


public:
    std::string exec(const char* cmd) {
        char buffer[128];
        std::string result = "";
        FILE* pipe = _popen(cmd, "r");
        if (!pipe) throw std::runtime_error("popen() failed!");
        try {
            while (fgets(buffer, sizeof buffer, pipe) != NULL) {
                result += buffer;
            }
        }
        catch (...) {
            _pclose(pipe);
            throw;
        }
        _pclose(pipe);
        return result;


    }
    int printerQuee(const std::string& luuk) {
        std::map<std::string, int> counterDefaultMap;

            std::string printerName = exec("powershell \"Get-Printer | Format-List Name\"");
            
            std::string delimiter = "\n";
            size_t pos = 0;
            std::vector <std::string> vectorOfPrinters;


            while ((pos = printerName.find(delimiter)) != std::string::npos) {
                std::string token = printerName.substr(0, pos);
                vectorOfPrinters.push_back(token);
                printerName.erase(0, pos + delimiter.length());
            }

            for (auto item : vectorOfPrinters)
            {
                if (item.substr(0, 4) == "Name")
                {
                    std::string tempValue = item;
                    tempValue.erase(0, 7);

                    if (counterDefaultMap.count(tempValue)) {
                        counterDefaultMap[tempValue]++;
                    }
                    else {
                        counterDefaultMap.insert({ tempValue, 0 });
                    }
                }
            }

            for (auto& element : counterDefaultMap) {
                std::cout << element.first << " -> " << element.second << '\n';
                if (element.second >= 4) {
                    std::cout << "!warning!\nPRINTER " << element.first << " OVERLOAD\n";
                }
            }
           
            if (counterDefaultMap.count(luuk))
            {

                return counterDefaultMap[luuk]+1;
            }
            else {
                return -1;//nigdy nie bedzie ujemnej liczby tasków w queue dlatego -1 oznacza b³¹d
            }
    }

    void getPrinters(std::vector<std::string>& printers) {

        std::string printerName = exec("powershell \"Get-Printer | Format-List Name\"");

        std::string delimiter = "\n";
        size_t pos = 0;
        std::vector <std::string> vectorOfPrinters;

        std::cout << printerName << std::endl;

        while ((pos = printerName.find(delimiter)) != std::string::npos) {
            std::string token = printerName.substr(0, pos);
            vectorOfPrinters.push_back(token);
            printerName.erase(0, pos + delimiter.length());
        }

        for (auto item : vectorOfPrinters)
        {
            if (item.substr(0, 4) == "Name")
            {
                
                std::string tempValue = item;
                tempValue.erase(0, 7);
                
                printers.push_back(tempValue);
            }
        }
    }

    int getPrinterJobsNum(const std::string& printerName)
    {
        std::string result = exec(("powershell \"(Get-Printer -Name '" + printerName + "' | Get-PrintJob).Count\"").c_str());
        
        if (result.length() == 0)
        {
            return 0;
        }

        result.erase(std::remove(result.begin(), result.end(), '\n'), result.cend());

        for (const char& ch : result)
        {
            if (!std::isdigit(ch))
            {
                return -1;
            }
        }
        
        //std::cout << ("powershell \"(Get-Printer -Name '" + printerName + "' | Get-PrintJob).Count\"").c_str();
        
        return stoi(result);
    }
};