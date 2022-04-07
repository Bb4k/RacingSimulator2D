#include <iostream>
#include <windows.h>
#include <GL/freeglut.h>
#include <cstdlib>

using namespace std;

GLdouble left_m = -100.0;
GLdouble right_m = 700.0;
GLdouble bottom_m = -140.0;
GLdouble top_m = 460.0;

double ok = 1;

double car_pos_y = 0.0;
double car_pos_x = 0.0;

double i = 0.0;

double contor = 0;
double loc_vert = 800;
int vector[3] = { 0, 160, 320 };
double height = vector[rand() % 3];
int score = 0;
double timp = 1;
int pct = 100; // rata la care creste timpul
double rsj, rdj, rss, rds = 0;

void init(void)
{
	glClearColor(0.98, 0.929, 0.792, 0.0);
	glMatrixMode(GL_PROJECTION);
	glOrtho(left_m, right_m, bottom_m, top_m, -1.0, 1.0);
}

void RenderString(float x, float y, void* font, const unsigned char* string)
{

	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2f(x, y);

	glutBitmapString(font, string);
}
void startgame(void)
{

	/*std::cout << "height: " << height << "car_pos_y: " << car_pos_y << std::endl;
	std::cout << "loc_vert: " << loc_vert << "car_pos_x: " << car_pos_x << std::endl;*/
	Sleep(1); // t/dt fix kb lag imput

	if (abs(height - car_pos_y) > std::numeric_limits<double>::epsilon() + timp || abs(loc_vert - car_pos_x) > std::numeric_limits<double>::epsilon() + timp) //double equal right way
	{

		if (i < -380) //190*2
		{
			i = 0;
		}
		i = i - 2 * timp;

		loc_vert -= timp;

		if (loc_vert < -150)
		{
			score += 100;
			height = vector[rand() % 3];
			//cout << "Score:  " << score << endl;
			loc_vert = 800;
		}

		if (score >= pct && pct <= 15000)
		{
			timp += 0.1;
			pct += 100;
		}

		glutPostRedisplay();
	}
	else {
		//std::cout << "should exit";
		ok = 0;
	}
}

void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	//desenam masina
	glPushMatrix();
	glTranslated(car_pos_x, car_pos_y, 0.0);

	glColor3f(0.996, 0.365, 0.149);
	glRecti(-45, -15, 45, 15); //dimensiunile dreptunghului

	if (ok == 0) //ceva neimplementat, probabil rotile
	{
		rsj = 8;
		rss = -8;
		rdj = -8;
		rds = 8;
	}


	glPopMatrix();
	
	glColor3f(0.55, 0.788, 0.451);

	// Iarba de jos
	glBegin(GL_POLYGON);
	glVertex2i(-100, -140);// Stanga jos
	glVertex2i(700, -140); // Dreapta jos
	glVertex2i(700, -80);  // Dreapta sus
	glVertex2i(-100, -80); // Stanga sus
	glEnd();

	// Iarba de sus
	glBegin(GL_POLYGON);
	glVertex2i(-100, 400);// Stanga jos
	glVertex2i(700, 400); // Dreapta jos
	glVertex2i(700, 460); // Dreapta sus
	glVertex2i(-100, 460);// Stanga sus
	glEnd();
	RenderString(200.0f, 425.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"Depaseste masinile!");

	// Delimitare sosea
	glLineWidth(3);
	glColor3f(1, 1, 1);

	// Delimitam soseaua de iarba partea de jos
	glBegin(GL_LINES);
	glVertex2i(-100, -80);
	glVertex2i(1500, -80);
	glEnd();

	// Delimitam soseaua de iarba partea de sus
	glBegin(GL_LINES);
	glVertex2i(-100, 400);
	glVertex2i(1500, 400);
	glEnd();

	// Liniile intrerupte
	glPushMatrix();
	glTranslated(i, 0.0, 0.0);
	

	glBegin(GL_LINES);
	glVertex2i(-100, 80);
	glVertex2i(1500, 80);
	glEnd();

	glBegin(GL_LINES);
	glVertex2i(-100, 240);
	glVertex2i(1500, 240);
	glEnd();
	glPopMatrix();



	


	if (ok == 0) {
		RenderString(250.0f, 200.0f, GLUT_BITMAP_8_BY_13, (const unsigned char*)"GAME OVER");
	}

	if (contor == 1 && (car_pos_y != 160 && car_pos_y != 320))
		car_pos_y = car_pos_y + 1;
	else if (contor == -1 && (car_pos_y != 160 && car_pos_y != 0))
		car_pos_y = car_pos_y - 1;
	else {
		contor = 0;
	}

	//desenam a doua masina (adversara)
	glPushMatrix();
	glTranslated(loc_vert, height, 0.0);

	glColor3f(0.471, 0.667, 0.949);
	glRecti(-45, -15, 45, 15);


	glPopMatrix();

	startgame();
	glutPostRedisplay();
	glutSwapBuffers();
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-100.0, 700.0, -140.0, 460.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void miscasus(void)
{

		if (car_pos_y < 320)
		{
			contor = 1;
			car_pos_y += 1;
		}

		glutPostRedisplay();
}

void miscajos(void)
{	

	if (car_pos_y > 0)
	{
		contor = -1;
		car_pos_y -= 1;
	}
		glutPostRedisplay();
}

void miscadreapta(void)
{	
	
		if (car_pos_x < 700)
			car_pos_x += 10;

		glutPostRedisplay();
	
}

void miscastanga(void)
{	
	
		if (car_pos_x > 0)
			car_pos_x -= 10;

		glutPostRedisplay();
}

void keyboard(int key, int x, int y)
{
	if (ok != 0)
		switch (key) {
			case GLUT_KEY_UP:
				miscasus();
				break;
			case GLUT_KEY_DOWN:
				miscajos();
				break;
			case GLUT_KEY_LEFT:
				miscastanga();
				break;
			case GLUT_KEY_RIGHT:
				miscadreapta();
				break;
			default: 
				break;
		}

}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Depaseste masinile - mini game");
	init();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(reshape);
	glutSpecialFunc(keyboard);

	glutMainLoop();
}