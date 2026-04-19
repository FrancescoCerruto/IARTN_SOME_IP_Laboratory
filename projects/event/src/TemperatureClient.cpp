// TemperatureClient.cpp
#include <iostream>
#include <string>
#include <thread>
#include <csignal>
#include <atomic>

#ifndef _WIN32
#include <unistd.h>
#endif

#include <CommonAPI/CommonAPI.hpp>
#include <v1/my_package/iartn/some_ip_demo/TemperatureProxy.hpp>

using namespace v1::my_package::iartn::some_ip_demo;

// Atomic flag to stop threads safely
std::atomic<bool> keepRunning(true);

// SIGINT handler
void signalHandler(int signum) {
    std::cout << "\nKilling TemperatureClient ..." << std::endl;
    keepRunning = false;
}

int main() {    
    CommonAPI::Runtime::setProperty("LogContext", "E01C");
    CommonAPI::Runtime::setProperty("LogApplication", "E01C");
    CommonAPI::Runtime::setProperty("LibraryBase", "Temperature");

    std::shared_ptr < CommonAPI::Runtime > runtime = CommonAPI::Runtime::get();

    std::string domain = "local";
    std::string instance = "my_package.iartn.some_ip_demo.Temperature";
    std::string connection = "Temperature_client";
    
    std::shared_ptr<TemperatureProxy<>> myProxy = runtime->buildProxy<TemperatureProxy>(domain, instance, connection);
    
    std::cout << "Checking availability!" << std::endl;
    while (!myProxy->isAvailable() && keepRunning)
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    std::cout << "Available..." << std::endl;
    
    //SIGINT (Ctrl+C) handler declaration
    signal(SIGINT, signalHandler);

    myProxy->getTemperatureEvent().subscribe([](const int& val) {
        std::cout << "EVENT value: " << val << std::endl;
    });

    while (keepRunning) {}

    return 0;
}
