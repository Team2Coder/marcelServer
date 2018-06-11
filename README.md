# marcelServer
Robotic home base server create afterwork! Run on Raspberry (ARM arch)

This is a base of TCP server, able to drive motorized robotique with Raspberry.
The input commands come from the android apk Marcel (client interface), or from your custom interface.
The output commands go to a linked Arduino by serial usb port.

It uses speech recognition CmuSphinx, and Computer Vision OpenCV libraries

### Prerequisites

Raspbian environment with WiringPi, rpi.gpio installed

c++ compilator like g++ installed

a few library dependencies readable in makefile!

finish install of OpenCV lib (prevent: you will probably have to build your own opencv)

finish install & test of CMUsphinx lib

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

#clean afer compilation + delete the binary
```
sudo make cleanall
```

### Running

#Enter command dans shell (You can do modify output TCP port, 4444 is the default port)
```
sudo ServerMarcel -p 4444
```


