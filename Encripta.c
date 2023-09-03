#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// PhD Ricardo Moreno Laverde Fecha creación 2017.Abr.09. Fecha de modificacion: 2021.Abr.23
// Este programa recibe un archivo de texto "original.txt" y lo muestra en pantalla, lo encripta con un desplazamiento por código ASCII
// y después lo encripta por una palabra clave(que no tiene caracteres repetidos).


//prototipo de funciones a utilizar
//void mostrartodo(); //muestra todo lo almacenado en archivo en disco "original.txt"
//void encriptadesplaza(); // Toma el archivo "original.txt" y genera siempre un nuevo archivo "F_desplazado"
//void encriptapalabra(); // Toma el archivo "F_desplazado" y genera siempre un nuevo archivo "F_palabraclave"
//int menu(); //muestra el menu principal
//void clear(); //limpia la pantalla, funciona para windows, linux, MAC-OS

void mostrartodo() {
  char valor=0;                       //variable que guardara el char que lea del archivo "original.txt"
  FILE *archivo=NULL;                 //crea un puntero de archivo que recibirá la direccion física donde inicia el archivo "original.txt"
  archivo=fopen("original.txt", "r"); // se pasa la ruta del archivo y el modo de apertura "r" indica lectura - (r)ead
  if(archivo==NULL)                   //Por si hay problema con la apertura del archivo o no existe en la carpeta donde se esta ejecutando el programa
    { printf("\nError en la apertura del archivo...\n");
      printf("\n\n\n\tPresione enter para regresar al menu principal...\n");
      getchar(); //esta función captura un char, desde el teclado sin "eco", para nuestra necesidad solo sirve para detener el proceso mientras presionamos cualquier tecla
    }
    else
    { printf("\n\tContenido visual del archivo original.txt \n");
      printf("---------------------------------------------------------------------------------------------------------------\n");
      while(!feof(archivo))
        {
        valor=fgetc(archivo);
        printf("%c",valor);//Imprimo en formato tipo char, cada carácter leído del archivo "original.txt"
        }//fin while
      printf("\n---------------------------------------------------------------------------------------------------------------\n");
      printf("\n\n\n\tPresione enter para regresar al menu principal...\n");getchar();
     }//fin if
   fclose(archivo);
   getchar();// esta función captura un char, desde el teclado sin "eco", para nuestra necesidad solo sirve para detener el proceso mientras presionamos cualquier tecla
   return;   // Un solo return - una sola salida de la función
}            // fin funcion mostrartodo()

void encriptadesplaza()
{ int vd=0;     //vd=valor a desplazar
  char valor=0; //guarda el caracter leido del archivo "original.txt"
  FILE *archivo=fopen("original.txt","r");
  FILE *desplazado=fopen("F_desplazado","w"); //lo crea nuevo aunque ya exista
  if(archivo==NULL||desplazado==NULL) //evalua si el archivo existe y/o no tiene problemas en la apertura en modo "r" en archivo "original.txt" OR el archivo "F_desplazado" al crearlo
    { printf("\n Al abrir original.txt o crear el archivo F_desplazado, se presentaron problemas ");
      getchar(); //detiene el programa esperando que se presione cualquier tecla. Usado para dar tiempo de leer el mensaje
    }
  else
    { printf("\nEntre valor a desplazar: ");
      scanf("%i",&vd);
      while(! feof(archivo) )
           {
           valor=fgetc(archivo);   //Leo un(1) carácter del archivo "original.txt" y lo asigno a la variable "valor"
           valor=valor+vd;         //Le sumo el valor a desplazar ya leído desde el teclado al "valor" leído del archivo "original.txt"
           fputc(valor,desplazado);//Grabo el carácter desplazado en el archivo "F_desplazado"
           }//fin while
    } //fin if
  fclose(archivo);
  fclose(desplazado);
  printf("\n---------------------------------------------------------------------------------------------------------------\n");
  printf("\n\n\n\tPresione enter para regresar al menu principal...\n");
  getchar();         //detiene el programa esperando que se presione cualquier tecla. Usado para dar tiempo de leer el mensaje
  return;            //Un solo return - una sola salida de la función
}                    //fin funcion encriptadesplaza()

