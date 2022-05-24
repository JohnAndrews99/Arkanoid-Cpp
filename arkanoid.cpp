#include<iostream>
#include<graphics.h>
#include<math.h>
#include<algorithm>
using namespace std;
#define pi 3.1415926

double signo(double num); /// Devuelve el signo de un numero (positivo, negativo, neutro)
double absoluto(double num); /// Devuelve el valor absoluto del numero
double lengthdir_x(double len,double dir); /// Le das la distancia y el angulo, te devuelve la extension x de esos valores
double lengthdir_y(double len,double dir); /// Le das la distancia y el angulo, te devuelve la extension y de esos valores
double speed(double vx, double vy); /// Devuelve la distancia total de una extension x e y
double direction(double vx, double vy); /// Devuelve la direccion de una extension x e y
double point_direction(double x1,double y1,double x2,double y2); ///Devuelve la distancia entre 2 puntos
double point_distance(double x1,double y1,double x2,double y2); /// Devuelve la direccion entre 2 puntos
double round(double num); /// Devuelve el numero ingresado pero redondeado

int main()
{
    cout<<"Juego en progreso";
    int pelotasperdidas=0,vidas=3,jugando=1,peg[9]={1,0,0,0,0,0,0,0,0},pelotas=1,maxspeed=16,pegpu=0,split=0,bloques=30,bloquesi=bloques;
    int cercano[9]={0,0,0,0,0,0,0,0,0},destruirbloques=0,destruirbloque[bloques]; /// BLOQUE MAS CERCANO A LAS PELOTAS
    int fuego[9]={0},frame=0;
    int shootpu=0,cercanop=-1,canshoot=1,shootx=-999,shooty,shooth;
    double powerupx,powerupy,poweruppower=0; ///PELOTA DE PODER, DA UN PODER AL AZAR!!!!!!!!!!!!!!!!!!!!!
    double paletax=300,paletay=600,paletaw=100,paletahs=0,paletavs=0,paletac=WHITE; /// VARIABLES DE LA PALETA
    double pelotax[9]={300},pelotay[9]={600},pelotar[9]={8},pelotahs[9]={0},pelotavs[9]={0},pelotac[9]={WHITE}; /// VARIABLES DE LAS PELOTAS
    double bloquex[bloques],bloquey[bloques],bloquew[bloques],bloqueh[bloques],bloquec[bloques],bloquel[bloques]; /// VARIABLES DE LOS BLOQUES
    bool skip[9]={0};
    initwindow(700, 700, "ARKANOID", 0, 0, false, true);///Inicializa la ventana de juego
    settextstyle(DEFAULT_FONT,HORIZ_DIR,2); ///Inicializa el texto
    for(int i=0;i<ceil(double(bloques)/10);i++)///Bloques default
    {
        for(int j=0;j<min(bloques-(i*10),10);j++)
        {
            bloquex[(i*10)+j]=(j*69.5)+5;
            bloquey[(i*10)+j]=50+(i*25);
            bloquew[(i*10)+j]=64.5;
            bloqueh[(i*10)+j]=20;
            if(j % 3 == 0)
            {
                bloquec[(i*10)+j]=WHITE;
                bloquel[(i*10)+j]=1;
            }
            if(j % 3 == 1)
            {
                bloquec[(i*10)+j]=YELLOW;
                bloquel[(i*10)+j]=2;
            }
            if(j % 3 == 2)
            {
                bloquec[(i*10)+j]=COLOR(255,100,50);
                bloquel[(i*10)+j]=3;
            }
            if((j+(i*10)) % 15 == 0)
            {
                bloquec[(i*10)+j]=COLOR(0,255,0);
                bloquel[(i*10)+j]=1;
            }
        }
    }
    for(int i=0;i<bloques;i++) /// Esto hace que el proceso a continuacion se repita para todos los bloques
    {
        setcolor(bloquec[i]); /// Inicializa el color de el bloque
        rectangle(bloquex[i],bloquey[i],bloquex[i]+bloquew[i],bloquey[i]+bloqueh[i]); // El bloque es un rectangulo, esto lo dibuja : rectangle(x minimo, y minimo, x maximo, y maximo)
    }
    setfillstyle(SOLID_FILL,BLACK); /// Esto inicializa el estilo de dibujado para borrar objetos
    while(jugando)///Mientras se este jugando, el ciclo continua
    {
        frame++;
        if(frame>60)
            frame=1;
        if(GetAsyncKeyState(VK_RIGHT))/// Funcion que recibe el input, en este caso VK_RIGHT es la tecla hacia la derecha
        {
            if(paletahs<maxspeed)/// Si la paleta no ha llegado a la velocidad maxima, se acelera por 1
                paletahs++;
        }
        if(GetAsyncKeyState(VK_LEFT))
        {
            if(paletahs>-maxspeed)/// Si la paleta no ha llegado a la velocidad minima, se desacelera por 1
                paletahs--;
        }
        if(GetAsyncKeyState(VK_UP)) /// Si se presiona flecha arriba, la pelota pegada a la paleta sale disparada
        {
            for(int i=0;i<pelotas;i++) /// Este ciclo hace que se evaluen todas las pelotas activas
            {
                if(peg[i]) /// esto solo servira si la pelota "i" esta pegada a la paleta
                {
                    if(pelotahs[i]==0) /// Esto es para saber si la pelota esta siendo lanzada por primera vez
                    {
                        if(paletahs!=0) /// Si la paleta se esta moviendo, la pelota se lanza en la direccion de su movimiento
                            pelotahs[i]=8*signo(paletahs);
                        else /// Si no, se lanza a la derecha
                            pelotahs[i]=8;
                    }
                    if(pelotavs[i]==0) /// Tambien para saber si la pelota se esta lanzando por primera vez
                    {
                        pelotavs[i]=-8;
                    }
                    pelotay[i]=paletay; /// Hace que la pelota sea lanzada desde la misma altura de la paleta
                    peg[i]=0; /// La pelota que se lanzo ya no esta pegada
                }
            }
        }
        if(split>0) /// La variable split controla cuantas pelotas se van a generar con el power up (contando la principal)
        {
            pelotas=split; /// Ahora hay tantas pelotas como el powerup split decia
            for(int i=1;i<pelotas;i++) /// Debido a que se generaron pelotas nuevas, estas deben ser inicializadas
            {
                skip[i]=false;
                pelotar[i]=pelotar[0]; /// El radio de la pelota "i" es igual al de la pelota principal
                pelotax[i]=pelotax[0]; /// La coordenada x de la pelota "i" es igual al de la pelota principal
                pelotay[i]=pelotay[0]; /// La coordenada y de la pelota "i" es igual al de la pelota principal
                pelotac[i]=pelotac[0]; /// El color de la pelota "i" es igual al de la pelota principal
                pelotahs[i]=lengthdir_x(speed(pelotahs[0],pelotavs[0])*1.3,direction(pelotahs[0],pelotavs[0])+(i*(360/pelotas)));/// Esto hace que la pelota nueva sea lanzada en una direccion
                pelotavs[i]=lengthdir_y(speed(pelotahs[0],pelotavs[0])*1.3,direction(pelotahs[0],pelotavs[0])+(i*(360/pelotas)));/// que sea contraria a la pelota principal, si hay 3 pelotas
            }                                                                                                                    /// las pelotas seran lanzadas en angulos de 135 grados.
            split=0; /// Ya no hay necesidad de separar la pelota asi que split queda en 0
        }
        if(!GetAsyncKeyState(VK_LEFT) && !GetAsyncKeyState(VK_RIGHT)) /// Si no se esta moviendo activamente la paleta, esta empieza a ralentizarse sola
        {                                                             /// El simbolo ! indica negacion, asi que esta evaluando que no se esten presionando ni izquierda ni derecha
            if((paletahs*signo(paletahs))<=.75)
                paletahs=0;/// Si la paleta va muy lento, se paraliza
            else if(paletahs!=0)
                paletahs-=signo(paletahs)*.75; /// En otro caso, se va ralentizando por 0.75 de velocidad

        }
        setcolor(BLACK);
        fillellipse(paletax,paletay,paletaw*1.5,30);
        paletax+=paletahs; /// La coordenada x de la paleta cambia dependiendo de la velocidad horizontal
        paletay+=paletavs; /// La coordenada y de la paleta cambia dependiendo de la velocidad vertical
        for(int i=0;i<pelotas;i++)
        {
            fillellipse(pelotax[i],pelotay[i],pelotar[i],pelotar[i]);
            pelotax[i]+=pelotahs[i];  ///Lo mismo pasa con las pelotas, la x cambia con la vel horizontal y asi...
            pelotay[i]+=pelotavs[i];
        }
        ///COSAS DE LA PALETA
        if(paletax-(paletaw/2)<0) /// Esto impide que la paleta se salga de la pantalla
        {
            paletax=(paletaw/2);
            paletahs=0;
        }
        if(paletax+(paletaw/2)>700) /// Esto impide que la paleta se salga de la pantalla, tambien...
        {
            paletax=700-(paletaw/2);
            paletahs=0;
        }
        ///DIBUJADO DE GRAFICOS
        ///BLOQUES
        for(int i=0;i<bloques;i++) /// Esto hace que el proceso a continuacion se repita para todos los bloques
        {
            for(int j=0;j<pelotas;j++) /// ESTO hace que cada bloque evalue su distancia hacia las pelotas
            {                          /// Ese if de abajo evalua si la distancia desde el bloque a la pelota "j" es menor a la distancia
                if(point_distance(pelotax[j],pelotay[j],bloquex[i]+(bloquew[i]/2),bloquey[i]+(bloqueh[i]/2))<point_distance(pelotax[j],pelotay[j],bloquex[cercano[j]]+(bloquew[cercano[j]]/2),bloquey[cercano[j]]+(bloqueh[cercano[j]]/2)))
                {                      /// Entre el bloque y la pelota mas cercana registrada anteriormente
                    cercano[j]=i;      /// Si la pelota "j" es mas cercana que la anterior mas cercana, esta es registrada en la variable cercano[j]
                }
            }
            if(paletay-bloquey[i]<=paletay-bloquey[cercanop] && bloquex[i]<paletax && bloquex[i]+bloquew[i]>paletax)
            {                   ///Este if de arriba hace algo parecido pero en vez de evaluarlo con la pelota lo hace con la paleta
                cercanop=i;     ///Y es para el power up de shoot, para registrar cual bloque sera afectado por el disparo
            }
            if(bloquex[cercanop]>paletax || bloquex[cercanop]+bloquew[cercanop]<paletax)
                cercanop=-1; /// Si no hay bloque en la vicinidad de el disparador, simplemente no hay bloque que afectar, la variable queda -1
        }
        if(shootx!=-999 && frame % 2 == 0)
        {
            setcolor(BLACK);
            rectangle(shootx-1,shooty,shootx+1,shooth); /// elimina el disparo anterior
            shootx=-999;
        }
        if(GetAsyncKeyState(VK_SPACE) && canshoot && shootpu>0) /// Si se presiona espacio, se puede disparar, y el power up shoot esta activo:
        {
            setcolor(BLACK);
            rectangle(shootx-1,shooty,shootx+1,shooth);/// elimina el disparo anterior
            setcolor(YELLOW); /// El disparo es de color amarillo
            canshoot=0;       /// Ya disparo asi que se pone 0 el canshoot, esto evita que se dispare continuamente
            if(cercanop>=0)   /// Afecta un bloque si hay bloque que disparar, el else hace que simplemente dispare al vacio
            {
                rectangle(paletax-1,paletay,paletax+1,bloquey[cercanop]+bloqueh[cercanop]); // se dibuja la bala de la paleta a el bloque
                shootx=paletax;
                shooty=paletay;
                shooth=bloquey[cercanop]+bloqueh[cercanop];
                shootpu--; /// Una municion menos
                bloquel[cercanop]--; /// La vida de el bloque disminuye por 1
                if(bloquec[cercanop]==COLOR(0,255,0)) /// Si el bloque era verde, lanza una pelota de power up
                {
                    powerupx=bloquex[cercanop]+(bloquew[cercanop]/2); /// La coordenada x de la pelota sera el centro del bloque
                    powerupy=bloquey[cercanop]+8; /// la coordenada y es la y del bloque
                    poweruppower=1+(frame%4); /// Asigna un power up aleatorio a la pelota del power up
                }
                if(bloquel[cercanop]==2)
                    bloquec[cercanop]=YELLOW;
                if(bloquel[cercanop]==1)
                    bloquec[cercanop]=WHITE;
                if(bloquel[cercanop]==0)
                    bloquec[cercanop]=BLACK;
                setcolor(bloquec[cercanop]); /// Inicializa el color de el bloque
                rectangle(bloquex[cercanop],bloquey[cercanop],bloquex[cercanop]+bloquew[cercanop],bloquey[cercanop]+bloqueh[cercanop]); // El bloque es un rectangulo, esto lo dibuja : rectangle(x minimo, y minimo, x maximo, y maximo)

                if(bloquel[cercanop]<=0) /// Si la vida del bloque es menor o igual a 0
                {
                    bloques--; /// Un bloque menos que evaluar en el ciclo for de los bloques
                    destruirbloque[destruirbloques]=cercanop; /// Que bloque destruir en el ciclo de destruccion
                    destruirbloques++; /// Hay tantos bloques que destruir, el ciclo de destruccion se repite estas veces
                }
            }
            else /// Este es el else en caso que no haya bloque que disparar
            {
                rectangle(paletax-1,paletay,paletax+1,0); /// La bala llega hasta el tope de la ventana
                shootx=paletax;
                shooty=paletay;
                shooth=0;
                shootpu--; /// Gastase una municion innecesariamente, buena campeon
            }
        }
        if(!GetAsyncKeyState(VK_SPACE)) /// Si ya no se esta presionando la tecla de disparo (espacio)
            canshoot=1; /// Puedes disparar nuevamente
        ///DIBUJADO DE LA PALETA
        setcolor(paletac); /// Si por casualidad a la paleta se le asigna un color, esto lo inicializa
        rectangle(paletax-(paletaw/2),paletay,paletax+(paletaw/2),paletay+10); /// se dibuja la paleta
        if(pegpu) /// Si el power up de sticky esta activado, dibuja unas lineas alrededor del tope de la paleta
        {
            line(paletax-(paletaw/2)-2,paletay-2,paletax+(paletaw/2)+2,paletay-2); ///Linea superior
            line(paletax-(paletaw/2)-2,paletay-2,paletax-(paletaw/2)-2,paletay+4); ///Linea izquierda
            line(paletax+(paletaw/2)+2,paletay-2,paletax+(paletaw/2)+2,paletay+4); ///Linea derecha
        }
        if(shootpu) /// Si hay balas que disparar:
        {
            line(paletax-10,paletay,paletax-3,paletay-7);         /// Estas lineas y el rectangulo
            line(paletax+10,paletay,paletax+3,paletay-7);         /// Solo estan dibujando el arma
            rectangle(paletax-3,paletay-14,paletax+3,paletay);    /// Encima de la paleta
            for(int i=0;i<shootpu;i++) /// Se repetira cuanta municion haya
            {
                rectangle(paletax+3+(i*2),paletay-8,paletax+5+(i*2),paletay-3); /// Dibuja la municion restante al lado del arma
            }
        }
        if(poweruppower>0)
        {
            setcolor(BLACK);
            fillellipse(powerupx,powerupy,12,12);
            setcolor(COLOR(0,255,0));
            powerupy+=5;
            circle(powerupx,powerupy,12);
            if(poweruppower==1)
                outtextxy(powerupx-8,powerupy-8,"F");
            if(poweruppower==2)
                outtextxy(powerupx-8,powerupy-8,"P");
            if(poweruppower==3)
                outtextxy(powerupx-8,powerupy-8,"3");
            if(poweruppower==4)
                outtextxy(powerupx-8,powerupy-8,"A");
            if(powerupx>paletax-(paletaw/2) && powerupx<paletax+(paletaw/2) && powerupy>paletay && powerupy<paletay+10)
            { /// El if anterior evalua si la pelota del power up colisiona con la paleta
                if(poweruppower==1)
                {
                    for(int j=0;j<pelotas;j++)
                    {
                        fuego[j]=1;
                    }
                }
                if(poweruppower==2)
                    pegpu=1;
                if(poweruppower==3)
                    split=3;
                if(poweruppower==4)
                    shootpu=6;

                poweruppower=0;
                setcolor(BLACK);
                circle(powerupx,powerupy,12);
            }
        }
        ///PELOTAS
        if(vidas<=0)
        {
            outtextxy(300,350,"GAME OVER");
        }
        for(int i=0;i<pelotas;i++) /// Se repite cuantas pelotas haya
        {
            if(pelotay[i]>700)
            {
                pelotay[i]=-999;
                skip[i]=true;
                pelotahs[i]=0;
                pelotavs[i]=0;
                pelotasperdidas++;
                if(pelotasperdidas>=pelotas)
                {
                    vidas--;
                    if(vidas>=1)
                    {
                        pelotasperdidas=0;
                        skip[0]=false;
                        pelotas=1;
                        pelotahs[0]=0;
                        pelotavs[0]=0;
                        pelotax[0]=paletax+paletahs;
                        pelotay[0]=paletay-pelotar[0];
                        peg[0]=1;
                    }
                }
            }
            if (skip[i]==false)///Si se debe de saltar, lo siguiente no se realiza
            {
                if(pelotax[i]-pelotar[i]<0) /// esto hace que rebote del borde izquierdo
                {
                    pelotax[i]=pelotar[i];
                    pelotahs[i]=-pelotahs[i];
                }
                if(pelotay[i]-pelotar[i]<0) /// esto hace que rebote del borde superior
                {
                    pelotay[i]=pelotar[i];
                    pelotavs[i]=-pelotavs[i];
                }
                if(pelotax[i]+pelotar[i]>700)  /// esto hace que rebote del borde derecho
                {
                    pelotax[i]=700-pelotar[i];
                    pelotahs[i]=-pelotahs[i];
                }
                ///COLISION DE PELOTA CON PALETA
                if(pelotax[i]+pelotar[i]>paletax-(paletaw/2) && pelotax[i]-pelotar[i]<paletax+(paletaw/2) && pelotay[i]<paletay && pelotay[i]+(pelotavs[i]*2)>paletay)
                { /// Ese if evalua si la pelota colisiona con la paleta
                    pelotavs[i]=lengthdir_y(10,direction(pelotax[i]-paletax,pelotay[i]-(paletay+(paletaw/2))));/// Estas 2 son para darle la nueva
                    pelotahs[i]=lengthdir_x(10,direction(pelotax[i]-paletax,pelotay[i]-(paletay+(paletaw/2))));/// Direccion a la pelota
                    if(pegpu>0) /// Si el powerup sticky esta activo, la pelota se pega
                    {
                        peg[i]=1;
                    }
                }
                ///COLISION DE PELOTA CON BLOQUES
                if((pelotax[i]+pelotar[i]>=bloquex[cercano[i]] && pelotax[i]-pelotar[i]<=bloquex[cercano[i]]+bloquew[cercano[i]]) || (pelotax[i]+pelotar[i]+pelotahs[i]>=bloquex[cercano[i]] && pelotax[i]-pelotar[i]+pelotahs[i]<=bloquex[cercano[i]]+bloquew[cercano[i]]))
                { /// El cilco anterior de los bloques evaluando la pelota mas cercana es para esto, la pelota solo evalua colisiones con el bloque mas cercano!
                    if((pelotay[i]-pelotar[i]<=bloquey[cercano[i]] && pelotay[i]+pelotavs[i]+pelotar[i]+1>=bloquey[cercano[i]]) || (pelotay[i]+pelotar[i]>=bloquey[cercano[i]]+bloqueh[cercano[i]] && pelotay[i]+pelotavs[i]-pelotar[i]-1<=bloquey[cercano[i]]+bloqueh[cercano[i]]))
                    {
                        pelotavs[i]=-pelotavs[i];
                        bloquel[cercano[i]]--;
                        if(fuego[i])
                            bloquel[cercano[i]]=0;
                        if(bloquec[cercano[i]]==COLOR(0,255,0)) /// Si el bloque era verde, lanza una pelota de power up
                        {
                            powerupx=bloquex[cercano[i]]+(bloquew[cercano[i]]/2); /// La coordenada x de la pelota sera el centro del bloque
                            powerupy=bloquey[cercano[i]]+8; /// la coordenada y es la y del bloque
                            poweruppower=1+(frame%4); /// Asigna un power up aleatorio a la pelota del power up
                        }
                        if(bloquel[cercano[i]]==2)
                            bloquec[cercano[i]]=YELLOW;
                        if(bloquel[cercano[i]]==1)
                            bloquec[cercano[i]]=WHITE;
                        if(bloquel[cercano[i]]==0)
                            bloquec[cercano[i]]=BLACK;
                        setcolor(bloquec[cercano[i]]); /// Inicializa el color de el bloque
                        rectangle(bloquex[cercano[i]],bloquey[cercano[i]],bloquex[cercano[i]]+bloquew[cercano[i]],bloquey[cercano[i]]+bloqueh[cercano[i]]); // El bloque es un rectangulo, esto lo dibuja : rectangle(x minimo, y minimo, x maximo, y maximo)

                        if(bloquel[cercano[i]]<=0)
                        {
                            bloques--;
                            destruirbloque[destruirbloques]=cercano[i];
                            destruirbloques++;
                        }
                    }
                }
                if((pelotay[i]+pelotar[i]>=bloquey[cercano[i]] && pelotay[i]-pelotar[i]<=bloquey[cercano[i]]+bloqueh[cercano[i]]) || (pelotay[i]+pelotar[i]+pelotavs[i]>=bloquey[cercano[i]] && pelotay[i]-pelotar[i]+pelotavs[i]<=bloquey[cercano[i]]+bloqueh[cercano[i]]))
                {
                    if((pelotax[i]+pelotar[i]<=bloquex[cercano[i]] && pelotax[i]+pelotahs[i]+pelotar[i]+1>=bloquex[cercano[i]]) || (pelotax[i]-pelotar[i]>=bloquex[cercano[i]]+bloquew[cercano[i]] && pelotax[i]+pelotahs[i]-pelotar[i]-1<=bloquex[cercano[i]]+bloquew[cercano[i]]))
                    {
                        pelotahs[i]=-pelotahs[i];
                        bloquel[cercano[i]]--;
                        if(bloquec[cercano[i]]==COLOR(0,255,0)) /// Si el bloque era verde, lanza una pelota de power up
                        {
                            powerupx=bloquex[cercano[i]]+(bloquew[cercano[i]]/2); /// La coordenada x de la pelota sera el centro del bloque
                            powerupy=bloquey[cercano[i]]+8; /// la coordenada y es la y del bloque
                            poweruppower=1+(frame%4); /// Asigna un power up aleatorio a la pelota del power up
                        }
                        if(fuego[i])
                            bloquel[cercano[i]]=0;
                        if(bloquel[cercano[i]]==2)
                            bloquec[cercano[i]]=YELLOW;
                        if(bloquel[cercano[i]]==1)
                            bloquec[cercano[i]]=WHITE;
                        if(bloquel[cercano[i]]==0)
                            bloquec[cercano[i]]=BLACK;
                        setcolor(bloquec[cercano[i]]); /// Inicializa el color de el bloque
                        rectangle(bloquex[cercano[i]],bloquey[cercano[i]],bloquex[cercano[i]]+bloquew[cercano[i]],bloquey[cercano[i]]+bloqueh[cercano[i]]); // El bloque es un rectangulo, esto lo dibuja : rectangle(x minimo, y minimo, x maximo, y maximo)

                        if(bloquel[cercano[i]]<=0  || fuego[i])
                        {
                            bloques--;
                            destruirbloque[destruirbloques]=cercano[i];
                            destruirbloques++;
                        }
                    }
                }
                ///DESTRUCTOR DE BLOQUES
                for(int j=0;j<destruirbloques;j++) /// El ciclo que destruye bloques
                {
                    for(int k=destruirbloque[j];k<bloquesi-1;k++) /// Esto hace que se evaluen menos bloques en vez de todos
                    {                                             /// Desplaza las variables de los bloques desde el que se va
                        bloquex[k]=bloquex[k+1];                  /// A destruir hasta el ultimo bloque de todos
                        bloquey[k]=bloquey[k+1];
                        bloquew[k]=bloquew[k+1];
                        bloqueh[k]=bloqueh[k+1];
                        bloquec[k]=bloquec[k+1];
                        bloquel[k]=bloquel[k+1];
                    }
                }
                destruirbloques=0; ///ya no se deben destruir mas bloques
                ///COSAS MISCELANEAS
                if(peg[i]) /// Si la pelota esta pegada
                {
                    pelotax[i]-=pelotahs[i];        /// Esto evita que la pelota se mueva horizontalmente
                    pelotay[i]-=pelotavs[i];        /// Esto evita que la pelota se mueva verticalmente
                    pelotax[i]+=paletahs;           /// Esto hace que la pelota se mueva con la paleta
                    pelotay[i]=paletay-(pelotar[i]);/// Esto es para que la pelota aparezca encima de la paleta
                }
                setcolor(YELLOW); /// Fuego amarillo
                if(!fuego[i]) /// si la pelota no esta prendida en fuego, se colorea normal
                    setcolor(pelotac[i]);
                else
                    circle(pelotax[i],pelotay[i],pelotar[i]/2);
                circle(pelotax[i],pelotay[i],pelotar[i]); /// la pelota es un circulo
            }
        }
        Sleep(20);
    }
    closegraph(); /// termina la accion de graficar

    system("CLS"); /// una vez sales del ciclo de juego, sale un mensaje en la consola de que ya terminaste
    cout<<"Juego finalizado";
    return 0;
}

