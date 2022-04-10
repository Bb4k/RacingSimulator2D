// sys
#define _CRT_SECURE_NO_WARNINGS 1
#define _CRT_SECURE_NO_WARNINGS_GLOBALS 1

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
#include <tuple>
#include <sstream>
#include <vector>

// --site-packages--
#include <GL/freeglut.h>


// -- defines work area --
GLdouble left_m			= -100.0;
GLdouble right_m		= 700.0;
GLdouble bottom_m		= -140.0;
GLdouble top_m			= 460.0;

// -- logic game grid --
#define GRID_Y_LOWER	0
#define GRID_Y_MID		160
#define GRID_Y_UPPER	320

#define GRID_X_LEFT		0
#define GRID_X_MID		320
#define GRID_X_RIGHT	600

// -- app --
#define MAIN_MENU		10
#define OPTIONS			11
#define IN_GAME			12
#define LEAD_B			13
#define PRE_GAME		14
#define END_GAME		15
#define ANIM			16
#define SPLASH_SCREEN	-1

#define ENDLESS			20
#define CAMPAIGN		21

int _prev_scr			= -1;
int _run				= 1;
int _win				= 0;
int _ee					= 0;
int game_mode			= 20;

int screen				= 0; // different screen values to determine what window is open
int hover				= 0;
int mouse_x				= 0;
int mouse_y				= 0;

// -- p(layer)_car status --
double	p_car_pos_y		= GRID_Y_MID;
double	p_car_pos_x		= -200.0;

int		p_car_pos_x_values[3] = {
									GRID_X_LEFT,
									GRID_X_MID,
									GRID_X_RIGHT
};
double	p_car_angle		= 0.0;
int		p_car_moving_x	= 0;
int		p_car_moving_y	= 0;
int		p_car_crashed	= 0;
// -- used for swift animation from one grid area to another
int		contor_y = 0; // increase/decrease contor relative to start position 
int		contor_x = 0;

double	p_car_color_r	= 0.5;
double	p_car_color_g	= 0.8;
double	p_car_color_b	= 0.2;

//										 -r- -g- -b-
double	p_car_color_values[][3] = { 
										{0.5,0.8,0.2}, //green ciucas (default)
										{0.9,0.1,0.1}, //red ca focu
										{0.7,0.7,0.1}, //yellow de invidie 
										{0.1,0.1,0.9}, //blue de voronet
										{1.0,0.1,0.7}, //ciclam

									};
int		p_car_selected_color = 0;

int		p_car_powerup	= 0;


// -- c(omputer)_car status --
double	c_car_pos_x = 800; //start position of car (inversely proportional to time_pos*c_car_speed --^)
int		c_car_pos_y_values[3] = {
									GRID_Y_LOWER,
									GRID_Y_UPPER,
									GRID_Y_MID
								};

double	c_car_pos_y		= c_car_pos_y_values[rand() % 2];
double	c_car_speed		= 1;
double	c_car_max_speed = 3;

double	x_car_pos_x		= -350;

// -- general game status --
#define WIN_SCORE		20000
int		p_score			= 0;
int		pts_to_speed_incr = 100;	// speed increase rate
double	action_speed	= 0;	// when accel/brake speedup/slowdown everythng a bit to seem more realistic
	// -- powerups --
int		powerup_gen		= 0;
int		powerup_pos_x	= 0;
int		powerup_pos_y	= 0;
int		powerup_size	= 20;
int		powerup_time_on = 5; //seconds


double street_line		= 1000;



// -- kb input --
std::vector <std::string> names(1);

void init(void) {
	glClearColor((GLclampf)0.6, (GLclampf)0.6, (GLclampf)0.6, (GLclampf)0.0);
	glMatrixMode(GL_PROJECTION);
	glShadeModel(GL_SMOOTH);
	glOrtho(left_m, right_m, bottom_m, top_m, -1.0, 1.0);
	srand(time(NULL));
}

void RenderString(GLdouble x, GLdouble y, void* font, const unsigned char* string) {

	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2f((GLfloat)x, (GLfloat)y);
	glutBitmapString(font, string);

}