void encriptapalabra()
{ char p_clave[256]="MmUuRrCcIiEeLlAaGgOo";//Entro directamente la palabra clave
  int esta=0; //bandera para indicar que tiene un caracter repetido
  int i=0,j=0,nbyte=0; //nbyte cuenta el numero de bytes leidos
  // verifico que la palabra no tenga caracteres repetidos
  /*

     do{ printf("\nEntre palabra clave: ");
      scanf("%s",p_clave); //en scanf no necesita el operador & por ser un arreglo (string)
      esta=0;
      for(i=0;p_clave[i]!='\0';i=i+1) //recorro toda la cadena p_clave caracter a caracter y comparo este con el
         for(j=i+1;p_clave[j]!='\0';j=j+1) //siguiente caracter hasta el final de la misma cadena p_clave
            if (p_clave[i]==p_clave[j])
               { printf(" Palabra con caracter repetido");
                 getchar();
                 esta=1; //bandera en alto indica que hay carácter repetido dentro de p_clave
               }
    }while(esta == 1); //ciclo que se repite si tiene algún carácter repetido
   */
   char valor=0;            //guarda el caracter leido del archivo "F_desplazado"
   FILE *desplazado=NULL;   //puntero para archivo "F_desplazado"
   desplazado=fopen("F_desplazado","r"); //lo abre en modo lectura
   FILE *apc=NULL; //crea un puntero de archivo para el archivo "F_palabraclave"
   apc=fopen("F_palabraclave", "w");  //Genera un nuevo archivo aunque exista
   if(apc==NULL||desplazado==NULL)    //evalua si los archivos existen y/o no tiene problemas de apertura
     { printf("\n Al abrir algun archivo se presentaron problemas ");
       getchar(); //detiene el programa esperando que se presione cualquier tecla. Usado para dar tiempo de leer el mensaje
     }
   else
     { while(!feof(desplazado))
            { valor=fgetc(desplazado); //leo un caracter del archivo "F_desplazado"
              nbyte=nbyte+1; //contando el numero de bytes leidos
              esta=0; //variable tipo bandera(flag)para indicar si el caracter extraído del archivo "F_desplazado" esta en el string  "p_clave"
              j=0;    //variable que guardara la posición (subíndice)  de "p_clave" si esta el caracter  leído de "F_desplazado"
              for(i=0;p_clave[i]!='\0'&& esta==0;i=i+1)
                 {if(p_clave[i]==valor)
                     { esta=1; //bandera en alto. Detiene la busqueda
                       j=i; //guarda posicion  (subíndice)  de "p_clave" si esta el caracter  leído de "F_desplazado"
                     }//fin if (p_clave[i]==valor)
                 }//fin for(i=0;p_clave[i]!='\0'&& esta==0;i=i+1)
              if(esta==1) //Si la bandera se puso en alto(cambio su valor igual a cero(0) al valor igual a uno(1)
                     fputc(j,apc);     //Grabo en "F_palabraclave" la posición(subíndice)  de "p_clave" del caracter  leído de "F_desplazado"  
              else
                     fputc(valor,apc); //Grabo en "F_palabraclave" el caracter leído de "F_desplazado"  
              //fin if(esta==1)
             }//fin while (!feof(desplazado)
      printf("\nTamaño archivo desplazado y encriptado x PC %li bytes",ftell(apc));
      fclose(apc); fclose(desplazado);
      printf("\n---------------------------------------------------------------------------------------------------------------\n");
    }//fin if(apc==NULL||desplazado==NULL)
    printf("\n\n\n\tPresione enter para regresar al menu principal...\n");getchar();
    return;    //Un solo return - una sola salida de la función
}              //fin funcion encriptapalabra()

/* int menu()
{
  int opcion=0; //opcion=variable para escoger opciones del menú
  printf("|===================================|\n");
  printf("| MENU GENERAL                      |\n");
  printf("|===================================|\n");
  printf("| 0. Salir del programa             |\n");
  printf("| 1. Visualizar archivo original    |\n");
  printf("| 2. Encripta desplaza              |\n");
  printf("| 3. Encripta p.Clave               |\n");
  printf("|===================================|\n");
  printf("Selecione la opcion que desea ejecutar: ");
  scanf("%i", &opcion);
  while(opcion<0||opcion>3) //Valido que entre un valor correcto entre [0 y 3]
       { printf("\n Recuerda que solo se aceptan opciones numericas entre 0 y 3\n");
         printf("Selecione la opcion que desea ejecutar: ");
         scanf("%i", &opcion);
       } //fin while(opcion<0||opcion>3)

  switch(opcion)
        {
         case 1:
                mostrartodo();
                break;              //Esta expresión solo se permite en este contexto de la estructura "switch
         case 2:
                encriptadesplaza();
                break;              //Esta expresión solo se permite en este contexto de la estructura "switch
         case 3:
                encriptapalabra();
                break;              //Esta expresión solo se permite en este contexto de la estructura "switch
         } // Fin del SWITCH
  return opcion; //Si la opcion=0 saldrá del programa
} // Fin funcion menu()
 */
void clear()
{
   if(system("cls"))   //Borra pantalla para S.O MS-Windows
     system("clear");  //Borra pantalla para S.O linux y/o MAC-OS
} // fin función clear()

/*
int main()
{ int salir=1;         //variable utilizada para salir del menu y terminar el programa
  while(salir != 0)
       { clear();
         salir=menu();
       }
  return 0;
} // fin función main
*/

