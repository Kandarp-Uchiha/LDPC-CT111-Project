clc ;
clearvars ;
HX = importdata("analytic_out.txt") ;
%imported csv The HX matrix received from CSV contains erasure probability for different values of p at different number of iterations.

% The 101 rows represent different values of erasure probability or p starting from 0 to 1 in steps of 0.01 
% The columns represent number of iterations.
p = 0:0.01:1;            % probability value

plot(p,1-HX(:,10),"linewidth",2)    
title("Analytic graph for 9*12 matrix")
xlabel("p")
ylabel("Probability of successful erasure removal")
grid