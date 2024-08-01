% programma che restituisce il valore del campo dati questi parametri
% che vanno passati necessariamente in quest'ordine:
% numero delle interfacce, e le loro coordinate z,
% valore scalare del campo elettrico incidente sulla prima, gli estremi e il numero di
% angoli di tale incidenza, da indicare in gradi,
% rispetto all'asse z che si considera essere ortogonale al piano (x,y)
% (piano appunto di incidenza), la lunghezza d'onda, gli indici di rifrazione (prima parte reale e poi
% immaginaria), gli estremi dell'intervallo lungo l'asse z in cui si voglono
% calcolare i punti e il numero dei medesimi, estremi e numero delle
% coordinate x.
% Infine porre uno per una polarizzazione di tipo TM, altrimenti porre qualunque altro numero.
% Nota: il file in cui inserire i dati va chiamato "data", in formato "txt".

%% PARSING DATA FILE

data=importdata("data.txt");
% definisco un counter per comodità
c=data(1)+2;
% letto il numero di interfacce
Vettoreinterfacce=ones(1,data(1));
% allocazione coordinata z di ciascuna interfaccia
for i=1:data(1)
    Vettoreinterfacce(i)=data(i+1);
end
% allocazione ampiezza campo incidente
E_inc=data(c);
% allocazione estremi e numero di angoli
teta1=deg2rad(linspace(data(c+1),data(c+2),data(c+3)));
% allocazione lunghezza d'onda
vacuum_wavelength=data(c+4);
% allocazione indici di rifrazione
n=ones(1,c-1);
l=c+4;
for i=1:c-1
    n(i)=data(i+l)-1i*data(i+l+1);
    l=l+1;
end
c=3*c+2;
% allocazione estremi e numero dei punti sull'asse z
z=linspace(data(c+1),data(c+2),data(c+3));
x=linspace(data(c+3),data(c+5),data(c+6));
yp=0.0;
number_of_pol=data(c+7);
if number_of_pol==1
    pol='TM';
else
    pol='TE';
end

%% DEFINE SOME CONSTANTS

global c0;
c0=299792458;
global mi0;
mi0=(4*pi)*1.0E-7;

%% FORMAT OUTPUT FILE

