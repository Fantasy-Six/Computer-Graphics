#include <GL/GLUT.H>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#define COLOR_NEW 1.0,0.0,0.0
#define FALSE 0
#define TRUE 1
struct Point
{
	int x;
	int y;
	Point(int a = 0, int b = 0)
	{
		x = a;
		y = b;
	}
};
struct Bian_list
{
	float jx;
	float dx;
	int ymin;
	int ymax;
	bool sp;
};
struct Xb
{
	float x[10];
	int num;
};
struct Huo_list
{
	int num;
	Bian_list* next[10];
};
void InitiateHuo_list(Huo_list* H)
{
	H->num = 0;
	H->next[0] = NULL;
}
void InsertHuo_list(Huo_list* H, Bian_list* b_list)
{
	H->next[H->num] = b_list;
	H->num++;
}
void Deleteb_list(Huo_list* H, int j)
{
	int i;
	for (i = j; i < H->num; i++)
	{
		H->next[i] = H->next[i + 1];
	}
	H->num--;
}
void pai_xuHuo_list(Xb* xb)
{
	int n = xb->num;
	int i, j;
	float temp;
	for (i = 0; i < n - 1; i++)
	{
		temp = xb->x[i + 1];
		j = i;
		while (j > -1 && temp < xb->x[j])
		{
			xb->x[j + 1] = xb->x[j];
			j--;
		}
		xb->x[j + 1] = temp;
	}
}
//ScanLine用于设多边形存储的顶点序列，顶点数等于边数

void ScanLine(Point* Polygon_point, int num_point)
{
	//point_ymax,point_ymin,num_smx用来确定扫描线链表的个数
	int i, j, point_ymax, point_ymin, num_smx;
	point_ymax = Polygon_point[1].y;
	point_ymin = Polygon_point[1].y;
	for (i = 0; i < num_point; i++)
	{
		if (Polygon_point[i].y > point_ymax)
			point_ymax = Polygon_point[i].y;
		if (Polygon_point[i].y < point_ymin)
			point_ymin = Polygon_point[i].y;
	}
	num_smx = point_ymax - point_ymin;
	//建立、初始化边表
	Bian_list* b_list = (Bian_list*)malloc(num_point * sizeof(Bian_list));
	//n个点n-1条边
	for (i = 0; i < num_point - 1; i++)
	{
		//jx其实是保存扫描线与边的当前交点值，即ymin对应的jx,Cpoint(jx,ymin)
		if (Polygon_point[i].y < Polygon_point[i + 1].y)
			b_list[i].jx = Polygon_point[i].x;
		else
			b_list[i].jx = Polygon_point[i + 1].x;
		if (Polygon_point[i].y != Polygon_point[i + 1].y)
		{
			//忽略斜率为无穷，即忽略水平线
			b_list[i].dx = (float)(Polygon_point[i].x - Polygon_point[i + 1].x) /
				(Polygon_point[i].y - Polygon_point[i + 1].y);
			b_list[i].sp = FALSE;
		}
		else
		{
			//dx取较大的x
			b_list[i].dx = (Polygon_point[i].x > Polygon_point[i + 1].x) ? Polygon_point[i].x : Polygon_point[i + 1].x;
			//jx取较小的x
			b_list[i].jx = (Polygon_point[i].x < Polygon_point[i + 1].x) ? Polygon_point[i].x : Polygon_point[i + 1].x;
			b_list[i].sp = TRUE;
		}
		b_list[i].ymax = (Polygon_point[i].y > Polygon_point[i + 1].y) ? Polygon_point[i].y : Polygon_point[i + 1].y;
		b_list[i].ymin = (Polygon_point[i].y < Polygon_point[i + 1].y) ? Polygon_point[i].y : Polygon_point[i + 1].y;
	}
	//建立、初始化新边表
	Xb xb;
	Huo_list* h_list = new(Huo_list);
	h_list->num = 0;
	for (i = point_ymin; i < point_ymax;)
	{
		for (j = 0; j < num_point - 1; j++)
			if (i == b_list[j].ymin)
			{
				InsertHuo_list(h_list, &b_list[j]);
			}
		int n = 0;
		xb.num = 0;
		while (n < h_list->num)
		{
			if (h_list->next[n]->sp)
			{
				xb.x[xb.num] = h_list->next[n]->jx;
				xb.num++;
				xb.x[xb.num] = h_list->next[n]->dx;
				xb.num++;
			}
			else
			{
				xb.x[xb.num] = h_list->next[n]->jx + h_list->next[n]->dx * (i - h_list->next[n]->ymin);
				xb.num++;
			}
			n++;
		}
		pai_xuHuo_list(&xb);
		bool tc = FALSE;
		for (j = 0; j < xb.num; j++, j++)
		{
			int x, x1, xr;
			x1 = xb.x[j];
			xr = xb.x[j + 1];
			x = x1;
			while (x <= xr)
			{
				glColor3f(COLOR_NEW);
				glVertex2i(x, i);
				x++;
			}
		}
		if (i == point_ymin)
			i--;
		i++;
		for (j = 0; j < h_list->num; j++)
			if (i == h_list->next[j]->ymax)
			{
				Deleteb_list(h_list, j);
			}
		if (i == point_ymin)
			i++;
	}

}
void LineDDA(int x0, int y0, int x1, int y1)

{
	float dy, dx, x, y, m;
	dx = x1 - x0;
	dy = y1 - y0;
	if (dx != 0)
	{

		m = dy / dx;
		if (m <= 1 && m >= -1)
		{
			y = y0;
			for (x = x0; x <= x1; x++)

			{
				y = y0;
				for (x = x0; x <= x1; x++)

				{
					glVertex2i(x, int(y + 0.5));
					y += m;
				}
			}
			if (m > 1 || m < -1)
			{

				m = 1 / m;
				x = x0;
				for (y = y0; y <= y1; y++)
				{
					glVertex2i(int(x + 0.5), y);
					x += m;
				}
			}
		}
		else
		{
			int x = x0, y;
			y = (y0 <= y1) ? y0 : y1;
			int d = fabs((double)(y0 - y1));
			while (d >= 0)
			{
				glVertex2i(x, y);
				y++;
				d--;
			}
		}
	}
}

void ScanLineSegment()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.1, 0.1);
	glBegin(GL_POINTS);
	Point spt[8];
	spt[0] = Point(200, 100);
	spt[1] = Point(240, 160);
	spt[2] = Point(220, 340);
	spt[3] = Point(330, 100);
	spt[4] = Point(400, 180);
	spt[5] = Point(300, 400);
	spt[6] = Point(120, 440);
	spt[7] = Point(100, 220);
	ScanLine(spt, 8);
	glEnd();
	glFlush();
}
void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("扫描填充算法");
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 500.0, 0.0, 500.0);
	glutDisplayFunc(ScanLineSegment);
	glutMainLoop();
}