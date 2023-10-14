%parameters
x = zeros(1, 101);
y = zeros(1, 1001);
mu_x = [25, 75, 25, 75];
mu_y = [1250, 1750, 1250, 1750];
der_x = [30, 30, 20, 30];
der_y = [300, 300, 200, 300];
corr = [0, 0, 0, 0];
f1 = zeros(101, 1001, 2); %the pairs of all x & y
f2 = zeros(101, 1001, 2); %the pairs of all x & y

%initialize x and y
for i = 1:101
    x(i) = i;
end

for i = 1:1001
    y(i) = i + 999;
end

%case 1:
for i = 1:2
    for j = 1:101
        for k = 1:1001
            diff_x = x(j) - mu_x(i);
            diff_y = y(k) - mu_y(i);
            z = (diff_x.^2 / der_x(i).^2) + (diff_y.^2 / der_y(i).^2) - (2 * corr(i) * diff_x * diff_y / (der_x(i) * der_y(i)));
            f1(j, k, i) = exp(-(z / (2 * (1 - corr(i).^2)))) / (2 * pi * der_x(i) * der_y(i) * (1 - corr(i).^2).^(1/2));
        end
    end
end


f1_max = max(f1, [], 'all');
counter1 = 0;
db1_x = zeros(1, 101 * 1001);
db1_y = zeros(1, 101 * 1001);
for i = 1:101
    for j = 1:1001
        if(abs(f1(i, j, 1) - f1(i, j, 2)) < (f1_max * 0.005))
            counter1 = counter1 + 1;
            db1_x(counter1) = x(1, i);
            db1_y(counter1) = y(1, j);
        end
    end
end

%print the plots
figure('name', 'Distribution1 in Case 1', 'NumberTitle','off');
imagesc([0 100], [1000 2000], f1(:,:,1));
set(gca,'YDir','normal'); %y-axis reverse
xlabel("X");
ylabel("Y");
colormap(jet);
colorbar; %display the represent of colors in colormap
title("Case 1 : Distribution 1");

figure('name', 'Distribution2 in Case 1', 'NumberTitle','off');
imagesc([0 100], [1000 2000], f1(:,:,2));
set(gca,'YDir','normal'); %y-axis reverse
xlabel("X");
ylabel("Y");
colormap(jet);
colorbar; %display the represent of colors in colormap
title("Case 1 : Distribution 2");

figure('Name','Decision Boundary in Case 1','NumberTitle','off');
plot(db1_x, db1_y);
axis([0, 100, 1000, 2000]);
xlabel('X');
ylabel('Y');
title('Case1 : Decision Boundary');

%-------------------------------------------------------------------------
%case 2:
for i = 3:4
    for j = 1:101
        for k = 1:1001
            diff_x = x(j) - mu_x(i);
            diff_y = y(k) - mu_y(i);
            z = (diff_x.^2 / der_x(i).^2) + (diff_y.^2 / der_y(i).^2) - (2 * corr(i) * diff_x * diff_y / (der_x(i) * der_y(i)));
            f2(j, k, (i - 2)) = exp(-(z / (2 * (1 - corr(i).^2)))) / (2 * pi * der_x(i) * der_y(i) * (1 - corr(i).^2).^(1/2));
        end
    end
end

f2_max = max(f2, [], 'all');
counter2 = 0;
db2_x = zeros(1, 101 * 1001);
db2_y = zeros(1, 101 * 1001);
for i = 1:101
    for j = 1:1001
        if(abs(f2(i, j, 1) - f2(i, j, 2)) < (f2_max * 0.005))
            counter2 = counter2 + 1;
            db2_x(counter2) = x(i);
            db2_y(counter2) = y(j);
        end
    end
end

%print the plots
figure('name', 'Distribution1 in Case 2', 'NumberTitle','off');
imagesc([0 100], [1000 2000], f2(:,:,1));
set(gca,'YDir','normal'); %y-axis reverse
xlabel("X");
ylabel("Y");
colormap(jet);
colorbar; %display the represent of colors in colormap
title("Case 2 : Distribution1");

figure('name', 'Distribution2 in Case 2', 'NumberTitle','off');
imagesc([0 100], [1000 2000], f2(:,:,2));
set(gca,'YDir','normal'); %y-axis reverse
xlabel("X");
ylabel("Y");
colormap(jet);
colorbar; %display the represent of colors in colormap
title("Case 2 : Distribution 2");

figure('Name','Decision Boundary in Case 2','NumberTitle','off');
plot(db2_x, db2_y);
axis([0, 100, 1000, 2000]);
xlabel('X');
ylabel('Y');
title('Case 2 : Decision Boundary');
