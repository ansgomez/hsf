<?php

/**  
 * EDF - FLAT TOPOLOGY GENERETING SCRIPT
 *
 * This script has three parameters:
 *
 * Param 1: Simulation time (ms)
 * Param 2: Periodicity (periodic, periodic_jitter)
 * Param 3: Mean Utilization
 * Param 4: Number of tasks (normally [2,40])
 * 
 * If periodicity=periodic_jitter, then jitter = period/4
 *
 */

/************************************************************
 *                 GLOBAL VARIABLES                         *
 ************************************************************/

$BCET = 10;  //min. execution time
$WCET = 30; //max. execution time

//Input Arguments
$sim_time_ms = $argv[1]; //simulation time
$periodicity = $argv[2]; //task periodicity
$utilization = $argv[3]; //mean utilization
$n_tasks = $argv[4]; //number of tasks

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
   global $utilization;
   global $n_tasks;
   global $periodicity;
   global $BCET;
   global $WCET;

   $EXE = mt_rand($BCET, $WCET);

   $period = $n_tasks*$EXE/($utilization/100);

   $period_r = ceil( mt_rand(0.75*$period, 1.25*$period));

   $jitter = ceil($period_r/4);

   echo "\n";
?>
   <runnable type="worker" periodicity="<?php echo $periodicity; ?>" task="<?php echo $task; ?>">
      <period value="<?php echo $period_r; ?>"  units="ms" />
<?php
      if($periodicity==="periodic_jitter") {
         echo "      <jitter value=\"$jitter\" units=\"ms\" />\n";
      }
      if($task==="busy_wait") {
         echo "      <wcet   value=\"$EXE\" units=\"ms\" />\n";
      }
      
      echo "      <relative_deadline value=\"$period_r\" units=\"ms\" /> \n";
      echo "   </runnable>\n";
}
?>
