%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%    This script applies the Reverse Cuthill-McKee algorithms        %
%    for various input matrices, and then stores the time elapsed    %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

for i = 1:35
    % Read the input matrix
    filename = ['../../matrices/input_' num2str(i*1000) '.csv'];
    input = csvread(filename);
    
    n = size(input,1);
    
    % Apply the RCM algorithm to it
    tic;
    r = symrcm(input);
    time = toc;
    
    % Store the matrix size along with time elapsed to a file
    fileID = fopen('matlab.txt','at');
    fprintf(fileID,'%d, %f\n', n, time);
    fclose(fileID);
end