
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glu32.lib")

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <gl/glut.h>
#include <cstring>

#define M_TWOPI 3.14159289


//Цилиндр и диск
GLUquadricObj *Cylinder;
GLUquadricObj *Disk;

struct tm *newtime;
time_t ltime;

GLfloat rx, ry, rz, angle;

//Освещение
GLfloat lightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat lightDiffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat lightPosition[] = { 5.0f, 25.0f, 15.0f, 1.0f };
GLfloat matSpeculiar[] = { 1.0, 1.0, 1.0, 1.0 };

static int viewState = 1;

void drawString(int x, int y, char *st)
{
	int l, i;
	l = strlen(st);
	glRasterPos3i(x, y, -1);
	for (i = 0; i < l; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]);
	}
}

static void deltaTime(int te)
{
	int timent;
	int i;

	rx = 30 * cos(angle);
	ry = 30 * sin(angle);
	rz = 30 * cos(angle);
	angle += 0.01;
	if (angle > M_TWOPI) angle = 0;

	glutPostRedisplay();
	glutTimerFunc(100, deltaTime, 1);
}

void Initialize(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	//Освещение
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);


	Cylinder = gluNewQuadric();
	gluQuadricDrawStyle(Cylinder, GLU_FILL);
	gluQuadricNormals(Cylinder, GLU_SMOOTH);
	gluQuadricOrientation(Cylinder, GLU_OUTSIDE);
	gluQuadricTexture(Cylinder, GL_TRUE);

	Disk = gluNewQuadric();
	gluQuadricDrawStyle(Disk, GLU_FILL);
	gluQuadricNormals(Disk, GLU_SMOOTH);
	gluQuadricOrientation(Disk, GLU_OUTSIDE);
	gluQuadricTexture(Disk, GL_TRUE);
}

void drawGear(void)
{
	int i;
	glPushMatrix();
	gluCylinder(Cylinder, 2.5, 2.5, 1, 16, 16);
	gluDisk(Disk, 0, 2.5, 32, 16);
	glTranslatef(0, 0, 1);
	gluDisk(Disk, 0, 2.5, 32, 16);
	glPopMatrix();
	for (i = 0; i < 8; i++) {
		glPushMatrix();
		glTranslatef(0.0, 0.0, 0.50);
		glRotatef((360 / 8) * i, 0.0, 0.0, 1.0);
		glTranslatef(3.0, 0.0, 0.0);
		glutSolidCube(1.0);
		glPopMatrix();
	}
}

void drawClock(GLfloat cx, GLfloat cy, GLfloat cz)
{

	int hourTicks, secondTicks;
	glPushMatrix();

	glTranslatef(cx, cy, cz);
	glRotatef(180, 1.0, 0.0, 0.0);

	glPushMatrix(); //Диск часов
	glTranslatef(0, 0, 1.0);
	gluDisk(Disk, 0, 6.75, 32, 16);

	glPopMatrix();

	glPushMatrix(); //Часовая стрелка
	glColor3f(.0, 0.5, 0.5);
	glTranslatef(0, 0, 0.0);
	glRotatef((360 / 12) * newtime->tm_hour + (360 / 60) * (60 / (newtime->tm_min + 1)), 0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, 2.0);
	drawGear();
	glPopMatrix();
	glRotatef(90, 1.0, 0.0, 0.0);
	gluCylinder(Cylinder, 0.75, 0, 4, 16, 16);
	glPopMatrix();

	glPushMatrix();//Минутная стрелка
	glColor3f(0.0, 0.8, 1.0);
	glTranslatef(0, 0, 0.0);
	glRotatef((360 / 60) * newtime->tm_min, 0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, 3.0);
	glScalef(0.5, 0.5, 1.0);
	drawGear();
	glPopMatrix();
	glRotatef(90, 1.0, 0.0, 0.0);
	gluCylinder(Cylinder, 0.5, 0, 6, 16, 16);
	glPopMatrix();

	glPushMatrix();//Секундная стрелка
	glColor3f(0.0, 1.0, 0.0);
	glTranslatef(0, 0, -0.0);
	glRotatef((360 / 60) * newtime->tm_sec, 0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, 4.0);
	glScalef(0.25, 0.25, 1.0);
	drawGear();
	glPopMatrix();
	glRotatef(90, 1.0, 0.0, 0.0);
	gluCylinder(Cylinder, 0.25, 0, 6, 16, 16);
	glPopMatrix();


	for (hourTicks = 0; hourTicks < 12; hourTicks++)
	{
		glPushMatrix();
		glColor3f(0.0, .5, 1.0);
		glTranslatef(0.0, 0.0, 0.0);
		glRotatef((360 / 12) * hourTicks, 0.0, 0.0, 1.0);
		glTranslatef(6.0, 0.0, 0.0);
		glutSolidCube(1.0);

		glPopMatrix();
	}
	for (secondTicks = 0; secondTicks < 60; secondTicks++)
	{
		glPushMatrix();
		glTranslatef(0.0, 0.0, 0.0);
		glRotatef((360 / 60) * secondTicks, 0.0, 0.0, 1.0);
		glTranslatef(6.0, 0.0, 0.0);
		glutSolidCube(0.25);
		glPopMatrix();
	}
	glPopMatrix();
}


void Display(void)
{

	time(&ltime); //Получаем системное время
	newtime = localtime(&ltime); //Переводим в местное время

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Показываем текст времени на экране
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-8.0, 8.0, -8.0, 8.0, 1.0, 60.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);

	glColor3f(1.0, 1.0, 1.0);
	//Смена режима камеры
	if (viewState == 0)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(60.0, 1, 1.0, 60.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(rx, 0.0, rz, 0.0, 0.0, -14.0, 0, 1, 0);
	}
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	drawClock(0.0, 0.0, -14.0);

	glutSwapBuffers();
}

void Resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void onKeyPress(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'v':
		viewState = abs(viewState - 1);
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(10, 10);
	glutCreateWindow(argv[0]);
	glutSetWindowTitle("Иванов С.С. Часы.");
	Initialize();
	glutDisplayFunc(Display);
	glutReshapeFunc(Resize);
	glutKeyboardFunc(onKeyPress);
	glutTimerFunc(10, deltaTime, 1);
	glutMainLoop();
	return 0;
}


