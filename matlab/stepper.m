clc
clear

n=1000;
spr=200*2;
alpha=2*pi/spr; % winkel pro step
tetta=n*alpha;  % Position
dt=2;
om=alpha/dt;    % [rad/sec]

n=100;
t=4000;
c=ones(n,1);

c(1)=(1/t)*sqrt(2*alpha/om);

for i = 2:n
  c(i)=c(i-1)-(2*c(i-1)/(4*i+1));
end

length(c)
a=1:n;
plot(a,c)