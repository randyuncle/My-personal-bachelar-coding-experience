%this random function is different with the function which is made 
%for HW2_1a
function [r] = rand_for_HW2_1c(rate1, rate2, rate3, num)
    r = rand(1, num);
    for i = 1:num
        if r(i) <= rate1
            r(i) = 1;
        elseif r(i) <= (rate1 + rate2)
            r(i) = 2;
        else
            r(i) = 3;
        end
    end
end