void startgame(void) {

	Sleep(1); // t/dt fix kb lag input

	//  std::numeric_limits<double>::epsilon() eroarea la trunchiere numar virgula mobila 1.0000..001123
	if (abs(c_car_pos_y - p_car_pos_y) > std::numeric_limits<double>::epsilon() + c_car_speed
		|| abs(c_car_pos_x - 100 - p_car_pos_x) > std::numeric_limits<double>::epsilon() + c_car_speed) { //double equal right way


		
		c_car_pos_x -= c_car_speed + action_speed;


		if (c_car_pos_x < -150) {
			p_score += 100;
			c_car_pos_y = c_car_pos_y_values[rand() % 3];
			//std::cout << "Score:  " << score << endl;
			c_car_pos_x = 800;
		}

		if (p_score >= pts_to_speed_incr) {
			c_car_speed += 0.1;
			pts_to_speed_incr += 200;
		}

		if (p_score % 1000 == 0 && !powerup_gen && p_score > 0) {
			powerup_gen = 1;
			powerup_pos_x = rand() % 650 + 100;
			powerup_pos_y = rand() % 335;
		}

		if (p_score >= WIN_SCORE && game_mode == CAMPAIGN) {
			_run = 0;
			_win = 1;
		}

		glutPostRedisplay();
	}
	else {
		if (c_car_speed / 2 >= c_car_max_speed / 4) {
			std::cout << c_car_speed/2 << " " << c_car_max_speed/4;
			c_car_speed = 1.4;
			p_car_pos_x += 3.0;
		}
		else {
			_run = 0;
			_win = 0;
		}
	}
}

void draw_background() {
	glColor3f((GLfloat)0.55, (GLfloat)0.788, (GLfloat)0.451);
	street_line -= (int)(c_car_speed) - 0.1;

	if (street_line < -1200)
		street_line = 700;
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

	if (screen == IN_GAME) {
		RenderString(200.0f, 425.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"Depaseste masinile! Scor:");
		RenderString(455.0f, 425.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)(std::to_string(p_score).c_str()));
	} else 
		if (screen == PRE_GAME)
			RenderString(200.0f, 425.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"Alege masina si modul de joc");

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

	glPushMatrix();
	// Liniile intrerupte
	for (int line = 0; line < 2; ++line) {
		glPushMatrix();
		glTranslated(street_line+line*800, 0.0, 0.0);
		glLineWidth(15);

		glBegin(GL_LINES);
		glVertex2i(0, 80);
		glVertex2i(300, 80);
		glEnd();

		glBegin(GL_LINES);
		glVertex2i(0, 240);
		glVertex2i(300, 240);
		glEnd();
		glPopMatrix();
	}
	glPopMatrix();

}

int index = 0;
void draw_car(double x_car_pos, double y_car_pos, double r, double g, double b) {

	glPushMatrix();
	glTranslated(x_car_pos, y_car_pos, 0.0);

	//wheels
		//fl
	glPushMatrix();
	glTranslated(30, 20, 0.0);
	glColor3f((GLfloat)0, (GLfloat)0, (GLfloat)0);
	glRecti(-10, -5, 10, 5);
	glPopMatrix();
	//fr
	glPushMatrix();
	glTranslated(30, -20, 0.0);
	glColor3f((GLfloat)0, (GLfloat)0, (GLfloat)0);
	glRecti(-10, -5, 10, 5);
	glPopMatrix();
	//rl
	glPushMatrix();
	glTranslated(-30, 20, 0.0);
	glColor3f((GLfloat)0, (GLfloat)0, (GLfloat)0);
	glRecti(-10, -5, 10, 5);
	glPopMatrix();
	//rr
	glPushMatrix();
	glTranslated(-30, -20, 0.0);
	glColor3f((GLfloat)0, (GLfloat)0, (GLfloat)0);
	glRecti(-10, -5, 10, 5);
	glPopMatrix();

	//body
	glPushMatrix();
	glColor3f((GLfloat)r, (GLfloat)g, (GLfloat)b);
	glRecti(-50, -20, 50, 20);
	glPopMatrix();

	//windshield
	glPushMatrix();
	glTranslated(15, 0, 0.0);
	glColor3f((GLfloat)0.7, (GLfloat)1, (GLfloat)1);
	glRecti(-15, -15, 15, 15);
	glPopMatrix();

	//roof
	glPushMatrix();
	glTranslated(-5, 0, 0.0);
	glColor3f((GLfloat)r + 0.1, (GLfloat)g + 0.1, (GLfloat)b + 0.1);
	glRecti(-15, -15, 15, 15);
	glPopMatrix();

	//rearwindow
	glPushMatrix();
	glTranslated(-25, 0, 0.0);
	glColor3f((GLfloat)0.7, (GLfloat)1, (GLfloat)1);
	glRecti(-5, -15, 5, 15);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();


}


