Sample_of_1n = irwin_hall_sample(1);
Sample_of_2n = irwin_hall_sample(2);
Sample_of_20n = irwin_hall_sample(20);

x1 = 0:0.01:1;
x2 = 0:0.01:2;
x20 = 4:0.01:16;
m_1n = 1 / 2;
m_2n = 2 / 2;
m_20n = 20 / 2;
s_1n = 1 / 12;
s_2n = 2 / 12;
s_20n = 20 / 12;
nordis_1n = (1/(s_1n*sqrt(2*pi)))*exp((-(x1-m_1n).^2)/(2*s_1n^2));
nordis_2n = (1/(s_2n*sqrt(2*pi)))*exp((-(x2-m_2n).^2)/(2*s_2n^2));
nordis_20n = (1/(s_20n*sqrt(2*pi)))*exp((-(x20-m_20n).^2)/(2*s_20n^2));

figure('name', 'Sample_of_1n', 'NumberTitle','off');
hold on
histogram(Sample_of_1n, 100, 'Normalization','pdf');
plot(x1, nordis_1n);
hold off
xlabel('Samples');
ylabel('probability density');
legend('irwing-hall distribution','normal distribution (approxiamation)');
title('Sample of 1n')

figure('name', 'Sample_of_2n', 'NumberTitle','off');
hold on
histogram(Sample_of_2n, 100, 'Normalization','pdf');
plot(x2, nordis_2n);
hold off
xlabel('Samples');
ylabel('probability density');
legend('irwing-hall distribution','normal distribution (approxiamation)');
title('Sample of 2n')

figure('name', 'Sample_of_20n', 'NumberTitle','off');
hold on
histogram(Sample_of_20n, 100, 'Normalization','pdf');
plot(x20, nordis_20n);
hold off
xlabel('Samples');
ylabel('probability density');
legend('irwing-hall distribution','normal distribution (approxiamation)');
title('Sample of 20n')