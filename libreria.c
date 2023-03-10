#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>

#include "para_menus.c"

void loguearUsuario();
void menuAdm();
int menuUsuario(char nick[40]);

int main(){
    
    // Obtenemos la fecha actual //

    time_t actual = time(NULL); 
    struct tm *tm = localtime(&actual);
    anio= tm->tm_year+1900;
    mes = tm->tm_mon+1;
    dia = tm->tm_mday;

    int opc=-1;

    do{
        validarFechaEntrega(); //Verifica las fechas de vencimiento y modifica el estado del usuario (mora)
       
       /* MENU PRINCIPAL */
       
        system("cls");
        printf("******** LIBRERIA **********\n\n");
        printf("1: Ingresar\n");
        printf("2: Registrarse\n");
        printf("0: Salir\n\n");
        printf("Opcion: ");
        scanf("%d",&opc);

        switch(opc){
            case 1:
                loguearUsuario();
                break;
            case 2:
                fflush(stdin);
                registrarUsuario();
                break;
            case 0:
                break;
            default:
                printf("\nOpccion no valida");
                break;
        }
    }
    while(opc!=0);
    return 0;
}

void loguearUsuario(){
    
    Usuarios usuario;
    int existe=0;
    char nick[40];
    char pass[40];

    FILE *fp = fopen("usuarios.bin", "rb");
    fflush(stdin);
    system("cls");
    printf("Ingrese su usuario: \n");
    gets(nick);
    printf("Ingrese su contrasenia: \n");
    gets(pass);
    
    fread(&usuario, sizeof(Usuarios), 1, fp);
    while(!feof(fp)){
        
        //Se validad las credenciales de administrador con las definidas en la macro
        if((strcmp(ADMIN, nick)==0) && (strcmp(PASS_ADMIN, pass))==0){
            
            menuAdm();
            existe = 1;
            break;
        }
        
        //Verifica si el usuario y la contraseña coinciden con alguno registrado
        if((strcmp(usuario.nombre, nick)==0) && (strcmp(usuario.password, pass))==0){
            
            //Aparte de entrar con las credenciales definidas en la macro, tambien se puede ingresar con 
            //un usuario admin registrado

            //Nombre de usuario concide con "admin", entonces se muestra el menu de administrador 
            if(strcmp(nick, "admin") == 0){ 
                menuAdm();
                existe = 1;
            }
            //Si el nombre de usuario es diferente a "admin", se muestra el menu para los usuarios normales
            else if(strcmp(nick, "admin") != 0){
                ID_USUARIO=usuario.id;
                existe = menuUsuario(nick);
            }
            break;
        }
        
        fread(&usuario, sizeof(Usuarios), 1, fp);
    }

    if(existe==0){
        printf("\nUsuario o contrasenia INCORRECTOS\n");
        getch();
    }
    
    fclose(fp);
}

int menuUsuario(char nick[40]){
    
    int opc2=-1;
    int opcion;
    int cod;
    int codLibro;
    int eliminar;
    int ok;
    
    while (opc2!=0)
    {
        validarFechaEntrega(); //Verifica las fechas de vencimiento y modifica el estado del usuario (mora)
        sinLibrosPrestados(); //Cambia el estado de prestamo del usuario si no tiene libros prestados


        /* MENU DEL USUARIO */

        system("cls");
        printf("***** Bienvenido %s *****\n\n", nick);
        printf("1: Libros disponibles\n");
        printf("2: Mis libros\n");
        printf("0: Volver\n");
        printf("\nOpcion: ");
        scanf("%i",&opc2);

        switch (opc2)
        {
        case 1:
            system("cls");
            prestarLibro();
            break;
        case 2:
            system("cls");
            cod = ID_USUARIO;
            ok = listarLibrosPrestados(cod);

            if(ok == 1){

                printf("\n---------------------\n");
                printf("\n1: Devolver libro");
                printf("\n0: volver");
                printf("\nOpcion: ");
                scanf("%i",&opcion);
                if(opcion == 1){
                    system("cls");
                    listarLibrosPrestados(cod);
                    printf("\nIngresa el ID del libro que quieres devolver: ");
                    scanf("%i", &codLibro);
                    eliminar = devolverLibro(cod, codLibro);
                    if(eliminar == 1){
                        eliminarPrestamo(cod, codLibro);
                        system("cls");
                        printf("Devolucion exitosa\n\n");
                        getch();
                    }
                }
                else{
                }
            }
            else{
                system("cls");
                printf("No tienes libros\n");
                getch();
            }
            
            break;
       
        default:
            break;
        }
    }
    return 1;
}


void menuAdm(){
    int opc2=-1;
    int cod;
    int codLibro;
    int opcion;
    int opcion1;
    while(opc2!=0){
        
        int existe;
        validarFechaEntrega();

        /* MENU DEL ADMINISTRADOR */
        
        system("cls");
        printf("***** Menu del Admin *****\n\n"); 
        printf("1: Libros\n"); 
        printf("2: Prestados por libro\n"); 
        printf("3: Prestados por usuario\n"); 
        printf("4: Usuarios retrasados\n"); 
        printf("0: Volver\n");
        printf("\nOpcion: ");
        scanf("%i",&opc2);
        
        switch(opc2)
        {
            case 1:
                system("cls");
                listarLibros();
                printf("---------------------\n");
                printf("\n1: Agregar libro");
                printf("\n2: Editar libro");
                printf("\n3: Borrar libro");
                printf("\n0: Volver");
                printf("\nOpcion: ");
                scanf("%i", &opcion);

                if(opcion == 1){
                    system("cls");
                    registrarLibro();
                    getch();
                }
                else if(opcion == 2){
                    system("cls");
                    listarLibros();
                    printf("-------------------------\n");
                    printf("1: Continuar\n");
                    printf("0: Volver\n");
                    printf("Opcion: ");
                    scanf("%i", &opcion1);
                    
                    if(opcion1 == 1){
                        system("cls");
                        listarLibros();
                        printf("\nIngresa el id del libro a editar: ");
                        scanf("%i", &codLibro);
                        editarLibro(codLibro);
                        getch();
                    }
                    
                }
                else if (opcion == 3){
                    system("cls");
                    listarLibros();
                    printf("\nIngresa el id del libro a eliminar: ");
                    scanf("%i", &codLibro);
                    eliminarLibro(codLibro);
                    eliminarPrestamoPorLibro(codLibro);//Elimina todos los prestamos realizados de un determinado libro
                    
                    getch();
                }
                
                break;
            case 2:
                system("cls");
                existe = prestamosPorLibro();//Lista de libros que han sido prestados por la biblioteca
                if(existe == 1){

                    printf("\nIngresa el id del libro para ver los usarios quienes lo han tomado prestado: ");
                    scanf("%i", &codLibro);
                    usariosPrestaronEseLibro(codLibro);
                } 
                else{
                    printf("\nNingun prestamo");
                }
                getch();
                break;
            case 3:
                system("cls");
                existe = listarUsuariosConPrestamos();
                if(existe == 1){

                    printf("\nIngrese ID de usario a consultar: \n");
                    scanf("%i", &cod);
                    system("cls");
                    listarLibrosPrestados(cod);
                }
                else{
                    printf("\nNingun prestamo");
                }
                getch();
                break;
            case 4:
                system("cls");
                existe = listarUsuariosEnMora(); // Lista los usuarios que tienen libros vencidos
                if (existe != 1)
                {
                    printf("\nNinguno");
                }
                getch();
                break;
            case 0: 
                break;
            default:
                printf("\nOpcion invalida\n");
                break;
        } 

    }
}