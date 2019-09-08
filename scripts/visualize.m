x = [7.96 7.57 4.2 6.14 2.58 8.36]
y = [5.07 9.82 3.78 0.96 2.49 0.31]

m = [10 20 30 40 50 60] * 2;
clf

hold on
for k=1:numel(x)
	plot(x(k), y(k), ".", "markersize", m(k));
end

% Note that indices here start with 1
split=1+1;

spx=x(split);
spy0=0;
spy1=10;

plot([spx spx], [spy0 spy1], "-")

split=4+1;

spy=y(split);
spx0=0;
spx1=10;

plot([spx0 spx], [spy spy], "-")


hold off
