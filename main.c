#include <stdio.h>
#include <prototype.h>

#define Nref 1024

#define Nspect 1024    //se pune dimensiunea spectrului
#define Npad 1024    //se pune numarul de zerouri ce se vor adaugate
#define N Nspect+Npad

void main()
{
	Word16 n, k, cr[N], ci[N];
	Word32 xr[N], xi[N], prodr, prodi;
	
	Word16 tab[Nref], sin[N], cos[N];
	UWord16 i, i1;    //i parcurge sin, i1 <= i*delta < i1+1
	Word16 t1, t2;    //tab[i1], tab[i1+1]
	UWord32 delta;
	UWord32 phaseaccum=0x00000000;  //index adresat %Nref pe 16MSB
	
	FILE *fp;
	
	delta=L_deposit_h(Nref)/(Word16)N;    //delta este calc a.i.
	//sin si cos sa aiba frecventa digitala fundamentala pe N elemente  
	
	
	/**********  Calculare sin, cos de baza (w0)  ******************/
	
	fp=fopen("sin.dat","r+b");
	fread(tab,2,Nref,fp);
	fclose(fp);
	
	for(i=0;i<N;i++)
	{
		i1=L_shr(phaseaccum,16)%Nref;
		t1=tab[i1];
		t2=tab[(i1+1)%Nref];
		sin[i]=add(t1,mult(sub(t2,t1),shr(phaseaccum,1)));
		phaseaccum+=delta;			
	}
	for(i=0;i<N;i++)
	{
		cos[i]=sin[(i+N/4)%N];
	}
	
	fp=fopen("sin_interp_sc.dat","w+b");
	fwrite(sin,2,N,fp);
	fclose(fp);
	fp=fopen("cos_interp_sc.dat","w+b");
	fwrite(cos,2,N,fp);
	fclose(fp);
	
	/***********************************************************/
	/******************  Calculare DFT  ************************/
	
	fp=fopen("spectrum_Re.dat","r+b");  //citire componente
	fread(cr,2,Nspect,fp);
	fclose(fp);
	fp=fopen("spectrum_Im.dat","r+b");
	fread(ci,2,Nspect,fp);
	fclose(fp);
	
	for(n=N-1;n>N-1-Nspect/2;n--)  //shift jumatatea dreapta
	{
		cr[n]=cr[n-Npad];
		ci[n]=ci[n-Npad];
	}
	for(n=Nspect/2;n<Nspect/2+Npad;n++)    //zero padding
	{
		cr[n]=0;
		ci[n]=0;
	}
	
	
	for(n=0;n<N;n++)
	{
		xr[n]=0;    xi[n]=0;
		for(k=0;k<N;k++)
		{
			//sn=sin[(n*k)%N];    cs=cos[(n*k)%N];
			prodr=L_mult(cr[k],cos[(n*k)%N]);
			prodr=L_msu(prodr,ci[k],sin[(n*k)%N]);
			prodi=L_mult(cr[k],sin[(n*k)%N]);
			prodi=L_mac(prodi,ci[k],cos[(n*k)%N]);
			xr[n]+=prodr;
			xi[n]+=prodi;
		}
	}
	fp=fopen("x_Re.dat","w+b");
	fwrite(xr,4,N,fp);
	fclose(fp);
	fp=fopen("x_Im.dat","w+b");
	fwrite(xi,4,N,fp);
	fclose(fp);
	fp=fopen("cr_shifted.dat","w+b");
	fwrite(cr,sizeof(cr[1]),N,fp);
	fclose(fp);
}