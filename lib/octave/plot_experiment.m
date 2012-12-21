%This scrip reads the output of experiments with GAMMA in {1, 5, 10}
function plot_experiment(file1, file2, file3)
gamma_1 = csvread(file1);
gamma_5 = csvread(file2);
gamma_10 = csvread(file3);

N=size(gamma_1,1);
t = linspace(1,N,N) + 1;

f = figure("visible", "off") ;
hold on;

stem(t, gamma_1(:,2),'r')
stem(t, gamma_5(:,2),'b')
stem(t, gamma_10(:,2),'g')

legend('\gamma=1', '\gamma=5', '\gamma=10');

xlabel('Number of Jobs');
ylabel('Implementation cost (us)');
title('HSF Simulation Results');

print('-dpng','../experiment_output_figure.png');
print('-dfig','../experiment_output_figure.fig');
print('-deps','../experiment_output_figure.eps');

end
