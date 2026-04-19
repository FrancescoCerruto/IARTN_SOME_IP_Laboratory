// HelloWorldStructClient.cpp
#include <iostream>
#include <string>
#include <thread>
#include <csignal>
#include <atomic>

#ifndef _WIN32
#include <unistd.h>
#endif

#include <CommonAPI/CommonAPI.hpp>
#include <v0/commonapi/examples/HelloWorldStructProxy.hpp>

using namespace v0::commonapi::examples;

// Atomic flag to stop threads safely
std::atomic<bool> keepRunning(true);

// SIGINT handler
void signalHandler(int signum) {
    std::cout << "\nKilling HelloWorldClient ..." << std::endl;
    keepRunning = false;
}

void callMethod(std::shared_ptr<HelloWorldStructProxy<>> myProxy) {
    CommonAPI::CallStatus callStatus;
    std::string returnMessage;

    CommonAPI::CallInfo info(1000);
    info.sender_ = 1234;

    v0::commonapi::examples::HelloWorldStruct::UserData myUser;
    myUser.setName("Mario");
    myUser.setId(12345);

    while (keepRunning) {

        myProxy->sayHello(myUser, callStatus, returnMessage, &info);
        if (callStatus != CommonAPI::CallStatus::SUCCESS) {
            std::cerr << "Remote call failed!\n";
            continue;
        }
        info.timeout_ = info.timeout_ + 1000;

        std::cout << "Got message: '" << returnMessage << "'\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    keepRunning = false;
}

int main() {
    CommonAPI::Runtime::setProperty("LogContext", "E01C");
    CommonAPI::Runtime::setProperty("LogApplication", "E01C");
    CommonAPI::Runtime::setProperty("LibraryBase", "HelloWorldStruct");

    std::shared_ptr < CommonAPI::Runtime > runtime = CommonAPI::Runtime::get();

    std::string domain = "local";
    std::string instance = "commonapi.examples.HelloWorldStruct";
    std::string connection = "HelloWorldStruct_client";

    std::shared_ptr<HelloWorldStructProxy<>> myProxy = runtime->buildProxy<HelloWorldStructProxy>(domain,
            instance, connection);

    std::cout << "Checking availability!" << std::endl;
    while (!myProxy->isAvailable() && keepRunning)
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    std::cout << "Available..." << std::endl;
    
    //SIGINT (Ctrl+C) handler declaration
    signal(SIGINT, signalHandler);

    std::thread threadMethod(callMethod, myProxy);

    threadMethod.join();

    return 0;
}
