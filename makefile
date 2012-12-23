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

hsf: core criteria #dispatchers pthread queues results schedulers servers tasks util 

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
