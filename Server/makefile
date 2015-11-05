CFLAGS = -m32 -c -g -Wno-write-strings -D_SERVER -D_LINUX -D_NO_GARBAGE_COLLECT -I../Shared -I../Libraries/squirrel -I../Libraries/ -I.
LDFLAGS = -m32 -lpthread -ldl
SOURCES = $(wildcard *.cpp)
SOURCES += $(wildcard ../Libraries/tinyxml/*.cpp)
SOURCES += ../Shared/CEvents.cpp ../Shared/CCommands.cpp ../Shared/CLibrary.cpp ../Shared/CSQLite.cpp ../Shared/CXML.cpp
SOURCES += $(wildcard ../Shared/Timers/*.cpp)
SOURCES += $(wildcard ../Shared/Threading/*.cpp)
SOURCES += ../Shared/Scripting/CSquirrelCommon.cpp
SOURCES += $(wildcard ../Shared/Scripting/Natives/*.cpp)
SOURCES += ../Shared/Scripting/CScriptingManager.cpp ../Shared/SharedUtility.cpp ../Shared/Scripting/CSquirrel.cpp ../Shared/Scripting/CSquirrelArguments.cpp
SOURCES += $(wildcard ../Shared/Network/*.cpp) ../Shared/CString.cpp ../Shared/CLogFile.cpp
SOURCES += $(wildcard ../Shared/md5/*.cpp) ../Shared/CSettings.cpp ../Shared/Linux.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = ../Binary/m2mp-svr
LIBS = ../Libraries/sqlite/libsqlite.a ../Libraries/squirrel/libsquirrel.a ../Libraries/RakNet/Source/libraknet.a

all: prepare $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	gcc $(CFLAGS) ../Libraries/mongoose/mongoose.c -o mongoose.o
	g++ $(OBJECTS) mongoose.o $(LDFLAGS) $(LIBS) -o $@

.cpp.o:
	g++ $(CFLAGS) $< -o $@

clean:
	rm -Rf $(OBJECTS) $(EXECUTABLE)

prepare:
	g++ -c StdInc.h -o StdInc.h.gch
