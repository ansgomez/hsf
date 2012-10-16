#!/bin/bash

#if old simulation data exists, move it
if [ -a simulation_output.csv ] 
   then 
   sudo rm simulation_output.csv 
fi
if [ -a simulation_output_figure.png ] 
   then 
   sudo rm simulation_output_figure.png 
fi
if [ -a simulation_stats.csv ] 
   then 
   sudo rm simulation_stats.csv 
fi
if [ -a simulation_traces.csv ] 
   then 
   sudo rm simulation_traces.csv 
fi

DATE=`date +%Y%m%d_%2H%M%S`;
DIR=old_results/$DATE;
mkdir $DIR

#foreach XML, simulate and analyze
for i in {2..40}
do
    sudo php xml.php $i > hsf.xml
    sudo bin/main.out
    sudo octave -qf metrics/analysis ../simulation
    
    #Make backup of the intermediate results
    PREFIX=$DIR/$i
    cp hsf.xml $PREFIX"_simulation_input.xml"
    cp simulation_output.csv $PREFIX"_simulation_output.csv"
    cp simulation_stats.csv $PREFIX"_simulation_stats.csv"
    cp simulation_traces.csv $PREFIX"_simulation_traces.csv"
done

#After experimenting, plot results
sudo octave -qf metrics/plotting ../simulation
#Copy experiment figure
cp simulation_output_figure.png $DIR/simulation_output_figure.png

