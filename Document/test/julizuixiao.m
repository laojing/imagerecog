function [minl xcz]=julizuixiao(X,p1,pt1)
    i=1;
    X1(i,1)=pt1(1,1);
    X1(i,2)=X(1,2);
    a1(i)=sqrt((X(1,1)-X1(i,1))^2+(X(1,2)-X1(i,2))^2);
    dt1=0.01; 
    X1(i+1,2)=X1(1,2)+dt1;
    X1(i+1,1)=polyval(p1,X1(i+1,2));   
    a1(i+1)=sqrt((X(1,1)-X1(i+1,1))^2+(X(1,2)-X1(i+1,2))^2);   
    if a1(i+1)==a1(i) 
        ax=(X1(i+1,2)-X1(i,2))/(X1(i+1,1)-X1(i,1));
        bx=1;
        cx=ax*X1(i-1,2)-X1(i-1,1);
    minl=abs(bx*X(1,1)-ax*X(1,2)+cx)/sqrt(ax^2+1);         
    xcz(1,2)=((X(1,2)/ax)+X(1,1)-X1(i,1)+ax*X1(i,2))/(ax+1/ax);
    xcz(1,1)=ax*(xcz(1,2)-X1(i,2))+X1(i,1);%´¹×ã  
    end
    if a1(i+1)>a1(i) 
         X1(i+1,2)=X1(1,2)-dt1;
         X1(i+1,1)=polyval(p1,X1(i+1,2));
        a1(i+1)=sqrt((X(1,1)-X1(i+1,1))^2+(X(1,2)-X1(i+1,2))^2); 
        i=2;
    while a1(i)<a1(i-1)
        i=i+1; 
        X1(i,2)=X1(i-1,2)-dt1;
         X1(i,1)=polyval(p1,X1(i,2));
        a1(i)=sqrt((X(1,1)-X1(i,1))^2+(X(1,2)-X1(i,2))^2);            
    end
    ax=(X1(i,1)-X1(i-1,1))/(X1(i,2)-X1(i-1,2));
    bx=1;
    cx=ax*X1(i-1,2)-X1(i-1,1); 
   
    minl=abs(bx*X(1,1)-ax*X(1,2)+cx)/sqrt(ax^2+1);         
      xcz(1,2)=((X(1,2)/ax)+X(1,1)-X1(i,1)+ax*X1(i,2))/(ax+1/ax);
    xcz(1,1)=ax*(xcz(1,2)-X1(i,2))+X1(i,1);%´¹×ã   
    else  
      i=2;
    while a1(i)<a1(i-1)
        i=i+1; 
        X1(i,2)=X1(i-1,2)+dt1;
         X1(i,1)=polyval(p1,X1(i,2));
        a1(i)=sqrt((X(1,1)-X1(i,1))^2+(X(1,2)-X1(i,2))^2);            
    end
    ax=(X1(i,1)-X1(i-1,1))/(X1(i,2)-X1(i-1,2));
    bx=1;
    cx=ax*X1(i-1,2)-X1(i-1,1);    

    minl=abs(bx*X(1,1)-ax*X(1,2)+cx)/sqrt(ax^2+1);         
      xcz(1,2)=((X(1,2)/ax)+X(1,1)-X1(i,1)+ax*X1(i,2))/(ax+1/ax);
    xcz(1,1)=ax*(xcz(1,2)-X1(i,2))+X1(i,1);
    end%´¹×ã   
% plot(X(:,1),X(:,2),'.',pt1,X(:,2),'r');hold on;    
end