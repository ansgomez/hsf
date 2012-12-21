function response_time ( name )
name_traces = strcat(name,'_traces.csv');

traces = csvread(name_traces);

%CONSTANTS
TASK_ARRIVAL = 0;
TASK_END = 4;

thread_ids=unique(traces(:,2));
num_threads = length(thread_ids);

%MAX_TRACES=length( find(traces(:,3)==4)  );
%response_times_ms = zeros(MAX_TRACES, num_threads);

actions=traces(:,3);

%output file
fileout = strcat(name, '_resp_ms_summary.txt');
fid = fopen(fileout, "w");

fprintf(fid, 'Response Times (ms): \n\n');
fprintf(fid, 'ID    N_Jobs      MIN       AVG       MAX \n');

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

    if N == 0
    	continue;
    end 

    min_i = minimum(response_times_ms(:,i1));
    avg = average(response_times_ms(:,i1));
    max_i =  max(response_times_ms(:,i1));
    
    if N ~= 0
         fprintf(fid, '%2d    %3d      %7.3f    %7.3f    %7.3f \n',thread_ids(i1), N,  min_i, avg, max_i);
    else
         fprintf(fid, '%d - has no completed jobs!\n', thread_ids(i1));
    end 
end

name_resp = strcat(name,'_resp_ms.csv');
csvwrite(name_resp, response_times_ms);

clear all;
