#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include <time.h>
#include "./Librerias/Struct.h"
/*
struct NewUser{
    char estado;
    char username[25];
    char password[25];
};
*/

/*Funciones main*/
unsigned char LeerFuncion(char lectura[50], float intervaloEIncremento[2], float *incremento, struct NewUser *user);
void OrganizarFuncion(char lectura[50], char funcion[52]);
void IdentificarCantidadDeTerminos(int argc, char *argv[],char funcion[52], float intervalo[2], float incremento);
void IdentificarExponentes(char funcion[52], unsigned char cantidadOperadores, int *exponentes);
int SacarExponentes(char funcion[52], unsigned char posicion);
void IdentificarCoeficientes(char funcion[52], unsigned char cantidadOperadores, int *coeficintes);
float EvaluarPunto(float x, float *rango, int *exponentes, int *coeficintes, int longitud);
int aux(int argc, char *argv[],struct NewUser *user);
void Proceso(int argc, char *argv[],struct NewUser *user, char lectura[50], char funcion[52], float intervalo[2], float incremento);


/*Funciones menu*/
void AsignarUsuario(int argc, char *argv[],unsigned char opcion, struct NewUser *user);
int IngresarUsername(int argc, char *argv[]);
void a();
int BuscarUser(struct NewUser *user, unsigned char soloBuscar, char username[25], char error,int argc, char *argv[]);
void CompararUsuario(FILE *usuarios, struct NewUser *user, unsigned char *encontrado, char username[25]);
unsigned char BuscarPassword(struct NewUser *user, int posicion, FILE *usuarios);
void MenuAdmin(int argc, char *argv[],struct NewUser *user);
void CrearUser(int argc, char *argv[],struct NewUser *user);
int ContarArchivo(FILE *usuarios);
void MenuUser(int argc, char *argv[],struct NewUser *user);
int aux(int argc, char *argv[],struct NewUser *user);
void CrearAcceso(int argc, char *argv[],struct NewUser *user, struct Accesos *acceso,float inicio, float final ,float incremento, char lectura[50]);
void LeerUsuario(int argc, char *argv[]);
void BorrarGrafico(int argc, char *argv[], struct NewUser *user);
void ListarAccesosUsuario(int argc, char *argv[],struct NewUser *user, char username[25], unsigned char root);
void EncriptarPassword(char password[25]);
void PasswordRoot(int argc, char *argv[]);
void EnmascararPassword(struct NewUser *user, char password[25], char opcion);

/*
Estructura users.txt
char campo de estado (habilitado o borrado)
char[25] username
char[25] password 
*/

int ContarArchivo(FILE *usuarios){
    int tamaño=0;
    while(!feof(usuarios)){
        fgetc(usuarios);
        tamaño++;
    }
    return tamaño;
}



int IngresarUsername(int argc, char *argv[]){
    struct NewUser user;
    user.estado=1;
    printf("Aplicativo-Graficador - UTP\n");
    printf("Username:\n");
    scanf("%s", user.username);

    FILE *usuarios=fopen("users", "r");

    if(usuarios==NULL){
        fseek(usuarios,0,SEEK_END);
        fclose(usuarios);
        if(!strcmp(user.username,"root")){
            AsignarUsuario(argc,argv,0, &user);
        }else{
            printf("Usuario incorrecto\n");
            getch();
        }
    }else{
        fseek(usuarios,0,SEEK_END);
        fclose(usuarios);
        AsignarUsuario(argc,argv,1, &user);
    }   
    return 0;
}


void AsignarUsuario(int argc, char *argv[],unsigned char opcion, struct NewUser *user){
    if(opcion){
        BuscarUser(user,0, user->username,0,argc,argv);
    }else{
        printf("Password:\n");
        //scanf("%s", user->password);
        EnmascararPassword(user, user->password,0);
        EncriptarPassword(user->password);
        if(strlen(user->password)<4){
            printf("Contraseña muy corta");
        }else{
            FILE *usuarios=fopen("users", "w");
            fwrite(user,51,1, usuarios);
            fseek(usuarios,0,SEEK_END);
            fclose(usuarios);

        }
    }
}

