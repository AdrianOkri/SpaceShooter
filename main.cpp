// *******************  Incluir librerías  *******************
#include <iostream>
#include <stdio.h>
#include <windows.h> // Poder mandar la posición al imprimir un valor
#include <conio.h>
// *******************  Constantes  *******************
#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80
// ******************* Clases *******************
// Clase que creará la nave, y todas sus ejecuciones
class NAVE {
	int x, y, salud, vidas; // Privado
	
	public:
		NAVE(int _x, int _y, int _salud, int _vidas): x(_x), y(_y), salud(_salud), vidas(_vidas) {} // El valor x = _x & y = _y
		void Pintar();
		void Borrar();
		void Mover();
		void PintarSalud();
		void QuitarSalud() { salud--; }
		void Explotar();
		int X() { return x; } // Nos regresará el valor de x
		int Y() { return y; } // Nos regresará el valor de y
};
// Clase que creará el asteroide, su posición inicial, su forma y el choque contra la nave
class ASTEROIDE {
	int x, y;
	
	public:
		ASTEROIDE(int _x, int _y): x(_x), y(_y) {}	
		void Crear();
		void Mover();
		void Colisionar(class NAVE &N);
};
// *******************  Funciones Normales  *******************
// Función que modificará la consola, para que puedas manipular las posiciones en ella
void Gotoxy(int x, int y) {
	HANDLE hCon; // Recuperar el identificador de la consola, para poder modificarlo
	hCon = GetStdHandle(STD_OUTPUT_HANDLE); // Tomar el control de salida en la consola
	
	COORD dwPos; // Crear el objeto para las coordenas X and Y
	dwPos.X = x; // Coordinada en X
	dwPos.Y = y; // Coordinada en Y
	
	SetConsoleCursorPosition(hCon, dwPos); // Dar las estructurá de las coordenadas de la variable dwPos
}

