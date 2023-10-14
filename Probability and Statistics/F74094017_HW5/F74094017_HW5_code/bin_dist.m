function output = bin_dist(x, n, p)
    combination = factorial(n)/(factorial(x)*factorial(n-x));
    pos = p.^x;
    q = (1-p).^(n-x);
    output = combination*pos*q;
end