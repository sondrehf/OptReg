%% ---- PROBLEM 10.4.3 ---- %%
%%Implementation of the nonlinear constraint - to be fed into fmincon
function [C, Ceq] = NONLNCON(z)
global alpha beta lambda_t mx N
C = zeros(N,1);
for k=1:N
    C(k) = alpha*exp(-beta*(z(1 + (k-1)*mx) - lambda_t)^2) - z(5+(k-1)*mx);
end
Ceq = [];
end