/* 
0 es la larga
printf("Ingrese la contraseña actual\n"); 1
printf("Ingrese la nueva contraseña\n"); 2
printf("Ingresela nuevamente\n"); 3
*/
void EnmascararPassword(struct NewUser *user, char password[25], char opcion){
    system("cls");
    char mascara[25];
    char entrada[25];
    char letra=0;
    unsigned char contador=0;
    for(unsigned char i=0;i<25;i++){
        entrada[i]='\0';
        mascara[i]='\0';
    }
    while(letra!=13){
        if(opcion==0){
            printf("Aplicativo-Graficador - UTP\n");
            printf("Username:\n");
            printf("%s\n",user->username);
            printf("Password:\n");
        }else if(opcion==1){
            printf("Ingrese la contraseña actual\n");
        }else if(opcion==2){
            printf("Ingrese la nueva contraseña\n");
        }else if(opcion==3){
            printf("Ingresela nuevamente\n");
        }else if(opcion==4){
            printf("Ingrese el password del nuevo usuario\n");
        }
    
        printf("%s",mascara);
        letra=getch();
        system("cls");
        if(letra==8){
            if(contador!=0){
                contador--;
                entrada[contador]='\0';
                mascara[contador]='\0';
            }
        }else{
            entrada[contador]=letra;
            mascara[contador]='*';
            contador++;
        }
    }
    entrada[contador-1]='\0';
    strcpy(password,entrada);   
}

int BuscarUser(struct NewUser *user, unsigned char soloBuscar, char username[25], char error,int argc, char *argv[]){
    //char error=0;
    int tamaño=0;
    FILE *usuarios=fopen("users","r");
    while(!feof(usuarios)){
        fgetc(usuarios);
        tamaño++;
    }
    unsigned char registros=tamaño/51;
    unsigned char encontrado=0;
    int posicion=0;
    unsigned char estado=1;

    for(unsigned char i=0; i<registros;i++){
        fseek(usuarios,i*51 ,SEEK_SET);
        estado=fgetc(usuarios);
        posicion=(i*51)+1;
        if(estado){
            CompararUsuario(usuarios,user,&encontrado, username);
            if(encontrado){
                i=registros;
            }
        }
    }

    if(!encontrado){
        posicion=0;
    }

    if(!soloBuscar){
        if(encontrado){
            if(BuscarPassword(user,posicion+25,usuarios)){
                //printf("contraseña correcta\n");
                system("cls");
                fseek(usuarios,0,SEEK_END);
                fclose(usuarios);
                if(!strcmp("root",user->username)){
                    MenuAdmin(argc,argv,user);
                }else{
                    MenuUser(argc,argv,user);
                }
            }else{
                fseek(usuarios,0,SEEK_END);
                fclose(usuarios);
                printf("contraseña incorrecta\n");
                error++;
                getch();
                if(error<3){
                    BuscarUser(user,soloBuscar,username,error,argc,argv);

                }
            }
        }else{
            fseek(usuarios,0,SEEK_END);
            fclose(usuarios);
            printf("usuario no existente");
        }
    }
    return posicion;
}


unsigned char BuscarPassword(struct NewUser *user, int posicion, FILE *usuarios){
    printf("Password:\n");
    EnmascararPassword(user, user->password,0);
    //scanf("%s", user->password);
    EncriptarPassword(user->password);
    fseek(usuarios, posicion,SEEK_SET);
    unsigned char igual=1;
    char letra=-1;
    printf("\n");
    for(unsigned char i=0;letra!='\0';i++){
        letra=fgetc(usuarios);
        if(letra!=user->password[i]){
            igual=0;
        }
    }
    return igual;
}

void auxListarAccesos(int argc, char *argv[],struct NewUser *user){
    char username[25];
    printf("Ingrese el nombre del usuario\n");
    scanf("%s", username);
    ListarAccesosUsuario(argc,argv,user,username,1);
    getch();
    MenuAdmin(argc,argv,user);
}

