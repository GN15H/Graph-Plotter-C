#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Librerias/Struct.h"


const int WIDTH = 820, HEIGHT = 820;


void GraficarPunto(SDL_Renderer *renderer, int x, int y);
void GraficarEjeY(SDL_Renderer *renderer, int x);
int AveriguarPuntoEjeY(float *dominio, unsigned char longitudDom, unsigned char positivosX, unsigned char negativosX, char hayCeroX, float *escalaX);
int AbrirVentana( int argc, char *argv[], float *dominio, float *rango, unsigned char longitudDom);
void ContarPositivosNegativosXY(float *dominio, float *rango, unsigned char longitudDom, unsigned char *positivosX, unsigned char *negativosX, char *hayCeroX, unsigned char *positivosY, unsigned char *negativosY, char *hayCeroY);
void MayorMenorY(float *rango, unsigned char longitudDom, float *mayor, float *menor);
int AveriguarPuntoEjeX(float *rango, unsigned char longitudDom, unsigned char positivosY, unsigned char negativosY, char hayCeroY, float *escalaY, float *mayor, float *menor/* , int mayor, int menor */);
void GraficarNumerosDom(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, float numero, int x, int y, Text *texto);


/*
Función que en el  x,y dado
genera una X
*/
void GraficarPunto(SDL_Renderer *renderer, int x, int y){
    SDL_SetRenderDrawColor(renderer, 255,255,255,SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer,x-5,y-5,x+5,y+5);
    SDL_RenderDrawLine(renderer,x-5,y+5,x+5,y-5);
}

/*
Función que grafica el eje y según la 
posición dada
*/
void GraficarEjeY(SDL_Renderer *renderer, int x){
    SDL_SetRenderDrawColor(renderer, 255,255,255,SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer,x,0,x,HEIGHT);
}

/*
Grafíca el eje x según la posición 
dada
*/
void GraficarEjeX(SDL_Renderer *renderer, int y){
    SDL_SetRenderDrawColor(renderer, 255,255,255,SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer,0,y,WIDTH,y);
}

/*
Esta función cuenta la cantidad de numeros positivos y negativos
en el dominio y el rango
*/
void ContarPositivosNegativosXY(float *dominio, float *rango, unsigned char longitudDom, unsigned char *positivosX, unsigned char *negativosX, char *hayCeroX, unsigned char *positivosY, unsigned char *negativosY, char *hayCeroY){
    for(unsigned char i=0; i<longitudDom;i++){
        if(dominio[i]==0)
        {*hayCeroX=i;}
        else if(dominio[i]>0)
        {*positivosX+=1;}
        else
        {*negativosX+=1;}
    
        if(rango[i]==0)
        {*hayCeroY=i;}
        else if(rango[i]>0)
        {*positivosY+=1;}
        else
        {*negativosY+=1;}
    }
}

/*
Esta función retorna el punto en el que
se grafica el eje y
*/
int AveriguarPuntoEjeY(float *dominio, unsigned char longitudDom, unsigned char positivosX, unsigned char negativosX, char hayCeroX, float *escalaX){
    *escalaX=(WIDTH-20)/(longitudDom-1);
    int punto=0;

    if(hayCeroX !=-1)
    {punto=(*escalaX*hayCeroX)+10;}
    else
    {punto=(*escalaX*(negativosX-1))+(*escalaX/2)+10;}

    return punto;
}

/*
Esta función retorna el punto en el que
se grafica el eje x
*/
int AveriguarPuntoEjeX(float *rango, unsigned char longitudDom, unsigned char positivosY, unsigned char negativosY, char hayCeroY, float *escalaY, float *mayor, float *menor/* , int mayor, int menor */){
    *mayor=rango[0];
    *menor=rango[0];
    
    MayorMenorY(rango,longitudDom,mayor,menor);
    *escalaY=(HEIGHT-20.0)/(*mayor-*menor);
    int punto=0;
    punto=(*escalaY*(*mayor))+10;
    return punto;
}

/*
Esta función grafica las X de cada punto
escala el eje X y el eje Y
une los puntos entre cada X para generar la forma de la grafica
*/
void GraficarPuntos(SDL_Renderer *renderer,TTF_Font *font, SDL_Color color, Text *textoDom, Text *textoRango, float *dominio, float *rango, unsigned char longitudDom, float escalaX, float escalaY, float mayor, float menor, int x, int y){
    for(unsigned char i=0;i<longitudDom;i++){
        GraficarPunto(renderer,((escalaX*i)+10),HEIGHT-((escalaY*(rango[i]-menor))+10));
        
        SDL_RenderDrawLine(renderer,((escalaX*i)+10),y-5,((escalaX*i)+10),y+5);
        SDL_RenderDrawLine(renderer,x-5,HEIGHT-((escalaY*(rango[i]-menor))+10),x+5, HEIGHT-((escalaY*(rango[i]-menor))+10));
        GraficarNumerosDom(renderer,font,color,dominio[i],((escalaX*i)+10),y+15,&textoDom[0]);
        GraficarNumerosDom(renderer,font,color,rango[i],x+15,HEIGHT-((escalaY*(rango[i]-menor))+10),&textoRango[0]);
        if(i<longitudDom-1){
            SDL_RenderDrawLine(renderer,((escalaX*i)+10),
                                HEIGHT-((escalaY*(rango[i]-menor))+10),
                                ((escalaX*(i+1))+10),
                                HEIGHT-((escalaY*(rango[i+1]-menor))+10));
            
        }
    }

}


