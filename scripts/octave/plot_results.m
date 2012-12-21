function plot_results ( name )
name = strcat(name,"_runtimes.csv");

output = csvread(name);

non_work = output(:,2)'; %'
N = length(non_work);
t = linspace(1,N,N) + 1;

f = figure("visible", "off") ;
stem(t,non_work);
xlabel('Number of Jobs');
ylabel('Implementation cost (%)');
title('HSF Simulation Results');

print('-dpng','../simulation_output_figure.png');
print('-dfig','../simulation_output_figure.fig');
print('-deps','../simulation_output_figure.eps');

clear all;
