#################   DEFINITIONS    #################

#Main compiler/linker variables
CXX=g++
CFLAGS=-Wall -I./src/
CARG=-lrt
LFLAGS= -lm 
LARG=-ldl -lpthread -lrt -lSM -lICE -lX11 -lXext -lpng -lz -Wl-rpath #-lGLU -lGL
M_FLAGS=-Wall -pthread -c -g -DVIEWER -lX11 #MJPEG Flags

#Architecture dependent variable
ARCH := $(shell getconf LONG_BIT)

#Directories
BINDIR=bin
LIBDIR=lib
OBJDIR=obj
SRCDIR=src

#Libraries
XML=xml/pugixml.cpp
MATHGL= -rdynamic /usr/lib/libmgl.so.7.0.0

#SOURCE FILES
HSFSRC=mainHSF.cpp
SIMFIGSRC=mainSimulationFigure.cpp
ANALYZESRC=mainAnalyze.cpp
SHOWSRC=mainShow.cpp
SIMULATESRC=mainSimulate.cpp

#################    MAIN TARGETS   #################

all: directories libraries hsf executable tools

hsf: core criteria dispatchers pthread queues results schedulers servers tasks util exe

#################     DIRECTORIES   #################

directories:
	mkdir -p $(BINDIR) $(OBJDIR) $(OBJDIR)/hsf $(OBJDIR)/lib

#################        HSF        #################

