#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "./Librerias/Menu.h"
#include "./Librerias/Struct.h"
#include "./Librerias/Grafica.h"

unsigned char LeerFuncion(int argc, char *argv[],char lectura[50], float intervaloEIncremento[2], float *incremento, struct NewUser *user);
void OrganizarFuncion(char lectura[50], char funcion[52]);
void IdentificarCantidadDeTerminos(int argc, char *argv[],char funcion[52], float intervalo[2], float incremento);
void IdentificarExponentes(char funcion[52], unsigned char cantidadOperadores, int *exponentes);
int SacarExponentes(char funcion[52], unsigned char posicion);
void IdentificarCoeficientes(char funcion[52], unsigned char cantidadOperadores, int *coeficintes);
float EvaluarPunto(float x, float *rango, int *exponentes, int *coeficintes, int longitud);
int aux(int argc, char *argv[],struct NewUser *user);
void Proceso(int argc, char *argv[],struct NewUser *user, char lectura[50], char funcion[52], float intervalo[2], float incremento);

/*
Función que lee los datos ingresados por el usuario para graficar
una función
*/
unsigned char LeerFuncion(int argc, char *argv[],char lectura[50], float intervaloEIncremento[2], float *incremento, struct NewUser *user){
    unsigned char valido=1;
    //struct Accesos acceso;
    float inicio;
    float final;
    float incre;
    printf("Ingrese la función\n");
    scanf("%s", lectura);
    printf("Ingrese el inicio del dominio\n");
    scanf("%f", &inicio);
    printf("Ingrese el final del dominio\n");
    scanf("%f", &final);
    printf("Ingrese el incremento de la función\n");
    scanf("%f", &incre);
    intervaloEIncremento[0]=inicio;
    intervaloEIncremento[1]=final;
    *incremento=incre;
    struct Accesos acceso;
    if(inicio>=final){
        valido=0;
    }
    if(valido){
        CrearAcceso(argc,argv,user, &acceso,inicio,final,incre,lectura);
    }else{
        printf("Dominio no valido\n");
    }
    
    return valido;
}

/*
Se le añade un espacio al principio al principio y al final
del string para facilitar su manejo
*/
void OrganizarFuncion(char lectura[50], char funcion[52]){
    strcat(funcion,lectura);
    strcat(funcion, " ");
}

/*
Identifica la cantidad de terminos en la funcion
segun los operadores de suma y resta que haya
en esta
despues indica los valores de x y f(x)
por ultimo abre la ventana del grafico
*/
void IdentificarCantidadDeTerminos(int argc, char *argv[],char funcion[52], float intervalo[2], float incremento){
    unsigned char cantidadOperadores=0;
    for(unsigned char i=0; funcion[i]!='\0';i++){
        if(funcion[i]== '+' || funcion[i] == '-'){
            cantidadOperadores++;
        }
    }
    if(funcion[1]=='-'){
        cantidadOperadores--;
    }
    
    int exponentes[cantidadOperadores+1];
    int coeficientes[cantidadOperadores+1];
    IdentificarExponentes(funcion,cantidadOperadores,exponentes);
    IdentificarCoeficientes(funcion,cantidadOperadores,coeficientes);

    int puntosX=(intervalo[1]-intervalo[0])/incremento;
    puntosX+=1;
    float dominio[puntosX];
    float rango[puntosX];

    for(int i=0;i<puntosX;i++){
        dominio[i]=(intervalo[0]*1.0)+(i*incremento*1.0);
        rango[i]=EvaluarPunto(dominio[i], rango, exponentes, coeficientes, cantidadOperadores+1);
        printf("x: %f f(X): %f",dominio[i],rango[i]);
    }   
    AbrirVentana(argc,argv,dominio,rango,puntosX);
    
}

/*
A un x dado evalua el f(x) según
la función ingresada
*/
float EvaluarPunto(float x, float *rango, int *exponentes, int *coeficintes, int longitud){
    float valor=0;
    printf("x: %f\n",x);
    printf("longitud %i\n", longitud);
    for(int i=0;i<longitud;i++){
        valor+=(coeficintes[i]*1.0)*(pow(x,exponentes[i]));
        printf("%f\n",(coeficintes[i]*1.0)*(pow(x,exponentes[i])));
    }
    return valor;
}