void ListarAccesosUsuario(int argc, char *argv[],struct NewUser *user, char username[25], unsigned char root){
    printf("Fecha         Nro Gráfico     Polinomio\n");
    unsigned char encontrado=0;
    unsigned char estado=0;
    int valor=0;
    char funcion[50];
    FILE *archivo=fopen("accesos", "r");
    int registros=ContarArchivo(archivo)/104;
    for(int i=0;i<registros;i++){

        encontrado=0;
        fseek(archivo,i*104,SEEK_SET);
        estado=fgetc(archivo);
        if(estado){
            fseek(archivo,(i*104)+17,SEEK_SET);
            CompararUsuario(archivo,user, &encontrado,username);
            if(encontrado){
                fseek(archivo,(i*104)+5,SEEK_SET);
                fread(&valor,4,1,archivo); //año
                printf("%i/", valor);

                fseek(archivo,(i*104)+9,SEEK_SET);
                fread(&valor,4,1,archivo); //mes
                printf("%i/", valor);

                fseek(archivo,(i*104)+13,SEEK_SET);
                fread(&valor,4,1,archivo); //dia
                printf("%i", valor);

                fseek(archivo,(i*104)+1,SEEK_SET);
                fread(&valor,4,1,archivo); //número
                printf("     %i", valor);

                fseek(archivo,(i*104)+42,SEEK_SET);
                fread(funcion,50,1,archivo);
                printf("                %s", funcion);
                printf("\n");
            }
        }
    }
    fseek(archivo,0,SEEK_END);
    fclose(archivo);
    if(!root){
        getch();
        MenuUser(argc,argv,user);
    }
}

void ListarTodosAccesos(int argc, char *argv[],struct NewUser *user){
    unsigned char estado=0;
    char username[25];
    FILE *archivo=fopen("users","r");
    int registros=ContarArchivo(archivo)/51;
    for(int i=0;i<registros-1;i++){
        fseek(archivo,(i*51)+51,SEEK_SET);
        estado=fgetc(archivo);
        if(estado){
            fseek(archivo,(i*51)+1+51,SEEK_SET);
            fread(username,25,1,archivo);
            printf("%s\n",username);
            ListarAccesosUsuario(argc,argv,user,username,1);
            printf("\n");
        }
    }
    fseek(archivo,0,SEEK_END);
    fclose(archivo);
    getch();
    MenuAdmin(argc,argv,user);
}

void CompararUsuario(FILE *usuarios, struct NewUser *user, unsigned char *encontrado, char username[25]){
    unsigned char igual=1;
    unsigned char letra=1;
    for(unsigned char i=0;letra!='\0';i++){
        letra=fgetc(usuarios);
        if(letra!=username[i]){
            igual=0;
        }
    }
    *encontrado=igual;
    
}

void CrearUser(int argc, char *argv[],struct NewUser *user){
    struct NewUser createdUser;
    createdUser.estado=1;
    printf("Ingrese el username del nuevo usuario\n");
    scanf("%s", createdUser.username);
    if(!BuscarUser(user,1,createdUser.username,0,argc,argv)){
        printf("Ingrese el password del nuevo usuario\n");
        EnmascararPassword(user,createdUser.password,4);
        //scanf("%s", createdUser.password);
        EncriptarPassword(createdUser.password);
        FILE *usuarios=fopen("users", "a");
        fwrite(&createdUser,51,1,usuarios);
        fseek(usuarios,0,SEEK_END);
        fclose(usuarios);

    }else{
        printf("El usuario ya existe");
    }
    MenuAdmin(argc,argv,user);
}

void BorrarUser(int argc, char *argv[]){
    char username[25];
    printf("Entre el usuario\n");
    scanf("%s", username);
    FILE *usuarios=fopen("users", "r+");
    fseek(usuarios,BuscarUser(usuarios,1,username,0,argc,argv)-1,SEEK_SET);
    fputc(0,usuarios);
    fseek(usuarios,0,SEEK_END);
    fclose(usuarios);
}

