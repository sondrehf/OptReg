%% Initialization and model definition
init06;

% Model
A_c = [0 1 0 0;
       0 0 -K_2 0;
       0 0 0 1;
       0 0 -K_1*K_pp -K_1*K_pd];
   
B_c = [0; 0; 0; K_1*K_pp];

I = eye(4); % Identity matrix

% Discrete time system model. x = [lambda r p p_dot]'
delta_t = 0.25; % Sampling time
A1 = I + delta_t * A_c;
B1 = delta_t * B_c;

%% Cost function matrices
Q = diag([1 1 1 1]);
R = 1;

K = dlqr(A1, B1, Q, R); % dlqr - K-matrix for discrete lqr




