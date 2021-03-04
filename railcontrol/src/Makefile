
#CC=gcc
#CPP=g++

CPPFLAGS+=-I. -g -O2 -Wall -Wextra -Werror -std=c++11
LDFLAGS+=-g -Wl,--export-dynamic
LIBS=-lpthread -ldl

TMPDIR=RailControl

OBJ= \
	ArgumentHandler.o \
	Config.o \
	DataModel/Accessory.o \
	DataModel/AccessoryBase.o \
	DataModel/Cluster.o \
	DataModel/Feedback.o \
	DataModel/HardwareHandle.o \
	DataModel/LayoutItem.o \
	DataModel/LockableItem.o \
	DataModel/Loco.o \
	DataModel/LocoFunctions.o \
	DataModel/Object.o \
	DataModel/Relation.o \
	DataModel/Route.o \
	DataModel/Serializable.o \
	DataModel/Signal.o \
	DataModel/Switch.o \
	DataModel/Track.o \
	DataModel/TrackBase.o \
	Hardware/CS2Tcp.o \
	Hardware/CS2Udp.o \
	Hardware/CcSchnitte.o \
	Hardware/Ecos.o \
	Hardware/HardwareHandler.o \
	Hardware/Hsi88.o \
	Hardware/M6051.o \
	Hardware/OpenDcc.o \
	Hardware/ProtocolMaerklinCAN.o \
	Hardware/RM485.o \
	Hardware/Virtual.o \
	Hardware/Z21.o \
	Languages.o \
	Logger/Logger.o \
	Logger/LoggerServer.o \
	Manager.o \
	Network/Serial.o \
	Network/TcpClient.o \
	Network/TcpConnection.o \
	Network/TcpServer.o \
	Network/UdpConnection.o \
	RailControl.o \
	Storage/Sqlite.o \
	Storage/StorageHandler.o \
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
	WebServer/HtmlTagButtonCommandPressRelease.o \
	WebServer/HtmlTagButtonCommandToggle.o \
	WebServer/HtmlTagButtonOK.o \
	WebServer/HtmlTagButtonPopup.o \
	WebServer/HtmlTagFeedback.o \
	WebServer/HtmlTagInput.o \
	WebServer/HtmlTagInputInteger.o \
	WebServer/HtmlTagInputSlider.o \
	WebServer/HtmlTagInputSliderLocoSpeed.o \
	WebServer/HtmlTagRoute.o \
	WebServer/HtmlTagSelect.o \
	WebServer/HtmlTagSignal.o \
	WebServer/HtmlTagSwitch.o \
	WebServer/HtmlTagTrackBase.o \
	WebServer/Response.o \
	WebServer/WebClient.o \
	WebServer/WebClientCluster.o \
	WebServer/WebClientSignal.o \
	WebServer/WebClientTrack.o \
	WebServer/WebClientTrackBase.o \
	WebServer/WebServer.o

OBJZLIB=Hardware/zlib/adler32.o \
	Hardware/zlib/compress.o \
	Hardware/zlib/crc32.o \
	Hardware/zlib/deflate.o \
	Hardware/zlib/gzclose.o \
	Hardware/zlib/gzlib.o \
	Hardware/zlib/gzread.o \
	Hardware/zlib/gzwrite.o \
	Hardware/zlib/infback.o \
	Hardware/zlib/inffast.o \
	Hardware/zlib/inflate.o \
	Hardware/zlib/inftrees.o \
	Hardware/zlib/trees.o \
	Hardware/zlib/uncompr.o \
	Hardware/zlib/zutil.o \
	Hardware/ZLib.o


CXXUNSORTED= $(subst .o,.cpp,$(OBJ))
CXXSORTED= $(shell ls -S $(CXXUNSORTED))
OBJSORTED= Timestamp.o Storage/sqlite/sqlite3.o $(subst .cpp,.o,$(CXXSORTED)) $(OBJZLIB)

all: $(OBJSORTED)
	$(CXX) $(LDFLAGS) $(OBJSORTED) -o railcontrol $(LIBS)


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
	rm -f *.o DataModel/*.o Hardware/*.o Hardware/zlib/*.o Logger/*.o Network/*.o Storage/*.o Utils/*.o WebServer/*.o
	rm -f railcontrol

clean-sqlite-shell:
	make -C Storage/sqlite clean

test:
	make -C test

tools:
	make -C tools

Timestamp.cpp:
	echo "#include <ctime>" > Timestamp.cpp
	echo "#include \"Timestamp.h\"" >> Timestamp.cpp
	echo "time_t GetCompileTime() { return `date +%s`; }" >> Timestamp.cpp

