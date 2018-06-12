[![Maintenance](https://img.shields.io/badge/Maintained%3F-no-red.svg)](https://bitbucket.org/lbesson/ansi-colors)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/590dc340b46c499c9116774015cec1d6)](https://www.codacy.com/app/cedleg/marcelServer?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Team2Coder/marcelServer&amp;utm_campaign=Badge_Grade)

# marcelServer
Robotic home base server created afterwork! It runs on Raspberry Pi (ARM architecture).

This is a based TCP server, able to drive motorized robot with Raspberry Pi.<br/>
Input commands come from the Android apk Marcel (client interface), or from your custom interface.<br/>
Output commands go to a linked Arduino by serial usb port.

It uses CmuSphinx speech recognition and Computer Vision OpenCV libraries.

### Prerequisites

Raspbian environment with WiringPi, rpi.gpio installed<br/>
c++ compilator like g++ installed<br/>
a few library dependencies readable in makefile!<br/>
finish install of OpenCV lib (Warning: you will probably have to build your own opencv)<br/>
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

#clean after compilation + delete the binary
```
sudo make cleanall
```

### Running

#Enter command in shell (you can change output TCP port, 4444 is the default port)
```
sudo ServerMarcel -p 4444
```

## Authors

* **Sébastien Lorrain** - *Initial work* - [sldevand](https://github.com/sldevand)
* **Cédric Leguay** - *Initial work* - [cedleg](https://github.com/cedleg)

