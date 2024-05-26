/*
 Ventana:80x50
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<setjmp.h>
#include<ctype.h>
#include<stdint.h>
#include<math.h>

void interfaz(char *direccionD, int *mascaraD, int *mascaraOpciD, int *mascara, int *mascaraSub);
void subneteo(int *mascaraD, int *mascaraOpciD, int *mascara, int *mascaraSub, int *direccionRed);
void generarMascara(int *mascara, int bitsD);
int ipValida(char *direccionD);
int esNumero(char *caracter);
int puntosSeguidos(char *caracter);
int mascaraValida(int d);
void limpiaBufferIn();
void error(char *s);

jmp_buf inicio;
char *progname;

int main(int argc, char *argv[]){
    char opcionD;
	char direccionD[17]; //(Dato)
    int mascara[4],mascaraSub[4],direccion[4], direccionRed[]={192,168,100,0};//10,128,0,0
    int mascaraD,mascaraOpciD,i=0;
    progname=argv[0];
    //Empieza Calculadora
    do{
        
        if(i!=0)
            limpiaBufferIn(); //Para el getchar
        setjmp(inicio);
        interfaz(direccionD,&mascaraD,&mascaraOpciD,mascara,mascaraSub);
        i++;
        printf("\n%.15s %d %d %d.%d.%d.%d",direccionD,mascaraD,mascaraOpciD, mascaraSub[0],mascaraSub[1],mascaraSub[2],mascaraSub[3]);
        fflush(stdout);
        if(mascaraOpciD!=-1)
            subneteo(&mascaraD, &mascaraOpciD, mascara,mascaraSub,direccionRed);
        printf("\nQuieres probar con otra dirección IP? (S/N): ");
    }while((opcionD=getchar()) == 115 || opcionD == 83);
    return 0;
}



//Imprime la interfaz y captura los datos desde el teclado
void interfaz(char *direccionD, int *mascaraD, int *mascaraOpciD, int *mascara, int *mascaraSub){
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

    if(fgets(direccionD, 17, stdin)){
        if(!ipValida(direccionD))
            error("Dirección inválida. Espere para empezar de nuevo");
    }
    
    printf("\033[1B\033[20D"); //Cursor a salida
    printf("\033[2A\033[42C"); //Cursor a segundo slot
    
    scanf("%d",&(*mascaraD));
    limpiaBufferIn();
    if(!mascaraValida(*mascaraD))
        error("Máscara de red inválida. Espere para empezar de nuevo");
    generarMascara(mascara,*mascaraD);
    printf("\033[1B\033[22D"); //Cursor a salida
    printf("\033[2A\033[60C"); //Cursor a tercer slot
    
    if(fgets(opci,sizeof(opci),stdin)){
        if(opci[0] == '\n') //No se introdujo mascaraD opcional
            *mascaraOpciD = -1; 
        else{
            if(opci[strlen(opci)-1] != '\n')
                limpiaBufferIn();
            opci[strlen(opci)-1] = '\0';
            if(esNumero(opci)){
                *mascaraOpciD = atoi(opci);
                if(!mascaraValida(*mascaraOpciD))
                    error("Máscara de red inválida. Espere para empezar de nuevo");
            }else
                error("Máscara de red inválida. Espere para empezar de nuevo");
            generarMascara(mascaraSub,*mascaraOpciD);
        }
    }
    printf("\033[0B\033[22D"); //Cursor a salida
}

//Calcula las direcciones de red de todas las subredes y muestra su informacion
void subneteo(int *mascaraD, int *mascaraOpciD, int *mascara, int *mascaraSub, int *direccionRed){
    int saltos,indiceSaltos,subredes,i,host;
    int copiaDRed[4];
    for(i=0; i<4; i++)
        copiaDRed[i] = direccionRed[i];
    printf("\n%d.%d.%d.%d",copiaDRed[0], copiaDRed[1], copiaDRed[2], copiaDRed[3]);
    subredes = (int)pow((double)2,(double)(*mascaraOpciD-*mascaraD));
    host =subredes*((int)pow((double)2,(double)(32-*mascaraOpciD))-2);
    //Para el ultimo octeto que no esta lleno de ceros y en donde se daran los saltos
    for(i=0; i<4; i++){
        if(mascaraSub[i] == 0x00){
            indiceSaltos = i-1;
            break;
        }else if(i==3)
            indiceSaltos = i;
    }
    saltos = 256-mascaraSub[indiceSaltos];
    //Ciclo para generar subredes
    for(i=0; i<subredes; i++){
        if(copiaDRed[indiceSaltos] >= 256){ //Para incrementar el octeto anterior
            copiaDRed[indiceSaltos] = 0;
            if(copiaDRed[indiceSaltos-1] < 256)
                copiaDRed[indiceSaltos-1] += 1;
            else if(copiaDRed[indiceSaltos-2] < 256){
                copiaDRed[indiceSaltos-1] = 0;
                copiaDRed[indiceSaltos-2] += 1;
            }
        }
        printf("\nSubred %d:\n\t %d.%d.%d.%d",i+1,copiaDRed[0], copiaDRed[1], copiaDRed[2], copiaDRed[3]);
        //Llamar aqui a la funcion 
        copiaDRed[indiceSaltos]+=saltos; 
        
    }
    printf("\n\nSubredes: %d\nHost utilizables: %d",subredes, host);
}


//Valida el formato de la direccionD ip ingresada
int ipValida(char *direccionD){
    int octetoN;
    char *octeto, cdireccion[16];
    if(direccionD[strlen(direccionD)-1] != '\n'){//Excedio numero caracteres permitidos
        limpiaBufferIn();
        return 0;
    } 
    
    direccionD[strlen(direccionD)-1] = '\0'; //Quitando salto de linea

    if(direccionD[0] == '.' || direccionD[strlen(direccionD)-1] == '.')
        return 0;
    
    if(puntosSeguidos(direccionD))   
        return 0;

    strcpy(cdireccion,direccionD);
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

//Genera la mascara a partir del dato ingresado
void generarMascara(int *mascara, int bitsD){
    uint32_t mascaraCompleta = 0xFFFFFFFF; //llena de 1's
    mascaraCompleta <<=(32-bitsD);  //desplaza bits

    //Separando los octetos operando con el ultimo octeto luego de desplazarlo
    mascara[0] = (mascaraCompleta >> 24) & 0xFF;
    mascara[1] = (mascaraCompleta >> 16) & 0xFF;
    mascara[2] = (mascaraCompleta >> 8) & 0xFF;
    mascara[3] = mascaraCompleta & 0xFF;
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


