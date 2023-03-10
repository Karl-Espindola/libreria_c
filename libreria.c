#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>

#define ADMIN "admin"
#define PASS_ADMIN "1234"

typedef struct{
    int id;
    char titulo[40];
    int ejemplares;
    int disponibles;

}Libros;

typedef struct{
    int id;
    char nombre[40];
    char password[40];
    int estado;
    int conPrestamo;
    int mora;

}Usuarios;

typedef struct{
    int idUsuario;
    int idLibro;
    int dia;
    int mes;
    int anio;
}Prestamos;

int ID_USUARIO;
int anio;
int mes;
int dia;

void registrarUsuario();
void listarUsuarios();
void listarUsuariosConPrestamos();
void loguearUsuario();
int verificarUsuariosRepetidos(char nick[40]);
void modificarEstadoUsuario();
void registrarLibro();
void listarLibros();
void prestarLibro();
int verificarEstadoMora(int usuarioID);
int validarPrestamo(int cod);
Libros consultarLibro(int cod);
int listarLibrosPrestados(int cod); //Lista de libros prestados por el usuario
void editarLibro(int cod);
void eliminarPrestamo(int idUsuario, int codLibro);
int devolverLibro(int idUsuario, int codLibro);
void prestamosPorLibro();//Lista de libros que han sido prestados por la biblioteca 
void eliminarPrestamoPorLibro(int codLibro);
void eliminarLibro(int codLibro);
void listarUsuariosEnMora();
void usariosPrestaronEseLibro(int codLibro);
void menuAdm();
int menuUsuario(char nick[40]);
void validarFechaEntrega();
void sinLibrosPrestados();

