#include <stdio.h>
#include <GL/glut.h>
#include <windows.h>
#include <vector>
#include <iostream>
#include <map>
#include <cmath>
#include <unistd.h>
#include <time.h>

using namespace std;
bool game_status = false;

bool ball_down = false, ball_up = false, ball_left = false, ball_right = false;
int score = 0;
int velocity = 2;
int ball_velocity = 1;
int delay = 50;
int gap = 140;

int boundary_width = 20, window_width = 480, window_height = 640; //windowsetup

void gameSetup();
class Color {
public:
    float R, G, B;
    Color(float r, float g, float b)
    {
        R = r;
        G = g;
        B = b;
    }
};

class Bar {
public:
    int position_x;
    int height;
    int width;
    int position_y;

    Bar(int y)
    {
        position_x = rand() % 380 + 20; // (480-20)-60  since min width of ball is 60(40+20)
        height = 20; //fixed height of bars
        width = (rand() % (320 - 60)) + 60; //max possble 460(480-20) -60
        if (position_x + width > 460) {

            width = 460 - position_x; //max possble 460(480-20) -60
        }
        position_y = y;
    }
    Bar(int x, int h, int w, int y)
    {
        position_x = x;
        height = h;
        width = w;
        position_y = y;
    }
};
vector<Bar> bars;
Bar ball = Bar(0);
Color red = Color(1.0, 0, 0);
Color green = Color(0, 1.0, 0);
Color blue = Color(0, 00.0, 1.0);

void drawRect(Bar b, Color c)
{
    glColor3f(c.R, c.G, c.B);

    glBegin(GL_POLYGON); //leftbar
    glVertex2f(b.position_x, b.position_y);
    glVertex2f(b.position_x + b.width, b.position_y);

    glVertex2f(b.position_x + b.width, b.position_y + b.height);
    glVertex2f(b.position_x, b.position_y + b.height);

    glEnd();

    glFlush();
}

void setup()
{
    glClearColor(0.0, 0.0, 0.0, 0.0); // BACKGROUND COLOUR
    gluOrtho2D(0, window_width, 0, window_height + boundary_width);
}

void drawBoundaries()
{

    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_POLYGON); //leftbar
    glVertex2f(0, 0);
    glVertex2f(boundary_width, 0);

    glVertex2f(boundary_width, window_height);
    glVertex2f(0, window_height);

    glEnd();

    glBegin(GL_POLYGON); //bottombar
    glVertex2f(0, 0);
    glVertex2f(window_width, 0);

    glVertex2f(window_width, boundary_width);
    glVertex2f(0, boundary_width);

    glEnd();

    glBegin(GL_POLYGON); //rightbar
    glVertex2f(window_width - boundary_width, 0);
    glVertex2f(window_width, 0);

    glVertex2f(window_width, window_height);
    glVertex2f(window_width - boundary_width, window_height);

    glEnd();
    glFlush();

    return;
}

inline bool checkoverlap(Bar a, Bar b)
{

    if (a.position_x + 40 >= b.position_x && a.position_x <= b.position_x + b.width) {

       // cout << b.position_y << " inline  \n";
        if (b.position_y >= a.position_y && b.position_y <= a.position_y + a.height) {
            drawRect(b, blue);
            glFlush();

            return false;
        }

        if (b.position_y + b.height >= a.position_y && b.position_y + b.height <= a.position_y + a.height) {
            drawRect(b, blue);
            glFlush();

            return false;
        }
         if (a.position_y >= b.position_y && a.position_y <= b.position_y + b.height) {
            drawRect(b, blue);
            glFlush();

            return false;
        }

        if (a.position_y + a.height >= b.position_y && a.position_y + a.height <= b.position_y + b.height) {
            drawRect(b, blue);
            glFlush();

            return false;
        }
    }
    return true;
}

inline bool check_if_gameover()
{
    //	cout<<"checking\n";
    if (ball.position_y > 640 - 40)
        return false;
    if (ball.position_y <= 20)
        return false;

    for (int i = 0; i < bars.size(); i++) {
        return checkoverlap(ball, bars[i]);
    }

    return true;
}

void startGame()
{
    if (game_status) {

        game_status = check_if_gameover();

        //cout<<"after checking\n";
        if (!game_status) {
            sleep(2);
            gameSetup();
        }
    }

    glClear(GL_COLOR_BUFFER_BIT);
    drawBoundaries();

    /*	if(ball_velocity<7)
		ball_velocity=7;
		if(ball_velocity>14)
		ball_velocity=14;
		*/
    if (game_status) {
        if (ball_down)
            ball.position_y -= ball_velocity;

        else if (ball_up)
            ball.position_y += ball_velocity;

        if (ball_right) {
            if (ball.position_x + ball_velocity + 40 <= 460)
                ball.position_x += ball_velocity;
            else
                ball.position_x = 420;
        }
        else if (ball_left) {
            if (ball.position_x - ball_velocity >= 20)
                ball.position_x -= ball_velocity;
            else
                ball.position_x = 20;
        }
    }

    drawRect(ball, green);
    for (int i = 0; i < bars.size(); i++) {
        if (game_status)
            bars[i].position_y += velocity;
        drawRect(bars[i], red);
    }
    if (bars[0].position_y >= 620) {
        bars.erase(bars.begin() + 0);
        score++;
    }
    if (bars[bars.size() - 1].position_y - 20 >= gap) {
        Bar b = Bar(0);
        bars.push_back(b);
    }

    glFlush();

    Sleep(delay);
}

void gameSetup()
{

    game_status = false;
    ball_down = false, ball_up = false, ball_left = false, ball_right = false;
    score = 0;
    velocity = 4;
    ball_velocity = 6;

    bars.clear();

    for (int i = 0; i < 6; i++) {
        Bar b = Bar(620 - gap * (i + 2));

        drawRect(b, red);
        bars.push_back(b);
    }

    ball.position_x = (460 - 20) / 2 - 20;
    ball.position_y = 560;
    ball.height = 40;
    ball.width = 40;

    return;
}
void keyboard(unsigned char key, int x, int y)
{
    cout << x << " " << y << endl;

    if (!game_status) {
        game_status = true;
        startGame();
    }
    else if (key == 27) //27 corresponds to the esc key
    {
        cout << "key pressed";
        if (game_status) {
            game_status = false;
        }
    }
    if (key == 65 || key == 97) {
        ball_right = false;
        ball_left = true;

        ball_up = false;
        ball_down = false;

    }

    if (key == 68 || key == 100) {


        ball_right = true;
        ball_left = false;

        ball_up = false;
        ball_down = false;
    }

    if (key == 119 || key == 87) {


        ball_up = true;
        ball_down = false;
        ball_right = false;

        ball_left = false;
    }
    if (key == 83 || key == 115) {


        ball_right = false;
        ball_left = false;
        ball_down = true;
        ball_up = false;
    }
}

void display()
{

    drawBoundaries();
    cout << "display\n";
    glColor3f(1.0, 1.0, 1.0);
    startGame();
    glFlush();
}

int main(int argc, char* argv[])
{

    gameSetup();
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("Hello World");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(startGame);
    setup();
    glutMainLoop();

    return 0;
}
