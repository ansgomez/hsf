#!/bin/bash

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

#if old simulation data exists, remove it
if [ -a simulation_output_figure.png ] 
   then 
   sudo rm simulation_output_figure.png 
fi
if [ -a simulation_output.csv ] 
   then 
   sudo rm simulation_output.csv 
fi
if [ -a simulation_stats.csv ] 
   then 
   sudo rm simulation_stats.csv 
fi
if [ -a simulation_traces.csv ] 
   then 
   sudo rm simulation_traces.csv 
fi
if [ -a simulation_resp_times_ms.csv ] 
   then 
   sudo rm simulation_resp_times_ms.csv
fi
if [ -a simulation_exec_times_ms.csv] 
   then 
   sudo rm simulation_exec_times_ms.csv
fi

DATE=`date +%Y%m%d_%2H%M%S`;
DIR=old_results/$DATE;
mkdir $DIR

tmr=$(timer)

   #foreach XML, simulate and analyze
   for n in {2..40}
   do
       sudo php experiments/tdma/periodic_flat_highutil_video.php $n 1 > hsf.xml
       sudo bin/main.out
       sudo octave -qf metrics/analysis ../simulation
       
       #Make backup of the intermediate results
       PREFIX=$DIR/$i
       cp hsf.xml $PREFIX"_simulation_input.xml"
       cp simulation_output.csv $PREFIX"_simulation_output.csv"
       cp simulation_stats.csv $PREFIX"_simulation_stats.csv"
       cp simulation_traces.csv $PREFIX"_simulation_traces.csv"
       cp simulation_resp_times_ms.csv $PREFIX"_simulation_resp_times_ms.csv"
       cp simulation_exec_times_ms.csv $PREFIX"_simulation_exec_times_ms.csv"
   done


printf 'Experiment lasted: %s\n' $(timer $tmr) 

#After experimenting, plot results
sudo octave -qf metrics/plotting ../simulation
#Copy experiment figure
cp simulation_output_figure.png $DIR/simulation_output_figure.png

