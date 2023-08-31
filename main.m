clear, close all;

% !Nspect even!
Nspect=1024;  N=2048;    n=0:Nspect-1;    k=n;    ni=0:N-1;  %ni=n interpolat
% x=0.75*sin(2*pi/Nspect*100*n);
% xe=0.75*sin(2*pi/Nspect*100*ni);
 
x=0.25*sin(2*pi/Nspect*4*n)+0.25*sin(2*pi/Nspect*8*n);
xe=0.25*sin(2*pi/Nspect*4*ni)+0.25*sin(2*pi/Nspect*8*ni);
 
% x=0.25*sin(2*pi/Nspect*4*n)+0.25*cos(2*pi/Nspect*8*n)...
%     +0.25*sin(2*pi/Nspect*16*n)+0.125*sin(2*pi/Nspect*128*n);
% x=0.25*sin(2*pi/Nspect*4*ni)+0.25*cos(2*pi/Nspect*8*ni)...
%     +0.25*sin(2*pi/Nspect*16*ni)+0.125*sin(2*pi/Nspect*128*ni);

% x=filter([1 1 1 1 1 1 1 1 1],1,square(2*pi/Nspect*3*n));
% xe=filter([1 1 1 1 1 1 1 1 1],1,square(2*pi/Nspect*3*ni));

% x=square(2*pi/Nspect*3*n);
% xe=square(2*pi/Nspect*3*ni);

figure(1);    plot(n,x,'.-');  title('x(n)');

X=1/Nspect*fft(x,Nspect);    %formula de la ASC, componente spect ecbivalente SF
figure(2);    plot(k,abs(X),'.-');
figure(3);    plot(k,real(X),'.-',k,imag(X),'.-');  legend('Re','Im');

fid=fopen('spectrum_Re.dat','w','b');
fwrite(fid,real(X)*2^15,'int16');
fclose(fid);
fid=fopen('spectrum_Im.dat','w','b');
fwrite(fid,imag(X)*2^15,'int16');
fclose(fid);

pause();

fid=fopen('x_Re.dat','r','b');    xr=fread(fid,N,'int32')/2^30;  xr=xr.';
% fread /2^31*2 = fread /2^30  %se mai inmulteste cu 2 pt a compensa sin,
% cos scalate
fclose(fid);
fid=fopen('x_Im.dat','r','b');    xi=fread(fid,N,'int32')/2^30;  xi=xi.';
fclose(fid);

figure(4);    plot(n/Nspect*N,x,'-x', ni,xr,'o-', ni,-xi,'o-');
legend('x(n)','x_s_cinterp(ni)','x_s_cinterp(ni)');  grid, grid minor;

% figure(5);    plot(ni,xe,'-x', ni,xr+xi,'o-');
% legend('x(n)','x_s_cinterp(ni)');  grid, grid minor;

fid=fopen('cr_shifted.dat','r','b');    cr_shifted=fread(fid,N,'int16')/2^15;
fclose(fid);