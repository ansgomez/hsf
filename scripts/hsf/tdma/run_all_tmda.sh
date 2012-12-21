#!/bin/bash

###########################
##      VALIDATION       ##
###########################

die () {
    echo >&2 "$@"
    exit 1
}

[ "$#" -eq 1 ] || die "1 argument (simulation_time in secs) required, $# provided"
echo $1 | grep -E -q '^[0-9]+$' || die "Numeric argument required, $1 provided"

###########################
##    INPUT VARIABLES    ##
###########################

#Simulation time (sec)
sim_time=$1;

###########################
##  PERIODICITY  SWEEP   ##
###########################

for topology in "flat" "tree"
do
	./p_sweep.sh "$topology" $sim_time 
done