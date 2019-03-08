% TTK4135 - Helicopter lab
% Hints/template for problem 2.
% Updated spring 2018, Andreas L. Fl�ten

%% Initialization and model definition
init06; % Change this to the init file corresponding to your helicopter

global lambda_t alpha beta mx N

%CONSTANTS
lambda_t = 2*pi/3; alpha = 0.2; beta = 20;


% Discrete time system model. x = [lambda r p p_dot]'
delta_t	= 0.25; % sampling time
A1 = [1  delta_t 0 0 0 0;
      0 1 -K_2*delta_t 0 0 0;
    0 0 1 delta_t 0 0;
    0 0 -K_1*K_pp*delta_t 1-K_1*K_pd*delta_t 0 0;
    0 0 0 0 1 delta_t;
    0 0 0 0 -delta_t*K_3*K_ep 1-delta_t*K_3*K_ed
    ];
B1 = [0 0 0 delta_t*K_1*K_pp 0 0; 0 0 0 0 0 delta_t*K_3*K_ep]';

% Number of states and inputs
mx = size(A1,2); % Number of states (number of columns in A)
mu = size(B1,2); % Number of inputs(number of columns in B)

% Initial values
x1_0 = pi;                              % Lambda
x2_0 = 0;                               % r
x3_0 = 0;                               % p
x4_0 = 0;                               % p_dot
x5_0 = 0;                               % e
x6_0 = 0;                               % e_dot
x0 = [x1_0 x2_0 x3_0 x4_0 x5_0 x6_0]';           % Initial values

% Time horizon and initialization
N  = 40;                                  % Time horizon for states
M  = N;                                 % Time horizon for inputs
z  = zeros(N*mx+M*mu,1);                % Initialize z for the whole horizon
z0 = z;                                 % Initial value for optimization

% Bounds
ul 	    = [-30*pi/180; -Inf];                   % Lower bound on control
uu 	    = [30*pi/180; Inf];                   % Upper bound on control

xl      = -Inf*ones(mx,1);              % Lower bound on states (no bound)
xu      = Inf*ones(mx,1);               % Upper bound on states (no bound)
xl(3)   = ul(1);                           % Lower bound on state x3
xu(3)   = uu(1);                           % Upper bound on state x3


% Generate constraints on measurements and inputs
[vlb,vub]       = gen_constraints(N,M,xl,xu,ul,uu); % hint: gen_constraints
vlb(N*mx+M*mu)  = 0;                    % We want the last input to be zero
vub(N*mx+M*mu)  = 0;                    % We want the last input to be zero

% Generate the matrix Q and the vector c (objecitve function weights in the QP problem) 
Q1 = zeros(mx,mx);
Q1(1,1) = 2;                            % Weight on state x1
Q1(2,2) = 0;                            % Weight on state x2
Q1(3,3) = 0;                            % Weight on state x3
Q1(4,4) = 0;                            % Weight on state x4
Q1(5,5) = 0;                            % Weight on state x5
Q1(6,6) = 0;                            % Weight on state x6
P1 = 1;                                 % Weight on input q1
P2 = 1;                                 % Weight on input q2
P = [P1 0 ; 0 P2];                     % Input matrix
Q = gen_q(Q1,P,N,M);                    % Generate Q, hint: gen_q
c = zeros(N*mx+M*mu,1);                 % Generate c, this is the linear constant term in the QP


%% Generate system matrixes for linear model
Aeq = gen_aeq(A1,B1,N,mx,mu);           % Generate A, hint: gen_aeq
beq = zeros(size(Aeq, 1),1);            % Generate b
beq(1:mx) = A1*x0; 

