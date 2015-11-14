%%
clc
clear
A=imread('90-1.bmp');
Gaussian(A);
image=imread('Gauss.bmp');
MM=2;
NN=3;
[th F]=sobell(MM,NN,image);

%当量值19.5
%%
n=length(th);
x=th(:,2);
y=th(:,1);
rl=3179.5;
plot(x,y,'.');hold on
[xc,yc,Re,a] = circfit(x,y);
t=0:0.001:2*pi;
xx=Re*cos(t)+xc;
yy=Re*sin(t)+yc;
plot(xx,yy,'-.');hold on;
text(xc,yc,sprintf('center (%g , %g );  R=%g',xc,yc,Re))
p=polyfit(x,y,1);
pt=polyval(p,x);
plot(x,pt,'r');hold on;
while abs(Re-rl)>=8
    x0=[0 0];
    F=@(xcz)([p(1,1)*xcz(1,2)+xcz(1,1)-p(1,1)*yc-xc;
        p(1,1)*xcz(1,1)+p(1,2)-xcz(1,2)]);
    xcz=fsolve(F,x0);%垂足
    r1=(rl-Re)*2/3;
    rc=sqrt((xc-xcz(1,1))^2+(yc-xcz(1,2))^2);
    x0=[10000 10000];
    F1=@(xcz1)([(xc-xcz(1,1))*(xcz1(1,2)-yc)-(xcz1(1,1)-xc)*(yc-xcz(1,2));
        (xcz1(1,1)-xc)^2+(xcz1(1,2)-yc)^2-r1^2]);
    xcz1=fsolve(F1,x0);%新圆心
    n1=length(x);
    q=zeros(n1,1);
    rr=zeros(n1,1);
    for i=1:n1
        q(i,1)=sqrt((xcz1(1,1)-x(i))^2+(xcz1(1,2)-y(i))^2);
    end
    r=max(q);
    e1=find(q>=r-10);
    w1=size(e1);
    for ii=1:w1(1,1);
        ps(ii,1)=x(e1(ii,1),:);
        ps(ii,2)=y(e1(ii,1),:);
    end
    figure(2)
    plot(ps(:,1),ps(:,2),'.');hold on   %齿顶
    [xc,yc,Re,a] = circfit(ps(:,1),ps(:,2));
    p=polyfit(ps(:,1),ps(:,2),1);
    pt1=polyval(p,ps(:,1));
    % plot(ps(:,1),pt1,'r');hold on;
end

%%
for i=1:n
    qt(i,1)=sqrt((xc(1,1)-x(i))^2+(yc(1,1)-y(i))^2);
end
ts=abs(qt-Re);
e3=find(ts<=2);
w3=size(e3);
for ii=1:w3(1,1);
    ps3(ii,1)=x(e3(ii,1),:);
    ps3(ii,2)=y(e3(ii,1),:);
end
figure(2)
plot(ps3(:,1),ps3(:,2),'b.');hold on   %齿顶
[xct,yct,Ret,a] = circfit(ps3(:,1),ps3(:,2));
%%
lcg=2948.7;
for i=1:n1
    qc(i,1)=sqrt((xct-x(i))^2+(yct-y(i))^2);
end
r=max(qc);
e2=find(qc<=lcg+5);
w2=size(e2);
for ii=1:w2(1,1);
    cg(ii,1)=x(e2(ii,1),:);
    cg(ii,2)=y(e2(ii,1),:);
end
plot(cg(:,1),cg(:,2),'r.');hold on  %齿根
%% 齿廓
for ii=1:w3(1,1);
    x(e3(ii,1),:)=0;
    y(e3(ii,1),:)=0;
end
for ii=1:w2(1,1);
    x(e2(ii,1),:)=0;
    y(e2(ii,1),:)=0;
end
C=find(x);%找出x中非零元素所在位置
N1=size(C);
for ii=1:N1(1,1);
    CK(ii,1)=x(C(ii,1),:);
    CK(ii,2)=y(C(ii,1),:);
end
figure(3)
plot(CK(:,1),CK(:,2),'k.');hold on
%%
n=size(CK);
t=1;
for i=1:n(1,1)-1
    if CK(i+1,1)-CK(i,1)<20
        continue;
    else
        fjd(t,1)=i;
        t=t+1;
    end
