function plotXY ( nameX, nameY )
fileY = strcat(nameY,".csv");

X = csvread('x.csv');
Y = csvread(fileY);

f = figure("visible", "off") ;
bar(X,Y, "facecolor", "b")
xlabel(nameX);
if (strcmp(nameY,"sys")==1)
    ylabel('Total System Cost (us/s)');
elseif (strcmp(nameY,"alloc")==1)
    ylabel('Resource Allocation Cost (us/s)');
elseif (strcmp(nameY,"deadlines")==1)
    ylabel('% Deadlines Missed');
end

fileOut = strcat(nameY,".eps");
%print('-dpng','../sys.png');
%print('-dfig','../sys.fig');
print('-deps', fileOut );

