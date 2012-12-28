function show_resourceAllocationCost(name)

name_runtimes = strcat(name,'_alloc_cost_us.csv' );

alloc_cost_us =  csvread(name_runtimes);

fprintf('\n\n  Resource Allocation Cost : \n\n');
fprintf ('\tResource Allocation Cost is  %7.3f (us/s)' ,alloc_cost_us );
