# IARTN SOME/IP Laboratory

SOME/IP laboratory session using the vSomeIP library and the CommonAPI C++ framework.

This repository provides a complete workflow to:
- install required dependencies
- generate CommonAPI-based applications
- build projects using CMake
- run SOME/IP client/server examples

---

## 📦 External components

- CommonAPI Core Runtime  
  https://github.com/COVESA/capicxx-core-runtime

- CommonAPI D-Bus Runtime  
  https://github.com/GENIVI/capicxx-dbus-runtime

- CommonAPI SOME/IP Runtime  
  https://github.com/GENIVI/capicxx-someip-runtime

- vSomeIP  
  https://github.com/GENIVI/vsomeip

- Code Generators:
  - https://github.com/GENIVI/capicxx-core-tools
  - https://github.com/GENIVI/capicxx-dbus-tools
  - https://github.com/GENIVI/capicxx-someip-tools

---

## 🖥️ System requirements

- Ubuntu Linux 24.04 LTS (recommended)
- Internet connection
- sudo privileges (only for system package installation)

---

## ⚠️ IMPORTANT RULES (READ CAREFULLY)

> [!WARNING]
> - Always place the repository inside your home directory
> - DO NOT use system directories such as `/`, `/opt`, `/usr`
> - DO NOT modify the internal folder structure
> - DO NOT execute development scripts with `sudo`
> - ALWAYS initialize the environment before running commands:
>   ```bash
>   source ./0_lab_setenv
>   ```

---

## First step

Install the git package 

```bash
  sudo apt-get update
  sudo apt-get install git -y
```

---

## 📥 Clone repository

```bash
git clone https://github.com/FrancescoCerruto/IARTN_SOME_IP_Laboratory.git
cd IARTN_SOME_IP_Laboratory
```

Make all the scripts executable

```bash
  sudo chmod +x ./0_lab_setenv
  sudo chmod +x ./1_install_libraries
  sudo chmod +x ./2_install_generators
  sudo chmod +x ./projects/0_build_examples
  sudo chmod +x ./projects/1_generate_project
  sudo chmod +x ./projects/2_generate_cmake
  sudo chmod +x ./projects/3_build_project
  sudo chmod +x ./projects/4_execute_client
  sudo chmod +x ./projects/4_execute_service
```

---

## 📌 System setup

The process is divided into two logical stages to ensure a clean environment and easy debugging.

The system setup should be done only one. Once the libraries are installed and builded, you can go directly to the Project setup section

### Install libraries
```bash
  ./1_install_libraries
```

> [!WARNING]
> During the script execution it will appear the following line
> ```bash 
>   Reversed (or previously applied) patch detected!  Assume -R? [n]
> ```
> please answer 
> ```bash 
>    y
> ```

> [!WARNING]
> If the building process of the library x exits with an errore, you must delete the library x folder and re-execute the install script 
> ```bash 
>   cd $SRC_LIB_FOLDER
>   rm -r x
>   cd $LAB_ROOT
>   ./install_libraries
> ```
> please answer 
> ```bash 
>    y
> ```

### Install CommonAPI code generators
```bash
  ./2_install_generators
```

At this point you shold have the following folder tree

```bash
IARTN_SOME_IP_Laboratory/
├── libraries/
│   ├── generators/
│   │   ├── commonapi-core-generator/
│   │   ├── commonapi-dbus-generator/
│   │   └── commonapi-someip-generator/
│   ├── install/
│   │   ├── bin/
│   │   ├── etc/
│   │   ├── include/
│   │   └── lib/
|   ├── src/
│   │   ├── capicxx-core-runtime/
│   │   ├── capicxx-dbus-runtime/
│   │   ├── capicxx-someip-runtime/
│   │   ├── dbus-1.12.16/
│   │   └── vsomeip
├── materials/
└── projects/
```

---

## 2. Project Setup
Each service must be placed in 
```bash
projects/<ServiceFolder>/
```

The project folder must contain 
* The **fidl** folder
  - Service interface definition (.fidl file) and SOME/IP specifications (.fdepl file)
* The **src** folder
  - Service application logic for both client and server sides
* SOME/IP stack configuration files
  - vsomeip_client.json
  - vsomeip_server.json 

You should obtain a folder tree like the following one

```bash
IARTN_SOME_IP_Laboratory/
├── projects/
│   ├── <ServiceFolder>/
│   │   ├── src/
│   │   │   ├── <ServiceName>Client.cpp
│   │   │   ├── <ServiceName>Service.cpp
│   │   │   ├── <ServiceName>StubImpl.cpp
│   │   │   └── <ServiceName>StubImpl.hpp
│   │   └── fidl/
│   │   │   ├── <ServiceName>.fdepl
│   │   │   └── <ServiceName>.fidl
│   │   ├── vsomeip_client.json
│   │   └── vsomeip_server.json
```

