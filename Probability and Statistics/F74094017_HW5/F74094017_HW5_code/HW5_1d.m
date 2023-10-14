r = [0;1;2;3;4;5;6;7;8;9;10;11;12;13;14;15;16;17;18;19;20;21;22;23;24];
mu = [5.5,6.0,6.5,7.0,7.5,8.0,8.5,9.0,9.5];

%calculate
pbr_poiss = zeros(25, 9);
sum1 = zeros(1, 9);
for i = 1:9
    for j = 1:25
        sum1(1, i) = sum1(1, i) + poiss_dist(j - 1, mu(1, i));
        pbr_poiss(j, i) = sum1(1, i);
    end
end

%setting the vertical of mu in the original table
mu5_5 = zeros(25:1);
for i = 1:25
    mu5_5(i, 1) = roundn(pbr_poiss(i, 1), -4);
end

mu6_0 = zeros(25:1);
for i = 1:25
    mu6_0(i, 1) = roundn(pbr_poiss(i, 2), -4);
end

mu6_5 = zeros(25:1);
for i = 1:25
    mu6_5(i, 1) = roundn(pbr_poiss(i, 3), -4);
end

mu7_0 = zeros(25:1);
for i = 1:25
    mu7_0(i, 1) = roundn(pbr_poiss(i, 4), -4);
end

mu7_5 = zeros(25:1);
for i = 1:25
    mu7_5(i, 1) = roundn(pbr_poiss(i, 5), -4);
end

mu8_0 = zeros(25:1);
for i = 1:25
    mu8_0(i, 1) = roundn(pbr_poiss(i, 6), -4);
end

mu8_5 = zeros(25:1);
for i = 1:25
    mu8_5(i, 1) = roundn(pbr_poiss(i, 7), -4);
end

mu9_0 = zeros(25:1);
for i = 1:25
    mu9_0(i, 1) = roundn(pbr_poiss(i, 8), -4);
end

mu9_5 = zeros(25:1);
for i = 1:25
    mu9_5(i, 1) = roundn(pbr_poiss(i, 9), -4);
end

T = table(r, mu5_5, mu6_0, mu6_5, mu7_0, mu7_5, mu8_0, mu8_5, mu9_0, mu9_5);
% Get the table in string form.
TString = evalc('disp(T)');
figure('name','Poisson distribution table','NumberTitle','off'); 
% Use TeX Markup for bold formatting and underscores.
TString = strrep(TString,'<strong>','\bf');
TString = strrep(TString,'</strong>','\rm');
TString = strrep(TString,'_','\_');
% Get a fixed-width font.
FixedWidth = get(0,'FixedWidthFontName');
% Output the table using the annotation command.
annotation(gcf,'Textbox','String',TString,'Interpreter','Tex','FontName',FixedWidth,'Units','Normalized','Position',[0 0 1 1]);