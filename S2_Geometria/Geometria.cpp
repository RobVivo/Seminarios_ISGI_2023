/***************************************************
*	Geometria.cpp
*
*	Dibujar un pentagono de tres modos diferentes
*
*	@author	R.Vivo' <rvivo@upv.es>
*	@date Oct,2023
* 
***************************************************/
#define PROYECTO "Geometria"

#include <iostream>	
#include <codebase.h>

using namespace std;
using namespace cb;

// Globales
static const GLfloat radio = 0.5f;
static GLuint pentaList;
static GLfloat coordenadas[15];
static const GLfloat colores[15] = { 1,1,1, 1,0,1, 0,1,1, 1,1,0, 0,0,1 };
static const GLuint pentagono[5] = { 0,1,2,3,4 };

// Funcion de inicializacion propia
void init()
{
	// Salida por consola
	cout << glGetString(GL_VERSION) << endl;

	/*
	// Construir una lista que dibuje un pentagono
	pentaList = glGenLists(1);
	glNewList(pentaList, GL_COMPILE);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 5; i++) {
		glVertex3f(radio * cosf(i * 2 * PI / 5), radio * sinf(i * 2 * PI / 5), 0);
	}
	glEnd();
	glEndList();
	*/

	// Construir los buffers
	for (int i = 0; i < 5; i++) {
		coordenadas[3*i] = radio * cosf(i * 2 * PI / 5);           //x
		coordenadas[3 * i + 1] = radio * sinf(i * 2 * PI / 5);     //y
		coordenadas[3 * i + 2] = 0;                                //z
	}
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, coordenadas);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, colores);
}

// Funcion de atencion al evento de dibujo
void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	/*
	// Dibujar tetera dos veces
	glColor3f(0, 0, 1);
	glutSolidTeapot(0.5);
	glColor3f(1, 1, 0);
	glutWireTeapot(0.51);
	*/

	/*
	// pentagono regular en modo inmediato
	glColor3f(0, 1, 0);
	glPolygonMode(GL_FRONT, GL_FILL);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 5; i++) {
		glVertex3f(radio * cosf(i * 2 * PI / 5), radio * sinf(i * 2 * PI / 5), 0);
	}
	glEnd();
	glColor3f(1, 1, 1);
	glPolygonMode(GL_FRONT, GL_POINT);
	glLineWidth(4);
	glPointSize(5);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 5; i++) {
		glVertex3f(radio * cosf(i * 2 * PI / 5), radio * sinf(i * 2 * PI / 5), 0);
	}
	glEnd();
	*/

	/*
	// Dibujar el pentagono usando display list
	glColor3f(0, 1, 0);
	glPolygonMode(GL_FRONT, GL_FILL);
	glCallList(pentaList);
	glColor3f(1, 1, 1);
	glPolygonMode(GL_FRONT, GL_LINE);
	glLineWidth(4);
	glCallList(pentaList);
	*/

	glColor3f(0, 1, 0);

	glDrawElements(GL_POLYGON, 5, GL_UNSIGNED_INT, pentagono);

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
