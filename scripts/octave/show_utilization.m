function show_utilization(name)

name_traces = strcat(name,'_utilization.csv');

if exist(name_traces, "file") == 0
  fprintf("\nFile: '%s' was not found!\n", name_traces);
  return;
end

utilization = csvread(name_traces);

fprintf('\n  Utilization : \n\n');
fprintf ('\tUtilization is %7.3f%%' ,utilization*100 );
