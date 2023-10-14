%plotting in probability density function
figure('name','p = 0.01','NumberTitle','off'); 
x1 = 0:10000;
y_b1 = binopdf(x1, 10000, 0.01);
plot(x1, y_b1);
hold on
y_p1 = poisspdf(x1, 10000*0.01);
plot(x1, y_p1);
hold off
legend('binomial distribution', 'poisson distribution');
title("n = 10^4, p = 0.01");

figure('name','p = 0.1','NumberTitle','off'); 
x2 = 0:10000;
y_b2 = binopdf(x2, 10000, 0.1);
plot(x2, y_b2);
hold on
y_p2 = poisspdf(x2, 10000*0.1);
plot(x2, y_p2);
hold off
legend('binomial distribution', 'poisson distribution');
title("n = 10^4, p = 0.1");

figure('name','p = 0.2','NumberTitle','off'); 
x3 = 0:10000;
y_b3 = binopdf(x3, 10000, 0.2);
plot(x3, y_b3);
hold on
y_p3 = poisspdf(x3, 10000*0.2);
plot(x3, y_p3);
hold off
legend('binomial distribution', 'poisson distribution');
title("n = 10^4, p = 0.2");

figure('name','p = 0.5','NumberTitle','off'); 
x4 = 0:10000;
y_b4 = binopdf(x4, 10000, 0.5);
plot(x4, y_b4);
hold on
y_p4 = poisspdf(x4, 10000*0.5);
plot(x4, y_p4);
hold off
legend('binomial distribution', 'poisson distribution');
title("n = 10^4, p = 0.5");

%error plotting
figure('name','errors for p = 0.01','NumberTitle','off');
error = zeros(1, 10001);
for i = 1:10001
    error(1, i) = abs(y_p1(1, i) - y_b1(1, i));
end
plot(x1, error);
title("errors on n = 10^4, p = 0.01");

figure('name','errors for p = 0.1','NumberTitle','off');
error = zeros(1, 10001);
for i = 1:10001
    error(1, i) = abs(y_p2(1, i) - y_b2(1, i));
end
plot(x2, error);
title("errors on n = 10^4, p = 0.1");

figure('name','errors for p = 0.2','NumberTitle','off');
error = zeros(1, 10001);
for i = 1:10001
    error(1, i) = abs(y_p3(1, i) - y_b3(1, i));
end
plot(x3, error);
title("errors on n = 10^4, p = 0.2");

figure('name','errors for p = 0.5','NumberTitle','off');
error = zeros(1, 10001);
for i = 1:10001
    error(1, i) = abs(y_p4(1, i) - y_b4(1, i));
end
plot(x4, error);
title("errors on n = 10^4, p = 0.5");