/*
Busca las X para así coger el exponente
*/
void IdentificarExponentes(char funcion[52], unsigned char cantidadOperadores, int *exponentes){
    //int exponentes[cantidadOperadores+1]; //arreglo que guardara los exponentes
    unsigned char posicionExponentes=0;  //contador que ira poniendo los exponentes en el arreglo

    for(unsigned char i=0; i<cantidadOperadores+1;i++){ //llena de ceros el arreglo de exponentes
        exponentes[i]=0;
    }
    for(unsigned char i=0;i<52;i++){ //mapea el arreglo para ver en que momento encuentra una X
        if(funcion[i]=='X'){
            if(funcion[i+1]<48 || funcion[i+1]>57)
            {exponentes[posicionExponentes]=1;}
            else
            {exponentes[posicionExponentes]=SacarExponentes(funcion,i+1);}
            posicionExponentes++;
        }
    }
    
}


/*
Saca los coeficientes de los valores de
cada termino de la funcion
*/
void IdentificarCoeficientes(char funcion[52], unsigned char cantidadOperadores, int *coeficientes){
    unsigned char posicionCoeficientes=0;
   
    for(unsigned char i=0; i<cantidadOperadores+1;i++){
        coeficientes[i]=0;
    }
    for(unsigned char i=0;i<52;i++){
        if(funcion[i]>=48 && funcion[i]<=57){
            coeficientes[posicionCoeficientes]=SacarExponentes(funcion,i);
            if(funcion[i-1]=='-'){
                coeficientes[posicionCoeficientes]*=-1;    
            }
            posicionCoeficientes++;
        }else if(funcion[i]=='X'&& (funcion[i-1]=='+' || (funcion[i-1]==' ' && i-1==0))){
            coeficientes[posicionCoeficientes]=1;
            posicionCoeficientes++;
        }else if(funcion[i]=='X'&& funcion[i-1]=='-'){
            coeficientes[posicionCoeficientes]=-1;
            posicionCoeficientes++;
        }
    }
   
}


/*
Busca todos los caracteres numericos que
hay a la derecha de la X
*/
int SacarExponentes(char funcion[52], unsigned char posicion){
    unsigned char longitudExp=0;// variable que guarda la longitud del exponente
   
    while(funcion[posicion]>=48 && funcion[posicion]<=57){ //hace un ciclo para contar la long del exponente
        longitudExp++;
        posicion++;
    }
   
    unsigned char aux=longitudExp; //variable auxiliar que guarda el valor de longitudExp
   
    unsigned char exponente[longitudExp]; //se crea un arreglo de longitud longitudExp para
                                          //guardar los digitos del exponente
    posicion--;
   
    while(funcion[posicion]>=48 && funcion[posicion]<=57){
        exponente[longitudExp-1]=funcion[posicion];
        funcion[posicion]= ' ';
        longitudExp--;
        posicion--;
    }

    int valor=0;
   
    for(unsigned char i=0;i<aux;i++)
    {valor=valor+((exponente[i]-48)*pow(10,aux-i-1));}
   
    return valor;
}

/*
Función auxiliar que llama a las otras funciones
*/
int aux(int argc, char *argv[],struct NewUser *user){
    char lectura[50];
    char funcion[52]= " ";
    float intervalo[2]={0,0};
    float incremento;
    unsigned char valido=1;
    valido=LeerFuncion(argc,argv,lectura,intervalo, &incremento, user);
    if(valido)
    {Proceso(argc,argv,user, lectura,funcion,intervalo,incremento);}
    /*
    OrganizarFuncion(lectura,funcion);
    IdentificarCantidadDeTerminos(funcion, intervalo, incremento); */
    getch();
    MenuUser(argc,argv,user);
    return 0;
}

/*
Función que llama a organizarFuncion(para hacer el tratamiento del string)
y a IdentificarCantidadDeTerminos(para hacer el proceso del tratamiento 
de los datos de la función)
*/
void Proceso(int argc, char *argv[],struct NewUser *user, char lectura[50], char funcion[52], float intervalo[2], float incremento){
    OrganizarFuncion(lectura,funcion);
    IdentificarCantidadDeTerminos(argc,argv,funcion, intervalo, incremento);
}   

int main(int argc, char *argv[]){
    IngresarUsername(argc,argv); //PRINCIPAL
    /* FILE *archivo=fopen("accesos", "r");
    a(archivo);
    fclose(archivo); */ 
    return 0;
}
//225+263+731