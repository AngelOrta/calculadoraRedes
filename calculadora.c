/*
 Ventana:80x50
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<setjmp.h>
#include<ctype.h>

void interfaz(char *direccion, int *mascara, int *mascaraOpci);
int ipValida(char *direccion);
int esNumero(char *caracter);
int puntosSeguidos(char *caracter);
int mascaraValida(int d);
void limpiaBufferIn();
void error(char *s);

jmp_buf inicio;
char *progname;

int main(int argc, char *argv[]){
    char opcion;
	char direccion[17]; //Direccion de un host o red
    int mascara,mascaraOpci,i=0;
    progname=argv[0];
    //Empieza Calculadora
    do{
        
        if(i!=0)
            limpiaBufferIn(); //Para el getchar
        setjmp(inicio);
        interfaz(direccion,&mascara,&mascaraOpci);
        i++;
        printf("\n%.15s %d %d",direccion,mascara,mascaraOpci);
        fflush(stdout);
        printf("\nQuieres probar con otra dirección IP? (S/N): ");
    }while((opcion=getchar()) == 115 || opcion == 83);
    return 0;
}



//Imprime la interfaz y captura los datos desde el teclado
void interfaz(char *direccion, int *mascara, int *mascaraOpci){
    char opci[4];
    system("clear");
    printf("\033[H\033[2J");
    fflush(stdout);
    
    printf("================================================================================\n\n");
    printf("\t\t\t    CALCULADORA DE REDES\n\n");
    printf("________________________________________________________________________________\n");
    printf("\t\t\t\t|  \t\t      |  Máscara de red   \t|\n"); 
    printf("\tDirección IP\t\t|  Máscara de red     |  para sub/superneteo\t|\n");
    printf("\t\t\t\t|  (24, por ejemplo)  |  (opcional)\t\t|\n");
    printf("________________________________|_____________________|________________________|\n");
    printf("                                /                     |                        |\n");
    printf("________________________________|_____________________|________________________|\n");
    printf("\033[2A\033[7C"); //Cursor a primer slot

    if(fgets(direccion, 17, stdin)){
        if(!ipValida(direccion))
            error("Dirección inválida. Espere para empezar de nuevo");
    }
    
    printf("\033[1B\033[20D"); //Cursor a salida
    printf("\033[2A\033[42C"); //Cursor a segundo slot
    
    scanf("%d",&(*mascara));
    limpiaBufferIn();
    if(!mascaraValida(*mascara))
        error("Máscara de red inválida. Espere para empezar de nuevo");
    printf("\033[1B\033[22D"); //Cursor a salida
    printf("\033[2A\033[60C"); //Cursor a tercer slot
    
    if(fgets(opci,sizeof(opci),stdin)){
        if(opci[0] == '\n') //No se introdujo mascara opcional
            *mascaraOpci = -1; 
        else{
            if(opci[strlen(opci)-1] != '\n')
                limpiaBufferIn();
            opci[strlen(opci)-1] = '\0';
            if(esNumero(opci)){
                *mascaraOpci = atoi(opci);
                if(!mascaraValida(*mascaraOpci))
                    error("Máscara de red inválida. Espere para empezar de nuevo");
            }else
                error("Máscara de red inválida. Espere para empezar de nuevo");
        }
    }
    printf("\033[0B\033[22D"); //Cursor a salida
}

//Valida el formato de la direccion ip ingresada
int ipValida(char *direccion){
    int octetoN;
    char *octeto, cdireccion[16];
    if(direccion[strlen(direccion)-1] != '\n'){//Excedio numero caracteres permitidos
        limpiaBufferIn();
        return 0;
    } 
    
    direccion[strlen(direccion)-1] = '\0'; //Quitando salto de linea

    if(direccion[0] == '.' || direccion[strlen(direccion)-1] == '.')
        return 0;
    
    if(puntosSeguidos(direccion))   
        return 0;

    strcpy(cdireccion,direccion);
    octeto = strtok(cdireccion,"."); 
    int puntos=0;
    while(octeto!=NULL){  //Para cada componente separado por puntos
        if(!esNumero(octeto)){
            return 0;
        }

        octetoN = atoi(octeto);
        if(octetoN >= 0 && octetoN <= 255){ //Avanza al siguiente octeto
        
            octeto = strtok(NULL, ".");
            if(octeto != NULL)
                puntos++;
        }else
            return 0;
    }
    return puntos == 3;
}

//Si una cadena esta formada por solo numeros
int esNumero(char *caracter){ 
    while(*caracter){
        if(!isdigit(*caracter))
            return 0;
        caracter++;
    }
    return 1;
}

//Si hay dos puntos seguidos en una cadena
int puntosSeguidos(char *caracter){ 
    char pasado=*caracter;
    do{
        caracter++;
        if(pasado == '.' && *caracter == '.')
            return 1;
        pasado=*caracter;
    }while(*caracter!='\0');
    return 0;
}

int mascaraValida(int d){
    if(d>=0 && d<=32)
        return 1;
    return 0;
}
void limpiaBufferIn(){
    int c;
    while((c=getchar()) != '\n' && c != EOF);
}

//Muestra un mensaje de error y hace un salto al inicio
void error(char *s){
    printf("\033[1B\033[20D");
    fflush(stdout);
	fprintf (stderr, "%s: %s", progname, s);
    sleep(4);
	longjmp(inicio, 0); //para imprimir nuevamente la interfaz
}


