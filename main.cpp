#include <iostream>
#include <fstream>
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
#include <vector>
#include <algorithm> 

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
#define SCORES_SCREEN 13
#define SPLASH_SCREEN -1

int _run = 1;
int _win = 0;

int screen = 0; // different screen values to determine what window is open
int hover = 0;
int mouse_x = 0;
int mouse_y = 0;

// -- p(layer)_car status --
double	p_car_pos_y = GRID_Y_MID;
double	p_car_pos_x = -200.0;
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

double	x_car_pos_x = -350;

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


double street_line = 1000;

// -- scores --
struct Score
{
	std::string name;
	int score;
};
std::vector<Score> scores;
std::ifstream file_scores_r;
std::ofstream file_scores_w;

void init(void) {
	glClearColor((GLclampf)0.98, (GLclampf)0.929, (GLclampf)0.792, (GLclampf)0.0);
	glMatrixMode(GL_PROJECTION);
	glShadeModel(GL_SMOOTH);
	glOrtho(left_m, right_m, bottom_m, top_m, -1.0, 1.0);
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
		|| abs(c_car_pos_x - p_car_pos_x) > std::numeric_limits<double>::epsilon() + c_car_speed) { //double equal right way

		c_car_pos_x -= c_car_speed + action_speed;


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
	glColor3f((GLfloat)0.55, (GLfloat)0.788, (GLfloat)0.451);
	street_line -= (int)(2 * c_car_speed) + 0.1;

	if (street_line < -1000)
		street_line = 1000;
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


void draw_car(double x_car_pos, double y_car_pos, double r, double g, double b) {

	glTranslated(x_car_pos, y_car_pos, 0.0);

	glPushMatrix();

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

	glPushMatrix();
	glPushMatrix();
	draw_car(p_car_pos_x, p_car_pos_y, 0.996, 0.365, 0.149);

	// --- move car to logical grid positions while position != any grid position x or y
	if (contor_y == 1 && (p_car_pos_y != GRID_Y_MID && p_car_pos_y != GRID_Y_UPPER)) {
		p_car_pos_y = p_car_pos_y + 1;
		glRotated(3, 0, 0, 1);
		p_car_moving_y = 1;
	}
	else if (contor_y == -1 && (p_car_pos_y != GRID_Y_MID && p_car_pos_y != GRID_Y_LOWER)) {
		p_car_pos_y = p_car_pos_y - 1;
		glRotated(-3, 0, 0, 1);
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
	glPopMatrix();
	glPopMatrix();


}
// -- masinia din contrasens --
void draw_c_car() {
	glPushMatrix();
	glPushMatrix();
	draw_car(c_car_pos_x, c_car_pos_y, 0.7, 0.2, 0.2);
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

int first_anim = 1;
int dialogue = 0;
int sec_anim = 0;
int index = 0;
int next_chr = 0;
char text[27] = "CATCH ME IF YOU CAN HAHAHA";
char aux[] = "";

void pre_start(void) {

	if (first_anim || sec_anim)
		glClear(GL_COLOR_BUFFER_BIT);
	draw_background();
	Sleep(1);
	if (first_anim == 1) {

		glPopMatrix();
		draw_p_car();
		glPushMatrix();

		glPushMatrix();
		draw_x_car(index);
		glPopMatrix();


		if (p_car_pos_x != GRID_X_MID)
			p_car_pos_x = p_car_pos_x + 0.5;
		else {
			dialogue = 1;
			first_anim = 0;
		}

		if (x_car_pos_x != GRID_X_LEFT)
			x_car_pos_x = x_car_pos_x + 0.5;
	}

	if (dialogue == 1) {
		draw_background();
		if (next_chr <= strlen(text)) {
			if (index % 10 == 0) {
				strncpy(aux, text, next_chr);
				RenderString(300, 200.0f, GLUT_BITMAP_8_BY_13, (const unsigned char*)aux);
				Sleep(100);
				++next_chr;
			}
			++index;
		}
		else {
			Sleep(1000);
			dialogue = 0;
			first_anim = 0;
			sec_anim = 1;
		}
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

	if (mouse_x > btn_pos_x && mouse_x < btn_pos_x + 2.0 * btn_w &&
		mouse_y > top_m - btn_pos_y - btn_h && mouse_y < top_m - btn_pos_y + btn_h) {
		glColor3f((GLdouble)0, (GLdouble)0.5, (GLdouble)0);
		glRecti(-btn_w - 5, -btn_h - 5, btn_w - 5, btn_h - 5);
		RenderString((GLdouble)(-btn_w) / 2 - (GLdouble)((sizeof(*str) / sizeof(char)) / 4 - (GLdouble)5), (GLdouble)((GLdouble)-btn_h / 2 - (GLdouble)5), GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)str);
	}
	else {
		glColor3f((GLfloat)0.55, (GLfloat)0.788, (GLfloat)0.451);
		glRecti(-btn_w, -btn_h, btn_w, btn_h);
		RenderString((GLdouble)(-btn_w) / 2 - GLdouble(sizeof(*str) / sizeof(char)) / 4, (GLdouble)-btn_h / 2, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)str);
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
	glColor3f((GLfloat)0.55, (GLfloat)0.788, (GLfloat)0.451);
	RenderString(280.0f, 400.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"Bb4k");
	RenderString(200.0f, 250.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"Click anywhere to enter game...");
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
			glutDisplayFunc(pre_start);
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
	case SPLASH_SCREEN:
		glutDisplayFunc(main_menu);
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

void load_scores() {
	Score player;
	file_scores_r.open("scores.txt");
	while (file_scores_r.is_open() && !file_scores_r.eof())
	{
		file_scores_r >> player.name >> player.score;
		std::cout << player.name << player.score;
		scores.push_back(player);
	}
	//file_scores_r.close();
}

bool compare_scores(Score score1, Score score2) {
	return score1.score > score2.score;
}

void top_scores_screen() {
	screen = SCORES_SCREEN;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f((GLfloat)0.55, (GLfloat)0.788, (GLfloat)0.451);
	RenderString(200.0f, 250.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"TOP 5 PLAYERS");
	float dim = 200.0f;
	std::sort(scores.begin(), scores.end(), compare_scores);
	for (auto player : scores) {
		std::string line = player.name + " . . . . . . . . . . . . " + std::to_string(player.score);
		RenderString(100.0f, dim, GLUT_BITMAP_TIMES_ROMAN_24, reinterpret_cast<const unsigned char*>(line.c_str()));
		dim -= 50.0f;

	}

	glutSwapBuffers();
	glFlush();
}

int main(int argc, char** argv)
{
	load_scores();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Depaseste masinile - mini game");
	init();
	glutSpecialFunc(keyboard);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mouse_pos);
	glutDisplayFunc(top_scores_screen);
	glutReshapeFunc(reshape);


	glutMainLoop();
}