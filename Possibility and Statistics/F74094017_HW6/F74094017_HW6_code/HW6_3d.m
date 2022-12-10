f = load('MRI_brain_14slices.mat');
result = reshape(f.MRI_brain, [], 1, 1);
%tissue1 -> 200~550; tissue 2 -> 550~1000; tissue 3 -> 1000~1308

%count the amount of each segments
counter1 = 0;
counter2 = 0;
counter3 = 0;
for i = 1:880*640*14
    if(result(i) >= 200 && result(i) <= 550)
        counter1 = counter1 + 1;
    end
    if(result(i) >= 550 && result(i) <= 1000)
        counter2 = counter2 + 1;
    end
    if(result(i) >= 1000 && result(i) <= 1308)
        counter3 = counter3 + 1;
    end
end

%insert data
t1 = zeros(1, counter1);
t2 = zeros(1, counter2);
t3 = zeros(1, counter3);
counter1 = 0;
counter2 = 0;
counter3 = 0;
for i = 1:880*640*14
    if(result(i) >= 200 && result(i) <= 550)
        counter1 = counter1 + 1;
        t1(counter1) = result(i);
    end
    if(result(i) >= 550 && result(i) <= 1000)
        counter2 = counter2 + 1;
        t2(counter2) = result(i);
    end
    if(result(i) >= 1000 && result(i) <= 1308)
        counter3 = counter3 + 1;
        t3(counter3) = result(i);
    end
end

%normal distribution factor made
m1 = mean(t1);
m2 = mean(t2);
m3 = mean(t3);
s1 = std(t1);
s2 = std(t2);
s3 = std(t3);

%using normrnd to make the random variable, then check which number it hits
%to make y1, y2, y3 save the amount of the result (like a histogram)
x = 1:2500;
y1 = zeros(1, 2500);
y2 = zeros(1, 2500);
y3 = zeros(1, 2500);
ran1 = normrnd(m1, s1, [1, counter1]);
ran2 = normrnd(m2, s2, [1, counter2]);
ran3 = normrnd(m3, s3, [1, counter3]);
for i = 1:counter1
    val = ran1(i);
    y1(ceil(val)) = y1(ceil(val)) + 1;
end
for i = 1:counter2
    val = ran2(i);
    y2(ceil(val)) = y2(ceil(val)) + 1;
end
for i = 1:counter3
    val = ran3(i);
    y3(ceil(val)) = y3(ceil(val)) + 1;
end
yt = y1 + y2 + y3;

figure('name', 'normal distribution for tissues in 1st image', 'NumberTitle','off');
hold on;
plot(x, y1);
plot(x, y2);
plot(x, y3);
plot(x, yt);
hold off
xlabel("image intensity");
ylabel("frequency (pixel count)");
legend('tissue 1', 'tissue 2', 'tissue 3', 'all tisues');
title("normal distribution");


