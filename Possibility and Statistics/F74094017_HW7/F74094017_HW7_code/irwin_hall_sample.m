function [out] = irwin_hall_sample(n)
%IRWIN_HALL_SAMPLE Summary of this function goes here
%   Detailed explanation goes here
sample = zeros(1, 1000000);
for i = 1:n
    sample = sample + rand(1, 1000000);
end
out = sample;
end

