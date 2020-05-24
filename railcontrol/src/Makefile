
#CC=gcc
#CPP=g++
CCRASPI=aarch64-linux-gcc
CPPRASPI=aarch64-linux-g++

CPPFLAGS+=-I. -g -O2 -Wall -Wextra -Werror -std=c++11
LDFLAGS+=-g -Wl,--export-dynamic
LIBS=-lpthread -ldl

TMPDIR=RailControl

OBJ= \
	WebServer/WebClient.o \
	Manager.o \
	ArgumentHandler.o \
	Config.o \
	DataModel/Accessory.o \
	DataModel/AccessoryBase.o \
	DataModel/Feedback.o \
	DataModel/HardwareHandle.o \
	DataModel/LayoutItem.o \
	DataModel/LockableItem.o \
	DataModel/Loco.o \
	DataModel/Object.o \
	DataModel/Relation.o \
	DataModel/Serializable.o \
	DataModel/Signal.o \
	DataModel/Street.o \
	DataModel/Switch.o \
	DataModel/Track.o \
	DataModel/TrackBase.o \
	Hardware/HardwareHandler.o \
	Languages.o \
	Logger/Logger.o \
	Logger/LoggerServer.o \
	Network/Serial.o \
	Network/TcpClient.o \
	Network/TcpConnection.o \
	Network/TcpServer.o \
	Network/UdpConnection.o \
	RailControl.o \
	Storage/StorageHandler.o \
	Timestamp.o \
	Utils/Utils.o \
	WebServer/HtmlFullResponse.o \
	WebServer/HtmlResponse.o \
	WebServer/HtmlResponseNotFound.o \
	WebServer/HtmlResponseNotImplemented.o \
	WebServer/HtmlTag.o \
	WebServer/HtmlTagAccessory.o \
	WebServer/HtmlTagButton.o \
	WebServer/HtmlTagButtonCancel.o \
	WebServer/HtmlTagButtonCommand.o \
	WebServer/HtmlTagButtonCommandToggle.o \
	WebServer/HtmlTagButtonOK.o \
	WebServer/HtmlTagButtonPopup.o \
	WebServer/HtmlTagFeedback.o \
	WebServer/HtmlTagInput.o \
	WebServer/HtmlTagInputInteger.o \
	WebServer/HtmlTagInputSlider.o \
	WebServer/HtmlTagInputSliderLocoSpeed.o \
	WebServer/HtmlTagSelect.o \
	WebServer/HtmlTagSignal.o \
	WebServer/HtmlTagStreet.o \
	WebServer/HtmlTagSwitch.o \
	WebServer/HtmlTagTrackBase.o \
	WebServer/Response.o \
	WebServer/WebServer.o

all: $(OBJ)
	+make -C Hardware
	+make -C Storage
	$(CXX) $(LDFLAGS) $(OBJ) -o railcontrol $(LIBS)
	rm Timestamp.cpp
	rm Timestamp.o

dist: all
	strip railcontrol
	strip Hardware/*.so
	strip Storage/*.so
	tar cvJf railcontrol.tar.xz Hardware/*.so Storage/*.so railcontrol railcontrol.conf.dist html/*
	mkdir $(TMPDIR)
	mkdir $(TMPDIR)/Hardware
	mkdir $(TMPDIR)/Storage
	cp -r \
		html \
		railcontrol.conf.dist \
		railcontrol \
		$(TMPDIR)
	cp -r Hardware/*.so $(TMPDIR)/Hardware
	cp -r Storage/*.so $(TMPDIR)/Storage
	tar cvJf railcontrol.`date +"%Y%m%d"`.tar.xz $(TMPDIR)/* $(TMPDIR)/html/*
	rm -r $(TMPDIR)

sqlite-shell:
	make -C Storage/sqlite

Timestamp.o: Timestamp.cpp Timestamp.h
	$(CXX) $(CPPFLAGS) -c -o $@ $<

%.o: %.cpp *.h DataModel/*.h Hardware/HardwareHandler.h Logger/*.h Network/*.h Storage/StorageHandler.h Utils/*.h WebServer/*.h
	$(CXX) $(CPPFLAGS) -c -o $@ $<

clean:
	make -C Hardware clean
	make -C Storage clean
	rm -f *.o DataModel/*.o Hardware/*.o Logger/*.o Network/*.o Storage/*.o Utils/*.o WebServer/*.o
	rm -f railcontrol

clean-sqlite-shell:
	make -C Storage/sqlite clean

test:
	make -C test

Timestamp.cpp:
	echo "#include <ctime>" > Timestamp.cpp
	echo "#include \"Timestamp.h\"" >> Timestamp.cpp
	echo "time_t GetCompileTime() { return `date +%s`; }" >> Timestamp.cpp

