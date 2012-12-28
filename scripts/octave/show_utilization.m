function show_utilization(name)

name_traces = strcat(name,'_utilization.csv');
utilization = csvread(name_traces);

fprintf('\n  Utilization : \n\n');
fprintf ('\tUtilization is %7.3f%%' ,utilization*100 );
