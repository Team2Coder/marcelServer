# marcelServer
Robotic home base server create afterwork! Run on Raspberry (ARM arch)

This is a base of TCP server, able to drive motorized robotique with Raspberry.
The input commands come from the android apk Marcel (client interface), or from custom interface.
The output commands go to a linked Arduino by serial usb port. (Arduino also ships are codes)

It uses speech recognition CmuSphinx, and Computer Vision OpenCV libraries

#pre-required
Rasbian environment with WiringPi, rpi.gpio, lib installed
C++ compilator like g++ installed
A few library dependencies readable in makefile!
finish install of OpenCV lib (prevent: you will probably have to build your own opencv)
finish install & test of CMUsphinx lib

#Code compilation (without install output directories speech recognition & Computer Vision)
sudo make instlight

#Code compilation & full install
sudo make install

#clean after compilation
sudo make clean

#clean afer compilation + delete the binary
sudo make cleanall

