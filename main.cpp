#include <iostream>
#include <windows.h>
#include <cstdlib>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <chrono>

// --site-packages--
#include <GL/freeglut.h>


// -- defines work area --
GLdouble left_m = -100.0;
GLdouble right_m = 700.0;
GLdouble bottom_m = -140.0;
GLdouble top_m = 460.0;

// -- logic game grid -
#define GRID_Y_LOWER 0
#define GRID_Y_MID 160
#define GRID_Y_UPPER 320

#define GRID_X_LEFT 0
#define GRID_X_MID 320
#define GRID_X_RIGHT 600

// -- defines if game has ended --
int _run = 1;

// -- defines p(layer)_car status --
double	p_car_pos_y				= 0.0;
double	p_car_pos_x				= 0.0;
int		p_car_powerup			= 0;
int		p_car_pos_x_values[3]	= { GRID_X_LEFT, GRID_X_MID, GRID_X_RIGHT };
	// -- used for swift animation from one grid area to another
	int		contor_y			= 0; // increase/decrease contor relative to start position 
	int		contor_x			= 0;


// -- c(omputer)_car status --
double	c_car_pos_x				= 800; //start position of car (inversely proportional to time_pos*c_car_speed --^)
int		c_car_pos_y_values[3]	= { GRID_Y_LOWER, GRID_Y_MID, GRID_Y_UPPER }; 
double	c_car_pos_y				= c_car_pos_y_values[rand() % 3]; 
double	c_car_speed				= 1;

// -- general game status --
#define WIN_SCORE			15000
int		p_score				= 0;
int		pts_to_speed_incr	= 100; // speed increase rate
	// -- powerups --
	int		powerup_gen		= 0;
	int		powerup_pos_x	= 0.0;
	int		powerup_pos_y	= 0.0;
	int		powerup_size	= 20;
	int		powerup_time_on = 5; //seconds

	// -- ??? --
	double rsj, rdj, rss, rds = 0; //?

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

	Sleep(1); // t/dt fix kb lag imput

	if (abs(c_car_pos_y - p_car_pos_y) > std::numeric_limits<double>::epsilon() + c_car_speed || abs(c_car_pos_x - p_car_pos_x) > std::numeric_limits<double>::epsilon() + c_car_speed) //double equal right way
	{

		c_car_pos_x -= c_car_speed;

		if (c_car_pos_x < -150)
		{
			p_score += 100;
			c_car_pos_y = c_car_pos_y_values[rand() % 3];
			//std::cout << "Score:  " << score << endl;
			c_car_pos_x = 800;
		}

		if (p_score >= pts_to_speed_incr && pts_to_speed_incr <= WIN_SCORE)
		{
			c_car_speed += 0.1;
			pts_to_speed_incr += 100;
		}

		if (p_score % 1000 == 0  && !powerup_gen && p_score > 0) {
			powerup_gen = 1;
			powerup_pos_x = rand() % 650 + 100;
			powerup_pos_y = rand() % 335;
		}

		glutPostRedisplay();
	}
	else {
		//std::cout << "should exit";
		_run = 0;
	}
}

void draw_background() {
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
	glTranslated(time_pos, 0.0, 0.0);


	glBegin(GL_LINES);
	glVertex2i(-100, 80);
	glVertex2i(1500, 80);
	glEnd();

	glBegin(GL_LINES);
	glVertex2i(-100, 240);
	glVertex2i(1500, 240);
	glEnd();
	glPopMatrix();
}
void draw_p_car() {
	glPushMatrix();
	glTranslated(p_car_pos_x, p_car_pos_y, 0.0);

	glColor3f(0.996, 0.365, 0.149);
	glRecti(-45, -15, 45, 15); //dimensiunile dreptunghului
	glPopMatrix();

	// --- move car to logical grid positions while position != any grid position x or y
	if (contor_y == 1 && (p_car_pos_y != GRID_Y_MID && p_car_pos_y != GRID_Y_UPPER))
		p_car_pos_y = p_car_pos_y + 1;
	else if (contor_y == -1 && (p_car_pos_y != GRID_Y_MID && p_car_pos_y != GRID_Y_LOWER))
		p_car_pos_y = p_car_pos_y - 1;
	else {
		contor_y = 0;
	}

	if (contor_x == 1 && (p_car_pos_x != GRID_X_MID && p_car_pos_x != GRID_X_RIGHT))
		p_car_pos_x = p_car_pos_x + 1;
	else if (contor_x == -1 && (p_car_pos_x != GRID_X_MID && p_car_pos_x != GRID_X_LEFT))
		p_car_pos_x = p_car_pos_x - 1;
	else {
		contor_x = 0;
	}

}
void draw_c_car() {
	//desenam a doua masina (adversara)
	glPushMatrix();
	glTranslated(c_car_pos_x, c_car_pos_y, 0.0);

	glColor3f(0.471, 0.667, 0.949);
	glRecti(-45, -15, 45, 15);


	glPopMatrix();
}
void draw_powerup() {
	if (powerup_gen) {
		glBegin(GL_POLYGON);
		glColor3f(1, 0, 0);
		glVertex2i(powerup_pos_x, powerup_pos_y);// Stanga jos
		glVertex2i(powerup_pos_x + powerup_size, powerup_pos_y); // Dreapta jos
		glVertex2i(powerup_pos_x + powerup_size, powerup_pos_y + powerup_size); // Dreapta sus
		glVertex2i(powerup_pos_x, powerup_pos_y + powerup_size);// Stanga sus
		glEnd();
	}
	if ((p_score+500) % 1000 == 0)
		powerup_gen = 0;

}
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	draw_background();
	draw_p_car();
	draw_c_car();
	draw_powerup();
	
	
	

	


	// -- end game --
	if (_run == 0) {
		//TODO call end_screen 
		RenderString(250.0f, 200.0f, GLUT_BITMAP_8_BY_13, (const unsigned char*)"GAME OVER");
	}

	

	

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

void miscasus(void){

		if (p_car_pos_y < 320){
			contor_y = 1;
			p_car_pos_y += 1;
		}

		glutPostRedisplay();
}
void miscajos(void){	

	if (p_car_pos_y > 0)
	{
		contor_y = -1;
		p_car_pos_y -= 1;
	}
		glutPostRedisplay();
}
void miscadreapta(void){	
	if (p_car_pos_x < 700) {
		p_car_pos_x += 10;
		contor_x += 1;
	}
	glutPostRedisplay();
}
void miscastanga(void)
{	
	
	if (p_car_pos_x > 0) {
		p_car_pos_x -= 10;
		contor_x -= 1;
	}

	glutPostRedisplay();
}
void keyboard(int key, int x, int y)
{
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

void leftclick(void) {
	glutPostRedisplay();
}

void rightclick(void) {
	glutPostRedisplay();
}

void mouse(int button, int state,int x, int y){
	switch (button) {
		case GLUT_LEFT_BUTTON:
			leftclick();
			break;
		case GLUT_RIGHT_BUTTON:
			rightclick();
			break;
		default:
			break;
	

	}
}


void main_menu() {}

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