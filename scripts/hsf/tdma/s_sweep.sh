#!/bin/bash

###########################
##    INPUT VARIABLES    ##
###########################

#Experiment topology
topology="$1";

#Simulation time (sec)
sim_time=$2;

#Experiment periodicity
periodicity="$3";

#Experiment load
load="$4";

#Maximum number of tasks
if [ "$topology" == "flat" ]; then
	n_max=40;
else
	n_max=5; #In trees, n=levels
fi

###########################
##      SLOT   SWEEP     ##
###########################
#Slot size (in ms)
for slot in 13 103
do
	./n_sweep.sh "$topology" $sim_time "$periodicity" "$load" $slot $n_max
done