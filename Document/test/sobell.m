function [line F]=sobell(MM,NN,image)
[Nn Nm]=size(image);           
h=[-1 -2 -1;0 0 0;1 2 1];      
Gx=filter2(h,image);            
Gy=filter2(h',image);          
for i=1:Nn;
   for j=1:Nm;
F(i,j)=(Gx(i,j)).^2+(Gy(i,j)).^2; 
   end
end

Nn=Nn-30;
Nm=Nm-30;
flag = 0;
line = 0;
flag(Nn,Nm) = 0;
startx = 0;
starty = 0;
for i=1:Nm
    F(1,i) = 0;
end

for i=1:Nn
    F(i,1) = 0;
end
   for j=NN:Nm-1;
for i=MM:Nn-1;
        if F(i,j) > 80000
            startx = i;
            starty = j;
            break;
        end
   end
   if ( startx || starty )
       break;
   end
end

disp(num2str(startx));
disp(num2str(starty));

curx = startx;
cury = starty;
index = 1;
while curx < Nn && cury <Nm
    line(index,1) = curx;
    line(index,2) = cury;
    line(index,3) = F(curx, cury);
    index = index + 1;

    flag(curx, cury) = 1;
    next = -1;
    if ( curx > 1 && cury >1 )
    if ( F(curx-1, cury-1) > next && flag(curx-1,cury-1) < 1 )
        next = F(curx-1,cury-1);
        nextx = curx-1;
        nexty = cury-1;
    end
    end
    if ( cury > 1 )
    if ( F(curx, cury-1) > next && flag(curx,cury-1) < 1 )
        next = F(curx,cury-1);
        nextx = curx;
        nexty = cury-1;
    end

    
    if ( F(curx+1, cury-1) > next && flag(curx+1,cury-1) < 1 )
        next = F(curx+1,cury-1);
        nextx = curx+1;
        nexty = cury-1;
    end
    end
    if curx > 1
    if ( F(curx-1, cury) > next && flag(curx-1,cury) < 1 )
        next = F(curx-1,cury);
        nextx = curx-1;
        nexty = cury;
    end
    end
    if ( F(curx+1, cury) > next && flag(curx+1,cury) < 1 )
        next = F(curx+1,cury);
        nextx = curx+1;
        nexty = cury;
    end
    if ( curx > 1)
    if ( F(curx-1, cury+1) > next && flag(curx-1,cury+1) < 1 )
        next = F(curx-1,cury+1);
        nextx = curx-1;
        nexty = cury+1;
    end
    end
    if ( F(curx, cury+1) > next && flag(curx,cury+1) < 1 )
        next = F(curx,cury+1);
        nextx = curx;
        nexty = cury+1;
    end
    if ( F(curx+1, cury+1) > next && flag(curx+1,cury+1) < 1 )
        next = F(curx+1,cury+1);
        nextx = curx+1;
        nexty = cury+1;
    end
    

    if ( next < 0 || curx > Nn-2 || cury > Nm-1 )
        disp(num2str(next));
        disp(num2str(curx));
        disp(num2str(cury));
        break;
    else

        flag(curx+1,cury+1) = 1;
        flag(curx,cury+1) = 1;
        if curx > 1
        flag(curx-1,cury+1) = 1;
        flag(curx-1,cury) = 1;
        flag(curx-1,cury-1) = 1;
        end
        flag(curx+1,cury) = 1;
        flag(curx+1,cury-1) = 1;
        flag(curx,cury-1) = 1;
        curx = nextx;
        cury = nexty;
    end
end
plot(line(:,2),line(:,1),'.');
