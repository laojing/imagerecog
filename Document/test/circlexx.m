clc
clear
A=imread('1.bmp');
Gaussian(A);
image=imread('Gauss.bmp');
MM=2;
NN=3;
[th F]=sobell(MM,NN,image);
%%
n=length(th);
x=th(:,2);
y=th(:,1);
rl=3180;
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
F=@(xcz)([p(1,1)*xcz(1,2)+xcz(1,1)-p(1,1)*yc-xc;p(1,1)*xcz(1,1)+p(1,2)-xcz(1,2)]);
xcz=fsolve(F,x0);%����
r1=(rl-Re)*2/3;
rc=sqrt((xc-xcz(1,1))^2+(yc-xcz(1,2))^2);
x0=[10000 10000];
F1=@(xcz1)([(xc-xcz(1,1))*(xcz1(1,2)-yc)-(xcz1(1,1)-xc)*(yc-xcz(1,2));(xcz1(1,1)-xc)^2+(xcz1(1,2)-yc)^2-r1^2]);
xcz1=fsolve(F1,x0);%��Բ��
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
plot(ps(:,1),ps(:,2),'.');hold on   %�ݶ�
[xc,yc,Re,a] = circfit(ps(:,1),ps(:,2));
p=polyfit(ps(:,1),ps(:,2),1);
pt1=polyval(p,ps(:,1));
% plot(ps(:,1),pt1,'r');hold on;
end
%%
lcg=2949;
for i=1:n1
         qc(i,1)=sqrt((xc-x(i))^2+(yc-y(i))^2);     
end
r=max(qc);
e2=find(qc<=lcg+5);
w2=size(e2);
for ii=1:w2(1,1);
cg(ii,1)=x(e2(ii,1),:);
cg(ii,2)=y(e2(ii,1),:);
end
plot(cg(:,1),cg(:,2),'r.');hold on  %�ݸ�
%%
for ii=1:w1(1,1);
x(e1(ii,1),:)=0;
y(e1(ii,1),:)=0;
end
for ii=1:w2(1,1);
x(e2(ii,1),:)=0;
y(e2(ii,1),:)=0;
end
C=find(x);%�ҳ�x�з���Ԫ������λ��
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
%     if CK(i+1,1)-CK(i,1)>20      
%         s=s+1;
%         t=1;
%         continue;
%     else
%         fck(t,2*s-1:2*s)=CK(i,:);
%         t=t+1;
%     end
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
% %�صȾ��߸�˹�˲�
f1=gaosilvbo(pp1);
% %��׼��̬�ֲ������ܶȺ������ֱ�
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
ack=sck{1,6};
bck=sck{1,8};
ack(:,1)=ack(:,1)-xc;
ack(:,2)=ack(:,2)-yc;
bck(:,1)=bck(:,1)-xc;
bck(:,2)=bck(:,2)-yc;
A=2;
ap1=polyfit(ack(:,2),ack(:,1),3);
apt1=polyval(ap1,ack(:,2));
plot(ack(:,1),ack(:,2),'.',apt1,ack(:,2),'r');hold on;
an=size(ack);
i=1;
lack(i,2)=ack(1,2);
lack(i,1)=ap1(1)*lack(i,2).^3+ap1(2)*lack(i,2).^2+ap1(3)*lack(i,2)+ap1(4);
while lack(i,2)<=ack(an(1,1),2)
yd(i,1)=3*ap1(1)*lack(i,2).^2+2*ap1(2)*lack(i,2)+ap1(3);
dtx(i)=sqrt(A^2/(1+yd(i,1)^2));
lack(i+1,2)=lack(i,2)+dtx(i);
lack(i+1,1)=ap1(1)*lack(i+1,2).^3+ap1(2)*lack(i+1,2).^2+ap1(3)*lack(i+1,2)+ap1(4);
i=i+1;
end
 while lack(i,2)>=ack(an(1,1),2)
