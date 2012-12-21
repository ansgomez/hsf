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

###########################
##      LOAD  SWEEP      ##
###########################
for load in "mixed" "busy_wait"
do
	./s_sweep.sh "$topology" $sim_time "$periodicity" "$load"
done