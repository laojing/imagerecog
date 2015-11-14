function f=gaosilvbo(pp)
[M,N]=size(pp);
f=zeros(M,N);                    
alf=2.5;
n=13;%定义模板大小
n1=(n+1)/2;
w=[1 2 4 8 12 15 16 15 12 8 4 2 1];
w=1/100*w;
for j=1:N
    for i=7:M-6
    f(i,j)=w(1,1)*pp(i-6,j)+w(1,2)*pp(i-5,j)+w(1,3)*pp(i-4,j)+w(1,4)*pp(i-3,j)+w(1,5)*pp(i-2,j)+w(1,6)*pp(i-1,j)+w(1,7)*pp(i,j)+w(1,8)*pp(i+1,j)+w(1,9)*pp(i+2,j)+w(1,10)*pp(i+3,j)+w(1,11)*pp(i+4,j)+w(1,12)*pp(i+5,j)+w(1,13)*pp(i+6,j);
    end
end
for j=1:N
    for i=1
    f(i,j)=w(1,1)*pp(M-5,j)+w(1,2)*pp(M-4,j)+w(1,3)*pp(M-3,j)+w(1,4)*pp(M-2,j)+w(1,5)*pp(M-1,j)+w(1,6)*pp(M,j)+w(1,7)*pp(i,j)+w(1,8)*pp(i+1,j)+w(1,9)*pp(i+2,j)+w(1,10)*pp(i+3,j)+w(1,11)*pp(i+4,j)+w(1,12)*pp(i+5,j)+w(1,13)*pp(i+6,j);
    end
end
for j=1:N
    for i=2
    f(i,j)=w(1,1)*pp(M-4,j)+w(1,2)*pp(M-3,j)+w(1,3)*pp(M-2,j)+w(1,4)*pp(M-1,j)+w(1,5)*pp(M,j)+w(1,6)*pp(i-1,j)+w(1,7)*pp(i,j)+w(1,8)*pp(i+1,j)+w(1,9)*pp(i+2,j)+w(1,10)*pp(i+3,j)+w(1,11)*pp(i+4,j)+w(1,12)*pp(i+5,j)+w(1,13)*pp(i+6,j);
    end
end
for j=1:N
    for i=3
    f(i,j)=w(1,1)*pp(M-3,j)+w(1,2)*pp(M-2,j)+w(1,3)*pp(M-1,j)+w(1,4)*pp(M,j)+w(1,5)*pp(i-2,j)+w(1,6)*pp(i-1,j)+w(1,7)*pp(i,j)+w(1,8)*pp(i+1,j)+w(1,9)*pp(i+2,j)+w(1,10)*pp(i+3,j)+w(1,11)*pp(i+4,j)+w(1,12)*pp(i+5,j)+w(1,13)*pp(i+6,j);
    end
end
for j=1:N
    for i=4
    f(i,j)=w(1,1)*pp(M-2,j)+w(1,2)*pp(M-1,j)+w(1,3)*pp(M,j)+w(1,4)*pp(i-3,j)+w(1,5)*pp(i-2,j)+w(1,6)*pp(i-1,j)+w(1,7)*pp(i,j)+w(1,8)*pp(i+1,j)+w(1,9)*pp(i+2,j)+w(1,10)*pp(i+3,j)+w(1,11)*pp(i+4,j)+w(1,12)*pp(i+5,j)+w(1,13)*pp(i+6,j);
    end
end
for j=1:N
    for i=5
    f(i,j)=w(1,1)*pp(M-1,j)+w(1,2)*pp(M,j)+w(1,3)*pp(i-4,j)+w(1,4)*pp(i-3,j)+w(1,5)*pp(i-2,j)+w(1,6)*pp(i-1,j)+w(1,7)*pp(i,j)+w(1,8)*pp(i+1,j)+w(1,9)*pp(i+2,j)+w(1,10)*pp(i+3,j)+w(1,11)*pp(i+4,j)+w(1,12)*pp(i+5,j)+w(1,13)*pp(i+6,j);
    end