### HSF CORE
CORE:= $(patsubst $(SRCDIR)/core/%.cpp, %, $(wildcard $(SRCDIR)/core/*.cpp))
core: $(CORE)
$(CORE):
	$(CXX) $(CFLAGS) -c $(SRCDIR)/core/$@.cpp -o $(OBJDIR)/hsf/$@.o $(CARG)

### HSF CRITERIA
CRITERIA:= $(patsubst $(SRCDIR)/criteria/%.cpp, %, $(wildcard $(SRCDIR)/criteria/*.cpp))
criteria: $(CRITERIA)
$(CRITERIA):
	$(CXX) $(CFLAGS) -c $(SRCDIR)/criteria/$@.cpp -o $(OBJDIR)/hsf/$@.o $(CARG)


### HSF DISPATCHERS
DISPATCHERS:= $(patsubst $(SRCDIR)/dispatchers/%.cpp, %, $(wildcard $(SRCDIR)/dispatchers/*.cpp))
dispatchers: $(DISPATCHERS)
$(DISPATCHERS):
	$(CXX) $(CFLAGS) -c $(SRCDIR)/dispatchers/$@.cpp -o $(OBJDIR)/hsf/$@.o $(CARG)

### HSF PTHREAD
PTHREAD:= $(patsubst $(SRCDIR)/pthread/%.cpp, %, $(wildcard $(SRCDIR)/pthread/*.cpp))
pthread: $(PTHREAD)
$(PTHREAD):
	$(CXX) $(CFLAGS) -pthread -c $(SRCDIR)/pthread/$@.cpp -o $(OBJDIR)/hsf/$@.o $(CARG)

### HSF QUEUES
QUEUES:= $(patsubst $(SRCDIR)/queues/%.cpp, %, $(wildcard $(SRCDIR)/queues/*.cpp))
queues: $(QUEUES)
$(QUEUES):
	$(CXX) $(CFLAGS) -c $(SRCDIR)/queues/$@.cpp -o $(OBJDIR)/hsf/$@.o $(CARG)

### HSF RESULTS
RESULTS:= $(patsubst $(SRCDIR)/results/%.cpp, %, $(wildcard $(SRCDIR)/results/*.cpp))
results: $(RESULTS)
$(RESULTS):
	$(CXX) $(CFLAGS) -c $(SRCDIR)/results/$@.cpp -o $(OBJDIR)/hsf/$@.o $(CARG)

### HSF SCHEDULERS
SCHEDULERS:= $(patsubst $(SRCDIR)/schedulers/%.cpp, %, $(wildcard $(SRCDIR)/schedulers/*.cpp))
schedulers: $(SCHEDULERS)
$(SCHEDULERS):
	$(CXX) $(CFLAGS) -c $(SRCDIR)/schedulers/$@.cpp -o $(OBJDIR)/hsf/$@.o $(CARG)

### HSF SERVERS
SERVERS:= $(patsubst $(SRCDIR)/servers/%.cpp, %, $(wildcard $(SRCDIR)/servers/*.cpp))
servers: $(SERVERS)
$(SERVERS):
	$(CXX) $(CFLAGS) -c $(SRCDIR)/servers/$@.cpp -o $(OBJDIR)/hsf/$@.o $(CARG)

### HSF TASKS
TASKS:= $(patsubst $(SRCDIR)/tasks/%.cpp, %, $(wildcard $(SRCDIR)/tasks/*.cpp))
tasks: $(TASKS)
$(TASKS):
	$(CXX) $(CFLAGS) -c $(SRCDIR)/tasks/$@.cpp -o $(OBJDIR)/hsf/$@.o $(CARG)

### HSF UTIL
UTIL:= $(patsubst $(SRCDIR)/util/%.cpp, %, $(wildcard $(SRCDIR)/util/*.cpp))
util: $(UTIL)
$(UTIL):
	$(CXX) $(CFLAGS) -c $(SRCDIR)/util/$@.cpp -o $(OBJDIR)/hsf/$@.o $(CARG)

#################  HSF EXECUTABLE   #################
exe: executable
HSFLIBOBJS=$(OBJDIR)/lib/mjpeg.o $(OBJDIR)/lib/processframe.o $(OBJDIR)/lib/pugixml.o
executable:  
	$(CXX) $(CFLAGS) -c $(SRCDIR)/$(HSFSRC) -o $(OBJDIR)/hsf.o $(CARG)    #compile mainHSF.cpp
	$(CXX) $(LFLAGS) $(OBJDIR)/hsf/*.o $(HSFLIBOBJS) $(OBJDIR)/hsf.o -o $(BINDIR)/hsf $(LARG)  #link all object files

################# SIMFIG EXECUTABLE #################
SIMGFIGOBJ:=$(OBJDIR)/hsf/Trace.o $(OBJDIR)/hsf/Operators.o $(OBJDIR)/hsf/TimeUtil.o $(OBJDIR)/hsf/Enumerations.o $(OBJDIR)/lib/SimulationFigure.o
simfig:
	$(CXX) $(CFLAGS) -c $(LIBDIR)/mathgl/SimulationFigure.cpp -o $(OBJDIR)/lib/SimulationFigure.o $(CARG)
	$(CXX) $(CFLAGS) -c $(SRCDIR)/$(SIMFIGSRC) -o $(OBJDIR)/simfig.o $(CARG)    #compile mainSimulationFigure.cpp
	$(CXX) $(LFLAGS) $(SIMGFIGOBJ) $(OBJDIR)/simfig.o $(MATHGL) -o $(BINDIR)/simfig -lrt

#################   ANALYZE TOOL    #################
analyze:
	$(CXX) $(CFLAGS) -c $(SRCDIR)/$(ANALYZESRC) -o $(OBJDIR)/mainAnalyze.o
	$(CXX) $(LFLAGS) $(OBJDIR)/mainAnalyze.o -o $(BINDIR)/analyze 

#################    SHOW   TOOL    #################
show:
	$(CXX) $(CFLAGS) -c $(SRCDIR)/$(SHOWSRC) -o $(OBJDIR)/mainShow.o
	$(CXX) $(LFLAGS) $(OBJDIR)/mainShow.o -o $(BINDIR)/show 

#################   SIMULATE TOOL   #################
simulate:
	$(CXX) $(CFLAGS) -c $(SRCDIR)/$(SIMULATESRC) -o $(OBJDIR)/mainSimulate.o
	$(CXX) $(LFLAGS) $(OBJDIR)/mainSimulate.o -o $(BINDIR)/simulate

#################     LIBRARIES     #################
lib: libraries
libraries: XML MJPEG

### PUGI XML LIB
XML:
	$(CXX) -Wall -c $(LIBDIR)/$(XML) -o $(OBJDIR)/lib/pugixml.o 

### MJPEG LIB
MJPEG_SRC=mjpeg processframe
MJPEG: $(MJPEG_SRC)
$(MJPEG_SRC):
	$(CXX) $(M_FLAGS) $(LIBDIR)/mjpeg/$@.c -o $(OBJDIR)/lib/$@.o

#################  MISCELLANEOUS   #################

tools: simulate analyze simfig show 

run:
	sudo $(OBJDIR)/$(EXEC)

clean:
	rm -fr $(BINDIR)/*
	rm -fr $(OBJDIR)/*.o
	rm -fr $(OBJDIR)/hsf/*.o
	rm -fr $(OBJDIR)/lib/*.o
