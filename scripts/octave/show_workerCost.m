function show_workerCost(name)

name_workerCost = strcat(name,'_workerCost.csv');

if exist(name_workerCost, "file") == 0
  fprintf("\nFile: '%s' was not found!\n", name_workerCost);
  return;
end

workerCost = csvread(name_workerCost);

fprintf('\n  Worker Cost : \n\n');
for i=1:size(workerCost);
fprintf ('\t Worker %i=  %7.5f   \n' ,i , workerCost(i) );
end
