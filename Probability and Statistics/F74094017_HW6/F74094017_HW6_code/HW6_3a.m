f = load('MRI_brain_14slices.mat');
figure('name', 'brain', 'NumberTitle','off');
imagesc(f.MRI_brain(:, :, 1));
colormap(gray);
set(gca,'dataaspectratio',[1 1 1]);
title("brain");