void BorrarGrafico(int argc, char *argv[], struct NewUser *user){
    int numero=0;
    int entrada=0;
    printf("Ingrese el número del gráfico que quiere borrar\n");
    scanf("%i", &numero);
    FILE *archivo=fopen("accesos","r+");
    int registros=ContarArchivo(archivo)/104;
    for(int i=0;i<registros;i++){
        fseek(archivo,(i*104)+1,SEEK_SET);
        fread(&entrada,4,1,archivo);
        if(entrada==numero){
            fseek(archivo,i*104,SEEK_SET);
            fputc(0,archivo);
            i=registros;
        }
    }
    fseek(archivo,0,SEEK_END);
    fclose(archivo);

    /* if(!strcmp("root",user->username)){
        MenuAdmin(argc,argv,user);
    }else{
        MenuUser(argc,argv,user);
    } */
    
}

void GraficarRef(int argc, char *argv[],struct NewUser *user){
    unsigned char encontrado=1;
    char estadoUser=1;
    char estado=0;
    char funcion[52]= " ";
    int numero=0;
    int aux=-1;
    char lectura[50];
    float intervalo[2];
    float incremento;
    char usernameGrafico[25];
    printf("Ingrese el número del gráfico\n");
    scanf("%i", &numero);
    FILE *archivo=fopen("accesos", "r");
    int registros=ContarArchivo(archivo)/104;

    fseek(archivo,104*(numero-1),SEEK_SET);
    estado=fgetc(archivo);

    char letra=-1;
    fseek(archivo,104*(numero-1)+17,SEEK_SET);
    //printf("letra en el archivo archivo accesos:");
    for(unsigned char i=0;letra!='\0';i++){
        letra=fgetc(archivo);
        //printf("%hhu %c",i, letra);
        usernameGrafico[i]=letra;
    }
    //printf("usernameGrafico: %s", usernameGrafico);
    estadoUser=BuscarUser(user,1,usernameGrafico,0,argc,argv);

    if(registros<numero){
        //encontrado=0;
        estado=0;
        estadoUser=0;
    }

    if(estado && estadoUser){
        encontrado=0;
        fseek(archivo,(104*(numero-1))+17,SEEK_SET);
        CompararUsuario(archivo,user,&encontrado,user->username);
        if(encontrado || strcmp("root",user->username)==0){
            fseek(archivo,(104*(numero-1))+1,SEEK_SET);
            fread(&aux,4,1,archivo);
            if(aux==numero){
                fseek(archivo,(104*(numero-1))+42,SEEK_SET);
                fread(lectura,50,1,archivo);
                fseek(archivo,(104*(numero-1))+92,SEEK_SET);
                fread(&intervalo[0],4,1,archivo);
                fseek(archivo,(104*(numero-1))+96,SEEK_SET);
                fread(&intervalo[1],4,1,archivo);
                fseek(archivo,(104*(numero-1))+100,SEEK_SET);
                fread(&incremento,4,1,archivo);
            }
        }
        
    }

    fseek(archivo,0,SEEK_END);
    fclose(archivo);

    if(encontrado || (!strcmp("root", user->username) && (registros>=numero))){
   
        if(estado && estadoUser /*|| (!strcmp("root", user->username) && (registros>=numero))*/){
            Proceso(argc,argv,user,lectura,funcion,intervalo,incremento);
            
        }else
        {printf("Gráfico inexistente");}
    }else{
        printf("Gráfico no correspondiente al usuario\n");
    }
    //MenuUser(user);
    getch();
    if(!strcmp("root",user->username)){
        MenuAdmin(argc,argv,user);
    }else{
        MenuUser(argc,argv,user);
    }
}

