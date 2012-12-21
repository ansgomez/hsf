#!/bin/bash

class=TimeUtil;
dir=util;

grep_string="\""$class".h\"";
new_string="\""$dir/$class".h\"";
sed_string="'s/"$grep_string"/"$new_string"g'";

grep -rl "'$grep_string'" ./ xargs sed -i "$sed_string"
