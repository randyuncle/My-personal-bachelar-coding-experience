num = 10000;
result = zeros(1, 10);

for i = 1:10
    B1 = 0;
    B2 = 0;
    B3 = 0;
    B1_d = 0;
    B2_d = 0;
    B3_d = 0;
    
    %calculate the number of product that each factory probabil did
    r = rand_for_HW2_1c(0.3, 0.45, 0.25, num);
    for j = 1:num
        if(r(j) == 1)
            B1 = B1 + 1;
        elseif(r(j) == 2)
            B2 = B2 + 1;
        else
            B3 = B3 + 1;
        end
    end
    %the current percentage for the product made by each factory
    rB1 = B1 / num;
    rB2 = B2 / num;
    rB3 = B3 / num;
    
    %calculate the defective rate for each factory
    rd1 = randomMake(0.02, B1);
    rd2 = randomMake(0.03, B2);
    rd3 = randomMake(0.02, B3);
    for j = 1:B1
        if(rd1(j) == 1)
            B1_d = B1_d + 1;
        end
    end
    for j = 1:B2
        if(rd2(j) == 1)
            B2_d = B2_d + 1;
        end
    end
    for j = 1:B3
        if(rd3(j) == 1)
            B3_d = B3_d + 1;
        end
    end
    %the current percentage for the defective in each factory
    rB1_d = B1_d / B1;
    rB2_d = B2_d / B2;
    rB3_d = B3_d / B3;

    a = rB1*rB1_d + rB2*rB2_d + rB3*rB3_d;

    result(i) = rB3*rB3_d / a;
end

disp(result)
save('HW2_1c_out.mat', 'result');