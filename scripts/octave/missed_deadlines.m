function missed_deadlines(name)

%CONSTANTS
SCHED_START=1;
SCHED_END=2;
TASK_START = 3;
TASK_END = 4;

WORKER = 4;

name_traces = strcat(name,'_traces.csv');

if exist(name_traces, "file") == 0
  fprintf("\n   File: '%s' was not found!\n", name_traces);
  return;
end

traces = csvread(name_traces);

name_missed = strcat(name,'_missed_deadlines.csv');

if exist(name_missed, "file") == 0
  fprintf("\n   File: '%s' was not found!\n", name_missed);
  return;
end

[s, err, msg] = lstat(name_missed);

if(s.size == 0)
  return;
end;

missed = csvread(name_missed);

if (size(missed)==0)
  return;
end


name_runtimes = strcat(name,'_runtimes.csv');

if exist(name_runtimes, "file") == 0
  fprintf("\n   File: '%s' was not found!\n", name_runtimes);
  return;
end

runTime = csvread(name_runtimes);

%findes workers ids
worker = sort (runTime(find(runTime(:,1)==WORKER),2));

%finds thread ids and sorts them
thread_ids=unique(traces(:,2));
sorted_ids = sort(thread_ids);

ids_missed = unique(missed(:,2));
sorted_missed = sort (ids_missed);

%finds the number of jobs for each worker
for i=1:size(thread_ids,1);
  numJob(i) = size(find(traces(find(traces(:,2)==sorted_ids(i)),3)==TASK_END),1);
end

for i=1:size(thread_ids,1);
  numJob_missed(i) = size((find(missed(:,1)==sorted_ids(i))),1);
  missed_percentage(i) =(numJob_missed (i)/numJob(i));
end

for i=1:size(thread_ids,1);
lateness(i) =(average(missed(find(missed(:,1)==sorted_ids(i)),4))) - (average(missed(find(missed(:,1)==sorted_ids(i)),3)));
end

for i=1:size(sorted_ids,1);
worker_id(i) = find(sorted_ids(i)==worker);
end


matrix = [worker_id ; missed_percentage ; lateness];
deadlines_matrix =transpose(matrix);


name_missed_deadlines = strcat(name,'_deadline_metrics.csv');
csvwrite(name_missed_deadlines, deadlines_matrix , 'precision', '%2.3f');

clear all;
