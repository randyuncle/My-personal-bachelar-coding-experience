data = 1000000;
mu_exp = 10;
n = [10, 100];

sample_10 = zeros(10, data);
sample_100 = zeros(100, data);
for i=1:10
    sample_10(i,:) = exprnd(10, 1, data);
end
for i=1:100
    sample_100(i,:) = exprnd(10, 1, data);
end

m_10 = zeros(1, data);
m_100 = zeros(1, data);
for i=1:data
    m_10(i) = mean(sample_10(:,i));
    m_100(i) = mean(sample_100(:,i));
end

figure('name', 'Sample_size_of_10', 'NumberTitle','off');
histogram(m_10, 100, 'Normalization','pdf');
xlabel('X bar');
ylabel('probability density');
title('X bar of the sample size 10')

figure('name', 'Sample_size_of_100', 'NumberTitle','off');
histogram(m_100, 100, 'Normalization','pdf');
xlabel('X bar');
ylabel('probability density');
title('X bar of the sample size 100')