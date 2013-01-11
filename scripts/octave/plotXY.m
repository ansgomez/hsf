function plotXY ( nameX, nameY )
fileY = strcat(nameY,".csv");

%load data
X = csvread('x.csv');
Y = csvread(fileY);

%generate figure
f = figure("visible", "off") ;
bar(X,Y, "facecolor", "b", "edgecolor", "b")

%define labels
xlabel(nameX);
if (strcmp(nameY,"sys")==1)
    ylabel('Total System Cost (us/s)');
elseif (strcmp(nameY,"alloc")==1)
    ylabel('Resource Allocation Cost (us/s)');
elseif (strcmp(nameY,"deadlines")==1)
    ylabel('% Deadlines Missed');
end

%save to file
fileOut = strcat(nameY,".eps");
print('-deps', fileOut );
fileOut = strcat(nameY,".png");
print('-dpng',fileOut);
%print('-dfig',fileOut);

