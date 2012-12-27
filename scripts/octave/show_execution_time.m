function show_execution_time(name)

name_exec_ms = strcat(name,'_exec_ms.csv');
exec_ms = csvread(name_exec_ms);

[num_job , num_worker] = size(exec_ms);

fprintf('\n  Execution Times (ms): \n\n');
fprintf('ID    N_Jobs      MIN        AVG        MAX      TOTAL\n');

%for each worker
for i=1:num_worker;
  total=sum(exec_ms(:,i));
  min_i = minimum(exec_ms(:,i));
  avg = average(exec_ms(:,i));
  max_i  =  max(exec_ms(:,i));

  %output
  fprintf('%2d    %3d      %7.3f    %7.3f    %7.3f    %7.3f \n', i, num_job,  min_i , avg, max_i, total);
end