/*
Se pueden combinar CambiarPasswordRoot y CambiarPasswordUsuario
*/
void CambiarPasswordRoot(int argc, char *argv[],struct NewUser *user){
    char password[25];
    char password1[25];
    for(unsigned char i=0;i<25;i++){
        password1[i]='\0';
        password[i]='\0';
    }
    int posicion=BuscarUser(user,1,user->username,0,argc,argv);
    printf("Ingrese la contraseña actual\n");
    EnmascararPassword(user,password,1);
    //scanf("%s", password);
    EncriptarPassword(password);
    if(!strcmp(password,user->password)){
        printf("Ingrese la nueva contraseña\n");
        EnmascararPassword(user,password,2);
        //scanf("%s",password);
        printf("Ingresela nuevamente\n");
        EnmascararPassword(user,password1,3);
        //scanf("%s",password1);
        if(!strcmp(password1,password)){
            FILE *usuarios=fopen("users","r+");
            fseek(usuarios,posicion+25,SEEK_SET);
            EncriptarPassword(password);
            fwrite(password,25,1,usuarios);
            fseek(usuarios,0,SEEK_END);
            fclose(usuarios);
            printf("Contraseña modificada de manera exitosa\n");
        }else{
            printf("Error al digitar la contraseña");
            
        }

    }else{
        printf("Password incorrecto\n");
    }
    getch();
    if(!strcmp("root", user->username)){
        MenuAdmin(argc,argv,user);
    }else{
        MenuUser(argc,argv,user);
    }
}


void CambiaPasswordDeUsuario(int argc, char *argv[],struct NewUser *user){
    char username[25];
    char password[25];
    printf("Ingrese el usuario a cambiar la contraseña\n");
    scanf("%s",username);
    int posicion=BuscarUser(user,1,username,0,argc,argv);
    if(!posicion){
        printf("Usuario no existente");
    }else{
        printf("Ingrese la nueva contraseña\n");
        EnmascararPassword(user,username,2);
        printf("Ingresela nuevamente\n");
        EnmascararPassword(user,password,3);
        if(!strcmp(username,password)){
            FILE *usuarios=fopen("users","r+");
            fseek(usuarios,posicion+25,SEEK_SET);
            EncriptarPassword(password);
            fwrite(password,25,1,usuarios);
            fseek(usuarios,0,SEEK_END);
            fclose(usuarios);
            printf("Contraseña modificada de manera exitosa\n");
        }else{
            printf("Error al digitar la contraseña");
            
        }
    }
}

void CrearAcceso(int argc, char *argv[],struct NewUser *user, struct Accesos *acceso, float inicio, float final ,float incremento, char lectura[50]){
    //struct Accesos acceso;
    FILE *archivo=fopen("accesos", "a+");
    acceso->estado=1;
    acceso->numero=(ContarArchivo(archivo)/103)+1;
    acceso->inicio=inicio;
    acceso->final=final;
    acceso->incremento=incremento;
    unsigned char i=0;
    for(; lectura[i]!='\0';i++){
        acceso->funcion[i]= lectura[i];
    }
    acceso->funcion[i]='\0';
    i=0;
    for(; user->username[i]!='\0';i++){
        acceso->username[i]=user->username[i];
    }
    acceso->username[i]='\0';
    time_t t;
    t = time(NULL);
    struct tm tm = *localtime(&t);
    acceso->ano=tm.tm_year+1900;
    acceso->mes=tm.tm_mon+1;
    acceso->dia=tm.tm_mday;
    acceso->dia=5;
    fwrite(&acceso->estado,1,1,archivo);
    fwrite(&acceso->numero,4,1,archivo);
    fwrite(&acceso->ano,4,1,archivo);
    fwrite(&acceso->mes,4,1,archivo);
    fwrite(&acceso->dia,4,1,archivo);
    fwrite(acceso->username,25,1,archivo);
    fwrite(acceso->funcion,50,1,archivo);
    fwrite(&acceso->inicio,4,1,archivo);
    fwrite(&acceso->final,4,1,archivo);
    fwrite(&acceso->incremento,4,1,archivo);
    fseek(archivo,0,SEEK_END);
    fclose(archivo);

}

void LeerUsuario(int argc, char *argv[]){
    float valor;
    int valor1;
    char string[25];
    char string1[50];
    FILE *archivo=fopen("accesos","r");
    fread(&valor1,4,1,archivo);
    printf("Numero %i\n",valor1);
    fread(&valor1,4,1,archivo);
    printf("Año %i\n",valor1);
    fread(&valor1,4,1,archivo);
    printf("Mes %i\n",valor1);
    fread(&valor1,4,1,archivo);
    printf("Día %i\n",valor1);
    fread(string,25,1,archivo);
    printf("Username %s\n", string);
    fread(string1,50,1,archivo);
    printf("Función %s\n", string1);
    fread(&valor,4,1,archivo);
    printf("Inicio %f", valor);
    fread(&valor,4,1,archivo);
    printf("Final %f", valor);
    fread(&valor,4,1,archivo);
    printf("Incremento %f", valor);
}

