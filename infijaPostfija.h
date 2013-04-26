#include <stdio.h>
#include <stdlib.h>
#define TAM_TOKEN 20//UN TOKEN MAXIMO PUEDE TENER UNA LONGITUD DE 20
#define MAX_TOKEN 50 //POR CADA EXPRESION PUEDE HABER COMO MAXIMO 50 TOKENS

/*PILAS QUE CONTENDRAN LOS OPERADORES Y OPERANDOS PARA TRANSFORMARLOS A POSTFIJA*/
char operadores[MAX_TOKEN][TAM_TOKEN];
char postfija[MAX_TOKEN][TAM_TOKEN];

/*TAM_POST CONTENDRA EL TAMANHO DE LA PILA POSTFIJA*/
int tam_post;
int tamanho;
int errores=0;

/*FUNCION QUE INICIALIZA LAS PILAS GLOBALES*/
void inicializar()
{
    int x;
    for(x=0; x<MAX_TOKEN; x++)
    {
        strcpy(operadores[x], "");
        strcpy(postfija[x], "");
    }
}

/*FUNCION QUE TRANSFORMA LA EXPRESION INFIJA A POSTFIJA*/
void obtenerResultado(char *expresion){
int x;
   
    int hay_negativo=0;
    char anterior;
    int tam_op=0;
    tam_post=0;
    inicializar();
    int parentesis_abierto=0;
    int hubo_parentesis=0;
    
    for(x=0;x<strlen(expresion);x++){
        /*SI ENCUENTRA UN '(' GUARDA DIRECTAMENTE EN LA PILA OPERADORES*/
        if(expresion[x]=='(')
        {
        
               operadores[tam_op++][0]= expresion[x];
               operadores[tam_op-1][1]= '\0';
               parentesis_abierto=1;
        }
        
       else if(expresion[x]==')'){
                int k;
                
                for(k=tam_op-1; k>=0;k--)
                {
                    /*SI ES IGUAL A '(' SE DESAPILA PERO NO SE GUARDA EN LA PILA POSTFIJA*/
                    if(operadores[k][0]=='(')
                    {
                        
                        k=0;
                        tam_op--;
                        parentesis_abierto=0;
                        hubo_parentesis=1;
                    }
                    
                    else
                    {
                        /*CUANDO SEA DIGITO U OPERADOR GUARDA EN LA PILA POSTFIJA*/        
                        strcpy(postfija[tam_post++],operadores[k]);
                        tam_op--;
                        
                    }
                }
        }
        
        /*SI ES UN OPERADOR*/
        else if(esOperador(expresion[x]))
        {
                if((x==0 || expresion[x-1]=='(' || esOperador(expresion[x-1])==1) &&(expresion[x]=='-'))
                {
                   /*SI HAY UN NUMERO NEGATIVO PARA NO CONSIDERARLO COMO RESTA SINO COMO NUMERO NEGATIVO*/
                   hay_negativo=1;     
                }
                if(tam_op==0 && hay_negativo==0)
                {
                    /*SI NO HAY NEGATIVO...ES UN OPERADOR
                    SI TAM_OP ES IGUAL A CERO SE GUARDA DIRECTAMENTE EN LA PILA*/
                    operadores[tam_op++][0]= expresion[x];
                    operadores[tam_op-1][1]= '\0';
                }
                else
                {
                    /*SI HUBO PARENTESIS SE GUARDA EL OPERADOR ANTERIOR EN LA VARIABLE ANTERIOR*/
                    if(hubo_parentesis>0)
                    {
                            anterior=operadores[tam_op-1][0];
                    }
                    
                    /*SI ES DE IGUAL O MENOR PRECEDENCIA QUE EL ANTERIOR*/
                    if(esMenorPresedencia(operadores[tam_op-1], expresion[x])==1)
                    {
                          /*SI NO ESTA ABIERTO EL PARENTESIS Y ANTERIOR ES DISTINTO DE '\0'*/
                          if(parentesis_abierto==0 && anterior!='\0')
                          {
                            /*EL OPERADOR ANTERIOR SE GUARDARA EN LA PILA POSTFIJA*/
                            postfija[tam_post++][0]= anterior;
                            postfija[tam_post-1][1]= '\0';
                           
                            tam_op--;
                            }
                    }
                    /*SI NO ES UN NUMERO NEGATIVO SE GUARDA EN LA PILA OPERADORES*/
                    if(hay_negativo==0){
                    operadores[tam_op++][0]= expresion[x];
                    operadores[tam_op-1][1]= '\0';
                    }
                    
                }
              /*AL FINAL EL OPERADOR ACTUAL SE GUARDA COMO ANTERIOR*/
              anterior=expresion[x];  
              
              
        }
        /*SI ES UNA 'E' ES PARTE DE UN NUMERO*/
        else if(toupper(expresion[x])=='E')
        {        
                int i=0;
                x++;
                /*PUEDE VENIR UN NUMERO NEGATIVO O POSITIVO...SI ES POSITIVO SE IGNORA*/
                if(expresion[x]=='-')
                {/*SI ES NEGATIVO SE GUARDA COMO PARTE DE UN NUMERO*/
                         postfija[tam_post][0]= '-';
                         i=1;
                         x++;
                }
                else if (expresion[x]=='+')
                {
                         /*SINO I=0..i ES EL INDICE DEL NUEVO NUMERO*/
                         i=0;  
                         x++;    
                }
                
                /*SI ES UN NUMERO SE SIGUE GUARDANDO*/
                if(isdigit(expresion[x])){
                
                while(expresion[x]!=')' && expresion[x]!='\0' && esOperador(expresion[x])!=1)
                {
                       postfija[tam_post][i]= expresion[x];
                      
                       x++;
                       i++;
                }
                
                if(i>0){
                postfija[tam_post][x]= '\0';
                x--;
                tam_post++;
                }
                }
                /*PORQUE YA SE PASO EN LA EXPRESION LA 'E' AHORA SE GUARDA*/
                strcpy(postfija[tam_post++], "E");
        }
        else
        {
                /*SINO ES UN NUMERO*/
                if(x==strlen(expresion)-1)
                {
                       postfija[tam_post++][0]= expresion[x];
                       postfija[tam_post-1][1]= '\0';
                }
                else
                {
                int i=0;
                if(hay_negativo==1)
                {
                    hay_negativo=0;
                    i=1;
                    postfija[tam_post][0]='-';
                }
                while((isdigit(expresion[x]) || expresion[x]=='.') && expresion[x]!=')' && expresion[x]!='\0' && toupper(expresion[x])!='E' && esOperador(expresion[x])!=1)
                {
                       postfija[tam_post][i]= expresion[x];
                      
                       x++;
                       i++;
                }
                
                if(i>0){
                postfija[tam_post][x]= '\0';
                x--;
                tam_post++;
                }
             }
        }
        
     
        
    }
    
    /*SI QUEDA ALGO EN LA PILA DE OPERADORES*/
    for(x=tam_op-1; x>=0;x--)
    {
        
        strcpy(postfija[tam_post++], operadores[x]);
        tam_op--;
    }
    
    /*SE ENVIA LA EXPRESION POSTFIJA PARA QUE SE DE EL RESULTADO*/
    resolver(postfija, tam_post); 
}


    //METODO QUE VERIFICA SI EL CARACTER ES UN OPERADOR O NO
    int esOperador(car)
    {
        if(car=='/' || car=='*' || car=='+' || car=='-')
            return 1;
        else
            return 0;
    }
    //EN REALIDAD COMPRUEBA SI ES DE MENOR O IGUAL PRESEDENCIA
    int esMenorPresedencia(anterior, actual){
        if(nivelPresedencia(actual)<=nivelPresedencia(anterior))
            return 1;
        else
            return 0;
    }
    //METODO QUE RETORNA EL NIVEL DE PREDENCIA
    int nivelPresedencia(operando){
        switch(operando)
        {
            
            case '(':
                return 0;
            case '*':
                return 1;
            case '/':
                return 1;
            default:
                return 0;
        }
    }
 

    float efectuar(float op, float op2, char operador)
    {
        /*EL RESULTADO ES UN NUMERO REAL*/
        float resultado;
        /*CASOS DE OPERADOR*/
        switch(operador)
        {
            case '*':
                resultado = op * (op2);
                break;
            case '/':
                        if(op2==0)
                        {
                                    printf("Numero Indeterminado!!Divido cero no se permite\n");
                                    resultado=0;
                                    errores=1;
                                    break;
                        }
                resultado = op / op2;
                break;
            case '+':
                resultado = op + op2;
                break;
            case '-':
                resultado = op - op2;
                break;
            case 'E':
               
                resultado = op*pow(10,op2);
                break;
            case 'e':
                resultado = op *pow(10,(int)op2);
                break;
        }
        return resultado;
    }
    
 /*EN ESTA FUNCION SE DA EL RESULTADO DE LA ECPRESION POLACA O POSTFIJA*/
 int resolver(char matriz[tam_post][20], int cont)
 {
    int x; 
    int y=0;
    int tam=cont;
    
   /*SE CREA LA NUEVA PILA QUE IRA CONTENIENDO LOS RESULTADOS DE LAS OPERACIONES*/
    char nuevo[tam_post][20];
    int utilizado=0;
    
    for(x=0;x<tam_post;x++)
    {
        float temp=0;
       
        if(((esOperador(matriz[x+2][0]) && isdigit(matriz[x+2][1])==0) || toupper(matriz[x+2][0])=='E') && utilizado==0)
        {
               if(matriz[x][0]!='x' && matriz[x+1][0]!='x')
               {
                    temp = efectuar(atof(matriz[x]), atof(matriz[x+1]), matriz[x+2][0]);
               }
               char cad[20];
               /*SE GUARDA EN LA PILA NUEVO COMO STRING*/
               sprintf(cad,"%.2f", temp );
               strcpy(nuevo[y],cad);
          
               y++;
               tam-=2;
               
               /*YA SE HIZO ESTO ENTONCES EN ESTE CICLO NO SE VOLVERA A HACER*/ 
               utilizado=1;
               x+=2; 
        }
        else{
        strcpy(nuevo[y],matriz[x]);
        y++; 
        }
    }
    tam_post-=2;
    if(tam<2)
    {
        /*SE MUESTRA EL RESULTADO SI EL TAMANHO YA ES MENOR A DOS*/
        if(errores==0)
        printf("\nResultado de la expresion: %s\n", nuevo[0]);
        //getch();
    }
    else
    {
        /*EN FORMA RECURSIVA SE HACE LA FUNCION PERO AHORA LA PILA POSTFIJA SERA LA PILA NUEVO*/
        resolver(nuevo, tam);
    }
    
 }
