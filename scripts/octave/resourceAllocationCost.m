function resourceAllocationCost(name)

name_runtimes = strcat(name,'_runtimes.csv');

if exist(name_runtimes, "file") == 0
  fprintf("\nFile: '%s' was not found!\n", name_runtimes);
  return;
end

runTime = csvread(name_runtimes);

%finds the indeces of the sched and serv threads
scheduler  =  find(runTime(:,1)==2);
server     =  find(runTime(:,1)==3);

%finds the total runtimes of sched and serv threads
runT_scheduler  = sum(runTime(scheduler,3));
runT_server     = sum(runTime(server,3));
 

%total simulation time
total = sum(runTime(:,3));

%calculates the resource Allocation Cost
alloc_cost_us = ((runT_scheduler + runT_server)*(10^6) /total );

name_alloc_cost = strcat(name,'_alloc_cost_us.csv');
csvwrite(name_alloc_cost, alloc_cost_us);

