#!/bin/bash

#####################
##   DESCRIPTION   ##
#####################

#This script performs a sweep of Ntasks under one topology and saves all results.

#The following input must be specified in this order:

#Variable 1 -> Name of topology script
#Variable 2 -> Simulation time (in seconds)
#Variable 3 -> Periodicity (periodic, periodic_jitter)
#Variable 4 -> Task load (mixed, busy_wait)
#Variable 5 -> Slot size (in ms)
#Variable 6 -> Number of tasks (flat), number of levels (tree)

#####################
##    VARIABLES    ##
#####################

#Topology
topology="$1";
#Simulation time (sec)
sim_time=$2;
#Periodicity
periodicity="$3";
#Load
load="$4";
#Topology argument 1
slot=$5;
#Maximum number of tasks
n_max=$6;

#Experiment's short name
short_name="$topology"_"$periodicity"_"$load"_slotsize_"$slot";
#Experiment description
desc="Algorithm: TDMA\nTopology: $topology\nPeriodicity: $periodicity\nLoad: $load\nSlot: $slot";

#####################
##    FUNCTIONS    ##
#####################

#function to measure elapsed time
function timer()
{
    if [[ $# -eq 0 ]]; then
        echo $(date '+%s')
    else
        local  stime=$1
        etime=$(date '+%s')

        if [[ -z "$stime" ]]; then stime=$etime; fi

        dt=$((etime - stime))
        ds=$((dt % 60))
        dm=$(((dt / 60) % 60))
        dh=$((dt / 3600))
        printf '%d:%02d:%02d' $dh $dm $ds
    fi
}

#####################
##       MAIN      ##
#####################

#if old simulation data exists, remove it
if [ -a simulation_* ] 
   then 
   sudo rm simulation_*
   sudo rm *util.sh
   sudo rm hsf.xml
fi

#create directory with a timestamp
DATE=`date +%Y%m%d_%2H%M%S`;
mkdir results/$DATE
#create subdirectory with experiment
DIR=results/$DATE/$short_name;
mkdir $DIR

#save description
echo -e $desc > "$DIR"/description.txt

#Iterate over selected 'overload' factors
for gamma in 1 5 10
do

   echo -e "\n***************************************"
   echo -e "**********     N_SWEEP:      **********"
   echo -e "***************************************"
   echo -e "\nRunning:\n\n$desc"

   #save the starting time
   tmr=$(timer)

   #create subdirectory for results
   mkdir $DIR/gamma_"$gamma"

   #foreach gamma, vary the topology, by increasing the number of tasks
   for (( n=2 ; n <= $n_max ; n++ ))
   do
       echo -e "\n\n******* Simulating: Gamma=$gamma & N_tasks = $n ********\n"   
   
       #Generate XML, execute HSf, and perform analysis
       php "$topology".php $sim_time $periodicity $load $slot $gamma $n > hsf.xml   
       sudo bin/main.out
       sudo octave --no-window-system -qf scripts/octave/analysis ../simulation

       #Copy intermediate results
       PREFIX=$DIR/gamma_"$gamma"/ntasks_"$n"
       mkdir $PREFIX
       cp hsf.xml $PREFIX/"simulation_input.xml"
       cp simulation* $PREFIX/

   done

   #compare starting time with current time, and save it
   echo "$(timer $tmr)" > $DIR/gamma_"$gamma"/runtime.txt
   #After experimenting, plot results
   sudo octave --no-window-system -qf metrics/plotting ../simulation
   #Copy experiment figures
   cp simulation_output_figure.* $DIR/gamma_"$gamma"/
   #Copy experiment output
   cp $PREFIX/"simulation_runtimes.csv" gamma"$gamma".csv
   #erase all temporary simulation files
   sudo rm simulation_*
done

#Combine all results under one plot
sudo octave --no-window-system -qf metrics/experiment_results ../gamma1.csv ../gamma5.csv ../gamma10.csv
#Save figures
sudo mv experiment_output_figure.* $DIR/
#clear auxiliary files
rm gamma*.csv
rm hsf.xml

