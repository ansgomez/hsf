#!/bin/bash

###########################
##    INPUT VARIABLES    ##
###########################

#Experiment topology
topology="$1";

#Simulation time (sec)
sim_time=$2;

###########################
##  PERIODICITY  SWEEP   ##
###########################

for periodicity in "periodic" "periodic_jitter"
do
	./l_sweep.sh "$topology" $sim_time "$periodicity";
done