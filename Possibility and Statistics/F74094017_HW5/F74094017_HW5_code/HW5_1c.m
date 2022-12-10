%setting the accuracy
digits(4);
%n and r is on the vertical side
n = ['1';' ';'2';' ';' ';'3';' ';' ';' ';'4';' ';' ';' ';' ';'5';' ';' ';' ';' ';' ';'6';' ';' ';' ';' ';' ';' ';'7';' ';' ';' ';' ';' ';' ';' ';];
r = [0;1;0;1;2;0;1;2;3;0;1;2;3;4;0;1;2;3;4;5;0;1;2;3;4;5;6;0;1;2;3;4;5;6;7];
%the probability array (horizontal)
p = [0.10,0.20,0.25,0.30,0.40,0.50,0.60,0.70,0.80,0.90];
prb = zeros(35, 10);

%calculate
counter = 0;
for rou = 1:7
    sum1 = zeros(1:10);
    for i = 1:(rou + 1)
        counter = counter + 1;
        for j = 1:10
            sum1(1, j) = sum1(1, j) + bin_dist(i - 1, rou, p(1, j));
            prb(counter, j) = sum1(1, j);
        end
    end
end

%setting the vertical of p in the original table
p0_10 = zeros(35:1);
for i = 1:35
    p0_10(i, 1) = roundn(prb(i, 1), -4);
end

p0_20 = zeros(35:1);
for i = 1:35
    p0_20(i, 1) = roundn(prb(i, 2), -4);
end

p0_25 = zeros(35:1);
for i = 1:35
    p0_25(i, 1) = roundn(prb(i, 3), -4);
end

p0_30 = zeros(35:1);
for i = 1:35
    p0_30(i, 1) = roundn(prb(i, 4), -4);
end

p0_40 = zeros(35:1);
for i = 1:35
    p0_40(i, 1) = roundn(prb(i, 5), -4);
end

p0_50 = zeros(35:1);
for i = 1:35
    p0_50(i, 1) = roundn(prb(i, 6), -4);
end

p0_60 = zeros(35:1);
for i = 1:35
    p0_60(i, 1) = roundn(prb(i, 7), -4);
end

p0_70 = zeros(35:1);
for i = 1:35
    p0_70(i, 1) = roundn(prb(i, 8), -4);
end

p0_80 = zeros(35:1);
for i = 1:35
    p0_80(i, 1) = roundn(prb(i, 9), -4);
end

p0_90 = zeros(35:1);
for i = 1:35
    p0_90(i, 1) = roundn(prb(i, 10), -4);
end

T = table(n, r, p0_10, p0_20, p0_25, p0_30, p0_40, p0_50, p0_60, p0_70, p0_80, p0_90);
% Get the table in string form.
TString = evalc('disp(T)');
figure('name','Binomial distribution table','NumberTitle','off'); 
% Use TeX Markup for bold formatting and underscores.
TString = strrep(TString,'<strong>','\bf');
TString = strrep(TString,'</strong>','\rm');
TString = strrep(TString,'_','\_');
% Get a fixed-width font.
FixedWidth = get(0,'FixedWidthFontName');
% Output the table using the annotation command.
annotation(gcf,'Textbox','String',TString,'Interpreter','Tex','FontName',FixedWidth,'Units','Normalized','Position',[0 0 1 1]);