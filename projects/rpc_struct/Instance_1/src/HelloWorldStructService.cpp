// HelloWorldStructService.cpp
#include <iostream>
#include <thread>
#include <csignal>
#include <atomic>

#include <CommonAPI/CommonAPI.hpp>
#include "HelloWorldStructStubImpl.hpp"

using namespace std;

// Atomic flag to stop threads safely
std::atomic<bool> keepRunning(true);

// SIGINT handler
void signalHandler(int signum) {
    std::cout << "\nKilling HelloWorldStructService ..." << std::endl;
    keepRunning = false;
}

void methodInvoked() {
    while (keepRunning) {
        std::cout << "Waiting method invocation... Press Ctrl + C to stop the execution" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {    
    CommonAPI::Runtime::setProperty("LogContext", "E01S");
    CommonAPI::Runtime::setProperty("LogApplication", "E01S");
    CommonAPI::Runtime::setProperty("LibraryBase", "HelloWorldStruct");

    std::shared_ptr<CommonAPI::Runtime> runtime = CommonAPI::Runtime::get();

    std::string domain = "local";
    std::string instance = "commonapi.examples.HelloWorldStruct.Instance_1";
    std::string connection = "HelloWorldStruct_Instance_1_server";

    std::shared_ptr<HelloWorldStructStubImpl> myService = std::make_shared<HelloWorldStructStubImpl>();
    bool successfullyRegistered = runtime->registerService(domain, instance, myService, connection);
        
    //SIGINT (Ctrl+C) handler declaration
    signal(SIGINT, signalHandler);

    while (!successfullyRegistered && keepRunning) {
        std::cout << "Register Service failed, trying again in 100 milliseconds..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        successfullyRegistered = runtime->registerService(domain, instance, myService, connection);
    }

    std::cout << "Successfully Registered Service!" << std::endl;

    std::thread serviceThread(methodInvoked);
    
    serviceThread.join();
    
    std::cout << "Stop Service!" << std::endl;
    return 0;
}
