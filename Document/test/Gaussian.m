function Gaussian(I)
%此函数对图像进行5x5高斯滤波
h = fspecial('gaussian',[5,5], 1.2); 
[m n p]=size(I); 
if p==1 
    I=double(I); 
    I=conv2(I,h,'same'); 
end 
if p==3 
    I=double(I); 
    I(:,:,1)=conv2(I(:,:,1),h,'same'); 
    I(:,:,2)=conv2(I(:,:,2),h,'same'); 
    I(:,:,3)=conv2(I(:,:,3),h,'same'); 
end 
I=uint8(I);
 imwrite(I,'Gauss.bmp','bmp');
% imtool(I);