#!/bin/bash

#if old simulation data exists, move it
if [ -a simulation_output.csv ]
  then 
    mv simulation_output.csv old_simulation_output.csv
fi

for i in {2..40}
do
    php xml.php $i > hsf.xml
    bin/main.out
    octave -qf metrics/analysis ../simulation
done
