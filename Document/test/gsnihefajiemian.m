function [a miu]=gsnihefajiemian(f,T,GS)
[M N]=size(f);


for j=1:N;
    for i=1:M;
       K=max(f(i,:))-min(f(i,:));m=min(f(i,:));
       Fi(i,j)=(f(i,j)-m)/K;
%     if Fi(i,j)<0
%         Fi(i,j)=0;
%     end
    end
end
e=size(GS);
for j=1:T;
    for i=1:M;
       
            for ii=1:e(1,1); 
               if Fi(i,j)<=0;
                a(i,j)=-5;
%                break;
               end  
              if Fi(i,j)<GS(ii,2);
                 break;
              end
             end
        del = (GS(ii,2) - Fi(i,j))/(GS(ii,2) - GS(ii-1,2));
        a(i,j) = GS(ii-1,1) + del*(GS(ii,1) - GS(ii-1,1));
       
        
     end
  
end
ap=mean(a,2);
t=[0,0.9,1.7,2.4,3,3.5,3.9,4.2,4.5,4.9,5.4,6,6.7,7.5,8.4];
% t=[0,1.25,2.35,3.3,4.1,4.75,5.25,5.6,5.95,6.45,7.1,7.9,8.85,9.95,11.2 ];
tp=mean(t,2);
at=a*t';
atp=at/T;
af=a.*a;
afp=mean(af,2);
miu=zeros(M,1);
for i=1:M;
   miu(i)=tp-ap(i)*(tp*ap(i)-atp(i))/((ap(i))^2-afp(i));
end
    if miu(i)<=0
       miu(i)=miu(i-1);
   end
      
end


















    