fileID= fopen("valorecampi.txt",'w');
fprintf(fileID,'Polarizzazione : %s\n',pol);
for i=1:length(teta1)
    fprintf(fileID,'\t teta = %f',teta1(i));
    fprintf(fileID,'\t yp = %e\n',yp);
    for j=1:length(x)
        Matrixplot=ones(8,length(z));
        fprintf(fileID,'\txp = %e\n',x(j));
        for k=1:length(z)
            fprintf(fileID,'zp = %e\t',z(k));
            [Matrix]=CalcolocampiEH(n,teta1(i),pol,vacuum_wavelength,Vettoreinterfacce,E_inc,z(k),x(j));
            if strcmp(pol,'TM')
                Matrixplot(1,k)=real(Matrix(1,1));
                Matrixplot(2,k)=imag(Matrix(1,1));
                Matrixplot(3,k)=real(Matrix(3,1));
                Matrixplot(4,k)=imag(Matrix(3,1));
                Matrixplot(5,k)=real(Matrix(2,2));
                Matrixplot(6,k)=imag(Matrix(2,2));
                fprintf(fileID,'Re(Ex) = %e\t',real(Matrix(1,1)));
                fprintf(fileID,'Im(Ex) = %e\t',imag(Matrix(1,1)));
                fprintf(fileID,'|Ex| = %e\t',abs(Matrix(1,1)));
                fprintf(fileID,'fase(Ex) = %e\t',angle(Matrix(1,1)));
                fprintf(fileID,'Re(Ez) = %e\t',real(Matrix(3,1)));
                fprintf(fileID,'Im(Ez) = %e\t',imag(Matrix(3,1)));
                fprintf(fileID,'|Ez| = %e\t',abs(Matrix(3,1)));
                fprintf(fileID,'fase(Ez) = %e\t',angle(Matrix(3,1)));
                fprintf(fileID,'Re(Hy) = %e\t',real(Matrix(2,2)));
                fprintf(fileID,'Im(Hy) = %e\t',imag(Matrix(2,2)));
                fprintf(fileID,'|Hy| = %e\t',abs(Matrix(2,2)));
                fprintf(fileID,'fase(Hy) = %e\n',angle(Matrix(2,2)));
            else
                Matrixplot(7,k)=abs(Matrix(2,1));
                Matrixplot(8,k)=angle(Matrix(2,1));
                Matrixplot(5,k)=real(Matrix(2,1));
                Matrixplot(6,k)=imag(Matrix(2,1));
                Matrixplot(1,k)=real(Matrix(1,2));
                Matrixplot(2,k)=imag(Matrix(1,2));
                Matrixplot(3,k)=real(Matrix(3,2));
                Matrixplot(4,k)=imag(Matrix(3,2));
                fprintf(fileID,'Re(Hx) = %e\t',real(Matrix(1,2)));
                fprintf(fileID,'Im(Hx) = %e\t',imag(Matrix(1,2)));
                fprintf(fileID,'|Hx| = %e\t',abs(Matrix(1,2)));
                fprintf(fileID,'fase(Hx) = %e\t',angle(Matrix(1,2)));
                fprintf(fileID,'Re(Hz) = %e\t',real(Matrix(3,2)));
                fprintf(fileID,'Im(Hz) = %e\t',imag(Matrix(3,2)));
                fprintf(fileID,'|Hz| = %e\t',abs(Matrix(3,2)));
                fprintf(fileID,'fase(Hz) = %e\t',angle(Matrix(3,2)));
                fprintf(fileID,'Re(Ey) = %e\t',real(Matrix(2,1)));
                fprintf(fileID,'Im(Ey) = %e\t',imag(Matrix(2,1)));
                fprintf(fileID,'|Ey| = %e\t',abs(Matrix(2,1)));
                fprintf(fileID,'fase(Ey) = %e\n',angle(Matrix(2,1)));
            end
        end
        if strcmp(pol,'TM')
            plot(z,Matrixplot(1,:),'b',z,Matrixplot(2,:),'b--',z,Matrixplot(3,:),'g',z,Matrixplot(4,:),'g--');
            grid on;
            ylabel('E=(y=0) [V/m]');
            xlabel(['z [m] a x = ',num2str(x(j)),', teta = ',num2str(teta1(i))]);
            legend('Parte real Ex','Parte imm Ex','Parte real Ez','Parte imm Ez');
            figure;
            plot(z,Matrixplot(5,:),'b',z,Matrixplot(6,:),'b--');
            grid on;
            ylabel('Hy=(y=0) [V/m]');
            xlabel(['z [m] a x = ',num2str(x(j)),', teta = ',num2str(teta1(i))]);
            legend('Parte real Hy','Parte imm Hy');
        else
            plot(z,Matrixplot(7,:),'b');
            ylabel('E(y=x=0) [V/m]');
            grid on;
            xlabel(['z [m] a x = ',num2str(x(j)),', teta = ',num2str(teta1(i))]);
            figure;
            plot(z,Matrixplot(8,:),'b');
            ylabel('fase(E)');
            grid on;
            xlabel(['z [m] a x = ',num2str(x(j)),', teta = ',num2str(teta1(i))]);
            figure;
            plot(z,Matrixplot(1,:),'b',z,Matrixplot(2,:),'b--',z,Matrixplot(3,:),'g',z,Matrixplot(4,:),'g--');
            grid on;
            ylabel('Hx_e_Hz(y=0) [V/m]');
            xlabel(['z [m] a x = ',num2str(x(j)),', teta = ',num2str(teta1(i))]);
            legend('Parte real Hx','Parte imm Hx','Parte real Hz','Parte imm Hz');
            figure;
            plot(z,Matrixplot(5,:),'b',z,Matrixplot(6,:),'b--');
            grid on;
            ylabel('Ey(y=0) [V/m]');
            xlabel(['z [m] a x = ',num2str(x(j)),', teta = ',num2str(teta1(i))]);
            legend('Parte real Ey','Parte imm Ey');
            figure;
        end
    end
end

%% KEY FUNCTION

% funzione che ritorna il valore del campo elettrico e magnetico dato
% un punto dello spazio sul piano (x,z), il campo elettrico incidente(E_inc) sulla
% prima interfaccia e l'angolo della corrispondente incidenza
% rispetto all'asse z. Si considera essere senza perdite il primo mezzo.
% Ciascun mezzo si considera uniforme lungo l'asse y.
function [MatrixEH]=CalcolocampiEH(n,teta1,pol,lambda0,vettorez,E1inc,zp,xp)
% cerco a quale mezzo corrisponda lo zp in questione
mezzoi=Trovamezzo(vettorez,zp);
% Calcolo i seni degli angoli di incidenza dell'onda in ciascun mezzo
% rispepetto all'asse z.
sinteta=Calcolosinteta(n,teta1);
% trovo le componenti del campo elettrico e magnetico
% nel punto di coordinate (xp,0,zp) e le alloco in una matrice 3x2
% ove ogni riga corrisponde a una coordinata (x,y,z) e le cui due colonne
% si riferiscono rispettivamente a E e a H.
global c0;
global mi0;
eta0=mi0*c0;
MatrixEHprovv=VAmplitudeET(n,sinteta,pol,mezzoi,lambda0,vettorez,E1inc,zp,xp);
MatrixEH=zeros(3,2);
if strcmp(pol,'TM')
    MatrixEH(1,1)=sum(MatrixEHprovv);
    MatrixEH(3,1)=((MatrixEHprovv(2,1)-MatrixEHprovv(1,1))/sqrt(1-sinteta(mezzoi)^2))*sinteta(mezzoi);
    MatrixEH(2,2)=(n(mezzoi)/eta0)*(MatrixEHprovv(1,1)-MatrixEHprovv(2,1))/sqrt(1-sinteta(mezzoi)^2);
