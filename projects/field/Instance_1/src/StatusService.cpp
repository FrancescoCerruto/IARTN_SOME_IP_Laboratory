// StatusService.cpp
#include <iostream>
#include <thread>
#include <csignal>
#include <atomic>

#include <CommonAPI/CommonAPI.hpp>
#include "StatusStubImpl.hpp"

using namespace std;

// Atomic flag to stop threads safely
std::atomic<bool> keepRunning(true);

// SIGINT handler
void signalHandler(int signum) {
    std::cout << "\nKilling StatusService ..." << std::endl;
    keepRunning = false;
}

void updateFieldData(std::shared_ptr<StatusStubImpl> myService) {
    while (keepRunning) {
        // Incremental step every 5 seconds
        std::this_thread::sleep_for(std::chrono::seconds(5));
        // read internal value
        uint32_t currentValue = myService->getStatusAttribute();
        // incremental step
        uint32_t newValue = currentValue + 1;
        std::cout << "New value: " << newValue << std::endl;
        myService->setStatusAttribute(newValue);
    }
}

int main() {    
    CommonAPI::Runtime::setProperty("LogContext", "E01S");
    CommonAPI::Runtime::setProperty("LogApplication", "E01S");
    CommonAPI::Runtime::setProperty("LibraryBase", "Status");

    std::shared_ptr<CommonAPI::Runtime> runtime = CommonAPI::Runtime::get();

    std::string domain = "local";
    std::string instance = "commonapi.examples.Status.Instance_1";
    std::string connection = "Status_Instance_1_server";

    std::shared_ptr<StatusStubImpl> myService = std::make_shared<StatusStubImpl>();
    bool successfullyRegistered = runtime->registerService(domain, instance, myService, connection);
        
    //SIGINT (Ctrl+C) handler declaration
    signal(SIGINT, signalHandler);

    while (!successfullyRegistered && keepRunning) {
        std::cout << "Register Service failed, trying again in 100 milliseconds..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        successfullyRegistered = runtime->registerService(domain, instance, myService, connection);
    }

    std::cout << "Successfully Registered Service!" << std::endl;
    
    // start field thread
    std::thread threadField(updateFieldData, myService);

    threadField.join();
    
    return 0;
}
