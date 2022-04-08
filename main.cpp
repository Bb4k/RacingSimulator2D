#include <iostream>
#include <windows.h>
#include <cstdlib>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <chrono>
#include <string.h>
#include <string>


// --site-packages--
#include <GL/freeglut.h>


// -- defines work area --
GLdouble left_m = -100.0;
GLdouble right_m = 700.0;
GLdouble bottom_m = -140.0;
GLdouble top_m = 460.0;

// -- logic game grid -
#define GRID_Y_LOWER	0
#define GRID_Y_MID		160
#define GRID_Y_UPPER	320

#define GRID_X_LEFT		0
#define GRID_X_MID		320
#define GRID_X_RIGHT	600

// -- app --
#define MAIN_MENU 10
#define OPTIONS 11
#define INGAME 12
int _run = 1;
int _win = 0;

int screen = 0; // different screen values to determine what window is open
int hover = 0;
int mouse_x = 0;
int mouse_y = 0;

// -- p(layer)_car status --
double	p_car_pos_y = 0.0;
double	p_car_pos_x = 0.0;
int		p_car_powerup = 0;
int		p_car_pos_x_values[3] = { GRID_X_LEFT, GRID_X_MID, GRID_X_RIGHT };
int		p_car_moving_x = 0;
int		p_car_moving_y = 0;
// -- used for swift animation from one grid area to another
int		contor_y = 0; // increase/decrease contor relative to start position 
int		contor_x = 0;


// -- c(omputer)_car status --
double	c_car_pos_x = 800; //start position of car (inversely proportional to time_pos*c_car_speed --^)
int		c_car_pos_y_values[3] = { GRID_Y_LOWER, GRID_Y_MID, GRID_Y_UPPER };
double	c_car_pos_y = c_car_pos_y_values[rand() % 3];
double	c_car_speed = 1;

// -- general game status --
#define WIN_SCORE			20000
int		p_score = 0;
int		pts_to_speed_incr = 100;	// speed increase rate
double	action_speed = 0;	// when accel/brake speedup/slowdown everythng a bit to seem more realistic
	// -- powerups --
int		powerup_gen = 0;
int		powerup_pos_x = 0;
int		powerup_pos_y = 0;
int		powerup_size = 20;
int		powerup_time_on = 5; //seconds

// -- ??? --
double rsj, rdj, rss, rds = 0; //?

int street_line = 1000;

void init(void)
{
	glClearColor(0.98, 0.929, 0.792, 0.0);
	glMatrixMode(GL_PROJECTION);
	glShadeModel(GL_SMOOTH);
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

	if (abs(c_car_pos_y - p_car_pos_y) > std::numeric_limits<double>::epsilon() + c_car_speed
		|| abs(c_car_pos_x - p_car_pos_x) > std::numeric_limits<double>::epsilon() + c_car_speed) { //double equal right way

		c_car_pos_x -= c_car_speed + action_speed;
		street_line -= 2*c_car_speed;

		if (street_line < -1000)
			street_line = 1000;

		if (c_car_pos_x < -150) {
			p_score += 100;
			c_car_pos_y = c_car_pos_y_values[rand() % 3];
			//std::cout << "Score:  " << score << endl;
			c_car_pos_x = 800;
		}

		if (p_score >= pts_to_speed_incr && pts_to_speed_incr <= WIN_SCORE) {
			c_car_speed += 0.1;
			pts_to_speed_incr = p_score + 200;
		}

		if (p_score % 1000 == 0 && !powerup_gen && p_score > 0) {
			powerup_gen = 1;
			powerup_pos_x = rand() % 650 + 100;
			powerup_pos_y = rand() % 335;
		}

		if (p_score >= WIN_SCORE) {
			_run = 0;
			_win = 1;
		}

		glutPostRedisplay();
	}
	else {
		//std::cout << "should exit";
		_run = 0;
		_win = 0;
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

	RenderString(200.0f, 425.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"Depaseste masinile! Scor:");
	RenderString(455.0f, 425.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)(std::to_string(p_score).c_str()));

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
	glTranslated(street_line, 0.0, 0.0);
	glLineWidth(15);

	glBegin(GL_LINES);
	glVertex2i(0, 80);
	glVertex2i(400, 80);
	glEnd();

	glBegin(GL_LINES);
	glVertex2i(0, 240);
	glVertex2i(400, 240);
	glEnd();
	glPopMatrix();
}

