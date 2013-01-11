#!/bin/bash

#####################
##   DESCRIPTION   ##
#####################

#This script performs a sweep of Ntasks under one topology and saves all results.

#Variable 1 -> Algorithm (EDF, FIFO, RateMonotonic, etc)
#Variable 2 -> Topology (flat or tree)
#Variable 3 -> Max. system utilization (range is [10,100])

#####################
##    CONSTANTS    ##
#####################

#Simulation time in millis
sim_time_ms=1000;
#Maximum number of tasks
n_tasks=2;
#Periodicity of tasks
periodicity="periodic"

#####################
##    VARIABLES    ##
#####################

#Algorithm to experiment with
algorithm="$1"
#Topology
topology="$2";
#Maximum system utilization ([0,1])
util_max="$3";

#Experiment's short name
short_name="$algorithm"_"$topology"_exp;
#Experiment description
desc="Algorithm: $algorithm\nTopology: $topology\nSweep: Utilization [10,"$util_max"]";

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
sudo rm -f "$algorithm"* sys.csv alloc.csv deadlines.csv

#create directory with the shortname
mkdir -p $short_name
#create subdirectory with the timestamp of the experiment
DATE=`date +%2H.%M.%S_%d.%m.%Y`;
DIR=$short_name/$DATE;
mkdir $DIR

cd $DIR

#save description
echo -e $desc > description.txt

echo -e "\n*****     U_SWEEP:      *****"
echo -e "\nRunning:\n\n$desc"

#save the starting time
tmr=$(timer)

#foreach util
for (( i=10 ; i <= $util_max ; i=i+10 ))
do
  echo -e "\n\n***   Simulating: N_tasks = $n ***\n"   
   
  #Generate XML
  php $HSF/scripts/hsf/"$algorithm"/"$topology".php $sim_time_ms $periodicity $i $n_tasks > "$algorithm".xml

  #Execute HSF
  sudo $HSF/bin/hsf $algorithm

  #Calculate all metrics plus figure
  $HSF/bin/calculate all $algorithm
  $HSF/bin/simfig $algorithm

  #Copy relevant results
  echo $i >> x.csv
  cat "$algorithm"_sys_cost_us.csv >> sys.csv
  cat "$algorithm"_alloc_cost_us.csv >> alloc.csv
  if [[ -f "$algorithm"_deadline_total.csv ]]; then
    cat "$algorithm"_deadline_total.csv >> deadlines.csv
  else
    echo "0,0" >> deadlines.csv
  fi

  #Publish results
  publish $algorithm

  #Move simulation results
  PREFIX=util_"$i"
  mkdir $PREFIX
  mv "$algorithm"* $PREFIX/
       
done

#Plot all measures for n_sweep
octave --no-window-system -q --eval "plotExperiment(\"System Utilization\");"

#compare starting time with current time, and save it
echo "$(timer $tmr)" > runtime.txt


