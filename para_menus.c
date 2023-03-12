#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>

#include "para_menus.h"

void tiempoActual(){ // Obtenemos la hora y fecha actuales //
    
    time_t actual = time(NULL); 
    struct tm *tm = localtime(&actual);
    anio= tm->tm_year+1900;
    mes = tm->tm_mon+1;
    dia = tm->tm_mday;
    hora = tm->tm_hour;
    min = tm->tm_min;
    seg = tm->tm_sec;
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
int listarUsuariosEnMora(){
    Usuarios usuario;
    int existe;
    FILE *fp = fopen("usuarios.bin", "rb");

    fread(&usuario, sizeof(Usuarios), 1, fp);
    while (!feof(fp))
    {
        if (usuario.mora == 1)
        {
            existe = 1;
            printf("ID: %i  Usuario: %s  Estado: %i   Con-Prestamo: %i  Mora: %i\n", usuario.id, usuario.nombre, usuario.estado, usuario.conPrestamo, usuario.mora);
        }
        fread(&usuario, sizeof(Usuarios), 1, fp);
    }
    fclose(fp);
    return existe;
}

int listarUsuariosConPrestamos(){
    Usuarios usuario;
    int existe;
    FILE *fp = fopen("usuarios.bin", "rb");

    fread(&usuario, sizeof(Usuarios), 1, fp);
    while (!feof(fp))
    {
        if(usuario.conPrestamo == 1){
            existe = 1;
            printf("ID: %i  Usuario: %s  Estado: %i Mora: %i\n", usuario.id, usuario.nombre, usuario.estado, usuario.mora);
        }
        fread(&usuario, sizeof(Usuarios), 1, fp);
    }
    fclose(fp);
    return existe;
    
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
    Prestamos prestamo;                      //Cuando se borra un libro se borran todos sus prestamos
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

int prestamosPorLibro(){ // Muestra los libros que tienen un numero de disponibles menor a su cantidad total
    Libros libro;        // de ejmplares (significa que alguien presto ese libro)
    int numPrestados;
    int existe;

    FILE *fp = fopen("libros.bin", "rb");
    fread(&libro, sizeof(Libros), 1, fp);
    while(!feof(fp)){
        if(libro.disponibles < libro.ejemplares){
            existe = 1;
            numPrestados = libro.ejemplares - libro.disponibles;
            printf("ID: %i  Titulo: %s - Prestados: %i\n", libro.id, libro.titulo, numPrestados);
        }
        fread(&libro, sizeof(Libros), 1, fp);
    }
    fclose(fp);
    return existe;
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

            if(strcmp(nuevoTitulo, "\n") != 0){ //Si se presiona Enter, entonces deja el titulo originar sin modificar

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


int registrarPrestamo(int cod){

    
    Prestamos prestamo;
    FILE *fp = fopen("prestamos.bin", "rb+");
    int id = 0;

    fread(&prestamo, sizeof(Prestamos), 1, fp);
    while (!feof(fp))
    {
        id = prestamo.id;
        fread(&prestamo, sizeof(Prestamos), 1, fp);
    }
    if(id != 0){
        prestamo.id = id + 1;
    }
    else{
        prestamo.id = 1;
    }

    /*  Registra la fecha en en el prestamo  */

    int resul = T_PRESTAMO / 60;
    int modulo = T_PRESTAMO % 60;
    int d = 0;
    int h = 0;
    int minutos = T_PRESTAMO;

    if(T_PRESTAMO >= 60 || T_PRESTAMO <= -60){
               
        d = 0;
        h = resul;
        minutos = modulo;

        if(h >= 24 || h <= -24){
            d = h / 24;
            h = h % 24;
        }
        
    }

    if(hora == 0 && h < 0){
        prestamo.hora = 24 + h;
    }
    if(hora == 23 && h > 0){
        prestamo.hora = h;
    }
    if(min == 0 && minutos < 0){
        prestamo.min = 60 + minutos;
    }
    if(min == 59 && minutos > 0){
        prestamo.min = minutos;
    }

    else{

        prestamo.idUsuario = ID_USUARIO;
        prestamo.idLibro = cod;
        prestamo.anio = anio;
        prestamo.mes = mes;
        prestamo.dia = dia + d;
        prestamo.hora = hora + h;
        prestamo.min = min + minutos;
        prestamo.seg = seg;
        prestamo.vencido = 0;
    }

    if(prestamo.anio < 1 || (prestamo.mes < 1 || prestamo.mes > 12) || (prestamo.dia < 1 || prestamo.dia > 31)){

        printf("\nFecha NO valida. Prestamo cancelado Vence: %.2d/%.2d/%.2d\n", prestamo.anio, prestamo.mes, prestamo.dia);
        fclose(fp);
        return -2;
    }
    else{
        
        fwrite(&prestamo, sizeof(Prestamos), 1, fp);
        fclose(fp);
        return 1;
    }

}

void validarFechaEntrega(){//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    Prestamos prestamo;
    Usuarios usuario;
    int demora;
    int mora;
    int existio;
    int lengS = 0;

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
        FILE *filePresta = fopen("prestamos.bin", "rb");

        existio = 0;
        if(!feof(filePresta)){
            rewind(filePresta);
            fread(&prestamo, sizeof(Prestamos), 1, filePresta);
        }
        while (!feof(filePresta)) //cada iteracion del for es el id de un usuario que 
        {                         // en el bucle while verificamos si tiene algun prestamo registrado
            demora = 0;
            mora = -1;

            if(prestamo.idUsuario == i){
                existio = 1;
                mora = 0;

                /* El siguiente bloque de código es la validacion de la fecha de entrega con la 
                fecha actual */
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
                        else if(prestamo.dia == dia){
                            if(prestamo.hora < hora){
                                demora = 1;
                            }
                            else if(prestamo.hora == hora){
                                if(prestamo.min < min){
                                    demora = 1;
                                }
                                else if(prestamo.min == min){
                                    if(prestamo.seg < seg){
                                        demora = 1;
                                    }
                                }
                            }
                        }
                    }
                }
                /* Fin de la validacion del plazo de entrega */
                
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
                            prestamoVencido(usuario.id, prestamo.id);
                            
                            break;
                        }
                        fread(&usuario, sizeof(Usuarios), 1, fileUsuario);
                    }
                }
            }
                

            fread(&prestamo, sizeof(Prestamos), 1, filePresta);
        }
        if(!feof(filePresta)){
            rewind(filePresta);
        }

        if (demora != 1){ // Usuario No tiene prestamos vencidos
            rewind(fileUsuario);
            fread(&usuario, sizeof(Usuarios), 1, fileUsuario);
            while (!feof(fileUsuario))
            {
                if(usuario.id == i){

                    fseek(fileUsuario, -sizeof(Usuarios), SEEK_CUR);
                    usuario.mora = 0;
                    fwrite(&usuario, sizeof(Usuarios), 1, fileUsuario); 

                    break;
                }
                fread(&usuario, sizeof(Usuarios), 1, fileUsuario);
            }
            rewind(fileUsuario);
        }
        
        if(mora == 0 && !feof(fileUsuario)){    //Este usuario tiene prestamos registrados y alguno sin vencer
            rewind(fileUsuario);
            fread(&usuario, sizeof(Usuarios), 1, fileUsuario);
            while (!feof(fileUsuario))
            {
                if(usuario.id == i && usuario.mora != 1){ // Este usuario tiene prestamos y ninguno vencido
                    
                    usuario.mora = 0;
                    fseek(fileUsuario, -sizeof(Usuarios), SEEK_CUR);
                    fwrite(&usuario, sizeof(Usuarios), 1, fileUsuario); 
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
                    rewind(fileUsuario);
                    break;
                }
                fread(&usuario, sizeof(Usuarios), 1, fileUsuario);
            }
            rewind(fileUsuario);
            
        }
        fclose(filePresta);
    }
    
   fclose(fileUsuario);
}


