#!/bin/bash

HSF=~/git/HSF
export HSF

PATH=$PATH:$HSF/bin
export PATH

OCTAVE_PATH=$HSF/scripts/octave
octave --no-window-system -qf --eval "addpath(\"$OCTAVE_PATH\"); savepath()"
