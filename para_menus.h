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

int ID_USUARIO; //Se le asigna valor cuando el usuario se loguea

/* Se les asigna la fecha actual */
int anio;
int mes;
int dia;

void registrarUsuario();
void listarUsuarios();
int listarUsuariosConPrestamos();
int verificarUsuariosRepetidos(char nick[40]);
void modificarEstadoUsuario();
void registrarLibro();
void listarLibros();
void prestarLibro();
int verificarEstadoMora(int usuarioID);
int validarPrestamo(int cod);
int registrarPrestamo(int cod);
Libros consultarLibro(int cod);
int listarLibrosPrestados(int cod); //Lista de libros prestados por el usuario
void editarLibro(int cod);
void eliminarPrestamo(int idUsuario, int codLibro);
int devolverLibro(int idUsuario, int codLibro);
int prestamosPorLibro();//Lista de libros que han sido prestados por la biblioteca 
void eliminarPrestamoPorLibro(int codLibro);
void eliminarLibro(int codLibro);
int listarUsuariosEnMora();
void usariosPrestaronEseLibro(int codLibro);
void validarFechaEntrega();
void sinLibrosPrestados();