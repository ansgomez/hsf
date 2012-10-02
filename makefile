#Main compiler/linker variables
CXX=g++
M_FLAGS=-Wall -pthread -c -g -DVIEWER -lX11
C_FLAGS=-Wall -pthread -c -g
C_ARG=-lrt
L_ARG=-ldl -lpthread -lrt # -lGLU -lGL -lSM -lICE -lX11 -lXext -lpng -lz -Wl-rpath
LFLAGS= -lm 
MATHGL= -rdynamic src/lib/mathgl/libmgl.so.6.0.0
#Directories
OBJDIR=bin
LIBDIR=src/lib
SRCDIR=src

#SOURCE FILES
MAIN_SRC=main.cpp
MJPEG_SRC=mjpeg processframe
HSF_SRC= Aperiodic.cpp BusyWait.cpp Deadline.cpp Dispatcher.cpp Enumerations.cpp Idle.cpp JobTrace.cpp Periodic.cpp Priorities.cpp Runnable.cpp Scheduler.cpp Simulation.cpp Task.cpp TDMA.cpp Thread.cpp Trace.cpp TimeUtil.cpp Worker.cpp Operators.cpp Statistics.cpp RuntimeStatistic.cpp

#AUX VARIABLES
EXEC=$(MAIN_SRC:.c=.out)
MAIN=$(SRCDIR)/$(MAIN_SRC)

all: $(HSF_SRC) SIMFIG $(EXEC)

hsf: $(HSF_SRC) 

$(EXEC):  $(SRCDIR)/$(MAIN_SRC)
	$(CXX) $(C_FLAGS) $(SRCDIR)/$@ -o $(OBJDIR)/$*.o $(C_ARG)
	$(CXX) $(LFLAGS) $(OBJDIR)/*.o $(MATHGL) -o $(OBJDIR)/$*.out $(L_ARG)

$(MJPEG_SRC):
	$(CXX) $(M_FLAGS) $(LIBDIR)/mjpeg/$@.c -o $(OBJDIR)/$@.o

SIMFIG: $(LIBDIR)/mathgl/SimulationFigure.h $(LIBDIR)/mathgl/SimulationFigure.cpp
	$(CXX) -Wall -c $(LIBDIR)/mathgl/SimulationFigure.cpp -o $(OBJDIR)/SimulationFigure.o

$(HSF_SRC):
	$(CXX) $(C_FLAGS) $(SRCDIR)/$@ -o $(OBJDIR)/$*.o $(C_ARG) 

run:
	sudo $(OBJDIR)/$(EXEC)

clean:
	rm -fr $(OBJDIR)/$(EXEC)
	rm -fr $(OBJDIR)/*.o

