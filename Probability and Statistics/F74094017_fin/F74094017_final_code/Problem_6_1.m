therical_10 = zeros(10, data);
therical_100 = zeros(100, data);
therical_30 = zeros(30, data);

sample_10 = zeros(10, data);
sample_100 = zeros(100, data);
sample_30 = zeros(30, data);
data = 100000;
der = 10;
varSam = der.^2;

for i=1:10
    sample_10(i,:) = rand(10, 1, data);
end
for i=1:100
    sample_100(i,:) = rand(10, 1, data);
end
for i=1:30
    sample_30(i,:) = rand(30, 1, data);
end

s2_10 = zeros(1, data);
s2_100 = zeros(1, data);
s2_30 = zeros(1, data);
for i=1:data
    s2_10(i) = var(sample_10(:,i));
    s2_100(i) = var(sample_100(:,i));
    s2_30(i) = var(sample_30(:,i));
end
