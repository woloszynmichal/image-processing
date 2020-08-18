q1=load('img');q1=q1.img1;
q2=load('img2');q2=q2.img1;
q3=load('img3');q3=q3.img1;
q4=load('img4');q4=q4.img1;
q5=load('img5');q5=q5.img1;

subplot(2,4,1)
imshow(q1)
subplot(2,4,5)
imshow(q2)
subplot(2,4,6)
imshow(q3)
subplot(2,4,7)
imshow(q4)
subplot(2,4,8)
imshow(q5)

w=q1;
[rx,ry]=size(w);
for x=1:rx
    for y=1:ry
        if q1(x,y)<=55000 || q1(x,y)>=65000 
            w(x,y)=0;
        else
            w(x,y)=q1(x,y);
        end
    end
end
figure()
subplot(1,5,1)
imshow(q1)
subplot(1,5,2)
imshow(w)

w=q1;
[rx,ry]=size(w);
for x=1:rx
    for y=1:ry
        if q1(x,y)<=43000 || q1(x,y)>=55000 
            w(x,y)=0;
        else
            w(x,y)=q1(x,y);
        end
    end
end
subplot(1,5,3)
imshow(w)

w=q1;
[rx,ry]=size(w);
for x=1:rx
    for y=1:ry
        if q1(x,y)<=30000 || q1(x,y)>=38000 
            w(x,y)=0;
        else
            w(x,y)=q1(x,y);
        end
    end
end
subplot(1,5,4)
imshow(w)

w=q1;
[rx,ry]=size(w);
for x=1:rx
    for y=1:ry
        if q1(x,y)<=21000 || q1(x,y)>=26000 
            w(x,y)=0;
        else
            w(x,y)=q1(x,y);
        end
    end
end
subplot(1,5,5)
imshow(w)

figure()
subplot(2,3,1)
imshow(q2)
subplot(2,3,2)
imshow(q3)
subplot(2,3,3)
imshow(q4)

w=q1;
[rx,ry]=size(w);
for x=1:rx
    for y=1:ry
        if q1(x,y)<=21000 || q1(x,y)>=26000 
            w(x,y)=0;
        else
            w(x,y)=q1(x,y);
        end
    end
end
subplot(2,3,6)
imshow(w)

w=q1;
[rx,ry]=size(w);
for x=1:rx
    for y=1:ry
        if q1(x,y)<=30000 || q1(x,y)>=38000 
            w(x,y)=0;
        else
            w(x,y)=q1(x,y);
        end
    end
end
subplot(2,3,5)
imshow(w)


w=q1;
[rx,ry]=size(w);
for x=1:rx
    for y=1:ry
        if q1(x,y)<=43000 || q1(x,y)>=55000 
            w(x,y)=0;
        else
            w(x,y)=q1(x,y);
        end
    end
end
subplot(2,3,4)
imshow(w)