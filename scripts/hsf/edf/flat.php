<?php
/**  EDF - FLAT TOPOLOGY GENERETING SCRIPT
 *
 * This script has three parameters:
 *
 * Param 1: Simulation time (ms)
 * Param 2: Periodicity (periodic, periodic_jitter)
 * Param 3: Number of tasks (normally [2,40])
 * 
 * If periodicity=periodic_jitter, then jitter = period/4
 *
 */

/************************************************************
 *                 GLOBAL VARIABLES                         *
 ************************************************************/

$WCET = 10; //WCET for busy_wait loops
$util = 0.5; //50% utilization

//Input Arguments
$sim_time_ms = $argv[1]; //simulation time
$periodicity = $argv[2]; //task periodicity
$n_tasks = $argv[3]; //number of tasks

//Calculated constants...
$period = $n_tasks*$WCET/$util;
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
function generateWorker($load) {
   global $period;
   global $jitter;
   global $periodicity;
   global $WCET;
   echo "\n";
?>
   <runnable type="worker" periodicity="<?php echo $periodicity; ?>" task="<?php echo $load; ?>">
      <period value="<?php echo $period; ?>"  units="ms" />
<?php
      if($periodicity==="periodic_jitter") {
         echo "      <jitter value=\"$jitter\" units=\"ms\" />\n";
      }
      if($load==="busy_wait") {
         echo "      <wcet   value=\"$WCET\" units=\"ms\" />\n";
      }
      echo "   </runnable>\n";
}
?>