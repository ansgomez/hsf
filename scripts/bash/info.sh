#!/bin/bash

if [[ $1 == "html" ]]; then
echo "<tr>"
echo "<td align=\"right\"><b>Kernel:</b></td>"
echo "<td>"`uname -r`"</td>"
echo "</tr>"
echo "<tr>"
echo "<td align=\"right\"><b>Arch:</b></td>"
echo "<td>x86_64"`uname -p`"</td>"
echo "</tr>"
echo "<tr>"
echo "<td align=\"right\"><b>Processor:</b></td>"
echo "<td>"`cat /proc/cpuinfo | grep "model name" -m 1 | awk -F':' '{printf$2}'`"</td>"
echo "</tr>"
echo "<tr>"
echo "<td align=\"right\"><b>RAM:</b></td>"
echo "<td>"`cat /proc/meminfo | grep "MemTotal" | awk -F':' '{print$2}'`"</td>"
echo "</tr>"

else
uname -r
uname -p
cat /proc/cpuinfo | grep "model name" -m 1 | awk -F':' '{printf$2}'
echo ""
cat /proc/meminfo | grep "MemTotal" | awk -F':' '{print$2}'
fi