double signo(double num)
{
    if(num!=0)
        return sqrt(num*num)/num; /// Okay, devuelve la raiz cuadrada de el numero potenciado al cuadrado
    else                          /// Esto te da el numero sin signo, pero al dividirlo denuevo por el numero
        return 0;                 /// con signo, te devuelve 1 o -1, el else es para que devuelva 0 en vez de NULL
}
double absoluto(double num)
{
    return num*signo(num); /// Aqui simplemente devuelve el numero multiplicado por su signo, hace uso de la funcion anterior
}
double lengthdir_x(double len,double dir)
{
    if((cos(dir*(pi/180)))!=0) /// Esto simplemente devuelve el cateto adyacente de una hipotenusa y un angulo
        return (cos(dir*(pi/180)))*len;
    else
        return 0; /// pero si el resultado da 0, debe devolver 0 en vez de NULL
}
double lengthdir_y(double len,double dir)
{
    if((sin(dir*(pi/180)))!=0) /// Esto simplemente devuelve el cateto opuesto de una hipotenusa y un angulo
        return -(sin(dir*(pi/180)))*len;
    else
        return 0;
}
double speed(double vx, double vy)
{
    return sqrt(pow(vx,2)+pow(vy,2)); /// Teorema de pitagoras
}
double direction(double vx, double vy)
{
    if(vx!=0 && vy!=0)
    {
        if(vx>0 && -vy>0)
            return atan(-vy/vx)*(180/pi); /// Devuelve la tangente inversa de un vector con extension x e y
        if(vx<0)
            return 180+atan(-vy/vx)*(180/pi); /// esto se evalua para los 4 cuadrantes
        if(vx>0 && -vy<0)
            return 270-atan(-vy/vx)*(180/pi); /// Esta evaluado 3 veces porque el 2do y 3er cuadrante se evaluan igual
    }
    if(vx==0)
    {
        if(vy>0)
            return 270; /// Si es el eje y solamente, devuelve 90 o 270
        else
            return 90;
    }
    if(vy==0)
    {
        if(vx>0) /// Si es el eje x solamente, devuelve 0 o 180
            return 0;
        else
            return 180;
    }
}
double point_direction(double x1, double y1,double x2, double y2)
{
    int vx=x2-x1,vy=y2-y1; /// Esto es literalmente lo mismo a lo de arriba pero estas operaciones convierten
    if(vx!=0 && vy!=0)     /// x1,y1,x2,y2 en extension x y extension y
    {
        if(vx>0 && -vy>0)
            return atan(-vy/vx)*(180/pi);
        if(vx<0)
            return 180+atan(-vy/vx)*(180/pi);
        if(vx>0 && -vy<0)
            return 270-atan(-vy/vx)*(180/pi);
    }
    if(vx==0)
    {
        if(vy>0)
            return 270;
        else
            return 90;
    }
    if(vy==0)
    {
        if(vx>0)
            return 0;
        else
            return 180;
    }
}
double point_distance(double x1,double y1,double x2,double y2)
{
    return sqrt(pow(absoluto(x1-x2),2)+pow(absoluto(y1-y2),2)); /// Teorema de pitagoras otra vez
}
double round(double num)
{
    int dec=(num-int(num))*10; /// Saca la decima de el numero, restandole al numero con decimal, el mismo numero pero sin decimal
    if(dec>=5)
        return int(num)+1; /// Si la decima es mayor a 5, devuelve el numero entero mas 1
    else
        return int(num); /// Si la decima es menor a 5, devuelve el numero entero normal
}

