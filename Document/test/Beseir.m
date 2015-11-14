function pp=Beseir(D,xx,yy,T)
%±´Èû¶ûÇúÃæ²åÖµ¸³»Ò¶ÈÖµ
 Z=size(xx);
 pp=zeros(Z(1,1)-1,T);
 for i=1:Z(1,1)-1
     for j=1:T
     zbx1=floor(xx(i,j));
     zbx2=zbx1+1;
     zbx3=zbx1-1;
     zbx4=zbx1+2;
     zby1=floor(yy(i,j));
     zby2=zby1+1;
     zby3=zby1-1;
     zby4=zby1+2;
%      p11=D(zby3+1,zbx3+1);
%      p12=D(zby1+1,zbx3+1);
%      p13=D(zby2+1,zbx3+1);
%      p14=D(zby4+1,zbx3+1);
%      p21=D(zby3+1,zbx1+1);
%      p22=D(zby1+1,zbx1+1);
%      p23=D(zby2+1,zbx1+1);
%      p24=D(zby4+1,zbx1+1);
%      p31=D(zby3+1,zbx2+1);
%      p32=D(zby1+1,zbx2+1);
%      p33=D(zby2+1,zbx2+1);
%      p34=D(zby4+1,zbx2+1);
%      p41=D(zby3+1,zbx4+1);
%      p42=D(zby1+1,zbx4+1);
%      p43=D(zby2+1,zbx4+1);
%      p44=D(zby4+1,zbx4+1);
     p11=D(zby3,zbx3);
     p12=D(zby1,zbx3);
     p13=D(zby2,zbx3);
     p14=D(zby4,zbx3);
     p21=D(zby3,zbx1);
     p22=D(zby1,zbx1);
     p23=D(zby2,zbx1);
     p24=D(zby4,zbx1);
     p31=D(zby3,zbx2);
     p32=D(zby1,zbx2);
     p33=D(zby2,zbx2);
     p34=D(zby4,zbx2);
     p41=D(zby3,zbx4);
     p42=D(zby1,zbx4);
     p43=D(zby2,zbx4);
     p44=D(zby4,zbx4);
     P=[p11,p12,p13,p14;
        p21,p22,p23,p24;
        p31,p32,p33,p34;
        p41,p42,p43,p44];
    u=1/3+(xx(i,j)-zbx1)/3;
    w=1/3+(yy(i,j)-zby1)/3;
    U=[u^3,u^2,u,1];
    W=[w^3,w^2,w,1];
    B=[-1 3 -3 1;
        3 -6 3 0;
        -3 3 0 0;
        1 0 0 0;];
B=double(B);
U=double(U);
P=double(P);
W=double(W);
pp(i,j) = U*B*P*B'*W';
     end
    
 end


  

