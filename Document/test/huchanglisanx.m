function [lsd pt1 p1]=huchanglisanx(lsjl,bc,k,d,qs1,zz1)
if abs(k)<1
    p1=polyfit(d(:,1),d(:,2),1);
    pt1=polyval(p1,d(:,1));
    qs(1,1)=qs1(1,1);
    qs(1,2)=polyval(p1,qs1(1,1));
    zz(1,1)=zz1(1,1);
    zz(1,2)=polyval(p1,zz1(1,1));
 plot(d(:,1),d(:,2),'.',d(:,1),pt1,'r');hold on;
    if k>0
      if qs(1,1)>zz(1,1)
          q=1;
          lsd(q,1)=qs(1,1);
          lsd(q,2)=polyval(p1,lsd(q,1));          
          while lsd(q,1)>=zz(1,1)
              i=1;
              dtx(i)=lsd(q,1)-bc;
              dty(i)=polyval(p1,dtx(i));
              ll(i)=sqrt((dtx(i)-lsd(q,1))^2+(dty(i)-lsd(q,2))^2);
              while ll(i)<lsjl
              i=i+1;   
              dtx(i)=dtx(i-1)-bc;
              dty(i)=polyval(p1,dtx(i));
              ll(i)=ll(i-1)+sqrt((dtx(i)-dtx(i-1))^2+(dty(i)-dty(i-1))^2);
              end
              q=q+1;
              lsd(q,1)=dtx(i);
              lsd(q,2)=dty(i);
          end          
%           x减小
       end
      if qs(1,1)<zz(1,1)
        q=1;
          lsd(q,1)=qs(1,1);
          lsd(q,2)=polyval(p1,lsd(q,1));          
          while lsd(q,1)<=zz(1,1)
              i=1;
              dtx(i)=lsd(q,1)+bc;
              dty(i)=polyval(p1,dtx(i));
              ll(i)=sqrt((dtx(i)-lsd(q,1))^2+(dty(i)-lsd(q,2))^2);
              while ll(i)<lsjl
              i=i+1;
              dtx(i)=dtx(i-1)+bc;
              dty(i)=polyval(p1,dtx(i));
              ll(i)=ll(i-1)+sqrt((dtx(i)-dtx(i-1))^2+(dty(i)-dty(i-1))^2);  
              end
              q=q+1;
              lsd(q,1)=dtx(i);
              lsd(q,2)=dty(i);   
%          x增加   
          end 
      end
    else
        if qs(1,1)>zz(1,1)
            q=1;
          lsd(q,1)=qs(1,1);
          lsd(q,2)=polyval(p1,lsd(q,1));          
          while lsd(q,1)>=zz(1,1)
              i=1;
              dtx(i)=lsd(q,1)-bc;
              dty(i)=polyval(p1,dtx(i));
              ll(i)=sqrt((dtx(i)-lsd(q,1))^2+(dty(i)-lsd(q,2))^2);
              while ll(i)<lsjl
              i=i+1;
              dtx(i)=dtx(i-1)-bc;
              dty(i)=polyval(p1,dtx(i));
              ll(i)=ll(i-1)+sqrt((dtx(i)-dtx(i-1))^2+(dty(i)-dty(i-1))^2);      
              end
              q=q+1;
              lsd(q,1)=dtx(i);
              lsd(q,2)=dty(i);            
%           x减小
          end
        end
     if qs(1,1)<zz(1,1)
          q=1;
          lsd(q,1)=qs(1,1);
          lsd(q,2)=polyval(p1,lsd(q,1));          
          while lsd(q,1)<=zz(1,1)
              i=1;
              dtx(i)=lsd(q,1)+bc;
              dty(i)=polyval(p1,dtx(i));
              ll(i)=sqrt((dtx(i)-lsd(q,1))^2+(dty(i)-lsd(q,2))^2);
              while ll(i)<lsjl
              i=i+1;
              dtx(i)=dtx(i-1)+bc;
              dty(i)=polyval(p1,dtx(i));
              ll(i)=ll(i-1)+sqrt((dtx(i)-dtx(i-1))^2+(dty(i)-dty(i-1))^2);  
              end
              q=q+1;
              lsd(q,1)=dtx(i);
              lsd(q,2)=dty(i);           
