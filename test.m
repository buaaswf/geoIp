I=imread('E:\geo\testbmps\0o.bmp');
I=double(I)/255;
 
w= 5;       % bilateral filter half-width
sigma = [3 0.1]; % bilateral filter standard deviations
 
I1=bfilter2(I,w,sigma);
 
subplot(1,2,1);
imshow(I);
subplot(1,2,2);
imshow(I1);
