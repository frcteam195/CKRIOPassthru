# CKRIOPassthru

[![RoboRIO Build Status](https://github.com/frcteam195/CKRIOPassthru/actions/workflows/main.yml/badge.svg)](https://github.com/frcteam195/CKRIOPassthru/actions/workflows/main.yml)

To build this project for the RoboRIO, we recommend using one of our docker images.

---

### Dependencies
Before continuing, Docker must be installed on your host computer. [Learn how to install Docker here.](https://docs.docker.com/engine/install/) If you do not wish to install Docker, skip to the section on [host installation](#alternate)

---

### Running The Container On UNIX
Once Docker is installed, you can clone the repository and launch the docker container.

```
git clone https://github.com/frcteam195/CKRIOPassthru
cd CKRIOPassthru
./docker/run_container.sh
```

---

### Running The Container On Windows
1. Start by installing WSL2 according to [this link](https://docs.microsoft.com/en-us/windows/wsl/install).
2. Now, install the [Ubuntu distro from the Microsoft store](https://www.microsoft.com/en-us/p/ubuntu/9nblggh4msv6?activetab=pivot:overviewtab).
3. Open the Ubuntu app from the start menu to finish installation and setup your user account and password.
4. Set the Ubuntu distro as your default distro in WSL using the command `wsl --setdefault Ubuntu`
5. Once WSL2 and the Ubuntu distro are installed, install Docker Desktop.
6. Inside Docker Desktop Settings, under Resources > WSL Integration, enable WSL Integration for your Ubuntu installation.
7. Launch the WSL Ubuntu instance and clone the repository inside a repository folder inside your home directory. We recommend ${HOME}/repos. Make sure to clone the submodules, as well. [https://github.com/frcteam195/CKRIOPassthru](https://github.com/frcteam195/CKRIOPassthru)
```
mkdir repos
cd repos
git clone --recurse-submodules https://github.com/frcteam195/CKRIOPassthru
```
8. Once you have the repository cloned, navigate to the repository folder `cd CKRIOPassthru`. When you are in the root folder of the repository (CKRIOPassthru), you can run the following command to launch the container:
```
./docker/run_container.sh
```

---

### Building and Deploying

Once the container is running, now you can use gradle like normal. To build and deploy the robot code, do the following:

```
./gradlew build
./gradlew deploy
```

Please note that deploying requires the computer to be connected to the robot, however, building the first time will require the computer to be connected to the internet to download the required dependencies. You can build first, while online, in order to download all required dependencies, then connect to the robot, in order to deploy.

---

### <a name="alternate"></a> Alternate: Host Installation

#### Installation
Alternatively, you can install the dependencies natively on your host machine rather than use our docker image to build and deploy for the RoboRIO. In order to build this project, you specifically need protobuf version 3.18.0 (protoc). This should be compiled by source and installed in the system path of your host computer. You will also need to copy the shared library objects from the lib folder of this repository and place them in your RoboRIO toolchain lib folder. Make sure to copy them, as they are still needed in the local lib folder. Lastly, you will need the RoboRIO toolchain bin folder in your system path. 

Specific versions required from the lib folder:

* protobuf 3.18.0
* ZMQ 4.3.4 built with drafts enabled


#### Troubleshooting
If you are trying to build using the VSCode Command Pallette, it will fail unless the system paths are setup properly in the VSCode configuration. In the command pallete, open `>Preferences: Open Settings (JSON)`
then you can setup the path variable for your operating system specific environment:

```
"terminal.integrated.env.osx": {
  "JAVA_HOME": "/Users/macos/wpilib/2022/jdk",
  "PATH": "/Users/macos/wpilib/2022/jdk/bin:${env:PATH}:/usr/local/bin:/Users/macos/wpilib/2022/roborio/bin"
},
```

Please make sure to change the line `terminal.integrated.env.<platform>` for your specific platform
