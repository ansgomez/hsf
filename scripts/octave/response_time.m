function response_time ( name )

name_traces = strcat(name,'_traces.csv');

if exist(name_traces, "file") == 0
  fprintf("\nFile: '%s' was not found!\n", name_traces);
  return;
end

traces = csvread(name_traces);

%CONSTANTS
TASK_ARRIVAL = 0;
TASK_END = 4;

thread_ids=unique(traces(:,2));
num_threads = length(thread_ids);

actions=traces(:,3);

for i1=1:num_threads
	
    c=1;
    
    t_id = thread_ids(i1);
    
    %find finish and start times for thread i1
    end_times = intersect(find(traces(:,2)==t_id), find(actions==TASK_END));
    arrival_times = intersect(find(traces(:,2)==t_id), find(actions==TASK_ARRIVAL));
    
    %for each ith end time, subtract the ith start time and /1000 to get millis
    for i2=1:length(end_times)
        %R=end-start
        aux = traces(end_times(i2), 1)-traces(arrival_times(i2), 1);
        response_times_ms(c, i1) = aux/1000;
        c=c+1;
    end
    
    N = count(response_times_ms(:,i1));
end

name_resp = strcat(name,'_resp_ms.csv');
csvwrite(name_resp, response_times_ms);

clear all;
