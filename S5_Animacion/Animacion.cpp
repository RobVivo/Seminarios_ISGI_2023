/***************************************************
*	Animacion.cpp
*
*	Animacion coherente con el tiempo transcurrido
*
*	@author	R.Vivo' <rvivo@upv.es>
*	@date Oct,2023
*
***************************************************/
#define PROYECTO "Animacion"

#include <iostream>	
#include <sstream>
#include <codebase.h>

using namespace std;
using namespace cb;

// Globales
static float alfa = 0;
static float TASA_REFRESCO = 60; //FPS

// Funcion de inicializacion propia
void init()
{
	// Salida por consola
	cout << glGetString(GL_VERSION) << endl;

	// Configuracion del motor de render
	glEnable(GL_DEPTH_TEST);

}

// Funcion que cuenta los fotogramas por segundo calculados
// y los muestra en el titulo de la ventana
void FPS()
{
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	int ahora = glutGet(GLUT_ELAPSED_TIME);
	int tiempo_transcurrido = ahora - antes;
	static int fotogramas = 0;

	fotogramas++;

	if (tiempo_transcurrido >= 1000) {
		// Cambiar el titulo de la ventana
		stringstream titulo;
		titulo << "FPS=" << fotogramas;
		glutSetWindowTitle(titulo.str().c_str());
		fotogramas = 0;
		antes = ahora;
	}

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

	glPushMatrix();
	glRotatef(alfa, 0, 1, 0);
	glColor3f(0, 1, 0);
	glutSolidTeapot(0.5);
	glColor3f(1, 1, 1);
	glutWireTeapot(0.51);
	glPopMatrix();


	glTranslatef(0, -1, 0);
	glRotatef(alfa / 2, 0, 1, 0);
	glColor3f(0, 0, 1);
	glutSolidTeapot(0.5);
	glColor3f(1, 1, 1);
	glutWireTeapot(0.51);

	glutSwapBuffers();

	FPS();
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

void update()
{
	// Actualizacion de variables sin tener en cuenta el tiempo
	// alfa += 0.1;

	// Actualizacion dependiendo del tiempo transcurrido
	static const float vueltasxsegundo = 1;
	static int hora_anterior = glutGet(GLUT_ELAPSED_TIME);
	int hora_actual = glutGet(GLUT_ELAPSED_TIME);
	float tiempo_transcurrido = (hora_actual - hora_anterior) / 1000.0f;

	alfa += vueltasxsegundo * 360 /*grados por vuelta*/ * tiempo_transcurrido;

	hora_anterior = hora_actual;

	glutPostRedisplay();
}

// Atender al timer
void onTimer(int tiempo)
{
	glutTimerFunc(tiempo, onTimer, tiempo);
	update();
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
	//glutIdleFunc(update);
	glutTimerFunc(int(1000 / TASA_REFRESCO), onTimer, int(1000 / TASA_REFRESCO));

	// Inicio propio y del bucle de eventos
	init();
	glutMainLoop();
}
