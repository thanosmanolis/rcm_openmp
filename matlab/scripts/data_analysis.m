%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%    This script generates diagrams to analyze the results    %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Read results from files and plot them
for i = 1:3
    if i == 1
        file = '../results/seq.txt';
        line = '-o';
        color = 'b';
    elseif i == 2
        file = '../results/omp.txt';
        line = '-o';
        color = 'r';
    else
        file = '../results/matlab.txt';
        line = '-*';
        color = 'black';   
    end
    
    A_temp = load(file);
    A = sortrows(A_temp);
    
    % Calculate the speedup
    if i == 1
        max_time = max(A_temp(:,3));
        speedup = horzcat(A(:,1), A(:,3));
    elseif i == 2
        speedup(:,2) = speedup(:,2)./A_temp(:,3);
    end
    
    x = A(:,1);
    y = A(:,3);
    
    plot(x, y, line, 'color', color);
    hold on
end

%% General plot stuff
title('Reverse Cuthill-McKee Algorithm');
xlabel('n - Size of matrix (nxn)');
xticks(0:5000:max(A_temp(:,1))+5000)
ylabel('time (sec)');
yticks(0:0.5:max_time+0.5)
grid on;
axis square;

% TextBoxes
annotation('textbox', [.7 .4 .4 .3], 'String', 'Blue: Sequential', 'FitBoxToText', 'on', 'Color', 'Blue');
annotation('textbox', [.7 .5 .4 .3], 'String', 'Red: OpenMP', 'FitBoxToText', 'on', 'Color', 'Red');
annotation('textbox', [.7 .5 .4 .3], 'String', 'Black: Matlab', 'FitBoxToText', 'on', 'Color', 'Black');

%% Plot the speedup
x = speedup(:,1);
y = speedup(:,2);

figure;
plot(x, y, line, 'color', 'green');

title('Speedup of the parallel implementation');
xlabel('n - Size of matrix (nxn)');
xticks(0:5000:max(A_temp(:,1))+5000)
ylabel('Speedup');
yticks(1:0.1:max(speedup(:,2))+0.1)
grid on;
axis square;