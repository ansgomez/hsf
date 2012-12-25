#################   DEFINITIONS    #################

#Main compiler/linker variables
CXX=g++
CFLAGS=-Wall -pthread -I./src/
CARG=-lrt
LFLAGS= -lm 
LARG=-ldl -lpthread -lrt

#Architecture dependent variable
ARCH := $(shell getconf LONG_BIT)

#Libraries
XML=xml/pugixml.cpp

#Directories
OBJDIR=bin
LIBDIR=lib
SRCDIR=src

#SOURCE FILES
MAINSRC=main.cpp

#AUX VARIABLES
EXEC=$(MAIN_SRC:.cpp=.out)
MAIN=$(SRCDIR)/$(MAIN_SRC)

#################    MAIN TARGETS   #################

all: libraries hsf #executable

hsf: core criteria dispatchers pthread queues results schedulers servers tasks util 

#################        HSF        #################

### HSF CORE
CORE:= $(patsubst $(SRCDIR)/core/%.cpp, %, $(wildcard $(SRCDIR)/core/*.cpp))
core: $(CORE)
$(CORE):
	$(CXX) $(CFLAGS) -c $(SRCDIR)/core/$@.cpp -o $(OBJDIR)/$@.o $(CARG)

### HSF CRITERIA
CRITERIA:= $(patsubst $(SRCDIR)/criteria/%.cpp, %, $(wildcard $(SRCDIR)/criteria/*.cpp))
criteria: $(CRITERIA)
$(CRITERIA):
	$(CXX) $(CFLAGS) -c $(SRCDIR)/criteria/$@.cpp -o $(OBJDIR)/$@.o $(CARG)


### HSF DISPATCHERS
DISPATCHERS:= $(patsubst $(SRCDIR)/dispatchers/%.cpp, %, $(wildcard $(SRCDIR)/dispatchers/*.cpp))
dispatchers: $(DISPATCHERS)
$(DISPATCHERS):
	$(CXX) $(CFLAGS) -c $(SRCDIR)/dispatchers/$@.cpp -o $(OBJDIR)/$@.o $(CARG)

### HSF DISPATCHERS
PTHREAD:= $(patsubst $(SRCDIR)/pthread/%.cpp, %, $(wildcard $(SRCDIR)/pthread/*.cpp))
pthread: $(PTHREAD)
$(PTHREAD):
	$(CXX) $(CFLAGS) -c $(SRCDIR)/pthread/$@.cpp -o $(OBJDIR)/$@.o $(CARG)

### HSF QUEUES
QUEUES:= $(patsubst $(SRCDIR)/queues/%.cpp, %, $(wildcard $(SRCDIR)/queues/*.cpp))
queues: $(QUEUES)
$(QUEUES):
	$(CXX) $(CFLAGS) -c $(SRCDIR)/queues/$@.cpp -o $(OBJDIR)/$@.o $(CARG)

### HSF RESULTS
RESULTS:= $(patsubst $(SRCDIR)/results/%.cpp, %, $(wildcard $(SRCDIR)/results/*.cpp))
results: $(RESULTS)
$(RESULTS):
	$(CXX) $(CFLAGS) -c $(SRCDIR)/results/$@.cpp -o $(OBJDIR)/$@.o $(CARG)

### HSF SCHEDULERS
SCHEDULERS:= $(patsubst $(SRCDIR)/schedulers/%.cpp, %, $(wildcard $(SRCDIR)/schedulers/*.cpp))
schedulers: $(SCHEDULERS)
$(SCHEDULERS):
	$(CXX) $(CFLAGS) -c $(SRCDIR)/schedulers/$@.cpp -o $(OBJDIR)/$@.o $(CARG)

### HSF SERVERS
SERVERS:= $(patsubst $(SRCDIR)/servers/%.cpp, %, $(wildcard $(SRCDIR)/servers/*.cpp))
servers: $(SERVERS)
$(SERVERS):
	$(CXX) $(CFLAGS) -c $(SRCDIR)/servers/$@.cpp -o $(OBJDIR)/$@.o $(CARG)

### HSF TASKS
TASKS:= $(patsubst $(SRCDIR)/tasks/%.cpp, %, $(wildcard $(SRCDIR)/tasks/*.cpp))
tasks: $(TASKS)
$(TASKS):
	$(CXX) $(CFLAGS) -c $(SRCDIR)/tasks/$@.cpp -o $(OBJDIR)/$@.o $(CARG)

### HSF UTIL
UTIL:= $(patsubst $(SRCDIR)/util/%.cpp, %, $(wildcard $(SRCDIR)/util/*.cpp))
util: $(UTIL)
$(UTIL):
	$(CXX) $(CFLAGS) -c $(SRCDIR)/util/$@.cpp -o $(OBJDIR)/$@.o $(CARG)

#################    EXECUTABLE     #################

executable: $(SRCDIR)/$(MAINSRC)
	$(CXX) $(CFLAGS) -c $(SRCDIR)/$@ -o $(OBJDIR)/$*.o $(CARG)    #compile main.cpp
	$(CXX) $(LFLAGS) $(OBJDIR)/*.o -o $(OBJDIR)/$*.out $(LARG)  #link all object files

#################     LIBRARIES     #################

libraries: XML

XML:
	$(CXX) -Wall -c $(LIBDIR)/$(XML) -o $(OBJDIR)/pugixml.o 

#################  MISCELLANEOUS   #################

run:
	sudo $(OBJDIR)/$(EXEC)

clean:
	rm -fr $(OBJDIR)/*.out
	rm -fr $(OBJDIR)/*.o
