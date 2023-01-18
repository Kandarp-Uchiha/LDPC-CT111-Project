clearvars
% NOTE: You would have to change the address and give the correct appropriate address of 
% where the output H matrix files are to plot the graphs

% % For joint graph plottings
% T1 = readtable('C:\Users\kanda\Desktop\ct11_project_final\soft_BSC\Hmatrix3_out.csv');
% T2 = readtable('C:\Users\kanda\Desktop\ct11_project_final\hard_BSC\Hmatrix3_out.csv');
% T3 = readtable('C:\Users\kanda\Desktop\ct11_project_final\soft_BEC\Hmatrix3_out.csv');
% T4 = readtable('C:\Users\kanda\Desktop\ct11_project_final\hard_BEC\Hmatrix3_out.csv');
% 
% x1 = T1{:,1};
% y1 = T1{:,2};
% 
% x2 = T2{:,1};
% y2 = T2{:,2};
% 
% x3 = T3{:,1};
% y3 = T3{:,2};
% 
% x4 = T4{:,1};
% y4 = T4{:,2};
% 
% plot(x1,y1,':','linewidth',2);
% grid on;
% title('Probability of successful decoding for H(9 x 12)');
% xlabel('p');
% ylabel('Probability of decoding success');
% hold on;
% 
% plot(x2,y2,':','linewidth',2);
% plot(x3,y3,':','linewidth',2);
% plot(x3,y3,':','linewidth',2);
% 
% legend('soft BSC','hard BSC','soft BEC','hard BEC');


% for individual graph plotting
% 1. SOFT BSC
% T = readtable('C:\Users\kanda\Desktop\ct11_project_final\soft_BSC\Hmatrix3_out.csv');
% x = T{:,1};
% y = T{:,2};
% plot(x,y,':','linewidth',2);
% grid on;
% title('Probability of successful decoding for soft BSC for H(9 x 12)');
% xlabel('p');
% ylabel('Probability of decoding success');


% % for individual graph plotting
%1. SOFT BEC
% T = readtable('C:\Users\kanda\Desktop\ct11_project_final\soft_BEC\Hmatrix3_out.csv');
% x = T{:,1};
% y = T{:,2};
% plot(x,y,':','linewidth',2);
% grid on;
% title('Probability of successful decoding for soft BEC for H(9 x 12)');
% xlabel('p');
% ylabel('Probability of decoding success');


% % for individual graph plotting
%1. HARD BSC
% T = readtable('C:\Users\kanda\Desktop\ct11_project_final\hard_BSC\Hmatrix3_out.csv');
% x = T{:,1};
% y = T{:,2};
% plot(x,y,':','linewidth',2);
% grid on;
% title('Probability of successful decoding for hard BSC for H(9 x 12)');
% xlabel('p');
% ylabel('Probability of decoding success');


% % for individual graph plotting
%1. HARD BEC
% T = readtable('C:\Users\kanda\Desktop\ct11_project_final\hard_BEC\Hmatrix3_out.csv');
% x = T{:,1};
% y = T{:,2};
% plot(x,y,':','linewidth',2);
% grid on;
% title('Probability of successful decoding for hard BEC for H(9 x 12)');
% xlabel('p');
% ylabel('Probability of decoding success');



% 3792 x 5056
% 3000 x 5000
% 9 x 12