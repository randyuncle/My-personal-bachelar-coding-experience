x = [1,2,3,4,5,6,7,8,9,10,11,12,13,14];
%num = input('enter the number of the samples');
out = random_sample(10000);

figure('name', 'relative frequency plot', 'NumberTitle','off');
bar(x, out);
xlabel('x');
ylabel('frequency');
title('relative frequency plot');