> [!WARNING]
> Service names must NOT contain spaces
>  ```bash
>   ❌ <Service Name>
>   ✔️ <ServiceName>
>  ```
>  
> Folder names must follow the same rule
>  ```bash
>   ❌ <Service Folder>
>   ✔️ <ServiceFolder>
>  ```

### SOME/IP configuration
Inside **.fdepl** file
```bash
SomeIpServiceID = 1234 // must be unique through the entire netowrk
SomeIpUnicastAddress = "YOUR_IP_ADDRESS"
```
Inside the **vsomeip_client.json** and **vsomeip_server.json** file
```bash
"unicast" : "YOUR_IP_ADDRESS",
"routing" : "<ServiceName>_server", // for server
"routing" : "<ServiceName>_client", // for client
```
Inside the **vsomeip_server.json** file
```bash
"services" : [
        {
            "service" : "0x1234", <-- SomeIpServiceID in exadecimal format
            "instance" : "0x1234", <-- SomeIpInstanceID in exadecimal format
            "reliable" :  30499  <-- SomeIpReliable = true, SomeIpReliableUnicastPort
            --------------------------------------------------------------------------------
            "unreliable" :  30499  <-- SomeIpReliable = false, SomeIpUnreliableUnicastPort 
        }
    ],
```

> [!WARNING]
> If the same machine runs multiple servers or multiple clients, both the application source code and vsomeip.json files must be set properly.
>  * You must ensure that each application has an unique name and id
> ```bash
> //vsomeip_client.json file
>  "applications" : [
>        {
>            "name" : "<ServiceName>_client",
>            "id" : "0x3333"  <-- it must be unique inside the machine
>        }
>    ],
>  ```
> ```bash
> //vsomeip_server.json file
>  "applications" : [
>        {
>            "name" : "<ServiceName>_server",
>            "id" : "0x3333"  <-- it must be unique inside the machine
>        }
>    ],
>  ```
> * You must enable only one application as routing manager for each machine (it does not matter which application will be the routing manager). The other applications will use the single routing manager (it must be specified the name) as routing manager
> - The someip.json file of the routing manager must contains, in this case, all the name of the applications and all the services (offered and searched)' parameters 
> ```bash
>  //vsomeip.json file for the routing manager (e.g., service Bob)
>  "applications" : [ <-- it must contain all the applications (both clients and servers) running on the machine
>        {
>            "name" : "",
>            "id" : ""
>        }
>    ],
>  "services" : [ <-- it must contain all the services
>         {
>            "service" : "",
>            "instance" : ""
>            "reliable"/"unreliable": ""
>        }
>    ],
>  "routing" : "Bob",
>  ```
> - The someip.json file of the other applications should contain only both their application name and service (offered and searched)' parameters. In addition, the file must specify the routing manager
> ```bash
>  //vsomeip.json file for a non-routing manager application (e.g., service Alice)
>  "applications" : [ <-- my application name and ID
>        {
>            "name" : "",
>            "id" : ""
>        }
>    ],
>  "services" : [ <-- my service (if i am running the server side of the application
>         {
>            "service" : "",
>            "instance" : ""
>            "reliable"/"unreliable": ""
>        }
>    ],
>  "routing" : "Bob", <-- link to the single routing manager
>  ```

---

### 3. Project build
In the projects folder there are several scripts to simplify the generation and building processes.
* **1_generate_project**: this script invokes the CommonAPI generators
```bash
./1_generate_project <ServiceName> <ServiceFolder>
```
* **2_generate_cmake**: both the libraries and projects are linked with cmake files. This parametrized script creates a custom cmake file for the provided project
```bash
./2_generate_cmake <ServiceName> <ServiceFolder> <ServicePackage>
```
* **3_build_project**: this parametrized script builds the project and link the executable to the CommonAPI and vsomeip libraries
```bash
./3_build_project <ServiceFolder>
```

---

### 4. Project execution
For each service, SOME/IP client and SOME/IP server must run on different machines
-  By default the CommonAPI framework assumes that both client and server run on the same machine
-  By default client and server communicate through UNIX sockets

* both client and server machines must be on the same subnet
> [!WARNING]
> If using a VM, use Bridged Adapter mode
> If using Windows Subsystem for Linux
> * enable Hyper-V services (if not already enabled) and restart your computer
> * run the Powershell as administrator
> * retrievethe network interface name
> ```bash
> Get-NetAdapter
> ```
> create bridged network
> ```bash
> New-VMSwitch -Name "WSL_Bridge" -NetAdapterName "Your_Adapter_Name" -AllowManagementOS $true
> ```
> press WIN + R and type %USERPROFILE%
> modify (or create) .wslconfig file
> 
> ```bash
> [wsl2]
> networkingMode=bridged
> vmSwitch=WSL_Bridge
> ```

* both client and server machines must use the same multicast address
  - Multicast routing must be configured manually
```bash
sudo ip route add 224.244.224.245 dev <network_interface>
```
> [!WARNING]
To find the interface
```bash
ip a
```

To run the client/server instance of the service, execute the related script
```bash
./4_execute_<client/service> <ServiceName> <ServiceFolder>
```
