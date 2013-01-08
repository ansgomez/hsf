#!/bin/bash

php $HSF/scripts/php/html.php $1 > $1.html

mkdir -p $1/

mv "$1"_figure* $1/

mv $1_* $1/
