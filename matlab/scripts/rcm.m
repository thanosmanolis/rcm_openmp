%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%    This script reads a matrix out of a csv file and      %
%    applies the Reverse Cuthill-McKee algorithm in it.    %
%                                                          %
%    It plots the input matrix, the matrix generated,      %
%    and the matrix generated from the C implementation    %
%    of this algorithm.                                    %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

size = 3;
n = 1000;

%% Read the input matrix
figure;
filename = ['../matrices/inputs/input_' num2str(n) '.csv'];
input = csvread(filename);
spy(input, size);
title('Input matrix');

%% Implementation in C
figure;
filename = ['../matrices/seq/output_' num2str(n) '.csv'];
output_c = csvread(filename);
spy(output_c, size);
title('Reverse Cuthill-McKee (C)');

%% Matlab builtin function
figure;
tic
r = symrcm(input);
toc 
spy(input(r,r), size);
title('Reverse Cuthill-McKee (Matlab)');