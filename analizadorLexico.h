#include <stdio.h>
#include <string.h>
#include "infijaPostfija.h"
/*CONSTANTES*/
#define TAM_TOKEN 20//UN TOKEN MAXIMO PUEDE tener una longitud de 20
#define MAX_TOKEN 50 //POR CADA EXPRESION PUEDE HABER COMO MAXIMO 50 TOKENS
#define underLine "===================================================================\n\n"
#define HEADER "==============CALCULADORA DE EXPRESIONES ARITMETICAS===============\n"
FILE *archi; //CODIGO FUENTE
int comment;//CANTIDAD DE COMENTARIOS
int cant_error = 0;

void errorLexico(char *cadena, int linea)
{
        printf("ERROR LEXICO en la Linea %d: %s\n", linea, cadena);
        printf(underLine);
}
int analisisLexico()
{
    printf(HEADER);
    printf(underLine);
    char mensaje[81];
    int dato=0;
    char token[TAM_TOKEN]; //token
    int contador_tam_token=0;//contador de tamanho del token
    int i=0;//indice del token
    int row=0;//cantidad de columnas
    int error=0;//representa una variable booleana..hay error 1, no hay error 0
    int fila=0;//CANTIDAD DE TOKENS POR EXPRESION
    int linea=1;//Numero DE LINEA
    char grupo_token[MAX_TOKEN][TAM_TOKEN];/*TABLA DE SIMBOLOS QUE CONTIENE LOS TOKENS*/
        /*MIENTRAS NO SE FIN DE ARCHIVO*/
        while((dato=fgetc(archi))!=EOF){
            int comment=0;
            error=0;
            i=0;
            contador_tam_token=1;
            /*PARA COMENTARIOS*/
            if(dato=='#')
                {
                    while(dato!='\n' && dato!=EOF){
                            dato=fgetc(archi);
                        }
                    strcpy(token, "");
                    fila=0;
                    
                    /*PARA ESTA LINEA COMENTADA NO SE CUENTA LA LINEA*/
                }
            /*SI HAY ESPACIO SE CONTINUA*/
            else if(dato==' '||dato=='\t'){
                  continue;        
            }
            /*SI ES FIN DE LINEA SE ANALIZA LA EXPRESION*/
            else if(dato=='\n'){            
                    if(fila==0)
                    {
                        continue;
                    }
                    else{
                     analizar(linea, grupo_token, fila, error);
                     fila=0;
                     
                     linea++;
                     continue;
                     }
            }
            /*SI EL DATO ES NUMERICO
              SE EVALUA EL AUTOMATA*/
            else if (isdigit(dato)){
                     int estado=0;
                     int aceptacion=0;
                     token[i]=dato;
                
                     while(!aceptacion)	{
                          if (contador_tam_token++<=TAM_TOKEN){
                                switch(estado){
                                /*LUEGO DEL DIGITO VIENE OTRO DIGITO*/
                                     case 0: 
                                             dato=fgetc(archi);
                                             if (isdigit(dato)){
                                             token[++i]=dato;
                                             estado=0;
                                             }
                                             else if(dato=='.'){
                                             token[++i]=dato;
                                             estado=1;
                                             }
                                             else if(toupper(dato)=='E'){
                                             token[++i]=dato;
                                             estado=3;
                                             }
                                             else{
                                             estado=6;
                                             }
                                             break;
                                     /*LUEGO DEL DIGITO PUEDE VENIR UN .*/
                                      case 1:
                                             dato=fgetc(archi);
                                             if (isdigit(dato)){
                                             token[++i]=dato;
                                                       estado=2;
                                                   }
                                                   else{
                                                       estado=-1;
                                                   }
                                                   break;
                                      /*LA PARTE FRACCIONARIA*/
                                      case 2:
                                                   dato=fgetc(archi);
                                                   if (isdigit(dato)){
                                                       token[++i]=dato;
                                                       estado=2;
                                                   }
                                                   else if(toupper(dato)=='E'){
                                                       token[++i]=dato;
                                                       estado=3;
                                                   }
                                                   else
                                                       estado=6;
                                                   break;
                                      /*LA PARTE DE EXPONENTE E, DEBE VENIR UN + O - O UN NUMERO*/
                                       case 3:
                                                   dato=fgetc(archi);
                                                   if (dato=='+' || dato=='-'){
                                                       token[++i]=dato;
                                                       estado=4;
                                                   }
                                                   else if(isdigit(dato)){
                                                       token[++i]=dato;
                                                       estado=5;
                                                   }
                                                   else{
                                                       estado=-1;
                                                   }
                                                   break;
                                       /*LUEGO DEL + O - DEBE VENIR UN NUMERO*/
                                        case 4:
                                                  dato=fgetc(archi);
                                                  if (isdigit(dato)){
                                                      token[++i]=dato;
                                                      estado=5;
                                                   }
                                                  else{
                                                      estado=-1;
                                                      ungetc(dato,archi);
                                                  }
                                                  break;
                                        /*SECUENCIA DE NUMEROS DE LA PARTE EXPONENCIAL*/
                                        case 5:
                                                  dato=fgetc(archi);
                                                  if (isdigit(dato)){
                                                     token[++i]=dato;
                                                     estado=5;
                                                  }
                                                  else
                                                     estado=6;
                                                  break;
                                        /*ESTADO DE ACEPTACION xD*/
                                        case 6:
                                                 if (dato!=EOF)
                                                     ungetc(dato,archi);
                                                 aceptacion=1;
                                                 break;
                                        /*ESTADO DE ERROR*/
                                        case -1:
                                                 token[++i]=dato;
                                                 token[++i]='\0';
                                                 if (dato==EOF){
                                                        errorLexico("No se esperaba el fin de archivo!",linea);

                                                 }
                                                 else
                                                     errorLexico("No se esperaba caracter",linea);
                                                 error=1;
                                                 aceptacion=1;
                                         }//fin switch
                           }//end if (contador_tam_token++)
                           else{
                               error=1;
                               errorLexico("numero demasiado grande ",linea);
                               break;
                           }
                     }//fin while(!aceptacion)

                  }//end if (isdigit(dato))
                  
                  /*SI ES UN OPERADOR + - */
                 else if(esEstoOperador(dato)){           
                          token[i]=dato; 
                 }
                 /*SI ES PARENTESIS*/
                 else if(esParentesis(dato))
                 {
                        token[i]=dato;
                 }
                 else {
                        char cadNoValida[100];
                        int n=0;
                        while(dato!='\n' && dato!=EOF && dato!=' ' && dato!='\t' ){
                               cadNoValida[n++]=dato;
                               dato=fgetc(archi);
                               }
                        cadNoValida[n]='\0';
                        error=1;
                        if (n>0)
                               sprintf(mensaje,"No se esperaba '%s'", cadNoValida);
                               errorLexico(mensaje, linea);
                               
                 }   
                if(error==0){
                /*SI NO HUBO ERRORES SE GUARDA EL TOKEN EN EL GRUPO DE TOKENS*/
                    token[++i]='\0';
                    strcpy(grupo_token[fila++],token);
                }
                else{ 
                /*SI HUBO ALGUN ERROR SE LEE EL RESTO IGNORANDOLO*/                   
                     while(dato!='\n' && dato!=EOF){
                          dato=fgetc(archi);                             
                     }
                     linea++;
                     fila=0;
                }
                
         }                
         /*AL FINALIZAR EL ANALISIS LEXICO SE ANALIZARA EN FORMA SINTACTICA Y SE RESOLVERA LA EXPRESION*/
        analizar(linea, grupo_token,fila,error);
               
      return 0;  
}


