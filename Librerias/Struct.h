#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    char content[20];
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Rect destination;
} Text;


struct NewUser{ //51
    char estado;
    char username[25];
    char password[25];
};

struct Accesos{ //104
    char estado;    //1  1
    int numero;     //4  5   
    int ano;        //4  9
    int mes;        //4  13
    int dia;        //4
    char username[25];   //25 
    char funcion[50];    //50
    float inicio;       //4
    float final;        //4
    float incremento;       //4
};