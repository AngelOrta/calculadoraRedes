/*
 Ventana:80x50
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
void interfaz(char *direccion, int *mascara, int *mascaraOpci);
int main(){
	char direccion[17];
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
    printf("================================================================================\n\n");
    printf("\t\t\t    CALCULADORA DE REDES\n\n");
    printf("________________________________________________________________________________\n");
    printf("\t\t\t\t|  \t\t      |  Máscara de subred\t|\n");//  Mascara de subred  
    //printf("  Mascara de subred  |");
    //printf("  Mascara de subred\t|\n");
    printf("\tDirección IP\t\t|  Máscara de subred  |  para sub/superneteo\t|\n");
    printf("\t\t\t\t|  (24, por ejemplo)  |  (opcional)\t\t|\n");
    printf("________________________________|_____________________|________________________|\n");
    printf("                                |                     |                        |\n");
    printf("________________________________|_____________________|________________________|\n");
    printf("\033[2A\033[7C"); //Cursor a primer slot
    fflush(stdout);
    fgets(direccion, 17, stdin);
    fflush(stdin);
    printf("\033[1B\033[20D"); //Cursor a salida
    printf("\033[2A\033[42C"); //Cursor a segundo slot
    fflush(stdout);
    scanf("%d",&(*mascara));
    fflush(stdin);
    printf("\033[1B\033[22D"); //Cursor a salida
    printf("\033[2A\033[60C"); //Cursor a tercer slot
    fflush(stdout);
    scanf("%d",&(*mascaraOpci));
    fflush(stdin);
    printf("\033[0B\033[22D"); //Cursor a salida
    fflush(stdout);
}