// - masina politite inceput --
void draw_x_car(int index) {

	glPushMatrix();
	draw_car(x_car_pos_x, 160.0, 0.1, 0.5, 0.7);
	//circoblitz
	glPushMatrix();
	glTranslated(x_car_pos_x + 5, 167, 0.0);
	glColor3f((GLfloat)1, (GLfloat)0, (GLfloat)0);
	if (index % 3 == 0)
		glRecti(-12, -12, 12, 12);
	else
		glRecti(-10, -10, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x_car_pos_x + 5, 153, 0.0);
	glColor3f((GLfloat)0, (GLfloat)0, (GLfloat)1);
	if (index % 3 != 0)
		glRecti(-12, -12, 12, 12);
	else
		glRecti(-10, -10, 10, 10);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

// -- masina playerului --
void draw_p_car() {
	//Sleep(25);
	
	glPushMatrix();
	// --- move car to logical grid positions while position != any grid position x or y

	
	if (p_car_crashed) {
		glRotatef(p_car_angle + 0.05, 0, 0, 1);
		p_car_angle += 0.05;
	}

	if (contor_y == 1 && (p_car_pos_y != GRID_Y_MID && p_car_pos_y != GRID_Y_UPPER)) {
		p_car_pos_y = p_car_pos_y + 1;
		if (p_car_pos_y > GRID_Y_MID && p_car_pos_y < (GRID_Y_MID + (GRID_Y_UPPER - GRID_Y_MID) / 2)) {
			glRotatef(p_car_angle + 0.04, 0, 0, 1);
			p_car_angle += 0.04;
		}		
		else if (p_car_pos_y > GRID_Y_LOWER && p_car_pos_y < GRID_Y_LOWER + (GRID_Y_UPPER - GRID_Y_MID) / 2) {
			glRotatef(p_car_angle + 0.04, 0, 0, 1);
			p_car_angle += 0.04;
		}
		else {
			glRotatef(p_car_angle - 0.04, 0, 0, 1);
			p_car_angle -= 0.04;
		}
		p_car_moving_y = 1;
	}
	else if (contor_y == -1 && (p_car_pos_y != GRID_Y_MID && p_car_pos_y != GRID_Y_LOWER)) {
		p_car_pos_y = p_car_pos_y - 1;
		if (p_car_pos_y < GRID_Y_MID && p_car_pos_y > (GRID_Y_LOWER + (GRID_Y_UPPER - GRID_Y_MID) / 2)) {
			glRotatef(p_car_angle - 0.04, 0, 0, 1);
			p_car_angle -= 0.04;
		}
		else if (p_car_pos_y < GRID_Y_UPPER && p_car_pos_y > GRID_Y_MID + (GRID_Y_UPPER - GRID_Y_MID) / 2) {
			glRotatef(p_car_angle - 0.04, 0, 0, 1);
			p_car_angle -= 0.04;
		}
		else {
			glRotatef(p_car_angle + 0.04, 0, 0, 1);
			p_car_angle += 0.04;
		}
		p_car_moving_y = 1;
	}
	else {
		if (!p_car_crashed) {
			p_car_angle = 0;
			glRotated(p_car_angle, 0, 0, 1);
			contor_y = 0;
			p_car_moving_y = 0;
		}
	}

	if (contor_x == 1 && (p_car_pos_x != GRID_X_MID && p_car_pos_x <= GRID_X_RIGHT)) {
		p_car_pos_x = p_car_pos_x + 1;
		action_speed = 0.2;
		p_car_moving_x = 1;
	}
	else if (contor_x == -1 && (p_car_pos_x != GRID_X_MID && p_car_pos_x != GRID_X_LEFT)) {
		p_car_pos_x = p_car_pos_x - 1;
		action_speed = -0.2;
		p_car_moving_x = 1;
	}
	else {
		contor_x = 0;
		action_speed = 0;
		p_car_moving_x = 0;

	}

	if (_ee) {
		++index;
		draw_car(p_car_pos_x, p_car_pos_y, 0.1, 0.5, 0.7);
		//circoblitz
		glPushMatrix();
		glTranslated(p_car_pos_x + 5, p_car_pos_y + 7, 0.0);
		glColor3f((GLfloat)1, (GLfloat)0, (GLfloat)0);
		if (index % 3 == 0)
			glRecti(-12, -12, 12, 12);
		else
			glRecti(-10, -10, 10, 10);
		glPopMatrix();

		glPushMatrix();
		glTranslated(p_car_pos_x + 5, p_car_pos_y - 7, 0.0);
		glColor3f((GLfloat)0, (GLfloat)0, (GLfloat)1);
		if (index % 3 != 0)
			glRecti(-12, -12, 12, 12);
		else
			glRecti(-10, -10, 10, 10);
		glPopMatrix();

		glPopMatrix();
	}
	else 
		draw_car(p_car_pos_x, p_car_pos_y, p_car_color_r, p_car_color_g, p_car_color_b);



	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

}
// -- masinia din contrasens --
void draw_c_car() {
	glPushMatrix();
	glRotated(180, 0, 0, 0);
	glPushMatrix();
		draw_car(-c_car_pos_x, -c_car_pos_y, 0.7, 0.2, 0.2);
	glPopMatrix();
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
/* TODO implement buttons start / options */
void draw_button(GLdouble btn_pos_x, GLdouble btn_pos_y, int btn_h, int btn_w, char* str) {

	glPushMatrix();
	glTranslated((GLdouble)btn_pos_x, (GLdouble)btn_pos_y, 0.0);

	if (mouse_x > btn_pos_x + btn_w / 4 && mouse_x < btn_pos_x + 2.0 * btn_w + btn_w / 4 &&
		mouse_y > top_m - btn_pos_y - btn_h && mouse_y < top_m - btn_pos_y + btn_h) {
		glColor3f((GLdouble)0, (GLdouble)0.5, (GLdouble)0);
		glRecti(-btn_w - 5, -btn_h - 5, btn_w - 5, btn_h - 5);
		RenderString((GLdouble)(-btn_w) / 2 - (GLdouble)((sizeof(*str) / sizeof(char)) / 4 + (GLdouble)5), (GLdouble)((GLdouble)-btn_h / 2 - (GLdouble)5), GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)str);
	}
	else {
		glColor3f((GLfloat)0.55, (GLfloat)0.788, (GLfloat)0.451);
		glRecti(-btn_w, -btn_h, btn_w, btn_h);
		RenderString((GLdouble)(-btn_w) / 2 - GLdouble((sizeof(*str) / sizeof(char)) / 4 * (GLdouble)10), (GLdouble)-btn_h / 2, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)str);
	}
	glPopMatrix();
}
void end_game() {

	screen = END_GAME;
	glClear(GL_COLOR_BUFFER_BIT);
	c_car_speed = 1;
	draw_background();

	if (c_car_pos_x < -150) {
		c_car_pos_x = 900;
		c_car_pos_y = c_car_pos_y_values[rand() % 3];
	}
	c_car_pos_x -= c_car_speed;
	glPushMatrix();
	draw_c_car();
	glPopMatrix();

	if (_win)
		RenderString(225.0f, 400.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"YOU WIN! Score:");
	else 
		RenderString(225.0f, 400.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"GAME OVER! Score:");
	
	RenderString(500.0f, 400.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)(std::to_string(p_score).c_str()));

	for (size_t i = 0; i < names.size(); ++i)
	{
		std::ostringstream oss;
		oss << "NAME: " << names[i];

		void* font = GLUT_BITMAP_TIMES_ROMAN_24;
		const int fontHeight = glutBitmapHeight(font);
		glRasterPos2i(150, 360 - (fontHeight * (i + 1)));
		glutBitmapString(font, (const unsigned char*)(oss.str().c_str()));
	}
	RenderString(-50.0f, 300.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"LEADERBOARD");

	// read from file insert str
	for (int lb_score = 0; lb_score < 5; ++lb_score) {
		RenderString(100.0f, 250.0f - lb_score * 50, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"TEST                      3500 points");
	}
	// -------------------- actions ------------------
	// main menu
	draw_button((GLdouble)50, (GLdouble)0, 20, 100, "MAIN MENU");
	// save score
	draw_button((GLdouble)300, (GLdouble)0, 20, 100, "SAVE SCORE");
	// restart
	draw_button((GLdouble)550, (GLdouble)0, 20, 100, "RESTART");

	glutPostRedisplay();
	glutSwapBuffers();
	glFlush();
	Sleep(5);
}

