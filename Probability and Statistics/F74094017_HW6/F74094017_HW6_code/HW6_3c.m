f = load('MRI_brain_14slices.mat');
result = reshape(f.MRI_brain, [], 1, 1);
%background -> 200~400; tissue1 -> 200~550; tissue 2 -> 550~1000; tissue 3 -> 1000~1308
t1 = zeros(1, 880*640*14);
t2 = zeros(1, 880*640*14);
t3 = zeros(1, 880*640*14);
for i = 1:880*640*14
    if(result(i) >= 200 && result(i) <= 550)
        t1(i) = 1;
    end
    if(result(i) >= 550 && result(i) <= 1000)
        t2(i) = 1;
    end
    if(result(i) >= 1000 && result(i) <= 1308)
        t3(i) = 1;
    end
end

tissue1 = reshape(t1, 880, 640, 14);
tissue2 = reshape(t2, 880, 640, 14);
tissue3 = reshape(t3, 880, 640, 14);

figure('name', 'Tissue 1', 'NumberTitle','off');
imagesc(tissue1(:, :, 1));
colormap(gray);
set(gca,'dataaspectratio',[1 1 1]);
title("Tissue 1");

figure('name', 'Tissue 2', 'NumberTitle','off');
imagesc(tissue2(:, :, 1));
colormap(gray);
set(gca,'dataaspectratio',[1 1 1]);
title("Tissue 2");

figure('name', 'Tissue 3', 'NumberTitle','off');
imagesc(tissue3(:, :, 1));
colormap(gray);
set(gca,'dataaspectratio',[1 1 1]);
title("Tissue 3");