void prestamoVencido(int idUsuario, int idPrestamo){ //cambia el estado del prestamo a vencido 
    Prestamos prestamo;
    Usuarios usuario;
    int vence;
    FILE *fPrestamo = fopen("prestamos.bin", "rb+");
    FILE *fUsuarios = fopen("usuarios.bin", "rb");

    fread(&usuario, sizeof(Usuarios), 1, fUsuarios);
    while (!feof(fUsuarios))
    {
        if(usuario.id == idUsuario && usuario.mora == 1){
            vence = 1;
            break;
        }
        fread(&usuario, sizeof(Usuarios), 1, fUsuarios);
    }

    if(vence == 1){
        fread(&prestamo, sizeof(Prestamos), 1, fPrestamo);
        while (!feof(fPrestamo))
        {
            if (prestamo.id == idPrestamo)
            {
                
                prestamo.vencido = 1;
                fseek(fPrestamo, -sizeof(Prestamos), SEEK_CUR);
                fwrite(&prestamo, sizeof(Prestamos), 1, fPrestamo);
                break;
            }
            fread(&prestamo, sizeof(Prestamos), 1, fPrestamo);
        }
    }
    fclose(fPrestamo);
    fclose(fUsuarios);
}


/*  "validarPrestamo"  Determina si el usuario ya presto un determinado libro.
El id del libro se pasa por parametro, esta validacion sirve para evitar que un usuario
preste el mismo libro más de una vez        */ 

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
    char vence[40];
    FILE *fp = fopen("prestamos.bin", "rb");
    FILE *fileUsuario = fopen("usuarios.bin", "rb+");
    
    fread(&prestamo, sizeof(Prestamos), 1, fp);
    while(!feof(fp)){
        strcpy(vence, "");
        if(prestamo.idUsuario == cod){
            if(prestamo.vencido == 1){
                strcpy(vence, "VENCIDO");
            }
            libro = consultarLibro(prestamo.idLibro); //ConsultarLibro() devuelve el registro de tipo libro con su info
            printf("\nID: %i  Titulo: %s  %s \nVence: %.2d/%.2d/%.2d  %.2d:%.2d:%.2d\n", 
            libro.id, libro.titulo, vence, prestamo.anio, prestamo.mes, 
            prestamo.dia, prestamo.hora, prestamo.min, prestamo.seg);
            
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


void sinLibrosPrestados(){ //Cambia el estado de prestamo del usuario si no tiene libros prestados
    
    Prestamos prestamo;
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
    int cod;
    int valido;
    int existe;
    
    Libros libro;
    Usuarios usuario;

    FILE *fp = fopen("libros.bin", "rb+");
    FILE *fileUsuarios = fopen("usuarios.bin", "rb+");

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
                    valido = registrarPrestamo(cod);
                    if(valido != -2){
                        libro.disponibles = libro.disponibles-1;
                        fseek(fp, -(sizeof(Libros)), SEEK_CUR);
                        fwrite(&libro, sizeof(Libros), 1, fp);
                    }
                    break;
                }
                else{
                    valido = 0;
                }
                fread(&libro, sizeof(Libros), 1 ,fp);
            }

            if(existe == 1){

                if(valido == 1){
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
                else if (valido == 0)
                {
                    printf("\nNo hay ejemplares disponibles\n");
                }
            }
            else if (valido == -1)
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

int verificarEstadoMora(int usuarioID){ //Devuelve el estado de mora del usuario "0" sin libros vencidos
    Usuarios usuario;                   // "1" con libros vencidos
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

int devolverLibro(int idUsuario, int codLibro){ //Cuando se devuelve un libro, la cantidad de libros disponibles
    Libros libro;                               //aunmenta +1
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
        fclose(fp);
        return 0;
    }
    else{
        fclose(fp);
        return 1;
    }
    
    fclose(fp);
}


/* ++++++++++FUNCIONES EXTRA++++++++++++++ */

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
