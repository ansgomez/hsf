<?php
/**  TDMA - BINARY TREE TOPOLOGY GENERETING SCRIPT
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
$level = $argv[6]; //number of levels

//Calculated constants...
$n_jobs = pow(2,$level);
$period = $n_jobs*$ACET*$gamma;
$jitter = ceil($period/4);

$indent = "";
/************************************************************
 *                      XML OUTPUT                          *
 ************************************************************/
?>

<simulation name="simulation">

<duration value="<?php echo $sim_time; ?>" units="sec" />
<?php  sched_node($level, $indent); ?>

</simulation>

<?php
/************************************************************
 *                      FUNCTIONS                           *
 ************************************************************/

//This function generates a worker node
function generateWorker($indent) {
  global $periodicity;
  global $period;
  global $slot;
  global $jitter;
  global $WCET;
  global $load;

  static $n = 0;

  if ($n % 10 == 0 && $load==="mixed") {
    $load_str = "video";
  }
  else {
    $load_str = "busy_wait";
  }

  echo "\n$indent<runnable type=\"worker\" periodicity=\"$periodicity\" load=\"$load_str\">\n";
  echo "$indent   <period value=\"$period\" units=\"ms\" />\n";

  if($periodicity==="periodic_jitter") {
    echo "$indent   <jitter value=\"$jitter\" units=\"ms\" />\n";
  }
  if( ($n%10!=0 && $load==="mixed") || $load==="busy_wait") {
    echo "$indent   <wcet   value=\"$WCET\" units=\"ms\" />\n";
  }

  echo "$indent</runnable>\n";
  $n++;
}

//This function generates a Scheduler node
function sched_node($level, $indent) {
  global $slot;

  $indent = $indent."  ";

  if($level == 0) {
    generateWorker($indent);
    return;
  }

  echo "\n$indent<runnable type=\"scheduler\" algorithm=\"TDMA\">\n";

  sched_node($level-1, $indent);
  sched_node($level-1, $indent);   

  $slot_level = pow(2,$level-1)*$slot+1;

  echo "\n$indent  <time_slots>\n";
  echo "$indent     <time_slot value=\"$slot_level\" units=\"ms\" />\n";
  echo "$indent     <time_slot value=\"$slot_level\" units=\"ms\" />\n";
  echo "$indent  </time_slots>\n\n";
  echo "$indent</runnable>\n";
}
?>