int go_anim = 1;

void game_over_anim() {

	if (!go_anim)
		glutDisplayFunc(end_game);

	p_car_crashed = 1;
	screen = ANIM;
	Sleep(5);

	glClear(GL_COLOR_BUFFER_BIT);
	//glColor3f((GLfloat)0.55, (GLfloat)0.788, (GLfloat)0.451);

	draw_background();

	if (p_car_pos_x != GRID_X_MID && p_car_pos_y != GRID_Y_MID) {
		p_car_pos_x -= 1 * ((p_car_pos_x - GRID_X_MID) / abs(p_car_pos_x - GRID_X_MID));
		p_car_pos_y -= 1 * ((p_car_pos_y - GRID_Y_MID) / abs(p_car_pos_y - GRID_Y_MID));
	}
	

	
	if (x_car_pos_x != GRID_X_LEFT + 200)
		x_car_pos_x = x_car_pos_x + 0.5;
	else {
		x_car_pos_x = GRID_X_LEFT + 200;
		go_anim = 0;
	}

	draw_p_car();

	draw_x_car(index);
	++index;

	draw_c_car();


	
	glutPostRedisplay();
	glutSwapBuffers();
	glFlush();
	
}

void win_anim() {
	// TODO implement win animations
	glutDisplayFunc(end_game);
}