end
fjd=[0;fjd;n(1,1)];
%%
image=imread('Gauss.bmp');
T=15;
B0=0.2;
B=0.1;
A=3;
s=size(fjd);
sck=cell(1,s(1,1)-1);
for i=1:s(1,1)-1
    fck=CK(fjd(i,1)+1:fjd(i+1,1),:);
    n=size(fck);
    xx11=fck(1,:);
    xx22=fck(n(1,1),:);
    K1=(xx22(1,2)-xx11(1,2))/(xx22(1,1)-xx11(1,1));
    if abs(K1)>=1
        [xx1 yy1]=cubianjieniheshu(T,A,B0,B,image,fck);
    else
        [xx1 yy1]=cubianjienihe(T,A,B0,B,image,fck);
    end
    D=imread('Gauss.bmp');
    pp1=Beseir(D,xx1,yy1,T);
    % %沿等距线高斯滤波
    f1=gaosilvbo(pp1);
    % %标准正态分布概率密度函数积分表
    t=1;
    for xg=-5:0.01:5;
        Cg(t)=cdf('norm',xg,0,1);
        t=t+1;
    end
    Xg=-5:0.01:5;
    G=[Xg;Cg];
    GS1=G';
    GS=[-5,0;GS1;5,1];
    [a1 miu1]=gsnihefajiemian(f1,T,GS);
    [linear1]=jingbianjie(miu1,xx1,yy1);
    sck{1,i}=linear1;
end
figure(6)
% % t=[0,1.25,2.35,3.3,4.1,4.75,5.25,5.6,5.95,6.45,7.1,7.9,8.85,9.95,11.2 ];
t=[0,0.9,1.7,2.4,3,3.5,3.9,4.2,4.5,4.9,5.4,6,6.7,7.5,8.4 ];
for i=1:9
    subplot(3,3,i)
    plot(t(1,1:15),f1(i,:),'.');
end
%%
figure(5)
tr=1;
for j=1:2:15
    ock=sck{1,j};
    xi=ock(:,1)-xct;
    yi=ock(:,2)-yct;
    n=length(xi);
    a=zeros(n,1);
    rk=zeros(n,1);
    ak=zeros(n,1);
    b=zeros(n,1);
    ck=zeros(n,1);
    rb=2891.4;
    for i=1:n
        if (yi(i)/xi(i))<0
            a(i)=atan(yi(i)/xi(i));
        else
            a(i)=atan(yi(i)/xi(i))-pi;
        end
        rk(i)=sqrt((xi(i)).^2+(yi(i)).^2);
        ak(i)=acos(rb/rk(i));
        b(i)=tan(ak(i))-ak(i);
        ck(i)=a(i)+b(i);
    end
    M=mean(ck);
    MM(tr,1)=M;
    r1=max(ck);
    r2=min(ck);
    r3=min(ak);
    r4=max(ak);
    r5=min(b);
    r6=max(b);
    r7=min(rk);
    r8=max(rk);
    plot(xi,yi,'b.');hold on;
    tr=tr+1;
    t=r3:0.001:r4;
    x=rb*cos(M-(tan(t)-t))./cos(t);
    y=rb*sin(M-(tan(t)-t))./cos(t);
    plot(x,y,'k');hold on
end
tr=1;
for j=2:2:14
    ock=sck{1,j};
    xi=ock(:,1)-xct;
    yi=ock(:,2)-yct;
    n=length(xi);
    a=zeros(n,1);
    rk=zeros(n,1);
    ak=zeros(n,1);
    b=zeros(n,1);
    ck=zeros(n,1);
    rb=2891.4;
    for i=1:n
        if (yi(i)/xi(i))<=0
            a(i)=atan(yi(i)/xi(i));
        else
            a(i)=atan(yi(i)/xi(i))-pi;
        end
        rk(i)=sqrt((xi(i)).^2+(yi(i)).^2);
        ak(i)=acos(rb/rk(i));
        b(i)=tan(ak(i))-ak(i);
        ck(i)=a(i)-b(i);
    end
    M=mean(ck);
    MM(tr,2)=M;
    r1=max(ck);
    r2=min(ck);
    r3=min(ak);
    r4=max(ak);
    r5=min(b);
    r6=max(b);
    r7=min(rk);
    r8=max(rk);
    plot(xi,yi,'b.');hold on;
    tr=tr+1;
    t=r3:0.001:r4;
    x=rb*cos((tan(t)-t)+M)./cos(t);
    y=rb*sin((tan(t)-t)+M)./cos(t);
    plot(x,y,'k');hold on
end
for i=1:7
    dtm(i,:)=MM(i+1,:)-MM(i,:);
end
ycs=[xct,yct,Ret];
xlswrite('jiaoducha.xls',dtm);
xlswrite('xiangweijiao.xls',MM);
xlswrite('ycanshu.xls',ycs);
