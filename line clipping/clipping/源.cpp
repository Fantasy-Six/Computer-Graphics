#include <GL/GLUT.h>
#include <iostream>
using namespace std;
struct Point {
	float x, y;
	int code;
};
Point a, b;//定义两个点
int wmin = 200, wmax = 400;//裁剪区间的大小
int hmin = 200, hmax = 400; 
int flag = 1;                         //用于记录用户点击的次数，单次画点，两次画线。
int window_size = 600;           //显示界面的大小
void swap(Point* i, Point* j) {
	Point p = *i;
	*i = *j;
	*j = p;
}
void show_p(Point a) {
	cout << a.x << "===" << a.y << "::" << a.code << endl;
}


void point_coding(Point& p) {
	p.code = 0x00;//先初始化为0000；
	cout << p.x << "--" << p.y;
	if (p.x < wmin)
		p.code = p.code | 0x1;
	if (p.x > wmax)
		p.code = p.code | 0x2;
	if (p.y > hmax) 
		p.code = p.code | 0x4;
	if (p.y < hmin) 
		p.code = p.code | 0x8;
	cout << "coding end" << p.code << endl;
}
void  cut_by_coding(Point p1, Point p2) {
	int finish = 0;
	float k = 0; 
	while (!finish) { 
		cout << "kk" << endl;
		point_coding(p1);
		point_coding(p2);
	
		if (p1.code & p2.code) { 
			cout << "outer" << endl;
			return;
		}
		if (!(p1.code | p2.code)) { 
			cout << "inner" << endl;
			glBegin(GL_LINES);
			glColor3f(0.0, 1.0, 0.0);
			glVertex2f(p1.x, window_size - p1.y);
			glVertex2f(p2.x, window_size - p2.y);
			glEnd();
			glFlush();
			return;
		}
		if (!p1.code) { 
			swap(p1, p2); 
		}
		if (p1.x != p2.x) {
			k = (p2.y - p1.y) / (p2.x - p1.x);
		}
		if (p1.code & 0x1) {  
			p1.y += (wmin - p1.x) * k;
			p1.x = wmin; 
		}
		else if (p1.code & 0x2) {
			p1.y += (wmax - p1.x) * k;
			p1.x = wmax;
		}
		else if (p1.code & 0x4) {
			if (p2.x != p1.x) {
				p1.x += (hmax - p1.y) / k;
				p1.y = hmax;
			} 
		}
		else if (p1.code & 0x8) { 
			if (p2.x != p1.x) {
				p1.x += (hmin - p1.y) / k;
				p1.y = hmin;
			}
		}
		cout << "------" << endl;
		show_p(p1);
		show_p(p2);
		cout << "------" << endl;
	}
}
void InitEnvironment()
{
	glClearColor(0.0, 0.0, 0.0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(7);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluOrtho2D(0, window_size, 0, window_size);
}

void init_Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.98f, 0.625f, 0.12f);
	glRectf(wmin, hmax, wmax, hmin);
	glFlush();
	glFlush();
}

void OnMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (flag % 2) {
			glColor3f(1, 0, 0);
			glBegin(GL_POINTS);
			glVertex2f(x, window_size - y);
			a.x = x; a.y = y;
			point_coding(a);
			glEnd();
			glFlush();
		}
		else {
			glColor3f(1, 0, 0);
			glBegin(GL_POINTS);
			glVertex2f(x, window_size - y);
			glEnd();
			b.x = x; b.y = y;
			point_coding(b);
			glBegin(GL_LINES);
			glColor3f(1.0, 0.0, 0.0);
			glVertex2f(a.x, window_size - a.y);
			glVertex2f(b.x, window_size - b.y);
			glEnd();
			glFlush();
			cut_by_coding(a, b);
		}
		flag++;
	}
}

int main(int argc, char* argv[])
{
	cout << "begin!" << endl;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(300, 100);
	glutInitWindowSize(window_size, window_size);
	glutCreateWindow("hw1_cut_line");
	InitEnvironment();
	glutMouseFunc(&OnMouse);
	glutDisplayFunc(&init_Display);
	glutMainLoop();
	return 0;
}