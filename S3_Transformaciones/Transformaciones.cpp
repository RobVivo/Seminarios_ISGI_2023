/***************************************************
*	Transformaciones.cpp
*
*	Dibujar una campo de flores con transformaciones
*
*	@author	R.Vivo' <rvivo@upv.es>
*	@date Oct,2023
*
***************************************************/
#define PROYECTO "Transformaciones"

#include <iostream>	
#include <codebase.h>

using namespace std;
using namespace cb;

// Globales
static GLuint petalo, corola, flor;

// Funcion de inicializacion propia
void init()
{
	// Salida por consola
	cout << glGetString(GL_VERSION) << endl;

	// Petalo blanco
	petalo = glGenLists(1);
	glNewList(petalo, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1, 1, 1);
	glPushMatrix();
	glScalef(0.15, 0.5, 0.15);
	glutSolidSphere(1,20,20);
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// Corola amarilla y petalos blancos
	corola = glGenLists(1);
	glNewList(corola, GL_COMPILE);

	for (int i = 0; i < 12; i++) {
		glPushMatrix();
		glRotatef(i * 30, 0, 0, 1);
		glTranslatef(0, 0.25, 0);
		glScalef(0.5, 0.5, 0.5);
		glCallList(petalo);
		glPopMatrix();

	}

	glColor3f(1, 1, 0);
	glPushMatrix();
	glScalef(0.2, 0.2, 0.2);
	glutSolidSphere(1, 20, 20);
	glPopMatrix();

	glEndList();

	// Flor
	flor = glGenLists(1);
	glNewList(flor, GL_COMPILE);

	glColor3f(0, 1, 0);

	glPushMatrix();
	glTranslatef(0, -0.125, 0);
	glScalef(0.05, 0.75, 0.01);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.25, 0);
	glRotatef(12, 0, 0, 1);
	glScalef(0.5, 0.5, 0.5);
	glCallList(corola);
	glPopMatrix();

	glEndList();

}

// Funcion de atencion al evento de dibujo
void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for (int i = 0; i < 50; i++) {
		glPushMatrix();
		glTranslatef((rand() % 200 - 100) / 100.0,
			(rand() % 100 - 50) / 100.0,0);
		glScalef(0.5, 0.5, 0.5);
		glCallList(flor);
		glPopMatrix();
	}

	glFlush();
}

// Funcion de atencion al redimensionamiento
void reshape(GLint w, GLint h)
{
}

int main(int argc, char** argv)
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(50, 50);

	// Crear ventana
	glutCreateWindow(PROYECTO);

	// Registrar callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	// Inicio propio y del bucle de eventos
	init();
	glutMainLoop();
}