void OcultarCursor() {
	HANDLE hCon; // Recuperar el identificador de la consola, para poder modificarlo
	hCon = GetStdHandle(STD_OUTPUT_HANDLE); // Tomar el control de salida en la consola
	
	CONSOLE_CURSOR_INFO cci; // Nos ayudará a controlar la apariencia del cursos
	cci.dwSize = 50;
	cci.bVisible = FALSE;
	
	SetConsoleCursorInfo(hCon, &cci); // Dar la estructurá de la apariencia del cursor
}
// Función que nos pintará un borde en la consola, para que la nave no salga del rango 
void LimitarConsola() { // visible del juego
	for (int i = 2; i < 78; i++) { // Pintar Arriba y Abajo (Eje x)
		Gotoxy(i, 3); printf ("%c", 205);
		Gotoxy(i, 23); printf ("%c", 205);
	}
	for (int i = 4; i < 23; i++) { // Pintar Izquierda y Derecha (Eje y)
		Gotoxy(2, i); printf("%c", 186);
		Gotoxy(77,i); printf("%c", 186);
	}
	// Esquinas
	Gotoxy(2,3); printf("%c", 201);
	Gotoxy(2,23); printf("%c", 200);
	Gotoxy(77,3); printf("%c", 187);
	Gotoxy(77,23); printf("%c",188);
}
// *******************  Funciones para las Clases  *******************
// Función que llamá a la clase NAVE y a su función Pintar
void NAVE::Pintar() {
	Gotoxy(x,y); printf("  %c", 94); // Imprimir el caracter ASCII con un valor númerico
	Gotoxy(x,y+1); printf(" %c%c%c", 40,207,41); // Definir la estructurá de la nave
	Gotoxy(x,y+2); printf("%c%c %c%c", 94,190,190,94); // y mostrarla en pantalla
}
// Función que llamá a la clase NAVE y a su función Borrar 
void NAVE::Borrar() { // Esto será para que al moverse la nave, la posición anterior sea borrada
	Gotoxy(x,y);   printf("        "); // Pintar espacios en blanco, es equivalente a borrar el carácter
	Gotoxy(x,y+1); printf("        ");
	Gotoxy(x,y+2); printf("        ");
}
// Función que llamá a la clase NAVE y a su función Mover 
void NAVE::Mover() {
	// kbhit() detactar una tecla
		if(kbhit()) {
			char tecla = getch(); // Guardar la letra presionada en la variable tecla
			
			Borrar(); // Eliminar el carácter de la posición anterior
			
			if (tecla == IZQUIERDA && x > 3) x--; // Condiciones que al ser igual a una tecla presionada y no salir del límite
			if (tecla == DERECHA && x + 6 < 77) x++; // Moverá las coordenas, lo qué hará que podamos
			if (tecla == ARRIBA && y > 4) y--; // mover nuestra nave	
			if (tecla == ABAJO && y + 3 < 23) y++;
			if (tecla == 'e') { salud--; PintarSalud(); }
			Pintar(); // Mostar la Nave
			
		}
}
// Función que nos mostrará las vidas del jugador en la parte superior de la consola
void NAVE::PintarSalud() {
	Gotoxy(50, 2); printf("Vidas: %d", vidas); // Mostrar la cantidad de vidas en total
	Gotoxy(64, 2); printf("Salud: ");
	Gotoxy(70, 2); printf("      ");
	for (int i = 0; i < salud; i++) { // Ciclo que nos muestra las barras de salud
		Gotoxy(70 + i, 2); printf("%c",178);
	}
}
// Función que al momento de que la Salud sea 0, te baje una vida y muestré, una animación de explotar
void NAVE::Explotar() {
	if(salud == 0) { // Si llegamos a perder todas las barras de salud, explotar y quitar vida
		Borrar(); // Borrar la nave
		Gotoxy(x,y); 	 printf("  **  "); // Efecto de explotar
		Gotoxy(x,y + 1); printf(" **** ");
		Gotoxy(x,y + 2); printf("  **  ");
		Sleep(200); // Esperar 200 mlSegundos para que el siguiente efecto aparezca
		
		Gotoxy(x,y); 	 printf("* ** * "); // Efecto de explotar
		Gotoxy(x,y + 1); printf(" ****  ");
		Gotoxy(x,y + 2); printf("* ** * ");
		Sleep(200);
		
		vidas--; // Bajar la vida y regresar los valores por defecto
		salud = 3;
		Borrar();
		PintarSalud();
		Pintar();
	}	
}
// Función para imprimir el carácter del asteroide
void ASTEROIDE::Crear() {
	Gotoxy(x,y); printf("%c", 187);
}
// Función que hará que el asteroide se mueva en el eje de las Y, y aparezca en una posición al azar
void ASTEROIDE::Mover() { // en el eje de las X
	Gotoxy(x,y); printf(" "); // Borrar posición anterior del asteroide
	y++;
	if (y > 22) {
		x = (rand() % 71) + 4; // Dar un número al azar al asteroide, que será la posición inicial
		y = 4;
	}
	Crear(); // Llamar al método de la propia clase ASTEROIDE
}
// Función que nos ayudará a detectar la posición del asteroide y de la nave, si están en la misma posición
void ASTEROIDE::Colisionar(class NAVE &N) { // Se tomorá como una colisión
	if (x >= N.X() && x < N.X() + 5 && y >= N.Y() && y <= N.Y() + 2) { // Si la posición son iguales, entonces:
		N.QuitarSalud(); // Reducir Salud
		N.Pintar(); // Pintar nuevamente la nave
		N.PintarSalud(); // Colocar la barra de salud creducida
		x = (rand() % 71) + 4; // Dar un número al azar al asteroide, que será la posición inicial
		y = 4;
	}
}
// *******************  Inicio  *******************
int main(int argc, char** argv) {
	OcultarCursor();
	LimitarConsola();
	
	NAVE N(10, 10, 3, 3); // Crear un objeto NAVE con las coordenadas 7,7 y con 3 vidas
	N.Pintar(); // Llamar a sus métodos del objeto NAVE
	N.PintarSalud();
	
	ASTEROIDE AST(10,4), AST2(4,8), AST3(14,10), AST4(15, 4); // Crear el objeto Asteroide
	
	bool gameOver = false; // Nos dirá cuando el juego haya terminado, deteniendo el ciclo while
	while(!gameOver) {
		AST.Mover(); AST.Colisionar(N);
		AST2.Mover(); AST2.Colisionar(N);
		AST3.Mover(); AST3.Colisionar(N);
		AST4.Mover(); AST4.Colisionar(N);
		N.Explotar();
		N.Mover();
		Sleep(30); // Detener la ejecución por 30 mlSegundos
	}
	return 0;
}
