% TTK4135 - Helicopter lab
% Hints/template for problem 2.
% Updated spring 2018, Andreas L. Flåten

%% ---- PROBLEM 10.2.1 ---- %%
%%Initialization and continous model definition

init06; % Change this to the init file corresponding to your helicopter

A_c = [0 1 0 0;
       0 0 -K_2 0;
       0 0 0 1;
       0 0 -K_1*K_pp -K_1*K_pd];
   
B_c = [0; 0; 0; K_1*K_pp];

%% ---- PROBLEM 10.2.2 ---- %%
%%Discretized model

delta_t	= 0.25;                         % sampling time
I       = eye(4);
A1      = I + delta_t * A_c;
B1      = delta_t * B_c;
%% ---- PROBLEM 10.2.3 ---- %%
%%Calculate optimal trajectory over finite time horizon

% Number of states and inputs
mx = size(A1,2);                        % Number of states (number of columns in A)
mu = size(B1,2);                        % Number of inputs(number of columns in B)

% Initial values
x1_0 = pi;                              % Lambda
x2_0 = 0;                               % r
x3_0 = 0;                               % p
x4_0 = 0;                               % p_dot
x0   = [x1_0 x2_0 x3_0 x4_0]';          % Initial values

% Time horizon and initialization
N  = 100;                               % Time horizon for states
M  = N;                                 % Time horizon for inputs
z  = zeros(N*mx+M*mu,1);                % Initialize z for the whole horizon
z0 = z;                                 % Initial value for optimization

% Bounds
ul 	    = -30*pi/180;                   % Lower bound on control
uu 	    = 30*pi/180;                    % Upper bound on control

xl      = -Inf*ones(mx,1);              % Lower bound on states (no bound)
xu      = Inf*ones(mx,1);               % Upper bound on states (no bound)
xl(3)   = ul;                           % Lower bound on state x3
xu(3)   = uu;                           % Upper bound on state x3

% Generate constraints on measurements and inputs
[vlb,vub]       = gen_constraints(N,M,xl,xu,ul,uu); % hint: gen_constraints
vlb(N*mx+M*mu)  = 0;                    % We want the last input to be zero
vub(N*mx+M*mu)  = 0;                    % We want the last input to be zero

% Generate the matrix G and the vector c (objecitve function weights in the QP problem) 
Q1      = zeros(mx,mx);
Q1(1,1) = 2;                            % Weight on state x1
Q1(2,2) = 0;                            % Weight on state x2
Q1(3,3) = 0;                            % Weight on state x3
Q1(4,4) = 0;                            % Weight on state x4
P1      = 10;                           % Weight on input
G       = gen_q(Q1,P1,N,M);             % Generate G, hint: gen_q
c       = zeros(N*mx+M*mu, 1);          % Generate c, this is the linear constant term in the QP

%%Generate system matrixes for linear model
Aeq       = gen_aeq(A1,B1,N,mx,mu);     % Generate A, hint: gen_aeq
beq       = zeros(size(Aeq,1),1);       % Generate b
beq(1:mx) = A1*x0;

%%Solve QP problem with linear model
tic
[z,lambda] = quadprog(G,c,[],[],Aeq,beq,vlb,vub,x0); % hint: quadprog. Type 'doc quadprog' for more info 
t1=toc;

%%Calculate objective value
phi1        = 0.0;
PhiOut      = zeros(N*mx+M*mu,1);
for i = 1:N*mx+M*mu
  phi1      = phi1+G(i,i)*z(i)*z(i);
  PhiOut(i) = phi1;
end

%%Extract control inputs and states
u  = [z(N*mx+1:N*mx+M*mu);z(N*mx+M*mu)];% Control input from solution

x1 = [x0(1);z(1:mx:N*mx)];              % State x1 from solution
x2 = [x0(2);z(2:mx:N*mx)];              % State x2 from solution
x3 = [x0(3);z(3:mx:N*mx)];              % State x3 from solution
x4 = [x0(4);z(4:mx:N*mx)];              % State x4 from solution

num_variables = 5/delta_t;
zero_padding  = zeros(num_variables,1);
unit_padding  = ones(num_variables,1);

u   = [zero_padding; u; zero_padding];
x1  = [pi*unit_padding; x1; zero_padding]; % lambda
x2  = [zero_padding; x2; zero_padding];
x3  = [zero_padding; x3; zero_padding];    % Pitch
x4  = [zero_padding; x4; zero_padding];

%% ---- PROBLEM 10.2.4 ---- %%
%%Implement in simulink
% To workspace
t                        = 0:delta_t:delta_t*(length(u)-1);
input.signals.values     = u;
input.time               = t;
input.signals.dimensions = 1;

%% Plotting
%% Input
figure(3)
plot(t, u, 'DisplayName', ['q = ', num2str(P1)])
hold on;
grid on;

title('Input for different input weights', 'Interpreter', 'Latex')
xlabel('t[sec]', 'Interpreter', 'Latex')
ylabel('p[rad]', 'Interpreter', 'Latex')
legend('-DynamicLegend')
xlim auto
ylim auto

%% Lambda
figure(4)
plot(Travel.time, Travel.signals.values, 'DisplayName', ['\lambda_{q = ', num2str(P1), '}'])
hold on;
grid on;

title('$\lambda$ for different input weights', 'Interpreter', 'Latex')
xlabel('t[sec]', 'Interpreter', 'Latex')
ylabel('$\lambda$[rad]', 'Interpreter', 'Latex')
legend('-DynamicLegend')
xlim([0 35])
ylim([-0.3 3.8])

%% Pitch 
figure(5)
plot(Pitch.time, Pitch.signals.values, 'DisplayName', ['p_{real, q = ', num2str(P1), '}'])
hold on;
plot(t, x3, 'DisplayName', ['p_{optimal, q = ', num2str(P1), '}'])
grid on;

title('$p$, measurement vs optimal trajectory', 'Interpreter', 'Latex')
xlabel('t[sec]', 'Interpreter', 'Latex')
ylabel('$p$[rad]', 'Interpreter', 'Latex')
legend('-DynamicLegend')
xlim([0 35])

%% Shading effect
shader = patch([17 30 30 17], [0 0 3.5 3.5], [0.3 0.6 0.15]);
set(shader, 'FaceAlpha', 0.2)
shadel = patch([0 5 5 0], [0 0 3.5 3.5], [0.3 0.6 0.15]);
set(shadel, 'FaceAlpha', 0.2)
set(shadel, 'Annotation', 'off')
set(shader, 'Annotation', 'off')