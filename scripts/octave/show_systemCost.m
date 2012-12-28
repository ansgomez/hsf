function show_systemCost(name)

name_runtimes = strcat(name,'_sys_cost_us.csv');

if exist(name_runtimes, "file") == 0
  fprintf("\nFile: '%s' was not found!\n", name_runtimes);
  return;
end

sys_cost_us =  csvread(name_runtimes); 

fprintf('\n\n  System Cost : \n\n');
fprintf ('\tSystem Cost is  %7.3f (us/s)' ,sys_cost_us );