end
for j=1:N
    for i=6
    f(i,j)=w(1,1)*pp(M,j)+w(1,2)*pp(i-5,j)+w(1,3)*pp(i-4,j)+w(1,4)*pp(i-3,j)+w(1,5)*pp(i-2,j)+w(1,6)*pp(i-1,j)+w(1,7)*pp(i,j)+w(1,8)*pp(i+1,j)+w(1,9)*pp(i+2,j)+w(1,10)*pp(i+3,j)+w(1,11)*pp(i+4,j)+w(1,12)*pp(i+5,j)+w(1,13)*pp(i+6,j);
    end
end
for j=1:N
    for i=7:M-5
    f(i,j)=w(1,1)*pp(i-6,j)+w(1,2)*pp(i-5,j)+w(1,3)*pp(i-4,j)+w(1,4)*pp(i-3,j)+w(1,5)*pp(i-2,j)+w(1,6)*pp(i-1,j)+w(1,7)*pp(i,j)+w(1,8)*pp(i+1,j)+w(1,9)*pp(i+2,j)+w(1,10)*pp(i+3,j)+w(1,11)*pp(i+4,j)+w(1,12)*pp(i+5,j)+w(1,13)*pp(1,j);
    end
end
for j=1:N
    for i=7:M-4
    f(i,j)=w(1,1)*pp(i-6,j)+w(1,2)*pp(i-5,j)+w(1,3)*pp(i-4,j)+w(1,4)*pp(i-3,j)+w(1,5)*pp(i-2,j)+w(1,6)*pp(i-1,j)+w(1,7)*pp(i,j)+w(1,8)*pp(i+1,j)+w(1,9)*pp(i+2,j)+w(1,10)*pp(i+3,j)+w(1,11)*pp(i+4,j)+w(1,12)*pp(1,j)+w(1,13)*pp(2,j);
    end
end
for j=1:N
    for i=7:M-3
    f(i,j)=w(1,1)*pp(i-6,j)+w(1,2)*pp(i-5,j)+w(1,3)*pp(i-4,j)+w(1,4)*pp(i-3,j)+w(1,5)*pp(i-2,j)+w(1,6)*pp(i-1,j)+w(1,7)*pp(i,j)+w(1,8)*pp(i+1,j)+w(1,9)*pp(i+2,j)+w(1,10)*pp(i+3,j)+w(1,11)*pp(1,j)+w(1,12)*pp(2,j)+w(1,13)*pp(3,j);
    end
end
for j=1:N
    for i=7:M-2
    f(i,j)=w(1,1)*pp(i-6,j)+w(1,2)*pp(i-5,j)+w(1,3)*pp(i-4,j)+w(1,4)*pp(i-3,j)+w(1,5)*pp(i-2,j)+w(1,6)*pp(i-1,j)+w(1,7)*pp(i,j)+w(1,8)*pp(i+1,j)+w(1,9)*pp(i+2,j)+w(1,10)*pp(1,j)+w(1,11)*pp(2,j)+w(1,12)*pp(3,j)+w(1,13)*pp(4,j);
    end
end
for j=1:N
    for i=7:M-1
    f(i,j)=w(1,1)*pp(i-6,j)+w(1,2)*pp(i-5,j)+w(1,3)*pp(i-4,j)+w(1,4)*pp(i-3,j)+w(1,5)*pp(i-2,j)+w(1,6)*pp(i-1,j)+w(1,7)*pp(i,j)+w(1,8)*pp(i+1,j)+w(1,9)*pp(1,j)+w(1,10)*pp(2,j)+w(1,11)*pp(3,j)+w(1,12)*pp(4,j)+w(1,13)*pp(5,j);
    end
end
for j=1:N
    for i=7:M
    f(i,j)=w(1,1)*pp(i-6,j)+w(1,2)*pp(i-5,j)+w(1,3)*pp(i-4,j)+w(1,4)*pp(i-3,j)+w(1,5)*pp(i-2,j)+w(1,6)*pp(i-1,j)+w(1,7)*pp(i,j)+w(1,8)*pp(1,j)+w(1,9)*pp(2,j)+w(1,10)*pp(3,j)+w(1,11)*pp(4,j)+w(1,12)*pp(5,j)+w(1,13)*pp(6,j);
    end
end