void draw_p_car() {

	glPushMatrix();
	glTranslated(p_car_pos_x, p_car_pos_y, 0.0);

	glColor3f(0.996, 0.365, 0.149);

	// --- move car to logical grid positions while position != any grid position x or y
	if (contor_y == 1 && (p_car_pos_y != GRID_Y_MID && p_car_pos_y != GRID_Y_UPPER)) {
		p_car_pos_y = p_car_pos_y + 1;
		glRotated(5, 0, 0, 1);
		p_car_moving_y = 1;
	}
	else if (contor_y == -1 && (p_car_pos_y != GRID_Y_MID && p_car_pos_y != GRID_Y_LOWER)) {
		p_car_pos_y = p_car_pos_y - 1;
		glRotated(-5, 0, 0, 1);
		p_car_moving_y = 1;

	}
	else {
		glRotated(0, 0, 0, 1);
		contor_y = 0;
		p_car_moving_y = 0;
	}

	if (contor_x == 1 && (p_car_pos_x != GRID_X_MID && p_car_pos_x <= GRID_X_RIGHT)) {
		p_car_pos_x = p_car_pos_x + 1;
		action_speed = 0.5;
		p_car_moving_x = 1;
	}
	else if (contor_x == -1 && (p_car_pos_x != GRID_X_MID && p_car_pos_x != GRID_X_LEFT)) {
		p_car_pos_x = p_car_pos_x - 1;
		action_speed = -0.5;
		p_car_moving_x = 1;
	}
	else {
		contor_x = 0;
		action_speed = 0;
		p_car_moving_x = 0;
	}
	glRecti(-50, -20, 50, 20); //dimensiunile dreptunghului
	glPopMatrix();
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
		glPushMatrix();
		glBegin(GL_POLYGON);
		glColor3f(1, 0, 0);
		glVertex2i(powerup_pos_x, powerup_pos_y);// Stanga jos
		glVertex2i(powerup_pos_x + powerup_size, powerup_pos_y); // Dreapta jos
		glVertex2i(powerup_pos_x + powerup_size, powerup_pos_y + powerup_size); // Dreapta sus
		glVertex2i(powerup_pos_x, powerup_pos_y + powerup_size);// Stanga sus
		glEnd();
		glPopMatrix();
	}
	if ((p_score + 500) % 1000 == 0)
		powerup_gen = 0;

}

void draw_scene(void)
{
	screen = INGAME;

	glClear(GL_COLOR_BUFFER_BIT);

	// -- static objects --
	draw_background();
	draw_powerup();

	// -- dynamic objects --
	draw_p_car();
	draw_c_car();


	// -- end game --
	if (_run == 0) {
		//TODO call end_screen 
		if (_win)
			RenderString(250.0f, 200.0f, GLUT_BITMAP_8_BY_13, (const unsigned char*)"YOU WIN!");
		else
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



/* TODO implement buttons start / options */
void draw_button(GLdouble btn_pos_x, GLdouble btn_pos_y, int btn_h, int btn_w, char* str) {

	glPushMatrix();
	glTranslated((GLdouble)btn_pos_x, (GLdouble)btn_pos_y, 0.0);

	if (mouse_x > btn_pos_x && mouse_x < btn_pos_x + 2 * btn_w &&
		mouse_y > top_m - btn_pos_y - btn_h && mouse_y < top_m - btn_pos_y + btn_h) {
		glColor3f((GLdouble)0, (GLdouble)0.5, (GLdouble)0);
		glRecti(-btn_w - 5, -btn_h - 5, btn_w - 5, btn_h - 5);
		RenderString(-btn_w / 2 - GLdouble(sizeof(str) / sizeof(char) / 4) - 5, -btn_h / 2 - 5, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)str);
	}
	else {
		glColor3f((GLdouble)0.55, (GLdouble)0.788, (GLdouble)0.451);
		glRecti(-btn_w, -btn_h, btn_w, btn_h);
		RenderString(-btn_w / 2 - GLdouble(sizeof(str) / sizeof(char) / 4), -btn_h / 2, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)str);
	}
	glPopMatrix();
}
void main_menu() {
	screen = MAIN_MENU;

	int i = 0;

	// -- main menu text 
	glClear(GL_COLOR_BUFFER_BIT);
	RenderString(225.0f, 400.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"MAIN MENU");

	// -- start button --
	draw_button((GLdouble)300, (GLdouble)300, 20, 100, "START");
	//RenderString(260.0f, 290.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"START");

	// -- options button --
	draw_button((GLdouble)300, (GLdouble)200, 20, 100, "OPTIONS");
	//RenderString(250.0f, 190.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"OPTIONS");

	glutSwapBuffers();
	glFlush();

}

