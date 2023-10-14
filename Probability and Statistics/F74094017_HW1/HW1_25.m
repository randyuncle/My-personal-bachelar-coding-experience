x = [72.2,31.9,26.5,29.1,27.3,8.6,22.3,26.5,20.4,12.8,25.1,19.2,24.1,58.2,68.1,89.2,55.1,9.4,14.5,13.9,20.7,17.9,8.5,55.4,38.1,54.2,21.5,26.2,59.1,43.3];
sampleMean = mean(x);
sampleMedian = median(x);
trimmedmean = trimmean(x,10);

histogram(x,10,'Normalization','pdf')
ylabel('Relative Frequency')
xlabel('The families that are in the upper class (percentage)')
hold on
y = -5:0.1:90;
s = std(x);
f = exp(-(y-sampleMean).^2./(2*s^2))./(s*sqrt(2*pi));
plot(y,f,'LineWidth',1.5)