# marcelServer
Home robotic base server create afterwork! Run on Raspberry (ARM arch)

This is a base of TCP server, able to drive motorized robotic with Raspberry
Input commands come from the android apk Marcel (client interface), or from your custom interface
Output commands go to a linked Arduino by serial usb port

Optionally:
It uses speech recognition CmuSphinx, and Computer Vision OpenCV libraries

### Prerequisites

• Raspbian environment with WiringPi, rpi.gpio installed
• c++ compilator like g++ installed
• a few library dependencies readable in makefile!
• finish install of OpenCV lib (prevent: you will probably have to build your own opencv)
• finish install & test of CMUsphinx lib

### Installing

#Code compilation & lite install (without install output directories speech recognition & Computer Vision)
```
sudo make instlite
```
#Code compilation & full install
```
sudo make install
```

#clean after compilation
```
sudo make clean
```

#clean after compilation & delete the binary server
```
sudo make cleanall
```

### Running

#Enter command in shell (You can change the TCP output port, 4444 is the default port)
```
sudo ServerMarcel -p 4444
```


