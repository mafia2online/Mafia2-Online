CFLAGS = -c -g -std=c++0x -Wall -Wno-unused-function -Wno-unknown-pragmas -Wno-misleading-indentation -Wno-write-strings -D_SERVER -D_LINUX -D_NO_GARBAGE_COLLECT -I../Libraries/mongoose -I../Libraries/tinyxml -I../Libraries/sqlite -I../Libraries/RakNet/Source -I../Shared -I../Libraries/squirrel -I../Libraries/ -I.
LDFLAGS = -pthread -Wl,--no-as-needed -ldl
SOURCES = $(wildcard *.cpp)
SOURCES += ../Shared/CEvents.cpp ../Shared/CCommands.cpp ../Shared/CLibrary.cpp ../Shared/CSQLite.cpp ../Shared/CXML.cpp
SOURCES += $(wildcard ../Shared/Timers/*.cpp)
SOURCES += $(wildcard ../Shared/Threading/*.cpp)
SOURCES += ../Shared/Scripting/CSquirrelCommon.cpp
SOURCES += $(wildcard ../Shared/Game/*.cpp)
SOURCES += ../Shared/Scripting/Natives/CAreaNatives.cpp ../Shared/Scripting/Natives/CBlipNatives.cpp ../Shared/Scripting/Natives/CEventNatives.cpp ../Shared/Scripting/Natives/CSharedPlayerNatives.cpp ../Shared/Scripting/Natives/CSharedVehicleNatives.cpp ../Shared/Scripting/Natives/CSQLiteNatives.cpp ../Shared/Scripting/Natives/CSystemNatives.cpp ../Shared/Scripting/Natives/CTimerNatives.cpp ../Shared/Scripting/Natives/CUtilNatives.cpp ../Shared/Scripting/Natives/CXMLNatives.cpp ../Shared/Scripting/Natives/CHashNatives.cpp
SOURCES += ../Shared/Scripting/CScriptingManager.cpp ../Shared/SharedUtility.cpp ../Shared/Scripting/CSquirrel.cpp ../Shared/Scripting/CSquirrelArguments.cpp
SOURCES += $(wildcard ../Shared/Network/*.cpp) ../Shared/CString.cpp ../Shared/CLogFile.cpp
SOURCES += $(wildcard ../Shared/md5/*.cpp) ../Shared/CSettings.cpp ../Shared/Linux.cpp
OBJECTS = $(SOURCES:.cpp=.o)
LIBS = ../Libraries/lib/libsqlite.a ../Libraries/lib/libtinyxml.a ../Libraries/lib/libsquirrel.a ../Libraries/lib/libraknet.a
EXECUTABLE = ../Binary/m2mp-svr

all: prepare $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	gcc $(CFLAGS) ../Libraries/mongoose/mongoose.c -o mongoose.o
	g++ $(LDFLAGS) $(OBJECTS) mongoose.o $(LIBS) -o $@

.cpp.o:
	g++ $(CFLAGS) $< -o $@

clean:
	rm -Rf $(OBJECTS) $(EXECUTABLE)

prepare:
	g++ $(CFLAGS) StdInc.h -o StdInc.h.gch