void MenuAdmin(int argc, char *argv[],struct NewUser *user){
    system("cls");
    unsigned char opcion=0;
    printf("Aplicativo-Graficador - UTP\n");
    printf("Username:\n");
    printf("0. Salir del aplicativo\n");
    printf("1. Crear nuevo usuario\n");
    printf("2. Borrar usuario\n");
    printf("3. Listar accesos por usuario\n");
    printf("4. Listar accesos todos los usuarios\n");
    printf("5. Imprimir gráfico por referencia\n");
    printf("6. Cambiar password del usuario root\n");
    printf("7. Cambiar password de un usuario\n");
    scanf("%hhu", &opcion);
    switch (opcion)
    {
    case 0:
        return;
        break;
    case 1:
        CrearUser(argc,argv,user);
        break;
    case 2:
        BorrarUser(argc,argv);
        break;
    case 3:
        auxListarAccesos(argc,argv,user);
        break;
    case 4:
        ListarTodosAccesos(argc,argv,user);
        break;
    case 5:
        GraficarRef(argc,argv,user);
        break;
    case 6:
        CambiarPasswordRoot(argc,argv,user);
        break;
    case 7:
        CambiaPasswordDeUsuario(argc,argv,user);
        break;
    }

}

void MenuUser(int argc, char *argv[],struct NewUser *user){
    system("cls");
    unsigned char opcion=0;
    printf("Aplicativo-Graficador - UTP\n");
    printf("Username:\n");
    printf("0. Salir del aplicativo\n");
    printf("1. Crear un nuevo gráfico\n");
    printf("2. Borrar un gráfico existente\n");
    printf("3. Listar accesos por usuario\n");
    printf("4. Imprimir gráfico por referencia(si es suyo)\n");
    printf("6. Cambiar password del usuario %s\n", user->username);
    scanf("%hhu", &opcion);
    switch (opcion)
    {
    case 0:
        return;
        break;
    case 1:
        aux(argc,argv,user);
        break;
    case 2:
        BorrarGrafico(argc,argv,user);
        break;
    case 3:
        ListarAccesosUsuario(argc,argv,user,user->username,0);
        break;
    case 4:
        GraficarRef(argc,argv,user);
        break;
    case 6:
        CambiarPasswordRoot(argc,argv,user);
        break;
    }

}

/*
poner como parametro el username a buscar
*/
void EncriptarPassword(char password[25]){
    char palabra[]="murcielago";
    for(unsigned char i=0;password[i]!='\0';i++){
        password[i]+=30;
    }
    for(unsigned char i=0;password[i]!='\0';i++){
        for(unsigned char j=0;palabra[j]!='\0';j++){
            if(palabra[j]==password[i]){
                password[i]=j;
            }
        }
    }

}

void PasswordRoot(int argc, char *argv[]){
    FILE *archivo=fopen("users","r");
    fseek(archivo,26,SEEK_SET);
    char password[25];
    fread(password,25,1,archivo);
    char palabra[]="murcielago";
    for(unsigned char i=0;password[i]!='\0';i++){
        for(unsigned char j=0;palabra[j]!='\0';j++){
            if(j==password[i])
            password[i]=password[j];
        }
    }
    
    for(unsigned char i=0;password[i]!='\0';i++){
        password[i]-=30;
    }
    printf("%s",password);
    fseek(archivo,0,SEEK_END);
    fclose(archivo);
}

void a(FILE *archivo){
    char letra=0;
    int contador=1;
    while(!feof(archivo)){
        letra=fgetc(archivo);
        printf("%i. %i %c\n",contador,letra,letra);
        contador++;
    }
    fclose(archivo);
}