%          x增加   
           end    
     end
    end
else
    p1=polyfit(d(:,2),d(:,1),1);
    pt1=polyval(p1,d(:,2));
 plot(d(:,1),d(:,2),'.',pt1,d(:,2),'r');hold on;
    qs(1,2)=qs1(1,2);
    qs(1,1)=polyval(p1,qs1(1,2));
    zz(1,2)=zz1(1,2);
    zz(1,1)=polyval(p1,zz1(1,2));
    if k>0
       if qs(1,2)>zz(1,2)
          q=1;
          lsd(q,2)=qs(1,2);
          lsd(q,1)=polyval(p1,lsd(q,2));
          while lsd(q,2)>=zz(1,2)
              i=1;
              dty(i)=lsd(q,2)-bc;
              dtx(i)=polyval(p1,dty(i));
              ll(i)=sqrt((dtx(i)-lsd(q,1))^2+(dty(i)-lsd(q,2))^2);
              while ll(i)<lsjl
              i=i+1;
              dty(i)=dty(i-1)-bc;
              dtx(i)=polyval(p1,dty(i));
              ll(i)=ll(i-1)+sqrt((dtx(i)-dtx(i-1))^2+(dty(i)-dty(i-1))^2);   
              end
              q=q+1;
              lsd(q,2)=dty(i);
              lsd(q,1)=dtx(i); 
%           y减小
          end
       end
       if qs(1,2)<zz(1,2)
          q=1;
          lsd(q,2)=qs(1,2);
          lsd(q,1)=polyval(p1,lsd(q,2));
          while lsd(q,2)<=zz(1,2)
              i=1;
              dty(i)=lsd(q,2)+bc;
              dtx(i)=polyval(p1,dty(i));
              ll(i)=sqrt((dtx(i)-lsd(q,1))^2+(dty(i)-lsd(q,2))^2);
              while ll(i)<lsjl
              i=i+1;
              dty(i)=dty(i-1)+bc;
              dtx(i)=polyval(p1,dty(i));
              ll(i)=ll(i-1)+sqrt((dtx(i)-dtx(i-1))^2+(dty(i)-dty(i-1))^2);     
              end
              q=q+1;
              lsd(q,2)=dty(i);
              lsd(q,1)=dtx(i);
%          y增加   
          end           
       end       
    else
       if qs(1,2)>zz(1,2)
           q=1;
          lsd(q,2)=qs(1,2);
          lsd(q,1)=polyval(p1,lsd(q,2));
          while lsd(q,2)>=zz(1,2)
              i=1;
              dty(i)=lsd(q,2)-bc;
              dtx(i)=polyval(p1,dty(i));
              ll(i)=sqrt((dtx(i)-lsd(q,1))^2+(dty(i)-lsd(q,2))^2);
              while ll(i)<lsjl
              i=i+1;
              dty(i)=dty(i-1)-bc;
              dtx(i)=polyval(p1,dty(i));
              ll(i)=ll(i-1)+sqrt((dtx(i)-dtx(i-1))^2+(dty(i)-dty(i-1))^2);     
              end
              q=q+1;
              lsd(q,2)=dty(i);
              lsd(q,1)=dtx(i);  
          end        
%           y减小
       end
       if qs(1,2)<zz(1,2)
           q=1;
          lsd(q,2)=qs(1,2);
          lsd(q,1)=polyval(p1,lsd(q,2));
          while lsd(q,2)<=zz(1,2)
              i=1;
              dty=lsd(q,2)+bc;
              dtx=polyval(p1,dty);
              ll(i)=sqrt((dtx(i)-lsd(q,1))^2+(dty(i)-lsd(q,2))^2);
              while ll(i)<lsjl
              i=i+1;
              dty(i)=dty(i-1)+bc;
              dtx(i)=polyval(p1,dty(i));
              ll(i)=ll(i-1)+sqrt((dtx(i)-dtx(i-1))^2+(dty(i)-dty(i-1))^2);    
              end
              q=q+1;
              lsd(q,2)=dty(i);
              lsd(q,1)=dtx(i); 
          end        
%          y增加   
       end        
    end
    
end
%  plot(lsd(:,1),lsd(:,2),'*');hold on;