void draw_scene(void)
{
	screen = IN_GAME;

	glClear(GL_COLOR_BUFFER_BIT);

	// -- static objects --
	draw_background();
	//draw_powerup();

	// -- dynamic objects --
	draw_p_car();
	draw_c_car();


	// -- end game --
	if (_run == 0) {
		std::cout << "run = 0";
		//TODO call end_screen 
		if (_win)
			glutDisplayFunc(win_anim);
		else {
			x_car_pos_x = -150;
			glutDisplayFunc(game_over_anim);
		}
			

	}

	startgame();

	glutPostRedisplay();
	glutSwapBuffers();
	glFlush();
}

int first_anim = 1;
int dialogue = 0;
int sec_anim = 0;

int next_chr = 0;
char text[27] = "CATCH ME IF YOU CAN HAHAHA";
char aux[] = "";

void pre_start(void) {

	screen = IN_GAME;

	if (first_anim || sec_anim)
		glClear(GL_COLOR_BUFFER_BIT);
	draw_background();
	Sleep(1);
	if (first_anim == 1) {

		glPushMatrix();
		draw_p_car();
		glPushMatrix();

		glPushMatrix();
		draw_x_car(index);
		glPopMatrix();

		if (p_car_pos_x > 1000)
			p_car_pos_x = -150;

		if (p_car_pos_x != GRID_X_MID)
			p_car_pos_x = p_car_pos_x + 0.5;
		else if (x_car_pos_x != GRID_X_LEFT && p_car_pos_x > -100)
			x_car_pos_x = x_car_pos_x + 0.5;
		else {
			dialogue = 1;
			first_anim = 0;
		}

		
	}

	if (dialogue == 1) {
		draw_background();
		draw_x_car(index);
		Sleep(15); //slow-mo
		if (next_chr <= strlen(text)) {
	
			strncpy(aux, text, next_chr);
			RenderString(300, 200.0f, GLUT_BITMAP_8_BY_13, (const unsigned char*)aux);
			Sleep(100);
			++next_chr;
		}
		
		else {
			Sleep(1000);
			dialogue = 0;
			first_anim = 0;
			sec_anim = 1;
		}
		++index;
	}
	++index;
	if (sec_anim == 1) {

		glPushMatrix();
		draw_p_car();
		glPopMatrix();

		glPushMatrix();
		draw_x_car(index);
		glPopMatrix();

		if (p_car_pos_x != GRID_X_LEFT)
			p_car_pos_x = p_car_pos_x - 0.5;
		else
			sec_anim = 0;

		if (x_car_pos_x > -200)
			x_car_pos_x = x_car_pos_x - 0.5;
	}


	if (!first_anim && !sec_anim && !dialogue)
		glutDisplayFunc(draw_scene);

	// -- easter egg --
	if (p_car_pos_x < -200) {
		_ee = 1;
		p_car_pos_x = GRID_X_LEFT;
		p_car_pos_y = GRID_Y_MID;
		glutDisplayFunc(draw_scene);
		
	}

	glutPostRedisplay();
	glutSwapBuffers();
	glFlush();
}