/* TODO some fade in/out graphic*/
void options_screen() {
	screen = OPTIONS;


	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.55, 0.788, 0.451);


	// -- start button --
	draw_button(300, 300, 20, 100, "BACK");
	//RenderString(265.0f, 290.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"BACK");

	glutSwapBuffers();
	glFlush();
}

/* TODO ome fade in/out graphic*/
void splash_screen() {

	screen = -1;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.55, 0.788, 0.451);
	RenderString(280.0f, 400.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"Bb4k");
	RenderString(200.0f, 250.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"Click anywhere to enter game...");
	glutSwapBuffers();
	glFlush();
	Sleep(5);
	glutDisplayFunc(main_menu);
}

void misca_sus(void) {
	if (p_car_pos_y < 320) {
		contor_y = 1;
		p_car_pos_y += 1;
	}
	glutPostRedisplay();
}
void misca_jos(void) {
	if (p_car_pos_y > 0) {
		contor_y = -1;
		p_car_pos_y -= 1;
	}
	glutPostRedisplay();
}
void misca_dreapta(void) {
	if (p_car_pos_x < GRID_X_RIGHT) {
		p_car_pos_x += 10;
		contor_x += 1;
	}
	glutPostRedisplay();
}
void misca_stanga(void)
{
	if (p_car_pos_x > GRID_X_LEFT) {
		p_car_pos_x -= 10;
		contor_x -= 1;
	}

	glutPostRedisplay();
}
void keyboard(int key, int x, int y)
{	

		switch (key) {
		case GLUT_KEY_UP:
			if (!p_car_moving_y)
				misca_sus();
			break;
		case GLUT_KEY_DOWN:
			if(!p_car_moving_y)
				misca_jos();
			break;
		case GLUT_KEY_LEFT:
			if (!p_car_moving_x)
				misca_stanga();
			break;
		case GLUT_KEY_RIGHT:
			if (!p_car_moving_x)
				misca_dreapta();
			break;
		default:
			break;
		}
}

void leftclick(int x, int y) {
	switch (screen) {
	case MAIN_MENU:
		std::cout << "inside main menu left click";
		if (x > 300 && x < 500 && y > 140 && y < 180) {
			glutDisplayFunc(draw_scene);
			break;
		}
		if (x > 300 && x < 500 && y > 240 && y < 280) {
			glutDisplayFunc(options_screen);
			break;
		}
		break;
	case OPTIONS:
		std::cout << "inside options left click";
		if (x > 300 && x < 500 && y > 140 && y < 180) {
			glutDisplayFunc(main_menu);
			break;
		}
		break;
	case INGAME:
		std::cout << "inside game left click";
		break;
	default:
		break;
	}
}

void rightclick(int x, int y) {
	switch (screen) {
	case MAIN_MENU:
		std::cout << "inside main menu right click";
		break;
	case OPTIONS:
		std::cout << "inside options right click";
		break;
	case INGAME:
		std::cout << "inside game right click";
		break;
	default:
		break;
	}
}

void mouse(int button, int state, int x, int y) {

	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			leftclick(x, y);
		break;

	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			rightclick(x, y);
		break;

	default:
		break;
	}
}

void mouse_pos(int x, int y) {
	mouse_x = x;
	mouse_y = y;
	glutPostRedisplay();
	//std::cout << "(" << x << ", " << y << ")\n";
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Depaseste masinile - mini game");
	init();
	glutSpecialFunc(keyboard);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mouse_pos);
	glutDisplayFunc(splash_screen);
	glutReshapeFunc(reshape);


	glutMainLoop();
}