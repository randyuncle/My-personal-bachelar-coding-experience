%the function for HW2_1a
function [r] = randomMake(rate, num)
    r = rand(1, num);
    for i = 1:num
        if r(i) <= rate
            r(i) = 1;
        else
            r(i) = 0;
        end
    end
end