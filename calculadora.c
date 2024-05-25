/*
 Ventana:80x50
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
void interfaz(char *direccion, int *mascara, int *mascaraOpci);
void limpiaBufferIn();
int main(){
	char direccion[17]; //Direccion de un host o red
    int mascara,mascaraOpci;
    //Empieza Calculadora

    interfaz(direccion,&mascara,&mascaraOpci);
    printf("\n%.15s %d %d",direccion,mascara,mascaraOpci);
    fflush(stdout);
    sleep(8);
    return 0;
}



//Imprime la interfaz y captura los datos desde el teclado
void interfaz(char *direccion, int *mascara, int *mascaraOpci){
    char opci[4];
    printf("================================================================================\n\n");
    printf("\t\t\t    CALCULADORA DE REDES\n\n");
    printf("________________________________________________________________________________\n");
    printf("\t\t\t\t|  \t\t      |  Máscara de subred\t|\n"); 
    printf("\tDirección IP\t\t|  Máscara de subred  |  para sub/superneteo\t|\n");
    printf("\t\t\t\t|  (24, por ejemplo)  |  (opcional)\t\t|\n");
    printf("________________________________|_____________________|________________________|\n");
    printf("                                /                     |                        |\n");
    printf("________________________________|_____________________|________________________|\n");
    printf("\033[2A\033[7C"); //Cursor a primer slot
    
    if(fgets(direccion, 17, stdin)){
        if(direccion[strlen(direccion)-1] != '\n') //Excedio numero caracteres permitidos
            limpiaBufferIn();
        direccion[strlen(direccion)-1] = 0; //Si habia salto de linea
    }
    printf("\033[1B\033[20D"); //Cursor a salida
    printf("\033[2A\033[42C"); //Cursor a segundo slot
    
    scanf("%d",&(*mascara));
    limpiaBufferIn();
    printf("\033[1B\033[22D"); //Cursor a salida
    printf("\033[2A\033[60C"); //Cursor a tercer slot
    
    if(fgets(opci,sizeof(opci),stdin)){
        if(opci[0] == '\n') //No se introdujo mascara opcional
            *mascaraOpci = -1; 
        else{
            if(opci[strlen(opci)-1] != '\n')
                limpiaBufferIn();
            opci[strlen(opci)-1] = 0;
            *mascaraOpci = atoi(opci);
        }
    }
    printf("\033[0B\033[22D"); //Cursor a salida
}

void limpiaBufferIn(){
    int c;
    while((c=getchar()) != '\n' && c != EOF);
}