%% Solve QP problem with linear model
object_fun = @(z) (1/2*z'*Q*z);
z0(1:mx) = x0;
opt = optimoptions('fmincon', 'Algorithm', 'sqp', 'MaxFunEvals', 40000);
tic
%[z,lambda] = quadprog(Q, c, [], [], Aeq, beq, vlb, vub, x0); % hint: quadprog. Type 'doc quadprog' for more info 
[z, ZVAL, EXITFLAG] = fmincon(object_fun, z0, [], [], Aeq, beq, vlb, vub, @NONLNCON, opt);
t1=toc;

% Calculate objective value
phi1 = 0.0;
PhiOut = zeros(N*mx+M*mu,1);
for i=1:(N*mx+M*mu)
  phi1=phi1+Q(i,i)*z(i)*z(i);
  PhiOut(i) = phi1;
end

%% Extract control inputs and states
u1  = [z(N*mx+1:mu:N*mx+M*mu);z(N*mx+M*mu-1)]; % Control input 1 from solution
u2  = [z(N*mx+2:mu:N*mx+M*mu);z(N*mx+M*mu)]; % Control input 2 from solution

x1 = [x0(1);z(1:mx:N*mx)];              % State x1 from solution
x2 = [x0(2);z(2:mx:N*mx)];              % State x2 from solution
x3 = [x0(3);z(3:mx:N*mx)];              % State x3 from solution
x4 = [x0(4);z(4:mx:N*mx)];              % State x4 from solution
x5 = [x0(5);z(5:mx:N*mx)];
x6 = [x0(6);z(6:mx:N*mx)];

num_variables = 5/delta_t;
zero_padding = zeros(num_variables,1);
unit_padding  = ones(num_variables,1);

u1   = [zero_padding; u1; zero_padding];
u2   = [zero_padding; u2; zero_padding];
x1  = [pi*unit_padding; x1; zero_padding];
x2  = [zero_padding; x2; zero_padding];
x3  = [zero_padding; x3; zero_padding];
x4  = [zero_padding; x4; zero_padding];
x5  = [zero_padding; x5; zero_padding];
x6  = [zero_padding; x6; zero_padding];



%% Plotting
Q = diag([30 1 30 3 100 1]);
R = [1 0; 0 1];

%Plotting the constraint
nonlincon = zeros(length(x1), 1);
for i = 1: length(x1)
    nonlincon(i) = alpha*exp(-beta*(x1(i) - lambda_t)^2);
end
%Tuning matrices
K = dlqr(A1, B1, Q, R); % dlqr - K-matrix for discrete lqr
K_T = (K');
% old 
t = 0:delta_t:delta_t*(length(u1)-1);

%%Creating struct for simulink input
input.signals.values = [u1 u2];
input.time = t;
input.signals.dimensions = 2;
x_opt.signals.values = [x1 x2 x3 x4 x5 x6]; 
x_opt.time = t;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% References
figure(3)
hold on
plot(e_ref.time, reshape(e_ref.signals.values(1,1,:),[], 1), 'k', 'DisplayName', 'Elevation ref')
hold on;
plot(Elevation.time, Elevation.signals.values*(pi/180), 'b', 'DisplayName', 'Actual Elevation ')
legend('-DynamicLegend')
title('References')
%% Optimal vs actual input (with feedback)
figure(5)
plot(t, x1, 'b');
hold on;
grid on;
plot(Travel.time, Travel.signals.values, 'r');
shadel = patch([0 5 5 0], [-1 -1 5 5], [0.3 0.6 0.15]);
set(shadel, 'FaceAlpha', .2);
shader = patch([17 25 25 17], [-1 -1 5 5], [0.3 0.6 0.15]);
set(shader, 'FaceAlpha', .2);
legend('\lambda_{optimal}', '\lambda_{real}');
xlabel('$t[sec]$', 'Interpreter', 'Latex');
ylabel('\lambda[rad]')
title('Optimal $\lambda$ vs actual $\lambda$ (without feedback)', 'Interpreter', 'Latex');
xlim([0 25])

figure(6)
plot(t, x3, 'b');
hold on;
grid on;
plot(Pitch.time, Pitch.signals.values, 'r');
shadel = patch([0 5 5 0], [-1 -1 5 5], [0.3 0.6 0.15]);
set(shadel, 'FaceAlpha', .2);
shader = patch([17 25 25 17], [-1 -1 5 5], [0.3 0.6 0.15]);
set(shader, 'FaceAlpha', .2);
legend('p_{optimal}', 'p_{real}');
xlabel('$t[sec]$', 'Interpreter', 'Latex');
ylabel('p[rad]')
title('Optimal $p$ vs actual $p$ (with feedback)', 'Interpreter', 'Latex');
xlim([0 25])
ylim([-0.5 0.6])

figure(7)
plot(t, x5, 'b');
hold on;
grid on;
plot(Elevation.time, Elevation.signals.values, 'r');
plot(t, nonlincon, '--k')
shadel = patch([0 5 5 0], [-1 -1 5 5], [0.3 0.6 0.15]);
set(shadel, 'FaceAlpha', .2);
shader = patch([17 25 25 17], [-1 -1 5 5], [0.3 0.6 0.15]);
set(shader, 'FaceAlpha', .2);
legend('e_{optimal}', 'e_{real}');
xlabel('$t[sec]$', 'Interpreter', 'Latex');
ylabel('e[rad]')
title('Optimal $e$ vs actual $e$ (with feedback)', 'Interpreter', 'Latex');
xlim([0 25])
ylim([-0.2 0.3])

%% Optimal vs actual input (without feedback)
figure(5)
plot(t, x1, 'b');
hold on;
grid on;
plot(Travel.time, Travel.signals.values, 'r');
shadel = patch([0 5 5 0], [-1 -1 5 5], [0.3 0.6 0.15]);
set(shadel, 'FaceAlpha', .2);
shader = patch([17 25 25 17], [-1 -1 5 5], [0.3 0.6 0.15]);
set(shader, 'FaceAlpha', .2);
legend('\lambda_{optimal}', '\lambda_{real}');
xlabel('$t[sec]$', 'Interpreter', 'Latex');
ylabel('\lambda[rad]')
title('Optimal $\lambda$ vs actual $\lambda$ (without feedback)', 'Interpreter', 'Latex');
xlim([0 25])

figure(6)
plot(t, x3, 'b');
hold on;
grid on;
plot(Pitch.time, Pitch.signals.values, 'r');
shadel = patch([0 5 5 0], [-1 -1 5 5], [0.3 0.6 0.15]);
set(shadel, 'FaceAlpha', .2);
shader = patch([17 25 25 17], [-1 -1 5 5], [0.3 0.6 0.15]);
set(shader, 'FaceAlpha', .2);
legend('p_{optimal}', 'p_{real}');
xlabel('$t[sec]$', 'Interpreter', 'Latex');
ylabel('p[rad]')
title('Optimal $p$ vs actual $p$ (without feedback)', 'Interpreter', 'Latex');
xlim([0 25])
ylim([-0.5 0.6])

figure(7)
plot(t, x5, 'b');
hold on;
grid on;
plot(Elevation.time, Elevation.signals.values, 'r');
plot(t, nonlincon, '--k')
shadel = patch([0 5 5 0], [-1 -1 5 5], [0.3 0.6 0.15]);
set(shadel, 'FaceAlpha', .2);
shader = patch([17 25 25 17], [-1 -1 5 5], [0.3 0.6 0.15]);
set(shader, 'FaceAlpha', .2);
legend('e_{optimal}', 'e_{real}', 'e_{nonlinear constraint}');
xlabel('$t[sec]$', 'Interpreter', 'Latex');
ylabel('e[rad]')
title('Optimal $e$ vs actual $e$ (without feedback)', 'Interpreter', 'Latex');
ylim([-0.2 0.3])
xlim([0 25])

%%
figure(1)
plot(t, x5)
plot(t, nonlincon)

%% Different optimal states and inputs
figure(2)
hold on;
subplot(511)
plot(t,u1, stairs_color, 'DisplayName', ['q = ', num2str(P1)] ),grid
hold on;
legend('-DynamicLegend');
ylabel('u')
axis([0 20 -1 1])
subplot(512)
plot(t,x1, color, 'DisplayName', ['q = ', num2str(P1)]),grid
hold on;
legend('-DynamicLegend');
ylabel('lambda')
axis([0 20 -1 5])
subplot(513)
plot(t,x2, color, 'DisplayName', ['q = ', num2str(P1)]),grid
hold on;
legend('-DynamicLegend');
ylabel('r')
axis([0 20 -1 1])
subplot(514)
plot(t,x3, color, 'DisplayName', ['q = ', num2str(P1)]), grid %'m',t,x3,'.',
hold on;
legend('-DynamicLegend');
ylabel('p')
axis([0 20 -1 1])
subplot(515)
plot(t,x4, color, 'DisplayName', ['q = ', num2str(P1)]),grid
legend('-DynamicLegend');
xlabel('tid (s)'),ylabel('pdot')
axis([0 20 -1 1])
 

