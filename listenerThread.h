#pragma once
#include "ofMain.h"
#include <atomic>
#include "printQ.h"
#include "deleter.h"
#include "ofxImGui.h"
#include <chrono>

struct PrinterStats{
    int pendingJobs = 0;
    int maxJobs = 4;
};

class ListenerThreadObject : public ofThread{
    int updateFrequency = 1000;

    std::map<std::string, PrinterStats> printers;
public:
   
    bool automaticJobDelete = false;
    
    
    
    Deleter PrinterJobDeleter;

    std::string printerVar; ;
    void start() {
        startThread();
    }
    printQ x;
    void setup() {
        
        start();
    }

    void threadedFunction();
        
    inline int getUpdateFrequency() { //zwraca freq
        return updateFrequency;
    }
    inline void setUpdateFrequency(int value) { //do ustawiania freq
        updateFrequency = value;
    }
    inline void setPrinterQueueCounter(string printerName, int value) { //do ustawiania maxymalnej kolejki drukarki
        if (!printers.count(printerName)) {
            std::cout << "!!!ERROR BAD PRINTER NAME!!!";
            return;
        }
        printers[printerName].maxJobs = value;
    }

    inline std::map<std::string, PrinterStats>& GetPrinters() { //zwraca mape drukarek : -nazwy drukarek, -jaka jest ustawiona maxymalna kolejka, -ile jest obecnie elementów w kolejce
        return printers;
    }
};