p = [0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95];
n = 100;
mu3_X = zeros(19, 0);

for i = 1:19
    mu3_X(i, 1) = (1 - 2*p(1, i)) / sqrt(n*p(1, i)*(1 - p(1 ,i)));
end

figure('name', 'skewness of a binomial random variable', 'NumberTitle','off');
bar(p, mu3_X);
%plot(p, mu3_X);
xlabel("p");
ylabel("mu3_X");
title('skewness of a binomial random variable');

%binomial distribution
x = 0:100;
y1 = binopdf(x, 100, 0.1);
y2 = binopdf(x, 100, 0.3);
y3 = binopdf(x, 100, 0.5);
y4 = binopdf(x, 100, 0.7);
y5 = binopdf(x, 100, 0.9);

figure('name','p = 0.1','NumberTitle','off'); 
bar(x, y1);
xlabel("n");
ylabel("b(0:100, 100, 0.1)");
title("n = 10^2, p = 0.1");

figure('name','p = 0.3','NumberTitle','off'); 
bar(x, y2);
xlabel("n");
ylabel("b(0:100, 100, 0.3)");
title("n = 10^2, p = 0.3");

figure('name','p = 0.5','NumberTitle','off'); 
bar(x, y3);
xlabel("n");
ylabel("b(0:100, 100, 0.5)");
title("n = 10^2, p = 0.5");

figure('name','p = 0.7','NumberTitle','off'); 
bar(x, y4);
xlabel("n");
ylabel("b(0:100, 100, 0.7)");
title("n = 10^2, p = 0.7");

figure('name','p = 0.9','NumberTitle','off'); 
bar(x, y5);
xlabel("n");
ylabel("b(0:100, 100, 0.9)");
title("n = 10^2, p = 0.9");