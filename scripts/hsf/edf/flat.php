<?php

/**  
 * EDF - FLAT TOPOLOGY GENERETING SCRIPT
 *
 * This script has three parameters:
 *
 * Param 1: Simulation time (ms)
 * Param 2: Periodicity (periodic, periodic_jitter)
 * Param 3: Utilization (range is [0,100]
 * Param 4: Number of tasks (normally [2,40])
 * 
 * If periodicity=periodic_jitter, then jitter = period/4
 *
 */

/************************************************************
 *                 GLOBAL VARIABLES                         *
 ************************************************************/

$WCET = 10; //WCET for busy_wait loops

//Input Arguments
$sim_time_ms = $argv[1]; //simulation time
$periodicity = $argv[2]; //task periodicity
$util = $argv[3]; //system utilization
$n_tasks = $argv[4]; //number of tasks

//Calculated constants...
$period = $n_tasks*$WCET/($util/100);
$jitter = ceil($period/4);

/************************************************************
 *                      XML OUTPUT                          *
 ************************************************************/
?>

<simulation name="edf">

<duration value="<?php echo $sim_time_ms; ?>" units="ms"/>

<runnable type="scheduler" algorithm="EDF">
<?php
   //Generate Workers
   for($i=0;$i<$n_tasks;$i++) {
      generateWorker("busy_wait");
   }
?>

</runnable>

</simulation>


<?php

/************************************************************
 *                      FUNCTIONS                           *
 ************************************************************/

//This function generates one worker of a defined load type
function generateWorker($task) {
   global $period;
   global $jitter;
   global $periodicity;
   global $WCET;
   echo "\n";
?>
   <runnable type="worker" periodicity="<?php echo $periodicity; ?>" task="<?php echo $task; ?>">
      <period value="<?php echo $period; ?>"  units="ms" />
<?php
      if($periodicity==="periodic_jitter") {
         echo "      <jitter value=\"$jitter\" units=\"ms\" />\n";
      }
      if($task==="busy_wait") {
         echo "      <wcet   value=\"$WCET\" units=\"ms\" />\n";
      }
      
      echo "      <relative_deadline value=\"$period\" units=\"ms\" /> ";
      echo "   </runnable>\n";
}
?>