/*FUNCION PARA SABER SI EL DATO ES UN OPERADOR*/
int esEstoOperador(letra)
{
    if(letra=='*' || letra == '/' || letra == '+' || letra == '-')
        return 1;
    else
        return 0;
}

/*FUNCION PARA SABER SI EL DATO ES PARENTESIS*/
int esParentesis(letra)
{
    if(letra=='(' || letra==')')
        return 1;
    else
        return 0;
}




int analizar(int linea, char grupo_token[MAX_TOKEN][TAM_TOKEN],int fila,int error){
/*FILA MAYOR A CERO PORQUE TIENE QUE EXISTIR EL TOKEN
  Y ERROR == 0 PORQUE PUDO NO HABER ERRORES Y QUE NO EXISTA UN TOKEN*/
     
     
    char expresion[100];
    strcpy(expresion,"");
    int k;
    for(k=0; k<fila; k++)
    {
        strcat(expresion,grupo_token[k]);
    }
     
     
     if(fila>0 && error==0){
          if(cant_error==0)
          {
              
              printf("Linea %d: --> ",linea);
              int x;
              for(x=0;x<fila;x++)
                 printf("%s",grupo_token[x]);
              printf("\n");
              obtenerResultado(expresion);
              printf(underLine);
          }
             
     }
  return 0;
}
