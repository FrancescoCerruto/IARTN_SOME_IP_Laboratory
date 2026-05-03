// TemperatureService.cpp
#include <iostream>
#include <thread>
#include <csignal>
#include <atomic>

#include <CommonAPI/CommonAPI.hpp>
#include "TemperatureStubImpl.hpp"

using namespace std;

using namespace v1::my_package::iartn::some_ip_demo;

// Atomic flag to stop threads safely
std::atomic<bool> keepRunning(true);

// SIGINT handler
void signalHandler(int signum) {
    std::cout << "\nKilling TemperatureService ..." << std::endl;
    keepRunning = false;
}

void updateEventData(std::shared_ptr<TemperatureStubImpl> myService) {
    int counter = 1;
    while (keepRunning) {
        // Incremental step every 5 seconds
        std::this_thread::sleep_for(std::chrono::seconds(1));
        // read internal value
        int newValue = counter;
        myService->fireTemperatureEvent(newValue);
        counter++;
    }
}

int main() {
    CommonAPI::Runtime::setProperty("LogContext", "E01S");
    CommonAPI::Runtime::setProperty("LogApplication", "E01S");
    CommonAPI::Runtime::setProperty("LibraryBase", "Temperature");

    std::shared_ptr<CommonAPI::Runtime> runtime = CommonAPI::Runtime::get();

    std::string domain = "local";
    std::string instance = "my_package.iartn.some_ip_demo.Temperature.Instance_1";
    std::string connection = "Temperature_Instance_1_server";

    std::shared_ptr<TemperatureStubImpl> myService = std::make_shared<TemperatureStubImpl>();
    bool successfullyRegistered = runtime->registerService(domain, instance, myService, connection);
        
    //SIGINT (Ctrl+C) handler declaration
    signal(SIGINT, signalHandler);

    while (!successfullyRegistered && keepRunning) {
        std::cout << "Register Service failed, trying again in 100 milliseconds..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        successfullyRegistered = runtime->registerService(domain, instance, myService, connection);
    }

    std::cout << "Successfully Registered Service!" << std::endl;

    // start event thread
    std::thread threadEvent(updateEventData, myService);
   
    threadEvent.join();
   
    return 0;
}
