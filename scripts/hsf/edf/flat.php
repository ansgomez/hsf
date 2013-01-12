<?php

/**  
 * EDF - FLAT TOPOLOGY GENERETING SCRIPT
 *
 * This script has three parameters:
 *
 * Param 1: Simulation time (ms)
 * Param 2: Periodicity (periodic, periodic_jitter)
 * Param 3: Initial worker period (normally [50-1000] ms)
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
$period = $argv[3]; //initial worker period
$n_tasks = $argv[4]; //number of tasks

//Calculated constants...
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
      generateWorker("busy_wait", $i);
   }
?>

</runnable>

</simulation>


<?php

/************************************************************
 *                      FUNCTIONS                           *
 ************************************************************/

//This function generates one worker of a defined load type
function generateWorker($task, $i) {
   global $period;
   global $jitter;
   global $periodicity;
   global $WCET;

   $period_i = $period+10*$i;

   echo "\n";
?>
   <runnable type="worker" periodicity="<?php echo $periodicity; ?>" task="<?php echo $task; ?>">
      <period value="<?php echo $period_i; ?>"  units="ms" />
<?php
      if($periodicity==="periodic_jitter") {
         echo "      <jitter value=\"$jitter\" units=\"ms\" />\n";
      }
      if($task==="busy_wait") {
         echo "      <wcet   value=\"$WCET\" units=\"ms\" />\n";
      }
      
      echo "      <relative_deadline value=\"$period_i\" units=\"ms\" /> ";
      echo "   </runnable>\n";
}
?>
