clf
clear all
clear

M=load("-ascii", "../kd_lines.txt");

S=load("-ascii", "../kd_search.txt");

% Can be sorted, does not necessarily make it more clear
%M=sortrows(M);

m = M(:,1);
x = M(:,5);
y = M(:,6);

l = M(:,2);

mm=max(m);
m=(mm-m)*30+20;

N=numel(x);

m=ones(1,N) *20;

seed = 283049;
seed = 283;
rand("seed", seed);
colors=rand(N,3);

hold on
for k=1:N
	plot(x(k), y(k), ".", "markersize", m(k), "color", colors(k,:) );
end
	
% plot search item
if (numel(S) > 0)
   plot(S(1), S(2), "o", "markersize", 7, "color", "red" );
end

x1=M(:,7);
y1=M(:,8);
x2=M(:,9);
y2=M(:,10);

for k=1:N
        plot([x1(k) x2(k)], [y1(k) y2(k)], "-", "color", colors(k,:) )
end

labels={};
for i = 1:N
   labels = {labels{:}, ["", num2str(l(i))]};
end

h = legend(labels);
legend(h, "location", "northeastoutside")
legend boxoff

hold off
