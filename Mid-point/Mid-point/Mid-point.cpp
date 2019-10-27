#include <GL/GLUT.H>
#include <iostream>
#include <math.h>
using namespace std;
float xs, ys, xe, ye;
void MPLine(int x0, int y0, int x1, int y1) {
	int x, y, a, b, c, dm, dm1, dm2, z;
	float m;
	if (x1 < x0)//如果x1<x0，交换两点坐标
	{
		z = x0;
		x0 = x1;
		x1 = z;
		z = y0;
		y0 = y1;
		y1 = z;
	}
	a = y0 - y1; b = x1 - x0;
	x = x0; y = y0;
	if (b == 0)
		m = -1 * a * 100;
	else
		m = (float)a / (x0 - x1);

	if (m >= 0 && m <= 1)//分斜率不同的情况再执行
	{
		dm = 2 * a + b;
		dm1 = 2 * a;
		dm2 = 2 * (a + b);
		while (x < x1)
		{
			if (dm <= 0)
			{
				x++; y++;
				dm += dm2;
			}
			else
			{
				x++;
				dm += dm1;
			}
			glVertex2i(x, y);
		}
	}
	else if (m < 0 && m >= -1)
	{
		dm = 2 * a - b;
		dm1 = 2 * (a - b);
		dm2 = 2 * a;
		while (x < x1)
		{
			if (dm > 0)
			{
				x++; y--;
				dm += dm1;
			}
			else
			{
				x++;
				dm += dm2;
			}
			glVertex2i(x, y);
		}
	}
	else if (m > 1)
	{
		dm = a + 2 * b;
		dm1 = 2 * (a + b);
		dm2 = 2 * b;
		while (y < y1)
		{
			if (dm > 0)
			{
				x++; y++;
				dm += dm1;
			}
			else
			{
				y++;
				dm += dm2;
			}
			glVertex2i(x, y);
		}
	}
	else
	{
		dm = a - 2 * b;
		dm1 = -2 * b;
		dm2 = 2 * (a - b);
		while (y > y1)
		{
			if (dm <= 0)
			{
				x++; y--;
				dm += dm2;
			}
			else
			{
				y--;
				dm += dm1;
			}
			glVertex2i(x, y);
		}
	}
}
void lineSegment()
{
	glClear(GL_COLOR_BUFFER_BIT);
	//清除颜色缓冲以及深度缓冲
	glColor3f(0.0, 0.0, 0.0);
	//设置当前颜色
	glBegin(GL_POINTS);
	MPLine(xs, ys, xe, ye);
	glEnd();
	glFlush();
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	cout << "输入线段起始和终点坐标（范围为 0 - 500, 0-500）:";
	cin >> xs >> ys >> xe >> ye;
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("中点画线算法");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 500, 0.0, 500.0);
	glutDisplayFunc(lineSegment);
	//注册一个绘图函数， 这样操作系统在必要时刻就会对窗体进行重新绘制操作
	glutMainLoop();
	return 0;
}