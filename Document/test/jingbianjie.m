function [linear1]=jingbianjie(miu,xx,yy)
figure(4),
zz=size(miu);
% t=[0,1.25,2.35,3.3,4.1,4.75,5.25,5.6,5.95,6.45,7.1,7.9,8.85,9.95,11.2 ];
t=[0,0.9,1.7,2.4,3,3.5,3.9,4.2,4.5,4.9,5.4,6,6.7,7.5,8.4 ];
for i=1:zz(1,1);
    for j=1:15;
        if miu(i)>=t(j)
            break;
        end
    end
xf=j;
xg=xf+1;
del= (miu(i) - t(xf))/(t(xg)-t(xf));
xx_linear(i,1) = xx(i, xf) + del*(xx(i,xg) - xx(i, xf));
yy_linear(i,1) = yy(i, xf) + del*(yy(i,xg) - yy(i, xf));

 plot(xx_linear(i,1),yy_linear(i,1),'r.');hold on; 
end
linear1=[xx_linear,yy_linear];

end
