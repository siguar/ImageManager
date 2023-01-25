#include "listenerThread.h"
#include "ofApp.h"

void ListenerThreadObject::threadedFunction(){
    while (isThreadRunning()) {
        std::cout << "\n" << x.printerQuee(printerVar);

        std::vector<std::string> loadedPrinters;
        x.getPrinters(loadedPrinters);

        for (const auto& item : loadedPrinters){
            std::cout << "DDD: " << item << std::endl;

            int pendingJobs = x.getPrinterJobsNum(item);

            
            printers[item].pendingJobs = pendingJobs;

            if (printers.count(item)){
                
                std::cout << "fff: " << pendingJobs << std::endl;

                if (pendingJobs >= printers[item].maxJobs){
                    if (automaticJobDelete){
                        PrinterJobDeleter.taskDeleter(item);
                    }
                }

            }

        }
        

        ofSleepMillis(updateFrequency);

    }
}