int main(){
    
    time_t actual = time(NULL);
    struct tm *tm = localtime(&actual);
    anio= tm->tm_year+1900;
    mes = tm->tm_mon+1;
    dia = tm->tm_mday;

    int opc=-1;

    do{
        validarFechaEntrega();
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

void menuAdm(){
    int opc2=-1;
    int cod;
    int codLibro;
    int opcion;
    int opcion1;
    int opcion2;
    while(opc2!=0){
        validarFechaEntrega();
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
                prestamosPorLibro();//Lista de libros que han sido prestados por la biblioteca 
                printf("\nIngresa el id del libro para ver los usarios quienes lo han tomado prestado: ");
                scanf("%i", &codLibro);
                usariosPrestaronEseLibro(codLibro);
                getch();
                break;
            case 3:
                system("cls");
                listarUsuariosConPrestamos();
                printf("\nIngrese ID de usario a consultar: \n");
                scanf("%i", &cod);
                system("cls");
                listarLibrosPrestados(cod);
                getch();
                break;
            case 4:
                system("cls");
                listarUsuariosEnMora();
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

void usariosPrestaronEseLibro(int codLibro){
    
    Prestamos prestamo;
    Usuarios usuario;
    int existe;
    FILE *fprestamos = fopen("prestamos.bin", "rb");
    FILE *fusuarios = fopen("usuarios.bin", "rb");
    
    fread(&prestamo, sizeof(Prestamos), 1, fprestamos);
    while (!feof(fprestamos))
    {
        if(prestamo.idLibro == codLibro){
            existe = 1;
            fread(&usuario, sizeof(Usuarios), 1, fusuarios);
            while (!feof(fusuarios)){
                if(usuario.id == prestamo.idUsuario){
                    printf("Usuario: %s\n", usuario.nombre);
                }
                fread(&usuario, sizeof(Usuarios), 1, fusuarios);
            }
            rewind(fusuarios);
        }
        fread(&prestamo, sizeof(Prestamos), 1, fprestamos);
    }
    if(existe != 1){
        printf("Este libro no ha sido prestado o No existe\n");
    }
    fclose(fprestamos);
    fclose(fusuarios);
    
}

int verificarUsuariosRepetidos(char nick[40]){
    Usuarios usuario;
    FILE *fp = fopen("usuarios.bin", "rb");
    fread(&usuario, sizeof(Usuarios), 1, fp);
    while (!feof(fp))
    {
        if(strcmp(nick, usuario.nombre) == 0){
            fclose(fp);
            return 1;
        }
        fread(&usuario, sizeof(Usuarios), 1, fp);
    }
    fclose(fp);
    return 0;
}
void listarUsuariosEnMora(){
    Usuarios usuario;
    FILE *fp = fopen("usuarios.bin", "rb");

    fread(&usuario, sizeof(Usuarios), 1, fp);
    while (!feof(fp))
    {
        if (usuario.mora == 1)
        {
            printf("ID: %i  Usuario: %s  Estado: %i   Con-Prestamo: %i  Mora: %i\n", usuario.id, usuario.nombre, usuario.estado, usuario.conPrestamo, usuario.mora);
        }
        fread(&usuario, sizeof(Usuarios), 1, fp);
    }
    fclose(fp);
    
}
void listarUsuariosConPrestamos(){
    Usuarios usuario;
    FILE *fp = fopen("usuarios.bin", "rb");

    fread(&usuario, sizeof(Usuarios), 1, fp);
    while (!feof(fp))
    {
        if(usuario.conPrestamo == 1){
            printf("ID: %i  Usuario: %s  Estado: %i Mora: %i\n", usuario.id, usuario.nombre, usuario.estado, usuario.mora);
        }
        fread(&usuario, sizeof(Usuarios), 1, fp);
    }
    fclose(fp);
    
}
void eliminarLibro(int codLibro){

    Libros libro;
    int existe;

    FILE *archivoLibro = fopen("libros.bin", "rb");
    FILE *temporalLibro = fopen("temporal.bin", "wb");

    fread(&libro, sizeof(Libros), 1, archivoLibro);

    while (!feof(archivoLibro))
    {
        if(libro.id == codLibro){
            existe = 1;
        }
        if(libro.id != codLibro){
            fwrite(&libro, sizeof(Libros), 1, temporalLibro);
        }
        fread(&libro, sizeof(Libros), 1, archivoLibro);
    }
    if(existe != 1){
        printf("\nEL libro no existe\n");
    }
    else{
        printf("\nLibro eliminado\n\n");
    }
    fclose(archivoLibro);
    fclose(temporalLibro);
    remove("libros.bin");
    rename("temporal.bin", "libros.bin");
    
    
}
void eliminarPrestamoPorLibro(int codLibro){ //Elimina todos los prestamos realizados de un determinado libro
    Prestamos prestamo;
    FILE *presta = fopen("prestamos.bin", "rb+");
    FILE *temporalPresta = fopen("temporalPresta.bin", "wb");

    fread(&prestamo, sizeof(Prestamos), 1, presta);
    while(!feof(presta)){
        if(prestamo.idLibro != codLibro){
            fwrite(&prestamo, sizeof(Prestamos), 1, temporalPresta);
        }
        fread(&prestamo, sizeof(Prestamos), 1, presta);
    }
    fclose(presta);
    fclose(temporalPresta);
    remove("prestamos.bin");
    rename("temporalPresta.bin", "prestamos.bin");
}
void prestamosPorLibro(){
    Libros libro;
    int numPrestados;
    FILE *fp = fopen("libros.bin", "rb");
    fread(&libro, sizeof(Libros), 1, fp);
    while(!feof(fp)){
        if(libro.disponibles < libro.ejemplares){
            numPrestados = libro.ejemplares - libro.disponibles;
            printf("ID: %i  Titulo: %s - Prestados: %i\n", libro.id, libro.titulo, numPrestados);
        }
        fread(&libro, sizeof(Libros), 1, fp);
    }
    fclose(fp);
}
void editarLibro(int cod){
    Libros libro;
    char nuevoTitulo[40];
    int nuevoNumEjemplares;
    int existe;
    FILE *fp = fopen("libros.bin", "rb+");

    fread(&libro, sizeof(Libros), 1, fp);

    while(!feof(fp)){
        if(libro.id == cod){
            fflush(stdin);
            printf("\nIngrese el nuevo titulo: \n");
            fgets(nuevoTitulo, 39, stdin);

            if(strcmp(nuevoTitulo, "\n") != 0){

                printf("Ingrese el nuevo numero de disponibles: \n");
                strcpy(libro.titulo, nuevoTitulo);
                scanf("%i", &nuevoNumEjemplares);
                if(nuevoNumEjemplares > 0){
                    libro.disponibles = (nuevoNumEjemplares - libro.ejemplares) + libro.disponibles;
                    libro.ejemplares = nuevoNumEjemplares;

                    fseek(fp, -(sizeof(Libros)), SEEK_CUR);
                    fwrite(&libro, sizeof(Libros), 1, fp);
                    printf("\nEl libro fue editado\n");
                }
                else{
                    printf("\nCantidad menor a 1. No se realizo ningun cambio");
                }
            }
            else{

                printf("Ingrese el nuevo numero de disponibles: \n");
                scanf("%i", &nuevoNumEjemplares);
                
                if(nuevoNumEjemplares > 0){
                    libro.disponibles = (nuevoNumEjemplares - libro.ejemplares) + libro.disponibles;
                    libro.ejemplares = nuevoNumEjemplares;

                    fseek(fp, -(sizeof(Libros)), SEEK_CUR);
                    fwrite(&libro, sizeof(Libros), 1, fp);
                    printf("\nEl libro fue editado\n");
                }
                else{
                    printf("\nCantidad menor a 1. No se realizo ningun cambio");
                }
            }
            existe = 1;
            break;
        }
        fread(&libro, sizeof(Libros), 1, fp);
    }
    if(existe != 1){
        printf("\nEl libro no existe\n");
    }
    fclose(fp);
}

void listarUsuarios(){
    Usuarios usuario;
    FILE *fp = fopen("usuarios.bin", "rb");

    fread(&usuario, sizeof(Usuarios), 1, fp);
    while (!feof(fp))
    {
        printf("ID: %i  Usuario: %s  Estado: %i   Con-Prestamo: %i  Mora: %i\n", usuario.id, usuario.nombre, usuario.estado, usuario.conPrestamo, usuario.mora);
        fread(&usuario, sizeof(Usuarios), 1, fp);
    }
    
    fclose(fp);
}
void modificarEstadoUsuario(){
    Usuarios usuario;
    int idUsuario;
    int opc2=-1;
    int existe = 0;
    int estado ;
    long pos;
    FILE *fp = fopen("usuarios.bin", "rb+");
    listarUsuarios();
    printf("\nIngresar ID del usuario para modificar su estado: ");
    scanf("%i",&idUsuario);

    fread(&usuario, sizeof(Usuarios), 1, fp);

    while(!feof(fp)){
        if(usuario.id == idUsuario){
            pos = ftell(fp);
            do{
                system("cls");
                printf("ID: %i  Usuario: %s  Estado: %i\n\n", usuario.id, usuario.nombre, usuario.estado);
                printf("***** Cambiar estado de usuario *****\n\n");
                printf("0: Bloquear usuario\n");
                printf("1: Desbloquear usuario\n");
                printf("5: Cancelar\n\n");
                printf("Opcion: ");
                scanf("%i", &opc2);

            switch (opc2)
            {
                case 0:
                    estado = 0;
                    
                    break;
                case 1:
                    estado = 1;
                    break;
                case 5:
                    break;
                default:
                    break; 
                }
            }
            while(opc2!=5);
            if(estado == 0){
                usuario.estado = 0;
            }
            else if(estado == 1){
                usuario.estado = 1;
            }
            existe = 1;
            if(existe == 1){
                existe = 1;
            }
            break;
        }
        else{
        }
        fread(&usuario, sizeof(Usuarios), 1, fp);
    }
    if(existe == 0){
        printf("\nUsuario no existe\n");
    }
    else if(existe == 1){
        fseek(fp, pos-(sizeof(Usuarios)), SEEK_SET);
        fwrite(&usuario, sizeof(Usuarios), 1, fp);
    }

    fclose(fp);
}
void registrarUsuario(){

    Usuarios usuario;
    char nom[40];
    char pass[40];
    int cod;
    int repetido;
    int opc;

    system("cls");
    printf("\n1: Registrar nuevo usuario\n");
    printf("0: volver\n");
    printf("Opciones: ");
    scanf("%i", &opc);
    
    if (opc == 1)
    {
        system("cls");
        fflush(stdin);
        printf("Escriba un nombre: ");
        gets(nom);

        repetido = verificarUsuariosRepetidos(nom);

        FILE *fp=fopen("usuarios.bin", "rb+");
        fseek(fp, 0, SEEK_END);
        int pos= ftell(fp);
        if(pos==0){
            usuario.id=1;
            cod=1; 
        }
        else{
            fseek(fp, -(sizeof(usuario)), SEEK_END);
            fread(&usuario, sizeof(Usuarios), 1, fp);
            cod=usuario.id+1;
        }
        fseek(fp, 0, SEEK_END);
        usuario.id = cod;
        usuario.estado = 1;
        usuario.conPrestamo = 0;
        usuario.mora = 0;
        
        if(repetido == 0){
            
            strcpy(usuario.nombre, nom);
            printf("\nEscriba una contrasenia: ");
            gets(pass);
            strcpy(usuario.password, pass);
            fwrite(&usuario, sizeof(Usuarios), 1, fp);
            printf("\nSu registro fue exitoso.\n");

        }
        else if (repetido == 1)
        {
            printf("\nEste usuario ya existe.\n");
        }
        fclose(fp);
    }
}    

void loguearUsuario(){
    
    Usuarios usuario;
    int existe=0;
    char nick[40];
    char pass[40];
    // char amdin[10]="admin";
    
    FILE *fp = fopen("usuarios.bin", "rb");
    fflush(stdin);
    system("cls");
    printf("Ingrese su usuario: \n");
    gets(nick);
    printf("Ingrese su contrasenia: \n");
    gets(pass);
    
    fread(&usuario, sizeof(Usuarios), 1, fp);
    while(!feof(fp)){
        
        //Verifica si el usuario y la contraseña coinciden con alguno registrado
        if((strcmp(ADMIN, nick)==0) && (strcmp(PASS_ADMIN, pass))==0){
            
            menuAdm();
            existe = 1;
            break;
        }
        
        if((strcmp(usuario.nombre, nick)==0) && (strcmp(usuario.password, pass))==0){
            
            if(strcmp(nick, "admin") == 0){
                menuAdm();
                existe = 1;
            }
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
        validarFechaEntrega();
        sinLibrosPrestados();

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

void registrarLibro(){

    Libros libro;
    int cod;
    char titulo[40];
    int numEjemplares;
    int existe;

    FILE *fp = fopen("libros.bin", "rb+");

    system("cls");
    fflush(stdin);
    // printf("Cod: %i\n", cod);
    printf("Titulo del libro: \n");
    gets(titulo);

    // rewind(fp);
    fread(&libro, sizeof(Libros), 1, fp);
    while (!feof(fp))
    {
        if(strcmp(libro.titulo, titulo) == 0){
            printf("\nEste libro ya existe");
            existe = 1;
            break;
        }
        fread(&libro, sizeof(Libros), 1, fp);
    }

    if(existe != 1){
        fseek(fp, 0, SEEK_END);
        int pos = ftell(fp);
        if(pos==0){
            cod=1;
            libro.id=1;
        }
        else{
            fseek(fp, -(sizeof(libro)), SEEK_END);
            fread(&libro, sizeof(Libros), 1, fp);
            cod=libro.id+1;
        }
        fseek(fp, 0, SEEK_END);
        libro.id = cod;
        
        printf("Numero de ejemplares: \n");
        scanf("%i", &numEjemplares);
        strcpy(libro.titulo, titulo);
        libro.ejemplares = numEjemplares;
        libro.disponibles = numEjemplares;
        printf("\nRegistro exitoso");
    }

    fwrite(&libro, sizeof(Libros), 1, fp);
    fclose(fp);
}

void listarLibros(){
    Libros libro;
    FILE *fp = fopen("libros.bin", "rb");
    
    fread(&libro, sizeof(Libros), 1, fp);
    while (!feof(fp)){
        printf("ID: %i\n", libro.id);
        printf("Titulo: %s\n", libro.titulo);
        printf("Ejemplares: %i\n", libro.ejemplares);
        printf("Disponibles: %i\n\n", libro.disponibles);
        fread(&libro, sizeof(Libros), 1, fp);

    }

    fclose(fp);
}
void registrarPrestamo(int cod){
    Prestamos prestamo;
    FILE *fp = fopen("prestamos.bin", "ab");
    prestamo.idUsuario = ID_USUARIO;
    prestamo.idLibro = cod;
    system("cls");
    printf("Acontinuacion ingrese la fecha de entrega\n\n");
    printf("Anio: ");
    scanf("%i", &prestamo.anio);
    printf("\nMes: ");
    scanf("%i", &prestamo.mes);
    printf("\ndia: ");
    scanf("%i", &prestamo.dia);
    fwrite(&prestamo, sizeof(Prestamos), 1, fp);
    fclose(fp);

}
void validarFechaEntrega(){//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    Prestamos prestamo;
    Usuarios usuario;
    int demora;
    int mora;
    int existio;
    int lengS = 0;
    FILE *filePresta = fopen("prestamos.bin", "rb");
    FILE *fileUsuario = fopen("usuarios.bin", "rb+");

    
   fread(&usuario, sizeof(Usuarios), 1, fileUsuario);
   while (!feof(fileUsuario))
   {
    lengS = usuario.id; //lengS: cantidad maxima de usuarios 
    fread(&usuario, sizeof(Usuarios), 1, fileUsuario);
   }
   rewind(fileUsuario);
   
   for(int i = 1; i <= lengS; i++) 
    {   
        // demora = 0;
        existio = 0;
        if(!feof(filePresta)){
            rewind(filePresta);
            fread(&prestamo, sizeof(Prestamos), 1, filePresta);
        }
        while (!feof(filePresta)) //cada iteracion del for es el id de un usuario que 
        {                         // en el bucle while verificamos si tiene algu prestamo registrado
            demora = 0;
            mora = -1;
            if(prestamo.idUsuario == i){
                existio = 1;
                mora = 0;
                if(prestamo.anio < anio){
                    demora = 1;
                }
                else if (prestamo.anio == anio){
                    if(prestamo.mes < mes){
                        demora = 1;
                    }
                    else if(prestamo.mes == mes){
                        if(prestamo.dia < dia){
                            demora = 1;
                        }
                    }
                }
                
                if(demora == 1){ 
                    mora = 1;
                    rewind(fileUsuario);
                    fread(&usuario, sizeof(Usuarios), 1, fileUsuario);
                    while (!feof(fileUsuario))
                    {
                        if(usuario.id == i){
                            
                            usuario.mora = 1;
                            fseek(fileUsuario, -sizeof(Usuarios), SEEK_CUR);
                            fwrite(&usuario, sizeof(Usuarios), 1, fileUsuario); 
                            fseek(fileUsuario, sizeof(Usuarios), SEEK_CUR);
                            rewind(fileUsuario);
                            
                            break;
                        }
                        fread(&usuario, sizeof(Usuarios), 1, fileUsuario);
                    }
                }
            }
            
            fread(&prestamo, sizeof(Prestamos), 1, filePresta);
        }
        rewind(filePresta);
        
        if(mora == 0 && !feof(fileUsuario)){    //Este usuario tiene prestamos registrados y alguno sin vencer
            rewind(fileUsuario);
            fread(&usuario, sizeof(Usuarios), 1, fileUsuario);
            while (!feof(fileUsuario))
            {
                if(usuario.id == i && usuario.mora != 1){ // Este usuario tiene prestamos y ninguno vencido
                    
                    usuario.mora = 0;
                    fseek(fileUsuario, -sizeof(Usuarios), SEEK_CUR);
                    fwrite(&usuario, sizeof(Usuarios), 1, fileUsuario); 
                    // fseek(fileUsuario, sizeof(Usuarios), SEEK_CUR);
                    rewind(fileUsuario);
                    break;
                }
                fread(&usuario, sizeof(Usuarios), 1, fileUsuario);
            }
            rewind(fileUsuario);
        }
        else if(mora == -1 && existio == 0 && !feof(fileUsuario)){ //mora = -1, hay prestamos registraos pero niguno con este usuario 
            
            rewind(fileUsuario);
            fread(&usuario, sizeof(Usuarios), 1, fileUsuario);
            while (!feof(fileUsuario))
            {
                if(usuario.id == i){
                    
                    usuario.mora = 0;
                    fseek(fileUsuario, -sizeof(Usuarios), SEEK_CUR);
                    fwrite(&usuario, sizeof(Usuarios), 1, fileUsuario); 
                    // fseek(fileUsuario, sizeof(Usuarios), SEEK_CUR);;
                    rewind(fileUsuario);
                    break;
                }
                fread(&usuario, sizeof(Usuarios), 1, fileUsuario);
            }
            rewind(fileUsuario);
            
        }
        
    }
   fclose(fileUsuario);
   fclose(filePresta);
}
int validarPrestamo(int cod){
    Prestamos prestamo;
    int valido;
    FILE *fp = fopen("prestamos.bin", "rb");
    fread(&prestamo, sizeof(Prestamos), 1, fp);

    while(!feof(fp)){
        if(prestamo.idUsuario == ID_USUARIO && prestamo.idLibro == cod){
            valido = -1;
            break;
        }
        else{
            valido = 2;
        }
        fread(&prestamo, sizeof(Prestamos), 1, fp);
    }
    
    fclose(fp);
    return valido;
}
Libros consultarLibro(int cod){
    Libros libro;
    FILE *fp = fopen("libros.bin", "rb");
    fread(&libro, sizeof(Libros), 1, fp);
    while(!feof(fp)){
        if(libro.id == cod){
            fclose(fp);
            return libro;
        }
        fread(&libro, sizeof(Libros), 1, fp);
    }
    fclose(fp);
}

int listarLibrosPrestados(int cod){

    Prestamos prestamo;
    Libros libro;
    Usuarios usuario;
    int prestado;
    int existe;
    FILE *fp = fopen("prestamos.bin", "rb");
    FILE *fileUsuario = fopen("usuarios.bin", "rb+");
    
    fread(&prestamo, sizeof(Prestamos), 1, fp);
    while(!feof(fp)){
        if(prestamo.idUsuario == cod){
            libro = consultarLibro(prestamo.idLibro); //ConsultarLibro() devuelve el registro de tipo libro con su info
            printf("ID: %i  Titulo: %s\n", libro.id, libro.titulo);
            prestado = 1;
        }
        fread(&prestamo, sizeof(Prestamos), 1, fp);
    }
   
    if(prestado != 1){
        printf("\nUsario no tiene prestamos o no existe");
        fread(&usuario, sizeof(Usuarios), 1, fileUsuario);
        while(!feof(fileUsuario)){
            if(usuario.id == cod){

                usuario.conPrestamo = 0;
                fseek(fileUsuario, -sizeof(Usuarios), SEEK_CUR);
                fwrite(&usuario, sizeof(Usuarios), 1, fileUsuario);
                break;
            }
            fread(&usuario, sizeof(Usuarios), 1, fileUsuario);
        }
        fclose(fp);
        fclose(fileUsuario);
        return 0;
    }
    fclose(fp);
    fclose(fileUsuario);
    return 1;
}
void sinLibrosPrestados(){
    Prestamos prestamo;
    Libros libro;
    Usuarios usuario;
    int prestado;
    FILE *fp = fopen("prestamos.bin", "rb");
    FILE *fileUsuario = fopen("usuarios.bin", "rb+");
    fread(&prestamo, sizeof(Prestamos), 1, fp);
    while(!feof(fp)){
        if(prestamo.idUsuario == ID_USUARIO){
            prestado = 1;
        }
        fread(&prestamo, sizeof(Prestamos), 1, fp);
    }
    if(prestado != 1){
        fread(&usuario, sizeof(Usuarios), 1, fileUsuario);
        while(!feof(fileUsuario)){
            if(usuario.id == ID_USUARIO){

                usuario.conPrestamo = 0;
                fseek(fileUsuario, -sizeof(Usuarios), SEEK_CUR);
                fwrite(&usuario, sizeof(Usuarios), 1, fileUsuario);
                break;
            }
            fread(&usuario, sizeof(Usuarios), 1, fileUsuario);
        }
        fclose(fp);
        fclose(fileUsuario);
    }
    fclose(fp);
    fclose(fileUsuario);

}
void prestarLibro(){
    
    int usuarioID = ID_USUARIO;
    int mora;
    int opc2;
    
    Libros libro;
    Usuarios usuario;
    FILE *fp = fopen("libros.bin", "rb+");
    FILE *fileUsuarios = fopen("usuarios.bin", "rb+");
    int cod;
    int valido;
    int existe;

    system("cls");
    listarLibros();
    printf("------------------------\n");
    printf("\n1: Tomar un libro");
    printf("\n0: Volver");
    printf("\nOpcion: ");
    scanf("%i", &opc2);

    if(opc2 == 1){

        mora = verificarEstadoMora(usuarioID);

        system("cls");
        if(mora == 0){
            
            listarLibros();
            printf("\nEscribe el codigo el libro que quieres tomar: \n");
            scanf("%i", &cod);
            rewind(fp);
            fread(&libro, sizeof(Libros), 1 ,fp);
            valido = validarPrestamo(cod);
            while (!feof(fp) && valido!=-1)
            {
                if(libro.id == cod){
                    existe = 1;
                }
                if(libro.id == cod && libro.disponibles > 0){
                    libro.disponibles = libro.disponibles-1;
                    fseek(fp, -(sizeof(Libros)), SEEK_CUR);
                    fwrite(&libro, sizeof(Libros), 1, fp);
                    registrarPrestamo(cod);
                    valido = 1;
                    break;
                }
                else{
                    valido = 0;
                }
                fread(&libro, sizeof(Libros), 1 ,fp);
            }

            if(existe == 1){

                if(valido==1){
                    fread(&usuario, sizeof(Usuarios), 1 ,fileUsuarios);
                    while (!feof(fileUsuarios))
                    {
                        if(usuario.id == ID_USUARIO){
                            usuario.conPrestamo = 1;
                            fseek(fileUsuarios, -sizeof(Usuarios), SEEK_CUR);
                            fwrite(&usuario, sizeof(Usuarios), 1 , fileUsuarios);
                            break;
                        }
                        fread(&usuario, sizeof(Usuarios), 1 ,fileUsuarios);
                    }
                    printf("\nPrestamo realizado\n");
                }
                else if (valido==0)
                {
                    printf("\nNo hay ejemplares disponibles\n");
                }
            }
            else if (valido==-1)
            {
                printf("\nYa tomaste este libro\n");
            }
            else{
                printf("\nLibro no existe\n");
            }
        }
        else if(mora == 1){
            printf("\nTienes libros vencidos. Devuelve tus libros vencidos\n");
        }
        getch();
    }
    fclose(fp);
    fclose(fileUsuarios);
        
}

int verificarEstadoMora(int usuarioID){
    Usuarios usuario;
    FILE *fp = fopen("usuarios.bin", "rb");

    fread(&usuario, sizeof(Usuarios), 1, fp);
    while(!feof(fp)){
        if(usuario.id == usuarioID){
            fclose(fp);
            return usuario.mora;
        }
        fread(&usuario, sizeof(Usuarios), 1, fp);
    }
}
void eliminarPrestamo(int idUsuario, int codLibro){
    Prestamos prestamo;
    FILE *presta = fopen("prestamos.bin", "rb+");
    FILE *temporalPresta = fopen("temporalPresta.bin", "wb");

    fread(&prestamo, sizeof(Prestamos), 1, presta);
    while(!feof(presta)){
        if(prestamo.idUsuario == idUsuario && prestamo.idLibro != codLibro){
            fwrite(&prestamo, sizeof(Prestamos), 1, temporalPresta);
        }
        if(prestamo.idUsuario != idUsuario){
            fwrite(&prestamo, sizeof(Prestamos), 1, temporalPresta);
        }
        fread(&prestamo, sizeof(Prestamos), 1, presta);
    }
    fclose(presta);
    fclose(temporalPresta);
    remove("prestamos.bin");
    rename("temporalPresta.bin", "prestamos.bin");
}
int devolverLibro(int idUsuario, int codLibro){
    Libros libro;
    int existe;
    FILE *fp = fopen("libros.bin", "rb+");
    fread(&libro, sizeof(Libros), 1, fp);
    while(!feof(fp)){
        if(libro.id == codLibro){
            libro.disponibles = libro.disponibles + 1;
            fseek(fp, -sizeof(Libros), SEEK_CUR);
            fwrite(&libro, sizeof(Libros), 1, fp);
            existe = 1;
            break;
        }
        else{
        }
        fread(&libro, sizeof(Libros), 1, fp);
    } 
    if(existe != 1){
        printf("\nNo tienes ese libro\n");
        getch();
        return 0;
    }
    else{
        fclose(fp);
        return 1;
        // eliminarPrestamo(idUsuario, codLibro);
    }
    
    fclose(fp);
}