yd(i,1)=3*ap1(1)*lack(i,2).^2+2*ap1(2)*lack(i,2)+ap1(3);
dtx(i)=sqrt(A^2/(1+yd(i,1)^2));
lack(i+1,2)=lack(i,2)-dtx(i);
lack(i+1,1)=ap1(1)*lack(i+1,2).^3+ap1(2)*lack(i+1,2).^2+ap1(3)*lack(i+1,2)+ap1(4);
i=i+1;
end
plot(lack(:,1),lack(:,2),'k.');hold on;
bp1=polyfit(bck(:,2),bck(:,1),3);
bpt1=polyval(bp1,bck(:,2));
plot(bck(:,1),bck(:,2),'.',bpt1,bck(:,2),'r');hold on;
bn=size(bck);
i=1;
lbck(i,2)=bck(1,2);
lbck(i,1)=bp1(1)*lbck(i,2).^3+bp1(2)*lbck(i,2).^2+bp1(3)*lbck(i,2)+bp1(4);
while lbck(i,2)<=bck(bn(1,1),2)
yd(i,1)=3*bp1(1)*lbck(i,2).^2+2*bp1(2)*lbck(i,2)+bp1(3);
dtx(i)=sqrt(A^2/(1+yd(i,1)^2));
lbck(i+1,2)=lbck(i,2)+dtx(i);
lbck(i+1,1)=bp1(1)*lbck(i+1,2).^3+bp1(2)*lbck(i+1,2).^2+bp1(3)*lbck(i+1,2)+bp1(4);
i=i+1;
end
 while lbck(i,2)>=bck(bn(1,1),2)
yd(i,1)=3*bp1(1)*lbck(i,2).^2+2*bp1(2)*lbck(i,2)+bp1(3);
dtx(i)=sqrt(A^2/(1+yd(i,1)^2));
lbck(i+1,2)=lbck(i,2)-dtx(i);
lbck(i+1,1)=bp1(1)*lbck(i+1,2).^3+bp1(2)*lbck(i+1,2).^2+bp1(3)*lbck(i+1,2)+bp1(4);
i=i+1;
end
plot(lbck(:,1),lbck(:,2),'k.');hold on;
%%
figure(8)
g=size(lack);
t=1;
for alf=pi/30-0.01:0.0001:pi/30+0.01
for i=1:g(1,1)
bt(i,1)=atan(lack(i,2)/lack(i,1));    
rt=sqrt((lack(i,1))^2+(lack(i,2))^2);
zack(i,1)=rt*((lack(i,1)/rt)*cos(alf)-(lack(i,2)/rt)*sin(alf));
zack(i,2)=rt*((lack(i,2)/rt)*cos(alf)+(lack(i,1)/rt)*sin(alf));
X=zack(i,:);
XX=polyval(bp1,X(1,2));
[minl xcz]=julizuixiao(X,bp1,XX); 
minlz(i,1)=minl;
end
zd(t,1)=sum(minlz(:)); 
t=t+1;       
end
tw= find(zd==min(zd));
alf=pi/30-0.01+(tw-1)*0.0001;
for i=1:g(1,1)
bt(i,1)=atan(lack(i,2)/lack(i,1));    
rt=sqrt((lack(i,1))^2+(lack(i,2))^2);
zack(i,1)=rt*((lack(i,1)/rt)*cos(alf)-(lack(i,2)/rt)*sin(alf));
zack(i,2)=rt*((lack(i,2)/rt)*cos(alf)+(lack(i,1)/rt)*sin(alf));
end
cp1=polyfit(zack(:,2),zack(:,1),3);
cpt1=polyval(cp1,zack(:,2));
plot(zack(:,1),zack(:,2),'k.',cpt1,zack(:,2),'r');hold on;
plot(lbck(:,1),lbck(:,2),'b.-');hold on;
plot(lack(:,1),lack(:,2),'r.');hold on;











