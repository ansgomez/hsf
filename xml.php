<simulation name="simulation">

<duration units="sec" value="10" />

<runnable type="scheduler" algorithm="TDMA">
<?php
//Variables
   $slot = 10;//$argv[1];
$n_jobs = $argv[1];
$period = $n_jobs*$slot;

//generate workers
for($i=0;$i<$n_jobs;$i++) {
?>
	<runnable type="worker" periodicity="periodic" load="busy_wait">
     		<period value="<?php echo $period; ?>"  units="ms" />
     		<wcet value="<?php echo $slot; ?>" units="ms" />
   	</runnable>
<?php
}
?>
   	<time_slots>
<?php
for($i=0;$i<$n_jobs;$i++) {
?>
     		<time_slot value="<?php echo $slot; ?>"  units="ms" />
<?php
}
?>
   	</time_slots>
</runnable>

</simulation>
