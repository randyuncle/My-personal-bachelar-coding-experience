f = load('MRI_brain_14slices.mat');
result = reshape(f.MRI_brain, [], 1, 1);
figure('name', 'intensity histogram of 14 images', 'NumberTitle','off');
histogram(result, 200);
axis([-50 2500 0 1000000]);
xlabel("image intensity");
ylabel("frequency (pixel count)");
title("intensity histogram of 14 images");