void Escalar(SDL_Renderer *renderer, int x, int y, char orientacion){
    if(orientacion){
        SDL_RenderDrawLine(renderer,x,y-5,x,y+5);
    }else{
        SDL_RenderDrawLine(renderer,x-5,y,x+5,y);
    }
}

/*
Esta función maneja todos  los datos de la función
y llama al resto de funciones
*/
void GraficarFuncion(SDL_Renderer *renderer,TTF_Font *font, SDL_Color color, Text *textoDom, Text *textoRango,
                     float *dominio, float *rango, unsigned char longitudDom){
    unsigned char positivosX=0;  //variables del dominio
    unsigned char negativosX=0;
    char hayCeroX=-1;

    unsigned char positivosY=0;  // variables del rango
    unsigned char negativosY=0;
    char hayCeroY=-1;

    int x=0;  //posicion del eje x
    int y=0;  //posicion del eje y  

    float escalaX=0;
    float escalaY=0;

    float mayor=rango[0];
    float menor=rango[0];

    ContarPositivosNegativosXY(dominio,rango,longitudDom, &positivosX, &negativosX, &hayCeroX, &positivosY,&negativosY, &hayCeroY);
    x=AveriguarPuntoEjeY(dominio,longitudDom,positivosX,negativosX,hayCeroX,&escalaX);
    y=AveriguarPuntoEjeX(rango,longitudDom,positivosY,negativosX,hayCeroY,&escalaY, &mayor ,&menor);
    if(positivosX || hayCeroX!=-1){
        GraficarEjeY(renderer,x);
    }
    if(positivosY || hayCeroY!=-1){
        GraficarEjeX(renderer,y);
    }
    GraficarPuntos(renderer,font,color,textoDom, textoRango ,dominio,rango,longitudDom,escalaX,escalaY,mayor,menor,x,y);

}


/*
Averigua cual es el mayor y menor numero en el rango
*/
void MayorMenorY(float *rango, unsigned char longitudDom, float *mayor, float *menor){
    for(unsigned char i=0;i<longitudDom;i++){
        if(rango[i]>*mayor){
            *mayor=rango[i];
        }
        if(rango[i]<*menor){
            *menor=rango[i];
        }
    }
}


/*
Renderiza los valores del dominio y el rango
en la escala de los ejes
*/
void GraficarNumerosDom
(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, 
float numero, int x, int y, Text *texto){
    char numeroString[25];
    int numeroI = (int) numero;
    sprintf(numeroString,"%d",numeroI);
    strcpy(texto->content,numeroString);
    texto->surface= TTF_RenderText_Solid(font, texto->content,color);
    texto->texture = SDL_CreateTextureFromSurface(renderer, texto->surface);
    texto->destination.x = x;
    texto->destination.y = y;
    texto->destination.w = texto->surface->w;
    texto->destination.h = texto->surface->h;
    SDL_RenderCopy(renderer, texto->texture, NULL, &texto->destination);
    
}

/*
Esta función abre la ventana del gráfico e inicializa todas las variables
necesarias para graficar
*/
int AbrirVentana( int argc, char *argv[], float *dominio, float *rango, unsigned char longitudDom){
 

    SDL_Init( SDL_INIT_EVERYTHING);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow("Hello SDL WORLD", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED); 

    if( NULL == window){
        printf("nonas pa");
        return 1;
    }else{
        printf(" ");
    }
    
    TTF_Font* font = TTF_OpenFont("fonts/arial.ttf", 12);
    SDL_Color color = {255, 255, 255};
   
    unsigned char gameIsRunning=1;

    Text textoDom[longitudDom];
    Text textoRango[longitudDom];
    
    while (gameIsRunning){

        SDL_Event windowEvent;
        while(SDL_PollEvent(&windowEvent)){
        
            if (SDL_QUIT == windowEvent.type){
                gameIsRunning=0;
            }
        }

        SDL_SetRenderDrawColor(renderer,0,0,0,SDL_ALPHA_OPAQUE);        
        SDL_SetRenderDrawColor(renderer,255,255,255,SDL_ALPHA_OPAQUE);
        GraficarFuncion(renderer,font,color,textoDom,textoRango,dominio,rango,longitudDom);
        SDL_RenderPresent(renderer);
    }
    /*
    for (int i = 0; i < longitudDom; i++) {
        SDL_DestroyTexture(textoDom[i].texture);
        SDL_FreeSurface(textoDom[i].surface);
        //SDL_DestroyTexture(textoRango[i].texture);
        //SDL_FreeSurface(textoRango[i].surface);
    }
    */
    SDL_DestroyTexture(textoDom[0].texture);
    SDL_FreeSurface(textoDom[0].surface);

    TTF_CloseFont(font);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}