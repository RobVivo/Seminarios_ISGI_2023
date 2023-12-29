/***************************************************
*	Mezcla.cpp
*
*	Uso del blending y la niebla en OpenGL
*
*	@author	R.Vivo' <rvivo@upv.es>
*	@date Dic,2023
*
***************************************************/
#define PROYECTO "Mezcla"

#include <iostream>	
#include <codebase.h>

using namespace std;
using namespace cb;

// Globales
static enum { ALAMBRICO, SOLIDO, DOBLE } modo;
//static float girox = 0, giroy = 0;
static float giroxTV = 0, giroyTV = 0, giroxTA = 0, giroyTA = 0;
static int xanterior, yanterior;
static GLubyte objeto[1];

// Ids de texturas
static GLuint erre, nubes, rayas, ventana;

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

	// LUCES
	GLfloat A[] = { 1, 1, 1, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, A);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, GRISOSCURO);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, BLANCO);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 12);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 70);

	// TEXTURAS
	glGenTextures(1, &erre);
	glBindTexture(GL_TEXTURE_2D, erre);
	loadImageFile((char*)"R.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glGenTextures(1, &nubes);
	glBindTexture(GL_TEXTURE_2D, nubes);
	loadImageFile((char*)"nubes.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glGenTextures(1, &rayas);
	glBindTexture(GL_TEXTURE_2D, rayas);
	loadImageFile((char*)"rayas.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glGenTextures(1, &ventana);
	glBindTexture(GL_TEXTURE_2D, ventana);
	loadImageFile((char*)"ventana.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Configuracion del motor de render
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_FOG);
	glFogfv(GL_FOG_COLOR, BLANCO);
	glFogf(GL_FOG_DENSITY, 0.2);


}

// Funcion de atencion al evento de dibujo
void display()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, nubes);
	//texturarFondo();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Situar luces fijas a la camara
	GLfloat PL0[] = { 0,0,1,0 };
	glLightfv(GL_LIGHT0, GL_POSITION, PL0);

	// Situacion y orientacion de la camara
	gluLookAt(3, 3, -3, 0, 0, 0, 0, 1, 0);

	ejes();

	// Situar luces fijas en la escena
	GLfloat PL1[] = { 0,5,0,1 };
	glLightfv(GL_LIGHT1, GL_POSITION, PL1);
	GLfloat DL1[] = { 0,-1,0 };
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, DL1);

	//glRotatef(girox, 1, 0, 0);
	//glRotatef(giroy, 0, 1, 0);


	// Suelo
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glBindTexture(GL_TEXTURE_2D, erre);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	GLfloat Dm_S[] = { 0.5,0.7,0.6,1.0 };
	GLfloat s_S = 20.0;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Dm_S);

	if (modo == ALAMBRICO) glPolygonMode(GL_FRONT, GL_LINE);
	else glPolygonMode(GL_FRONT, GL_FILL);


	GLfloat v0[] = { -1,0,1 };
	GLfloat v1[] = { 1,0,1 };
	GLfloat v2[] = { 1,0,-1 };
	GLfloat v3[] = { -1,0,-1 };
	quadtex(v0, v1, v2, v3, 0, 2, 0, 2, 100, 100);
	glPopAttrib();


	/*
	glBegin(GL_POLYGON);
	glNormal3f(0, 1, 0);
	glTexCoord2f(2, 0);
	glVertex3f(-1, 0, 1);
	glTexCoord2f(0, 0);
	glVertex3f(1, 0, 1);
	glTexCoord2f(0, 2);
	glVertex3f(1, 0, -1);
	glTexCoord2f(2, 2);
	glVertex3f(-1, 0, -1);
	glEnd();
	*/


	// Donut
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, rayas);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	static float planoS[] = { 1,0,0,0 };
	static float planoT[] = { 0,1,0,0 };
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, planoS);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, planoT);

	GLfloat s_TA = 40.0;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, BRONCE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ORO);
	glMaterialf(GL_FRONT, GL_SHININESS, s_TA);
	glTranslatef(-0.5, 0.5, 0);
	glRotatef(giroxTA, 1, 0, 0);
	glRotatef(giroyTA, 0, 1, 0);
	glColor3f(0, 0, 1);

	glutSolidTorus(0.2, 0.5, 20, 20);
	glPopMatrix();
	glPopAttrib();

	// Objetos traslucidos --------------------------------------------------

	glDepthMask(GL_FALSE);

	// Tetera 
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glCullFace(GL_FRONT);
	glBindTexture(GL_TEXTURE_2D, nubes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	GLfloat Dm_TV[] = { 0.5,0.7,0.6,0.5 };
	GLfloat Sm_TV[] = { 0.8,0.8,0.8,1.0 };
	GLfloat s_TV = 20.0;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Dm_TV);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Sm_TV);
	glMaterialf(GL_FRONT, GL_SHININESS, s_TV);
	glPushMatrix();
	glColor3f(0, 1, 0);
	glTranslatef(0.5, 0.5, 0);
	glRotatef(giroxTV, 1, 0, 0);
	glRotatef(giroyTV, 0, 1, 0);
		glutSolidTeapot(0.5);
	glPopMatrix();
	glPopAttrib();

	// Colocar una imagen pegada a la camara con canal alfa
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	static GLfloat c0[] = { -1,-1,0 };
	static GLfloat c1[] = { 1,-1,0 };
	static GLfloat c2[] = { 1,1,0 };
	static GLfloat c3[] = { -1,1,0 };
	glBindTexture(GL_TEXTURE_2D, ventana);
	quadtex(c0, c1, c2, c3, 0, 1, 0, 1, 1, 1);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();


	glDepthMask(GL_TRUE);

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

	// Apilar el estado de OpenGL
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDisable(GL_LIGHTING);

	// Tetera verde
	glPushMatrix();
	glColor3f(0, 1, 0);
	glTranslatef(0.5, 0.5, 0);
	glRotatef(giroxTV, 1, 0, 0);
	glRotatef(giroyTV, 0, 1, 0);
	glutSolidTeapot(0.5);
	glPopMatrix();

	// Tetera azul
	glTranslatef(-0.5, 0.5, 0);
	glRotatef(giroxTA, 1, 0, 0);
	glRotatef(giroyTA, 0, 1, 0);
	glColor3f(0, 0, 1);
	glutSolidTeapot(0.5);

	glPopAttrib();
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
	FreeImage_Initialise();
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
	FreeImage_DeInitialise();
}