else
    MatrixEH(2,1)=sum(MatrixEHprovv);
    MatrixEH(1,2)=(n(mezzoi)/eta0)*(MatrixEHprovv(2,1)-MatrixEHprovv(1,1))*sqrt(1-sinteta(mezzoi)^2);
    MatrixEH(3,2)=(n(mezzoi)/eta0)*sum(MatrixEHprovv)*sinteta(mezzoi);
end
end

%% I, C++
% funzione che trova il mezzo in questione a partire
% dalla coordinata z inserita.
function [mezzo]=Trovamezzo(vettorez,zp)
if (zp<=vettorez(1))
    mezzo=1;
elseif (zp>vettorez(1) && zp<=vettorez(length(vettorez)) && length(vettorez)>1)
    for i=1:length(vettorez)-1
        if (zp>vettorez(i) && zp<=vettorez(i+1))
            mezzo=i+1;
            break;
        end
    end
else
    mezzo=length(vettorez)+1;
end
end


% funzione che ritorna il valore del campo elettrico trasversale componente
% progressiva e regressiva nel mezzo considerato e lo alloca in un vettore riga.
function [VettET]=VAmplitudeET(n,sinteta,pol,mezzoi,lambda0,vettorez,E1inc,zp,xp)
VettETprov=zeros(2,2);
[VettETprov(1,1),VettETprov(2,1),VettETprov(1,2)]=Calcoloampiezza(n,sinteta,pol,lambda0,vettorez,E1inc);
if (mezzoi==1)
    VettET=(Matricetotale(n,sinteta,pol,mezzoi,lambda0,vettorez,zp)\VettETprov(:,1))*exp(-1i*Wavenumb(n(mezzoi),lambda0)*sinteta(mezzoi)*xp);
elseif (mezzoi==length(n))
    VettET=(TrasmissionM2(n,sinteta,mezzoi,lambda0,zp-vettorez(mezzoi-1))\VettETprov(:,2))*exp(-1i*Wavenumb(n(mezzoi),lambda0)*sinteta(mezzoi)*xp);
else
    VettET=(Matricetotale(n,sinteta,pol,mezzoi,lambda0,vettorez,zp)\VettETprov(:,1))*exp(-1i*Wavenumb(n(mezzoi),lambda0)*sinteta(mezzoi)*xp);
end
end


% funzione che calcola le ampiezze del campo elettrico trasversale
% del primo mezzo e parte dell'ultimo mezzo (mezzo B). Si considera essere
% zero la componente regressiva dell'onda nel mezzo B dal momento che
% trattasi di una carta assorbente.
function [E1Tprogre,E1Tregre,EBTprogre]=Calcoloampiezza(n,sinteta,pol,lambda0,vettorez,E1inc)
MTemp=Matricetotale(n,sinteta,pol,length(n),lambda0,vettorez,vettorez(length(n)-1));
if strcmp(pol,'TM')
    E1Tprogre=E1inc*sqrt(1-sinteta(1)^2);
    EBTprogre=(MTemp(1,1))^-1*E1Tprogre;  % coefficiente di trasmissione * E1Tprogre
else
    E1Tprogre=E1inc;
    EBTprogre=(MTemp(1,1))^-1*E1Tprogre;  % coefficiente di riflessione/trasmissione * EBTprogre
end
E1Tregre=EBTprogre*MTemp(2,1);
end


% funzione che ritorna la matrice equivalente a una cascata di
% tutte le matrici di trasmissione.
% La posizione sull'asse z di ciscuna interfaccia è inserita in
% "vettorez".
function [Matrixtot]=Matricetotale(n,sinteta,pol,mezzo,lambda0,vettorez,zp)
Matrixtot=[1 0;0 1];
if(mezzo==1)
    Matrixtot=TrasmissionM2(n,sinteta,mezzo,lambda0,zp);
elseif(mezzo==2)
    Matrixtot=TrasmissionM1(n,sinteta,pol,mezzo)*TrasmissionM2(n,sinteta,2,lambda0,zp-vettorez(1));
