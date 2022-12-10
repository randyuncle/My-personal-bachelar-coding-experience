x = [1,2,3,4,5,6,7,8,9,10,11,12,13,14];
f_x = [0.04,0.06,0.07,0.08,0.08,0.11,0.07,0.06,0.05,0.03,0.06,0.12,0.10,0.07];

figure('name', 'Discrete Probability Distribution', 'NumberTitle','off');
bar(x,f_x);
xlabel('x');
ylabel('f(x), the probability');
title('Probability Distribution');