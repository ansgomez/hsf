#!/bin/bash

php $HSF/scripts/php/html.php $1 > $1.html

mkdir -p $1_data/

mv $1_*.* $1_data/
