#
#   Makefile for Marcel, a RaspBerry Pi and Arduino
#   controlled robot with computer vision and voice recognition 
#   features.
#   Authors: Cédric LEGUAY / Sébastien LORRAIN

EXEC = ServerMarcel
INSTPATH=/usr/local
OBJS = mainMarcel.o Mp3player.o Vision.o SignalHandler.o PracticalSocket.o CaptExp.o Motor.o Serial.o Ears.o Myconf.o Processes.o Mythread.o MyTcpClient.o MyTcpServer.o Action.o AI.o
CXX = g++-4.9
CXXFLAGS = -c -std=c++14 -fpermissive -pedantic -g
LXXFLAGS = -std=c++14 -fpermissive -pedantic -g
LIBS = -lwiringPi -lpthread -lmpg123 -lao -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_contrib -lopencv_objdetect -lpocketsphinx -lsphinxbase -lsphinxad -lconfig++

all : $(OBJS)
	$(CXX) $(LXXFLAGS) $(OBJS) -o $(EXEC) $(LIBS)

mainMarcel.o : mainMarcel.cpp mainMarcel.h
	$(CXX) $(CXXFLAGS) mainMarcel.cpp

Mp3player.o : Mp3player.h Mp3player.cpp
	$(CXX) $(CXXFLAGS) Mp3player.cpp

Vision.o : Vision.cpp Vision.h 
	$(CXX) $(CXXFLAGS) Vision.cpp

SignalHandler.o : SignalHandler.cpp SignalHandler.h
	$(CXX) $(CXXFLAGS) SignalHandler.cpp

PracticalSocket.o : PracticalSocket.cpp PracticalSocket.h
	$(CXX) $(CXXFLAGS) PracticalSocket.cpp

CaptExp.o : CaptExp.cpp CaptExp.h
	$(CXX) $(CXXFLAGS) CaptExp.cpp 

Motor.o : Motor.cpp  Motor.h
	$(CXX) $(CXXFLAGS) Motor.cpp 

Serial.o : Serial.cpp Serial.h
	$(CXX) $(CXXFLAGS) Serial.cpp

Ears.o : Ears.cpp Ears.h
	$(CXX) $(CXXFLAGS) Ears.cpp

Myconf.o: Myconf.cpp Myconf.h
	$(CXX) $(CXXFLAGS) Myconf.cpp

Processes.o : Processes.cpp Processes.h
	$(CXX) $(CXXFLAGS) Processes.cpp

Mythread.o : Mythread.cpp Mythread.h
	$(CXX) $(CXXFLAGS) Mythread.cpp

MyTcpServer.o : MyTcpServer.cpp MyTcpServer.h
	$(CXX) $(CXXFLAGS) MyTcpServer.cpp

MyTcpClient.o : MyTcpClient.cpp MyTcpClient.h
	$(CXX) $(CXXFLAGS) MyTcpClient.cpp

Action.o : Action.cpp Action.h
	$(CXX) $(CXXFLAGS) Action.cpp

AI.o : AI.cpp AI.h
	$(CXX) $(CXXFLAGS) AI.cpp

clean:
	\rm *.o

cleanall:
	\rm *.o ServerMarcel

tar:
	tar cfv ServerMarcel.tar makefile *.cpp *.h *.cfg *.txt *.dia tmp images mp3 wpigsetup scripts bin

dia:
	autodia -l C++ -d .

install:
	sudo cp $(EXEC) wpigsetup $(INSTPATH)/bin
	sudo chown pi:pi $(INSTPATH)/bin/$(EXEC)
	sudo cp -r images $(INSTPATH)/share/images
	sudo chown -R pi:pi $(INSTPATH)/share/images
	sudo cp -r tmp $(INSTPATH)/tmp	
	sudo chown -R pi:pi $(INSTPATH)/tmp
	sudo cp -r sphinxlm $(INSTPATH)/share
	sudo chown -R pi:pi $(INSTPATH)/share/sphinxlm
	sudo cp -r scripts $(INSTPATH)/share
	sudo chown -R pi:pi $(INSTPATH)/share/scripts

instlight:

	sudo cp $(EXEC) wpigsetup $(INSTPATH)/bin
	sudo chown pi:pi $(INSTPATH)/bin/$(EXEC)

