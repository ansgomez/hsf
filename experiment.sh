#!/bin/bash

#if old simulation data exists, move it
if [ -a simulation_output.csv ]
  then 
    mv simulation_output.csv old_simulation_output.csv
fi

#foreach XML, simulate and analyze
for i in {2..10}
do
    sudo php xml.php $i > hsf.xml
    sudo bin/main.out
    sudo octave -qf metrics/analysis ../simulation
done

#After experimenting, plot results
sudo octave -qf metrics/plotting ../simulation
