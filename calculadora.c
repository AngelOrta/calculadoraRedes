#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include<windows.h>
void interfaz(char *direccion, int *mascara, int *mascaraOpci);
int main(){
	char direccion[17];
    int mascara,mascaraOpci;
    //Cambia dimensiones de la ventana
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD newSize;
    newSize.X = 90; 
    newSize.Y = 50;  

    SMALL_RECT rect;
    rect.Left = 0;
    rect.Top = 0;
    rect.Right = newSize.X - 1;
    rect.Bottom = newSize.Y - 1;

    SetConsoleScreenBufferSize(hConsole, newSize);
    SetConsoleWindowInfo(hConsole, TRUE, &rect);
    //Empieza Calculadora
    interfaz(direccion,&mascara,&mascaraOpci);
    printf("\n%.15s %d %d",direccion,mascara,mascaraOpci);
    return 0;
}

//Imprime la interfaz y captura los datos desde el teclado
void interfaz(char *direccion, int *mascara, int *mascaraOpci){
    printf("=========================================================================================\n\n");
    printf("\t\t\t\t    CALCULADORA DE REDES\n\n");
    printf("_________________________________________________________________________________________\n");
    printf("\t\t\t\t\t|  \t\t      |  Máscara de subred\t|\n");//  Mascara de subred  
    //printf("  Mascara de subred  |");
    //printf("  Mascara de subred\t|\n");
    printf("\t      Dirección IP\t\t|  Máscara de subred  |  para sub/superneteo\t|\n");
    printf("\t\t\t\t\t|  (24, por ejemplo)  |  (opcional)\t\t|\n");
    printf("________________________________________|_____________________|_________________________|\n");
    printf("                                        |                     |                         |\n");
    printf("________________________________________|_____________________|_________________________|\n");
    printf("\033[2A\033[12C"); //Cursor a primer slot
    fflush(stdout);
    fgets(direccion, 17, stdin);
    fflush(stdin);
    printf("\033[1B\033[20D"); //Cursor a salida
    printf("\033[2A\033[50C"); //Cursor a segundo slot
    fflush(stdout);
    scanf("%d",&(*mascara));
    fflush(stdin);
    printf("\033[1B\033[22D"); //Cursor a salida
    printf("\033[2A\033[74C"); //Cursor a tercer slot
    fflush(stdout);
    scanf("%d",&(*mascaraOpci));
    fflush(stdin);
    printf("\033[0B\033[22D"); //Cursor a salida
}
