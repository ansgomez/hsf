<?php
/**  TDMA - FLAT TOPOLOGY GENERETING SCRIPT
 *
 * This script has three parameters:
 *
 * Param 1: Simulation time (secs)
 * Param 2: Periodicity (periodic, periodic_jitter)
 * Param 3: Load (mixed, busy_wait)
 * Param 4: Slot size (ms)
 * Param 5: Gamma - overload factor (normally 1,5, or 10)
 * Param 6: Number of tasks (normally [2,40])
 * 
 * If load=mixed, script returns one video task every 10 tasks.
 * If periodicity=periodic_jitter, then jitter = period/4
 * Busy wait loops have WCET = 19ms
 *
 */

/************************************************************
 *                 GLOBAL VARIABLES                         *
 ************************************************************/

$ACET = 17; //Average execution time (ms)
$WCET = 19; //WCET for busy_wait loops

//Input Arguments
$sim_time = $argv[1]; //simulation time
$periodicity = $argv[2]; //task periodicity
$load = $argv[3]; //load type
$slot = $argv[4]; //tdma slot size
$gamma = $argv[5]; //overload factor
$n_tasks = $argv[6]; //number of tasks

//Calculated constants...
$period = $n_tasks*$ACET*$gamma;
$jitter = ceil($period/4);

/************************************************************
 *                      XML OUTPUT                          *
 ************************************************************/
?>

<simulation name="simulation">

<duration value="<?php echo $sim_time; ?>" units="sec"/>

<runnable type="scheduler" algorithm="TDMA">
<?php
   //Generate Workers
   for($i=0;$i<$n_tasks;$i++) {
      if($i % 10 == 0 && $load==="mixed") {
         generateWorker("video");
      }
      else {
         generateWorker("busy_wait");
      }
   }

   //Print TDMA Timeslots
   echo "\n   <time_slots>\n";
   for($i=0;$i<$n_tasks;$i++) {
      echo "     <time_slot value=\"$slot\" units=\"ms\" />\n";
   }
   echo "   </time_slots>\n";
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
   <runnable type="worker" periodicity="<?php echo $periodicity; ?>" load="<?php echo $load; ?>">
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