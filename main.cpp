// *******************  Incluir librer�as  *******************
#include <iostream>
#include <stdio.h>
#include <windows.h> // Poder mandar la posici�n al imprimir un valor
#include <conio.h>
#include <list> // Poder manejar listas (Memoria din�mica)
#include <string>

using namespace std;
// *******************  Constantes  *******************
#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80
// ******************* Clases *******************
// Clase que crear� la nave, y todas sus ejecuciones
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
		int X() { return x; } // Nos regresar� el valor de x
		int Y() { return y; } // Nos regresar� el valor de y
		int V() { return vidas; }
};
// Clase que crear� el asteroide, su posici�n inicial, su forma y el choque contra la nave
class ASTEROIDE {
	int x, y;
	
	public:
		ASTEROIDE(int _x, int _y): x(_x), y(_y) {}	
		void Crear();
		void Mover();
		void Colisionar(NAVE &N);
		int X() { return x; } // Nos regresar� el valor de x
		int Y() { return y; } // Nos regresar� el valor de y
};
// Classe que nos crear� la bala que la nave va a disparar, destruir� los asteroides y sumar� puntos
class PROYECTIL {
	int x, y;
	
	public:
		PROYECTIL(int _x, int _y): x(_x), y(_y) {}
		void Crear();
		int X() { return x; }
		int Y() { return y; }
		bool Eliminar();
};
// *******************  Funciones Normales  *******************
// Funci�n que modificar� la consola, para que puedas manipular las posiciones en ella
void Gotoxy(int x, int y) {
	HANDLE hCon; // Recuperar el identificador de la consola, para poder modificarlo
	hCon = GetStdHandle(STD_OUTPUT_HANDLE); // Tomar el control de salida en la consola
	
	COORD dwPos; // Crear el objeto para las coordenas X and Y
	dwPos.X = x; // Coordinada en X
	dwPos.Y = y; // Coordinada en Y
	
	SetConsoleCursorPosition(hCon, dwPos); // Dar las estructur� de las coordenadas de la variable dwPos
}
// Funci�n que mostrar� un mensaje en pantalla dependiendo de las coordenadas
void Imprimir(int x, int y, string mesage) {
	Gotoxy(x,y); cout << mesage << endl;
}
void CrearInstrucciones() {
	int x = 90, y =  3;
	
	Imprimir(x, y, "==================");
	Imprimir(x, y + 2, "==================");
	Imprimir(x, y + 4,  "Instrucciones");
	Imprimir(x, y + 6,  "ARRIBA    = ^");
	Imprimir(x, y + 8,  "IZQUIERDA = <");
	Imprimir(x, y + 10, "DERECHA   = >");
	Imprimir(x, y + 12, "ABAJO     = v");
	Imprimir(x, y + 14, "DISPARAR  = a");	
	Imprimir(x, y + 16, "==================");
	Imprimir(x, y + 18, "==================");
}
void OcultarCursor() {
	HANDLE hCon; // Recuperar el identificador de la consola, para poder modificarlo
	hCon = GetStdHandle(STD_OUTPUT_HANDLE); // Tomar el control de salida en la consola
	
	CONSOLE_CURSOR_INFO cci; // Nos ayudar� a controlar la apariencia del cursos
	cci.dwSize = 50;
	cci.bVisible = FALSE;
	
	SetConsoleCursorInfo(hCon, &cci); // Dar la estructur� de la apariencia del cursor
}
// Funci�n que nos pintar� un borde en la consola, para que la nave no salga del rango 
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
// Funci�n que llam� a la clase NAVE y a su funci�n Pintar
void NAVE::Pintar() {
	Gotoxy(x,y); printf("  %c", 94); // Imprimir el caracter ASCII con un valor n�merico
	Gotoxy(x,y+1); printf(" %c%c%c", 40,207,41); // Definir la estructur� de la nave
	Gotoxy(x,y+2); printf("%c%c %c%c", 94,190,190,94); // y mostrarla en pantalla
}
// Funci�n que llam� a la clase NAVE y a su funci�n Borrar 
void NAVE::Borrar() { // Esto ser� para que al moverse la nave, la posici�n anterior sea borrada
	Imprimir(x, y, "        "); // Pintar espacios en blanco, es equivalente a borrar el car�cter
	Imprimir(x, y + 1, "        ");
	Imprimir(x, y + 2, "        ");
}
// Funci�n que llam� a la clase NAVE y a su funci�n Mover 
void NAVE::Mover() {
	// kbhit() detactar una tecla
		if(kbhit()) {
			char tecla = getch(); // Guardar la letra presionada en la variable tecla
			
			Borrar(); // Eliminar el car�cter de la posici�n anterior
			
			if (tecla == IZQUIERDA && x > 3) x--; // Condiciones que al ser igual a una tecla presionada y no salir del l�mite
			if (tecla == DERECHA && x + 6 < 77) x++; // Mover� las coordenas, lo qu� har� que podamos
			if (tecla == ARRIBA && y > 4) y--; // mover nuestra nave	
			if (tecla == ABAJO && y + 3 < 23) y++;
			if (tecla == 'e') { salud--; PintarSalud(); }
			Pintar(); // Mostar la Nave
			
		}
}
// Funci�n que nos mostrar� las vidas del jugador en la parte superior de la consola
void NAVE::PintarSalud() {
	Gotoxy(50, 2); printf("Vidas: %d", vidas); // Mostrar la cantidad de vidas en total
	Imprimir(64, 2, "Salud: ");
	Imprimir(70, 2, "      ");
	for (int i = 0; i < salud; i++) { // Ciclo que nos muestra las barras de salud
		Gotoxy(70 + i, 2); printf("%c",178);
	}
}
// Funci�n que al momento de que la Salud sea 0, te baje una vida y muestr�, una animaci�n de explotar
void NAVE::Explotar() {
	if(salud == 0) { // Si llegamos a perder todas las barras de salud, explotar y quitar vida
		Borrar(); // Borrar la nave
		Beep(300,200);
		Imprimir(x, y, "  **  "); // Efecto de explotar
		Imprimir(x, y + 1, " **** ");
		Imprimir(x, y + 2, "  **  ");
		Sleep(200); // Esperar 200 mlSegundos para que el siguiente efecto aparezca
		Imprimir(x, y, "* ** * ");
		Imprimir(x, y + 1, " ****  ");
		Imprimir(x, y + 2, "* ** * ");
		Sleep(200);
		
		vidas--; // Bajar la vida y regresar los valores por defecto
		salud = 3;
		Borrar();
		PintarSalud();
		Pintar();
	}	
}
// Funci�n para imprimir el car�cter del asteroide
void ASTEROIDE::Crear() {
	Gotoxy(x,y); printf("%c", 228);
}
// Funci�n que har� que el asteroide se mueva en el eje de las Y, y aparezca en una posici�n al azar
void ASTEROIDE::Mover() { // en el eje de las X
	Imprimir(x, y, " "); // Borrar posici�n anterior del asteroide
	y++;
	if (y > 22) {
		x = (rand() % 71) + 4; // Dar un n�mero al azar al asteroide, que ser� la posici�n inicial
		y = 4;
	}
	Crear(); // Llamar al m�todo de la propia clase ASTEROIDE
}
// Funci�n que nos ayudar� a detectar la posici�n del asteroide y de la nave, si est�n en la misma posici�n
void ASTEROIDE::Colisionar(NAVE &N) { // Se tomor� como una colisi�n
	if (x >= N.X() && x < N.X() + 6 && y >= N.Y() && y <= N.Y() + 2) { // Si la posici�n son iguales, entonces:
		N.QuitarSalud(); // Reducir Salud
		N.Borrar(); // Borrar todos los car�cters
		N.Pintar(); // Pintar nuevamente la nave
		N.PintarSalud(); // Colocar la barra de salud creducida
		x = (rand() % 71) + 4; // Dar un n�mero al azar al asteroide, que ser� la posici�n inicial
		y = 4;
	}
}
// Funci�n de la clase PROYECTIL, que nos crear� la bala y la animaci�n de movimiento
void PROYECTIL::Crear() {
	Imprimir(x - 1, y, "   "); // Borrar la posici�n anterior de la bala, para no dejar un rasto de car�cters
	y--;
	Imprimir(x - 1, y, ":::");
}
// Funci�n que nos ayudar� a eliminar un proyectil, una vez llegado al l�mite del mapa o al chocar con un asteroide
bool PROYECTIL::Eliminar() {
	if(y == 4) return true;
	return false;
}
// *******************  Inicio  *******************
int main(int argc, char** argv) {
	OcultarCursor();
	LimitarConsola();
	
	NAVE N(25, 20, 3, 3); // Crear un objeto NAVE con las coordenadas 7,7 y con 3 vidas
	N.Pintar(); // Llamar a sus m�todos del objeto NAVE
	N.PintarSalud();
	
	Imprimir(3, 2, "Space Shooter in C++");
	
	list <ASTEROIDE*> A;
	list <ASTEROIDE*>::iterator itA; // Nos ayudar� a recorrer la lista anterior
	
	for (int i = 0; i < 10; i++) { // Ciclo para crear los ASTEROIDES
		A.push_back(new ASTEROIDE(rand()%74 + 3, rand()%5 + 4));
	}
	
	list <PROYECTIL*> P; // Crear una lista de punteros de la clase PROYECTIL
	list <PROYECTIL*>::iterator it; // Nos ayudar� a recorrer la lista anterior
	CrearInstrucciones();
	bool gameOver = false; // Nos dir� cuando el juego haya terminado, deteniendo el ciclo while
	int puntaje = 0; // Cada que haya una colisi�n, se sumar� al puntaje
	while(!gameOver) {
		Gotoxy(30,2); printf("Puntaje: %d", puntaje);
		if(kbhit()) { // Si se presiona una tecla
			char t = getch(); // Guardar la letra presionada en la variable tecla
			// N.X() && N.Y() Son la posici�n actual de la nave
			if(t == 'a') {
				P.push_back(new PROYECTIL(N.X() + 2, N.Y() - 1)); // Crear o agregar un nuevo elemento al final de la lista
			} 
		}
		// Para una vez que hayamos creado el Proyectil, mover a todos
		for(it = P.begin(); it != P.end(); it++) { // |begin = inicio de la lista| |end = el �timo valor de la lista|
			(*it)->Crear(); // El puntero de la clase PROYECTIL, puede acceder al m�todo Crear para mandar la ejecuci�n
			if ((*it)->Eliminar()) { // Si la funci�n retorna true, eliminar la bala
				Gotoxy((*it)->X() - 1,(*it)->Y()); printf("   "); // Obtener la posici�n del proyectil y borrarlo
				delete(*it); // Eliminar el elemento de la lista
				it = P.erase(it); // Quitar un elemento, pero decir al iterator, que continue con la siguiente posici�n
			}
		}
		// Para una vez que hayamos creado el Asteroide, mover a todos
		for(itA = A.begin(); itA != A.end(); itA++) {
			(*itA)->Mover();
			(*itA)->Colisionar(N);
		}
		// Ciclo para recorrer ambas listas del Proyectil y del Asteroide, para calcular su posici�n y al momento de cumplir
		for(itA = A.begin(); itA != A.end(); itA++) { // la condici�n, ejecutar como si hubieran colisonado
			for(it = P.begin(); it != P.end(); it++) {
				if((*itA)->X() >= (*it)->X() - 1 && (*itA)->X() <= (*it)->X() + 1 && ((*itA)->Y() + 1 == (*it)->Y() || (*itA)->Y() == (*it)->Y())) {
					Imprimir((*it)->X() - 1, (*it)->Y(), "   "); // Eliminar bala
					delete(*it); it = P.erase(it); // Borrar el objeto de la lista
					
					A.push_back(new ASTEROIDE(rand()%74 + 3, 4)); // Crear un asteoride
					Imprimir((*itA)->X(), (*itA)->Y(), " "); // Eliminar asteroide en su �ltima posici�n
					delete(*itA); itA = A.erase(itA); // Borrar de la lista
					
					puntaje+=5;
				}
			}
		}
		// Si el n�mero de vidas es menor a 0, el juego ha terminado
		if (N.V() < 0) {
			gameOver = true;
			system("cls");
			Imprimir(35, 10, "Fin del Juego");
		}
		
		N.Explotar();
		N.Mover();
		Sleep(30); // Detener la ejecuci�n por 30 mlSegundos
	}
	return 0;
}
