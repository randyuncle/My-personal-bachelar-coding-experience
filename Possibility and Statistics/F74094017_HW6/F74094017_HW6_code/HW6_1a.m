%parameters
x = zeros(1, 101);
y = zeros(1, 1001);
mu_x = 50;
mu_y = 1500;
der_x = 20;
der_y = 200;
corr = [0, 0.3, 0.8, -0.8];

%initialize x and y
for i = 1:101
    x(i) = i;
end

for i = 1:1001
    y(i) = i + 999;
end

%calculate f(x, y)
f = zeros(101, 1001, 4); %the pairs of all x & y
for i = 1:4
    for j = 1:101
        for k = 1:1001
            diff_x = x(j) - mu_x;
            diff_y = y(k) - mu_y;
            z = (diff_x.^2 / der_x.^2) + (diff_y.^2 / der_y.^2) - (2 * corr(i) * diff_x * diff_y / (der_x * der_y));
            f(j, k, i) = exp(-(z / (2 * (1 - corr(i).^2)))) / (2 * pi * der_x * der_y * (1 - corr(i).^2).^(1/2));
        end
    end
end

%print the plots
figure('name', 'Distribution 1', 'NumberTitle','off');
imagesc([0 100], [1000 2000], f(:,:,1));
set(gca,'YDir','normal'); %y-axis reverse
xlabel("X");
ylabel("Y");
colormap(jet);
colorbar; %display the represent of colors in colormap
title("Distribution 1");

figure('name', 'Distribution 2', 'NumberTitle','off');
imagesc([0 100], [1000 2000], f(:,:,2));
set(gca,'YDir','normal'); %y-axis reverse
xlabel("X");
ylabel("Y");
colormap(jet);
colorbar; %display the represent of colors in colormap
title("Distribution 2");

figure('name', 'Distribution 3', 'NumberTitle','off');
imagesc([0 100], [1000 2000], f(:,:,3));
set(gca,'YDir','normal'); %y-axis reverse
xlabel("X");
ylabel("Y");
colormap(jet);
colorbar; %display the represent of colors in colormap
title("Distribution 3");

figure('name', 'Distribution 4', 'NumberTitle','off');
imagesc([0 100], [1000 2000], f(:,:,4));
set(gca,'YDir','normal'); %y-axis reverse
xlabel("X");
ylabel("Y");
colormap(jet);
colorbar; %display the represent of colors in colormap
title("Distribution 4");




