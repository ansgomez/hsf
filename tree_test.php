<?php
$slot = 40; //$argv[1];
$wcet = 9;
$levels = $argv[1];
$n_jobs = pow(2,$levels);
$period = $n_jobs*$slot;
?>

<simulation name="simulation">

<duration value="5" units="sec" />

<?php
   sched_node($levels, $period, $slot, $wcet);
?>

</simulation>

<?php
function workers($period, $slot, $wcet) {
?>
   <runnable type="worker" periodicity="periodic" load="busy_wait">
     		<period value="<?php echo $period; ?>"  units="ms" />
     		<wcet value="<?php echo $wcet; ?>" units="ms" />
   </runnable>  
<?php
}

function sched_node($level, $period, $slot, $wcet) {
   if($level == 0) {
      workers($period, $slot, $wcet);
      return;
   }
   
   echo '<runnable type="scheduler" algorithm="TDMA">'."\n";
   sched_node($level-1, $period, $slot, $wcet);
   sched_node($level-1, $period, $slot, $wcet);
?>
   <time_slots>
      <time_slot value="<?php echo pow(2,$level-1)*$slot*1.5; ?>"  units="ms" />
      <time_slot value="<?php echo pow(2,$level-1)*$slot*1.5; ?>"  units="ms" />
   </time_slots>
<?php
   echo "</runnable>\n";
}
?>