else
    % Quando sono nel mezzo i attraverso i-1 interfacce.
    for i=2:mezzo-1
    Matrixtot=Matrixtot*TrasmissionM1(n,sinteta,pol,i)*TrasmissionM2(n,sinteta,i,lambda0,vettorez(i)-vettorez(i-1));
    end
    Matrixtot=Matrixtot*TrasmissionM1(n,sinteta,pol,mezzo)*TrasmissionM2(n,sinteta,mezzo,lambda0,zp-vettorez(mezzo-1));
end
end


% funzione che calcola la matrice
% di trasmissione in un mezzo omogeneo alla posizione zp, ove questa
% coordinata è la distanza tra il punto in cui si vuole calcolare il campo
% e la z dell'interfaccia del mezzo corrispondente.
function [Mtrasm2]=TrasmissionM2(n,sinteta,mezzo,lambda0,zp)
Mtrasm2=[exp(1i*Phasethick(n(mezzo),sinteta(mezzo),lambda0,zp)) 0;0 exp(-1i*Phasethick(n(mezzo),sinteta(mezzo),lambda0,zp))];
end


%% I
% funzione che calcola la matrice di trasmissione
% nell'interfaccia tra il mezzo considerato e quello subito a esso precedente.
function [Mtrasm1]=TrasmissionM1(n,sinteta,pol,mezzo)
Mtrasm1=(TrasmissioncoeffT(n,sinteta,pol,mezzo)^-1)*[1 ReflectioncoeffT(n,sinteta,pol,mezzo);ReflectioncoeffT(n,sinteta,pol,mezzo) 1];
end


%% I, C++
% funzione che calcola il coefficiente di trasmissione in un'interfaccia tra
% il mezzo considerato e quello subito a esso precedente.
function [tauT]=TrasmissioncoeffT(n,sinteta,pol,mezzo)
tauT=1+ReflectioncoeffT(n,sinteta,pol,mezzo);
end


%% I
% funzione che calcola il coefficinte di riflessione
% nell'interfaccia tra il mezzo considerato e quello subito a esso
% precedente.
function [roT]=ReflectioncoeffT(n,sinteta,pol,mezzo)
if(sinteta(mezzo)>1)
    if(strcmp(pol,'TM'))
        roT=(-1i*n(mezzo-1)*sqrt((n(mezzo-1)*sinteta(mezzo-1))^2-n(mezzo)^2)*sqrt(1-sinteta(mezzo)^2))/(-1i*n(mezzo-1)*sqrt((n(mezzo-1)*sinteta(mezzo-1))^2-n(mezzo)^2)+n(mezzo)^2*sqrt((1-sinteta(mezzo)^2)));
    else
        roT=(ntrasversale(n(mezzo-1),sinteta(mezzo-1),pol)+1i*sqrt((n(mezzo-1)*sinteta(mezzo-1))^2-n(mezzo)^2))/(ntrasversale(n(mezzo-1),sinteta(mezzo-1),pol)-1i*sqrt((n(mezzo-1)*sinteta(mezzo-1))^2-n(mezzo)^2));
    end
else
    roT=(ntrasversale(n(mezzo-1),sinteta(mezzo-1),pol)-ntrasversale(n(mezzo),sinteta(mezzo),pol))/(ntrasversale(n(mezzo),sinteta(mezzo),pol)+ntrasversale(n(mezzo-1),sinteta(mezzo-1),pol));
end
end


%% I, C++
% funzione che calcola il phase thickness, con zp che è
% la differenza tra la coordinata ove si vuole calcolare il campo e
% le lunghezze di tutti i mezzi prima di quelllo in questione.
function [delta]=Phasethick(ni,sintetai,lambda0,zp)
if(sintetai>1)
    k0=Wavenumb(ni,lambda0)/ni;
    delta=-1i*k0*sqrt((ni*sintetai)^2-ni^2)*zp;
else
    delta=Wavenumb(ni,lambda0)*zp*sqrt(1-sintetai^2);
end
end


%% I, C++
% funzione che calcola il numero d'onda
% a partire dalla lunghezza d'onda nel vuoto e l'indice di rifrazione.
function [k]=Wavenumb(ni,lambda0)
k=2*pi*ni/lambda0;
end


%% I, C++
% funzione che calcola una matrice con i seni degli angoli
% dell'onda progressiva e regressiva in ciascun mezzo usando la legge di
% Snell.
function [sinteta]=Calcolosinteta(n,teta1)
sinteta=ones(1,length(n));
sinteta(1)=sin(teta1);
if length(n)>1
    for i=2:length(n)
    sinteta(i)=n(1)*sinteta(1)/n(i);
    end
end
end


%% I, C++
% funzione che calcola il numero d'onda trasversale del mezzo
% in base alla polarizzazione dell'onda.
function [niT]=ntrasversale(ni,sintetai,pol)
if strcmp(pol,'TE')
    niT=ni*sqrt(1-sintetai^2);
else
    niT=ni/sqrt(1-sintetai^2);
end
end
