function systemCost(name)

name_runtimes = strcat(name,'_runtimes.csv');

if exist(name_runtimes, "file") == 0
  fprintf("\nFile: '%s' was not found!\n", name_runtimes);
  return;
end

runTime = csvread(name_runtimes);

%finds the indeces of the disp, sched and serv threads
dispatcher =  find(runTime(:,1)==1);
scheduler  =  find(runTime(:,1)==2);
server     =  find(runTime(:,1)==3);

%finds the total runtimes of disp, sched and serv threads
runT_dispatcher = sum(runTime(dispatcher,3));
runT_scheduler  = sum(runTime(scheduler,3));
runT_server     = sum(runTime(server,3));
 

%total simulation time
total = sum(runTime(:,3));


%calculates the resource Allocation Cost
sys_cost_us = ((runT_dispatcher + runT_scheduler + runT_server)*(10^6) /total );


name_sys_cost = strcat(name,'_sys_cost_us.csv');
csvwrite(name_sys_cost, sys_cost_us);
