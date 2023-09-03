void GraficarPunto(SDL_Renderer *renderer, int x, int y);
void GraficarEjeY(SDL_Renderer *renderer, int x);
int AveriguarPuntoEjeY(float *dominio, unsigned char longitudDom, unsigned char positivosX, unsigned char negativosX, char hayCeroX, float *escalaX);
int AbrirVentana( int argc, char *argv[], float *dominio, float *rango, unsigned char longitudDom);
void ContarPositivosNegativosXY(float *dominio, float *rango, unsigned char longitudDom, unsigned char *positivosX, unsigned char *negativosX, char *hayCeroX, unsigned char *positivosY, unsigned char *negativosY, char *hayCeroY);
void MayorMenorY(float *rango, unsigned char longitudDom, float *mayor, float *menor);
int AveriguarPuntoEjeX(float *rango, unsigned char longitudDom, unsigned char positivosY, unsigned char negativosY, char hayCeroY, float *escalaY, float *mayor, float *menor/* , int mayor, int menor */);
void GraficarNumerosDom(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, float numero, int x, int y, Text *texto);
