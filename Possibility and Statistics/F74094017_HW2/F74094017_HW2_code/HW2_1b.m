B1 = 0.02;
num = 10000;
total = zeros(1, 1000);

for i = 1:1000
    r = randomMake(B1, num);
    for j = 1:num
        if(r(j) == 1)
            total(i) = total(i) + 1;
        end
    end
end

histogram(total,15,'Normalization','probability')
ylabel('Relative Frequency')
xlabel('total number of 1s')

save("HW2_1b_out.mat", 'total');