void main_menu() {
	screen = MAIN_MENU;
	
	Sleep(5);
	// -- main menu text 
	glClear(GL_COLOR_BUFFER_BIT);
	RenderString(225.0f, 400.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"MAIN MENU");
	draw_background();
	c_car_speed = 1;
	p_car_pos_y = 160;
	p_car_pos_x += 1;
	if (p_car_pos_x > 850)
		p_car_pos_x = -200;
	draw_p_car();
	if (c_car_pos_x < -150) {
		c_car_pos_x = 900;
		c_car_pos_y = c_car_pos_y_values[rand() % 2];
	}
	c_car_pos_x -= c_car_speed;
	glPushMatrix();
	draw_c_car();
	glPopMatrix();
	// -- start button --
	draw_button((GLdouble)300, (GLdouble)300, 20, 100, "START");
	//RenderString(260.0f, 290.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"START");

	// -- options button --
	draw_button((GLdouble)300, (GLdouble)200, 20, 100, "OPTIONS");
	//RenderString(250.0f, 190.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"OPTIONS");

	glutPostRedisplay();
	glutSwapBuffers();
	glFlush();

}

/* TODO some fade in/out graphic*/
void options_screen() {
	screen = OPTIONS;


	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f((GLfloat)0.55, (GLfloat)0.788, (GLfloat)0.451);


	// -- start button --
	draw_button(300, 300, 20, 100, "BACK");
	//RenderString(265.0f, 290.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"BACK");

	glutSwapBuffers();
	glFlush();
}

/* TODO ome fade in/out graphic*/
void splash_screen() {

	screen = SPLASH_SCREEN;
	glClear(GL_COLOR_BUFFER_BIT);
	//glColor3f((GLfloat)0.55, (GLfloat)0.788, (GLfloat)0.451);
	c_car_speed = 5;
	draw_background();
	p_car_pos_y = 160;
	p_car_pos_x +=1;
	if (p_car_pos_x > 850)
		p_car_pos_x = -200;
	draw_p_car();
	
	if (c_car_pos_x < -150) {
		c_car_pos_x = 900;
		c_car_pos_y = c_car_pos_y_values[rand() % 2];
	}
	c_car_pos_x -= c_car_speed;
	glPushMatrix();
	draw_c_car();
	glPopMatrix();


	RenderString(280.0f, 400.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"Bb4k");
	RenderString(150.0f, 250.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"Click anywhere to enter game...");

	glutPostRedisplay();
	glutSwapBuffers();
	glFlush();
	Sleep(5);
	//glutDisplayFunc(main_menu);
}

