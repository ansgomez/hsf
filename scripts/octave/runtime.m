function runtime(name)
clear total;
input = strcat(name, "_runtimes.csv");
trace = csvread(input);

%THREAD TYPES
IDLE=0;
SCHED=1;
DISP=2;
SERV=3;
WORKER=4;

work_time = 0;
non_work_time=0;

%WORK TIME
workers=( find(trace(:,1)==WORKER));
for i=1:length(workers)
    work_time = work_time + trace(workers(i),3);
end

%NON_WORK TIME
non_workers = find( trace(:,1)==SCHED | trace(:,1)==DISP | trace(:,1)==SERV );
for i=1:length(non_workers)
    non_work_time  = non_work_time + trace(non_workers(i),3);
end

%IDLE TIME
t_idle = workers=( find(trace(:,1)==IDLE) );
idle_time =  trace(t_idle,3);

%PERCENTAGES
total = work_time + non_work_time + idle_time;
work_perc = (work_time/total)*100;
non_work_perc = (non_work_time/total)*100;
idle_perc = (idle_time/total)*100;
total_perc = work_perc + non_work_perc + idle_perc;

#output file
outfile = strcat(name, "_runtimes_summary.txt");
fid = fopen(outfile, "w");

fprintf(fid, "\n\nRuntime Statistics: \n\n");
fprintf(fid, "Work:    %12d us  (%2.3f%%)\n", work_time, work_perc);
fprintf(fid, "Non Work:%12d us  (%2.3f%%)\n", non_work_time, non_work_perc);
fprintf(fid, "Idle:    %12d us  (%2.3f%%)\n",idle_time, idle_perc);
fprintf(fid, "-------------------------------------\n");
fprintf(fid, "Total:   %12d us  (%2.3f%%)\n\n", total, total_perc);

output = strcat(name, "_runtimes.csv");
file = fopen(output, "a");
fprintf(file, "%3.3f,%3.3f,%3.3f,%3.3f,%3.3f,%3.3f\n", work_time, non_work_time, idle_time, work_perc, non_work_perc, idle_perc);
fclose(file);

clear all;
