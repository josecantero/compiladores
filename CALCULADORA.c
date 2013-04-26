#include <stdio.h>
#include <string.h>
#include "analizadorLexico.h"

void endApp();
void endApp(){//System("pause"); antes del fin del programa
     char exit[1];
     exit[0] = getchar();
}


/*EVALUADOR DEL FUENTE*/
int main(int argc, char* agrs[])
{
char linea[100];
    if(argc>1)
    {
        /************CON ESTE CÓDIGO NO ME FUNCIONO EN WINDOWS XP COMPILADO CON DEV - C++ 4.9.9.2*/
        if(!(archi=fopen(agrs[1], "r")))
        {
                printf("Error al cargar el archivo!!!");
                getch();
                exit(-1);
        }
        else
        {
                analisisLexico();
        }
    }
    else
    {
        printf("debe pasar como parametro el path del archivo fuente!!!");
    }
    
    endApp();
    
    //CODIGO CON EL QUE ME FUNCIONO LA APLICACIÓN EN WINDOWS XP DE AQUÍ EN ADELANTE-->
    /*
        if(!(archi=fopen("fuente.txt", "r")))
        {
                printf("Error al cargar el archivo!!!");
                getch();
                exit(-1);
        }
        else
        {
                analisisLexico();
        
        }
        endApp();
        */
    
}