void pre_game() {

	screen = PRE_GAME;
	glClear(GL_COLOR_BUFFER_BIT);
	//glColor3f((GLfloat)0.55, (GLfloat)0.788, (GLfloat)0.451);
	c_car_speed = 2;
	draw_background();
	draw_car(150, 160,	p_car_color_values[p_car_selected_color][0], //r
						p_car_color_values[p_car_selected_color][1], //g
						p_car_color_values[p_car_selected_color][2]);//b


	//  ----------------------------------- game mode --------------------------------------------
	// campaign (default)
	draw_button((GLdouble)300, (GLdouble)320, 20, 75, "NEXT");

	// endless 
	draw_button((GLdouble)0, (GLdouble)320, 20, 75, "PREV");

	if (game_mode == ENDLESS)
		RenderString(100.0f, 320.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"ENDLESS");
	else if (game_mode == CAMPAIGN)
		RenderString(85.0f, 320.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"CAMPAIGN");
	// -------------------------------------------------------------------------------------------

	//  ----------------------------------- actions --------------------------------------------
	// back to menu
	draw_button((GLdouble)500, (GLdouble)240, 20, 75, "START");
	// start
	draw_button((GLdouble)500, (GLdouble)80, 20, 75, "MENU");

	// left arrow car select 
	draw_button((GLdouble)0, (GLdouble)160, 20, 75, "PREV");

	// right arrow car select
	draw_button((GLdouble)300, (GLdouble)160, 20, 75, "NEXT");

	// left arrow color select
	draw_button((GLdouble)0, (GLdouble)0, 20, 75, "PREV");

	// right arrow color select 
	draw_button((GLdouble)300, (GLdouble)0, 20, 75, "NEXT");

	// car color preview square
	glColor3f((GLfloat)p_car_color_r, (GLfloat)p_car_color_g, (GLfloat)p_car_color_b);
	glRecti(125, -25, 175, 25);
	// -------------------------------------------------------------------------------------------


	glutPostRedisplay();
	glutSwapBuffers();
	glFlush();
	Sleep(5);
	//glutDisplayFunc(main_menu);

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
void keyboard_input(unsigned char key, int x, int y) {

	if (key == 13)
	{
		// enter key
		names.push_back("");
		// write to file 
	}
	else if (key == 8)
	{
		// backspace
		if (names.back().size() > 0)
			names.back().pop_back();
	}
	else
	{
		// regular text
		names.back().push_back(key);
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
		if (!p_car_moving_y)
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
			glutDisplayFunc(pre_game);
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
	case IN_GAME:
		std::cout << "inside game left click";
		break;
	case SPLASH_SCREEN:
		glutDisplayFunc(main_menu);
		break;
	case PRE_GAME:

		// start
		if (x > 525 && x < 675 && y > 200 && y < 240) {
			glutDisplayFunc(pre_start);
			break;
		}
		// back 
		if (x > 525 && x < 675 && y > 360 && y < 400) {
			glutDisplayFunc(main_menu);
			break;
		}
		// -- left arrow change game mode --
		if (x > 20 && x < 175 && y > 120 && y < 160) {
			game_mode = ENDLESS;
			break;
		}
		// -- right arrow change game mode --
		if (x > 325 && x < 475 && y > 120 && y < 160) {
			game_mode = CAMPAIGN;
			break;
		}

		// -- left arrow change car --
		if (x > 20 && x < 175 && y > 280 && y < 320) {
			//glutDisplayFunc(options_screen);
			break;
		}
		// -- right arrow change car --
		if (x > 325 && x < 475 && y > 280 && y < 320) {
			//glutDisplayFunc(options_screen);
			break;
		}
		// -- left arrow change color --
		if (x > 20 && x < 175 && y > 445 && y < 480) {
			//prev color
			if (p_car_selected_color >= 1) {
				p_car_color_r = p_car_color_values[p_car_selected_color - 1][0];
				p_car_color_g = p_car_color_values[p_car_selected_color - 1][1];
				p_car_color_b = p_car_color_values[p_car_selected_color - 1][2];
				p_car_selected_color -= 1;
			}
			break;
		}
		// -- right arrow change color --
		if (x > 325 && x < 475 && y > 445 && y < 480) {
			//next color
			std::cout << sizeof(p_car_color_values) / sizeof(p_car_color_values[0]);
			if (p_car_selected_color < sizeof(p_car_color_values) / sizeof(p_car_color_values[0]) - 1) {
				p_car_color_r = p_car_color_values[p_car_selected_color + 1][0];
				p_car_color_g = p_car_color_values[p_car_selected_color + 1][1];
				p_car_color_b = p_car_color_values[p_car_selected_color + 1][2];
				p_car_selected_color += 1;
			}
			break;
		}
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
	case IN_GAME:
		std::cout << "inside game right click";
		break;
	case SPLASH_SCREEN:
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

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-100.0, 700.0, -140.0, 460.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
	glutKeyboardFunc(keyboard_input);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mouse_pos);
	glutDisplayFunc(splash_screen);
	glutReshapeFunc(reshape);


	glutMainLoop();
}