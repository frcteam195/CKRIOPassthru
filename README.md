# CKRIOPassthru
To build this project for the RoboRIO, you must currently use a UNIX machine of some kind. Windows support is planned to be added in the future.

-

### Dependencies
Before continuing, Docker must be installed on your host computer. [Learn how to install Docker here](https://docs.docker.com/engine/install/)

-

### Running The Container On UNIX
Once Docker is installed, you can clone the repository and launch the docker container.

```
git clone https://github.com/frcteam195/CKRIOPassthru
cd CKRIOPassthru
./docker/run_container.sh
```

-

### Running The Container On Windows (NOT CURRENTLY SUPPORTED)
Clone the repository using your git client of choice.
[https://github.com/frcteam195/CKRIOPassthru](https://github.com/frcteam195/CKRIOPassthru)

Once you have the repository cloned, open a command prompt and navigate to the repository folder. When you are in the root folder of the repository (CKRIOPassthru), you can run the following command to launch the container:

```
.\docker\run_container.cmd
```

-

### Building and Deploying

Once the container is running, now you can use gradle like normal. To build and deploy the robot code, do the following:

```
./gradlew build
./gradlew deploy
```

Please note that deploying requires the computer to be connected to the robot, however, building the first time will require the computer to be connected to the internet to download the required dependencies. You can build first, while online, in order to download all required dependencies, then connect to the robot, in order to deploy.

-

### Alternate Installation
Alternatively, you can install the dependencies natively on your host machine rather than use our docker image to build and deploy for the RoboRIO. In order to build this project, you specifically need protobuf version 3.18.0 (protoc). This should be compiled by source and installed in the system path of your host computer. You will also need to copy the shared library objects from the lib folder of this repository and place them in your RoboRIO toolchain lib folder. Lastly, you will need the RoboRIO toolchain bin folder in your system path.

Specific versions required from the lib folder:

* protobuf 3.18.0
* ZMQ 4.3.4 built with drafts enabled