/***************************************************
*	Interaccion.cpp
*
*	Manejo de eventos producidos por el usuario
*
*	@author	R.Vivo' <rvivo@upv.es>
*	@date Oct,2023
*
***************************************************/
#define PROYECTO "Interaccion"

#include <iostream>	
#include <codebase.h>

using namespace std;
using namespace cb;

// Globales
static enum {ALAMBRICO,SOLIDO,DOBLE} modo;
//static float girox = 0, giroy = 0;
static float giroxTV = 0, giroyTV = 0, giroxTA = 0, giroyTA = 0;
static int xanterior, yanterior;
static GLubyte objeto[1];

void onMenu(int valor)
{
	if (valor == 0) modo = ALAMBRICO;
	else if (valor == 1) modo = SOLIDO;
	else modo = DOBLE;

	glutPostRedisplay();
}

// Funcion de inicializacion propia
void init()
{
	// Salida por consola
	cout << glGetString(GL_VERSION) << endl;

	// Menu sencillo de popup
	glutCreateMenu(onMenu);
	glutAddMenuEntry("ALAMBRICO", 0);
	glutAddMenuEntry("SOLIDO", 1);
	glutAddMenuEntry("DOBLE", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	// Configuracion del motor de render
	glEnable(GL_DEPTH_TEST);

}

// Funcion de atencion al evento de dibujo
void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Situacion y orientacion de la camara
	gluLookAt(3, 3, 3, 0, 0, 0, 0, 1, 0);

	ejes();

	//glRotatef(girox, 1, 0, 0);
	//glRotatef(giroy, 0, 1, 0);

	// Tetera verde
	glPushMatrix();
	glColor3f(0, 1, 0);
	glRotatef(giroxTV, 1, 0, 0);
	glRotatef(giroyTV, 0, 1, 0);
	if(modo==ALAMBRICO)
		glutWireTeapot(0.5);
	else if(modo==SOLIDO)
		glutSolidTeapot(0.5);
	else {
		glutSolidTeapot(0.5);
		glColor3f(1, 1, 1);
		glutWireTeapot(0.51);
	}
	glPopMatrix();

	// Tetera azul
	glTranslatef(0, -1, 0);
	glRotatef(giroxTA, 1, 0, 0);
	glRotatef(giroyTA, 0, 1, 0);
	glColor3f(0, 0, 1);
	if (modo == ALAMBRICO)
		glutWireTeapot(0.5);
	else if (modo == SOLIDO)
		glutSolidTeapot(0.5);
	else {
		glutSolidTeapot(0.5);
		glColor3f(1, 1, 1);
		glutWireTeapot(0.51);
	}


	glutSwapBuffers();
}

void select()
{
	// Dibuja la escena en el backbuffer sin mostrar
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Situacion y orientacion de la camara
	gluLookAt(3, 3, 3, 0, 0, 0, 0, 1, 0);

	// Tetera verde
	glPushMatrix();
	glColor3f(0, 1, 0);
	glRotatef(giroxTV, 1, 0, 0);
	glRotatef(giroyTV, 0, 1, 0);
	glutSolidTeapot(0.5);
	glPopMatrix();

	// Tetera azul
	glTranslatef(0, -1, 0);
	glRotatef(giroxTA, 1, 0, 0);
	glRotatef(giroyTA, 0, 1, 0);
	glColor3f(0, 0, 1);
	glutSolidTeapot(0.5);
}

// Funcion de atencion al redimensionamiento
void reshape(GLint w, GLint h)
{
	float ra = (float)w / h;
	glViewport(0, 0, w, h);

	// Elegir la camara
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/*
	if (ra > 1)
		glOrtho(-2 * ra, 2 * ra, -2, 2, -1,1 );
	else
		glOrtho(-2, 2, -2 / ra, 2 / ra, -1, 1);
	*/

	gluPerspective(30, ra, 0.1, 10);
}

void onKey(unsigned char tecla, int x, int y)
{
	switch (tecla) {
	case 'a':
		modo = ALAMBRICO;
		break;
	case 's':
		modo = SOLIDO;
		break;
	case 'd':
		modo = DOBLE;
		break;
	case 27:
		exit(0);
	}

	glutPostRedisplay();

}

void onClick(int boton, int estado, int x, int y)
{
	// Registro la posicion del cursor
	if (boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {
		xanterior = x;
		yanterior = y;
	}

	// Converto las coordenadas del pixel en coordenadas del viewport
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLint vx = x;
	GLint vy = (viewport[3] - y);
	select();
	glReadPixels(vx, vy, 1, 1, GL_BLUE, GL_UNSIGNED_BYTE, objeto);
}

void onDrag(int x, int y)
{
	static float pixel2grados = 1;

	/*
	// Al mover el raton hacia la derecha la x aumenta y el giro es 
	// alrededor del eje Y positivo
	giroy += (x - xanterior) * pixel2grados;

	// Al mover el raton hacia abajo la y aumenta y el giro es
	// alrededor del eje X positivo
	girox += (y - yanterior) * pixel2grados;
	*/

	if (objeto[0] == 0x00) { // Tetera verde
		giroyTV += (x - xanterior) * pixel2grados;
		giroxTV += (y - yanterior) * pixel2grados;
	}
	else {
		giroyTA += (x - xanterior) * pixel2grados;
		giroxTA += (y - yanterior) * pixel2grados;
	}

	xanterior = x;
	yanterior = y;
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(50, 50);

	// Crear ventana
	glutCreateWindow(PROYECTO);

	// Registrar callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(onKey);
	glutMouseFunc(onClick);
	glutMotionFunc(onDrag);

	// Inicio propio y del bucle de eventos
	init();
	glutMainLoop();
}
