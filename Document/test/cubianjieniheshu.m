function [xx yy]=cubianjieniheshu(T,A,B0,B,image,linesf)
figure(4)
xx = 0;
yy = 0;
%粗边界拟合及等距线
p1=polyfit(linesf(:,2),linesf(:,1),3);
pt1=polyval(p1,linesf(:,2));
plot(linesf(:,1),linesf(:,2),'.',pt1,linesf(:,2),'r');hold on;

tt=(T+1)/2;
i=1;j=tt;
n=size(linesf);
yy(i,j)=linesf(1,2);
K=(linesf(1,1)-linesf(n(1,1),1))/(linesf(1,2)-linesf(n(1,1),2));
xx(i,j)=p1(1)*yy(i,j).^3+p1(2)*yy(i,j).^2+p1(3)*yy(i,j)+p1(4);

while yy(i,j)<=linesf(n(1,1),2)
	yd(i,j)=3*p1(1)*yy(i,j).^2+2*p1(2)*yy(i,j)+p1(3);
	dtx(i)=sqrt(A^2/(1+yd(i,j)^2));
	yy(i+1,j)=yy(i,j)+dtx(i);
	xx(i+1,j)=p1(1)*yy(i+1,j).^3+p1(2)*yy(i+1,j).^2+p1(3)*yy(i+1,j)+p1(4);
	i=i+1;
end


  while yy(i,j)>=linesf(n(1,1),2)
yd(i,j)=3*p1(1)*yy(i,j).^2+2*p1(2)*yy(i,j)+p1(3);
dtx(i)=sqrt(A^2/(1+yd(i,j)^2));
yy(i+1,j)=yy(i,j)-dtx(i);
xx(i+1,j)=p1(1)*yy(i+1,j).^3+p1(2)*yy(i+1,j).^2+p1(3)*yy(i+1,j)+p1(4);
i=i+1;
  end
%定义等距点
z=size(yy);
U=1;
dtb(U)=((tt-U+1)*(tt-U)/2)*B+(tt-U)*B0;
W=T;
dtb(W)=((W-tt+1)*(W-tt)/2)*B+(W-tt)*B0;



for i=1:z-1
    ddtx(i)=sqrt((yd(i,tt)^2/(1+yd(i,tt)^2)));
    ddty(i)=ddtx(i)/abs(yd(i,tt));
    if K>0
    xp1(i,U)=xx(i,tt)+dtb(U)*ddty(i);
    yp1(i,U)=yy(i,tt)-dtb(U)*ddtx(i);
    xpT(i,W)=xx(i,tt)-dtb(W)*ddty(i);
    ypT(i,W)=yy(i,tt)+dtb(W)*ddtx(i);
    else
    xp1(i,U)=xx(i,tt)+dtb(U)*ddty(i);
    yp1(i,U)=yy(i,tt)+dtb(U)*ddtx(i);
    xpT(i,W)=xx(i,tt)-dtb(W)*ddty(i);
    ypT(i,W)=yy(i,tt)-dtb(W)*ddtx(i);    
      
    end
     if  image(floor(yp1(i,U)),floor(xp1(i,U)))<image(floor(ypT(i,W)),floor(xpT(i,W)))
   for u=1:tt-1;
       dtb(u)=((tt-u+1)*(tt-u)/2)*B+(tt-u)*B0;
       if K>0
       yy(i,u)=yy(i,tt)-dtb(u)*ddtx(i);
       xx(i,u)=xx(i,tt)+dtb(u)*ddty(i);
       else
       yy(i,u)=yy(i,tt)+dtb(u)*ddtx(i);
       xx(i,u)=xx(i,tt)+dtb(u)*ddty(i);    
       end
   end
   for u=tt+1:T;
        dtb(u)=((u-tt+1)*(u-tt)/2)*B+(u-tt)*B0;
        if K>0
        yy(i,u)=yy(i,tt)+dtb(u)*ddtx(i);
        xx(i,u)=xx(i,tt)-dtb(u)*ddty(i);
        else
       yy(i,u)=yy(i,tt)-dtb(u)*ddtx(i);
        xx(i,u)=xx(i,tt)-dtb(u)*ddty(i);     
        end
   end
    i=i+1;
plot(xx(i-1,1:2*tt-1),yy(i-1,1:2*tt-1),'b.');hold on;
    else
     for u=1:tt-1;
       dtb(u)=((tt-u+1)*(tt-u)/2)*B+(tt-u)*B0;
       if K>0
       yy(i,u)=yy(i,tt)+dtb(u)*ddtx(i);
       xx(i,u)=xx(i,tt)-dtb(u)*ddty(i);
       else
       yy(i,u)=yy(i,tt)-dtb(u)*ddtx(i);
       xx(i,u)=xx(i,tt)-dtb(u)*ddty(i);   
       end
   end
   for u=tt+1:T;
        dtb(u)=((u-tt+1)*(u-tt)/2)*B+(u-tt)*B0;
         if K>0
        yy(i,u)=yy(i,tt)-dtb(u)*ddtx(i);
        xx(i,u)=xx(i,tt)+dtb(u)*ddty(i);
         else
        yy(i,u)=yy(i,tt)+dtb(u)*ddtx(i);
        xx(i,u)=xx(i,tt)+dtb(u)*ddty(i);   
         end
   end
    i=i+1;   
 plot(xx(i-1,1:2*tt-1),yy(i-1,1:2*tt-1),'b.');hold on;
    end
end
