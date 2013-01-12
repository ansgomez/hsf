#!/bin/bash

#####################
##   DESCRIPTION   ##
#####################

#This script performs a sweep of Ntasks under one topology and saves all results.

#Variable 1 -> EDF Topology (flat or tree)
#Variable 2 -> Max number of tasks (flat), number of levels (tree). range is [2-Nmax]

#####################
##    CONSTANTS    ##
#####################

sim_time_ms=1000
#Simulation time in millis
#Periodicity of tasks
periodicity="periodic"
#Algorithm to experiment with
algorithm="edf"
#Topology
topology="flat";

#####################
##    VARIABLES    ##
#####################

#Maximum initial period
period=80;
#Maximum number of tasks
n_max=5;

#Experiment's short name
short_name="$algorithm"_"$topology"_exp;
#Experiment description
desc="Algorithm: $algorithm\nTopology: $topology\nSweep: N_Tasks [2,"$n_max"]";

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

echo -e "\n*****     N_SWEEP:      *****"
echo -e "\nRunning:\n\n$desc"

#save the starting time
tmr=$(timer)

#foreach n_task
for (( n=2 ; n <= $n_max ; n++ ))
do
  echo -e "\n###   Simulating: N_tasks = $n ###\n"   
   
  #Generate XML
  php $HSF/scripts/hsf/"$algorithm"/"$topology".php $sim_time_ms $periodicity $period $n > "$algorithm".xml

  #Execute HSF
  sudo $HSF/bin/hsf $algorithm

  #Calculate all metrics plus figure
  echo -e "***   Calculating all metrics!  ***"
  $HSF/bin/calculate all $algorithm > /dev/null
  #$HSF/bin/simfig $algorithm

  #Copy relevant results
  echo $n >> n_tasks.csv
  cat "$algorithm"_utilization.csv >> util.csv
  cat "$algorithm"_sys_cost_us.csv >> sys.csv
  cat "$algorithm"_alloc_cost_us.csv >> alloc.csv
  if [[ -f "$algorithm"_deadline_total.csv ]]; then
    cat "$algorithm"_deadline_total.csv >> deadlines.csv
  else
    echo "0" >> deadlines.csv
  fi

  #Publish results
  publish $algorithm

  #Move simulation results
  PREFIX=ntasks_"$n"
  mkdir $PREFIX
  mv "$algorithm"* $PREFIX/
       
done

#Plot all measures for n_sweep
#octave --no-window-system -q --eval "plotExperiment(\"Number of Tasks\");"

#compare starting time with current time, and save it
echo "$(timer $tmr)" > runtime.txt


