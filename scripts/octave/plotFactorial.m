function plotFactorial()

  util = csvread('util.csv');
  n_tasks = csvread('n_tasks.csv');
  sys = csvread('sys.csv');
  alloc = csvread('alloc.csv');
  deadlines = csvread('deadlines.csv');

  util_mat = [util  deadlines];
  util_mat_sorted = sortrows(util_mat, 1);

  f = figure("visible", "off") ;
  xlabel('Utilization(%)');
  ylabel('Deadlines Missed(%)');
  bar(util_mat_sorted(:,1),util_mat_sorted(:,2));
  print('-deps', "util.eps" );
  print('-dpng', "util.png" );

  ntasks_mat = [n_tasks  sys alloc];
  ntasks_sorted = sortrows(ntasks_mat, 1);

  f = figure("visible", "off") ;
  xlabel('Number of Tasks');
  ylabel('Cost');
  hold on;
  bar(ntasks_sorted(:,1),ntasks_sorted(:,2), "facecolor", "r","edgecolor", "r");
  bar(ntasks_sorted(:,1),ntasks_sorted(:,3), "facecolor", "b","edgecolor", "b");
  print('-deps', "n_tasks.eps" );
  print('-dpng', "n_tasks.png" );

endfunction
