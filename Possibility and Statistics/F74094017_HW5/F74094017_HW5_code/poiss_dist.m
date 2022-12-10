function p = poiss_dist(x, mu)
    p = (exp(-mu)*(mu.^x))/factorial(x);
end

