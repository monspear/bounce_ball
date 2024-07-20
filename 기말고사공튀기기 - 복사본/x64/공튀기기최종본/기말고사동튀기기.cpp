#include <iostream>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h> 
#include <math.h>
using namespace std;

#define block_num 140
#define vertex_num 4

int w_w = 1000;
int w_h = 800;
float m_x = w_w / 2; // ���콺 ����
float m_y = w_h / 2 - 100;
int bar_row = 10;
int bar_col = 30;
float block_row = 10;
float block_col = 15;
float block_interval_x = 48;
float block_interval_y = 30;
int block_shpae[3] = { 25,50,75 };
int count1 = 0; // ���� ������ ���� +1 �� ���� ���� ����
struct bar
{
	float x[vertex_num] = { 0, };
	float y[vertex_num] = { 0, };
};

struct block
{
	float c[block_num * vertex_num] = { 0, };
	float r[block_num * vertex_num] = { 0, };
};

struct Point {
	float	x;
	float	y;
};

block b_line;
block center;
Point ball_center;
Point ball_velocity;
bar line;
float radius = 10.0;
float plus_valocity = 0.0;
int polygon_num = 18; // 18�� ������ �̷���� ��
Point* polygon_vertex = new Point[polygon_num];
float x = 50;
float y = w_h - 50;
void init()
{
	ball_center.x = w_w / 2;
	ball_center.y = w_h / 2;
	ball_velocity.x = 2.0;
	ball_velocity.y = 2.0;
}

void set_mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) // ���콺 ��Ŭ��
	{
		m_x = x;
		if (y < w_h - 200)
			m_y = 200;
		else
			m_y = w_h - y;
	}
	cout << x << " " << y << '\n';
	cout << m_x << " " << m_y << '\n';
	glutPostRedisplay();
}
void mouse_move(int x, int y)
{
	m_x = x;
	if (y < w_h - 200)
		m_y = 200;
	else
		m_y = w_h - y;
	glutPostRedisplay();
}

void Block_line(block b_line) // ���� �׵θ��� ������ ������ ǥ��
{
	for (int i = 0; i < block_num; i++)
	{
		glColor3f(0.0, 0.0, 0.0);
		glPointSize(2);
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < vertex_num; j++) // ��� �簢�� �׵θ� ���������     j����(��� ����� ����)// 0 1
		{                                                                                       // 3 2
			glVertex2f(b_line.c[i * vertex_num + j], b_line.r[i * vertex_num + j]);
		}
		glVertex2f(b_line.c[i * vertex_num], b_line.r[i * vertex_num]); // ���ڸ�(���� ���� 0���� ���� �׵θ��� ��ĥ��.)
		glColor3f(0.0, 1.0, 1.0); // ���� �� ����� �ٲٱ�
		glEnd();
	}
}

void make_blockindex() // ���� �ε��� �����ϱ�
{
	
	float x = 50; 
	float y = w_h - 50;

	for (int i = 0; i < block_num / 20; i++)
	{
		for (int j = 0; j < 20 * vertex_num; j++) { // �� �ٿ� 20 ���� �׸���

			if (j % vertex_num == 0) // ���� x,y �������� 4���� ��� block �� ����.
			{
				b_line.c[i * vertex_num * 20 + j] = x - block_col;
				b_line.r[i * vertex_num * 20 + j] = y + block_row;
			}
			if (j % vertex_num == 1)
			{
				b_line.c[i * vertex_num * 20 + j] = x + block_col;
				b_line.r[i * vertex_num * 20 + j] = y + block_row;
			}
			if (j % vertex_num == 2)
			{
				b_line.c[i * vertex_num * 20 + j] = x + block_col;
				b_line.r[i * vertex_num * 20 + j] = y - block_row;
			}
			if (j % vertex_num == 3)
			{
				b_line.c[i * vertex_num * 20 + j] = x - block_col;
				b_line.r[i * vertex_num * 20 + j] = y - block_row;

				center.c[i * vertex_num * 20 + j] = x;
				center.r[i * vertex_num * 20 + j] = y;

				x += block_interval_x; // block ������ ����
			}

		}
		x = block_shpae[i % 2]; //���� �ٿ� ���� ������ x ��ġ
		y -= block_interval_y; // block ������ y ����
	}
}

void Modeling_block(void) // ���� ��ǥ��� ���� ����
{
	glColor3f(0,1.0,1.0);
	
	
	for (int i = 0; i < block_num; i++)
	{
		glBegin(GL_POLYGON);
		for (int j = 0; j < vertex_num; j++) // �� 4���� �ϳ��� �� �׸���
		{
			glVertex2f(b_line.c[i * vertex_num + j], b_line.r[i * vertex_num + j]);
		}
		glEnd();
		Block_line(b_line); // �� ������ ���� �׵θ��� ������ ������ ǥ�� 
	}
}

void Modeling_bar_Screen(void) { // ����� �ٸ� ����(���콺�� ����.)
	glPointSize(5.0);
	glColor3f(1.0, 1.0, 1.0);
	
	line.x[0] = m_x - bar_col;
	line.y[0] = m_y + bar_row;
	line.x[1] = m_x + bar_col;
	line.y[1] = m_y + bar_row;
	line.x[2] = m_x + bar_col;
	line.y[2] = m_y - bar_row;
	line.x[3] = m_x - bar_col;
	line.y[3] = m_y - bar_row;
	glBegin(GL_POLYGON);
	for (int i = 0; i < vertex_num; i++) {
		glVertex2f(line.x[i], line.y[i]); // ���콺 ���� �� �����
	}                                     // ȭ�� �߽ɿ��� y�� 100 ������ ���� �߽����� �� �����
	glEnd();
}



void Modeling_ball() // �������
{
	float x = ball_center.x;
	float y = ball_center.y;

	
	float delta = 2.0 * 3.141592 / polygon_num; // polygon_num�� Ŭ���� ���� ����� ��� ���� (���� 18)

	for (int i = 0; i < polygon_num; i++) {
		polygon_vertex[i].x = cos(delta * i);
		polygon_vertex[i].y = sin(delta * i);
	}

	
	glColor3f(0, 255, 0);

	glBegin(GL_POLYGON);
	
	for (int i = 0; i < polygon_num; i++) // �� �׸���
		glVertex3f(x + radius * polygon_vertex[i].x, y + radius * polygon_vertex[i].y, 0.0); // �����߽� + ������ * ������� ������

	glEnd();

}

void ball_wall_collision() // ���� ���� �ε��� �� ���� ���� �ӵ��� �ö�.
{
	if (ball_center.y + radius > w_h && ball_velocity.y > 0.0) //��
	{	
		ball_velocity.y *= -1.0 - plus_valocity;
	}

	if (ball_center.y - radius < 0 && ball_velocity.y < 0.0) //��
	{
		ball_velocity.y *= -1.0 - plus_valocity;
	}

	if (ball_center.x - radius < 0 && ball_velocity.x < 0.0) //��
	{
		ball_velocity.x *= -1.0 - plus_valocity;
	}

	if (ball_center.x + radius > w_w && ball_velocity.x > 0.0) //��
	{
		ball_velocity.x *= -1.0 - plus_valocity;
	}
}

void ball_block_collison() // ���� ���� �浹�� ��
{
	int bin = 0;
	int num = block_num * vertex_num;
	for (int i = 0; i < num; i++)
	{
		float distance_x = (float)b_line.c[i] - ball_center.x; // ���� �߽���ǥ , ���� �߽�
		float distance_y = (float)b_line.r[i] - ball_center.y;
		//���� �� ������ x,y ��ǥ �Ÿ��� ����.
		float distance = sqrt(distance_x * distance_x + distance_y * distance_y);
		float block_distance = sqrt(5 * 5 + 5 * 5);
		if (distance <= radius) // ���� ���� �浹 �� ���
		{
			if (abs(distance_y) >= abs(distance_x) )
			{
				ball_velocity.y *= -1.0;
			}
			if (abs(distance_x) >= abs(distance_y) )
			{
				ball_velocity.x *= -1.0;
			}
			cout << distance_x << '\n';
			cout << distance_y << '\n';
			count1++;
			bin = i / vertex_num; // ��� ������ �浹�ߴ��� i = ������(140) * ���� ����(4) �� 0���� 559������ ���� ������.
								  // bin�� ��� ������ �浹�� �Ͼ������ ���� ���̴�.
			/*
			glColor3f(1.0, 1.0, 0.0); // Ȥ�ó� �ؼ� ����κ�
			glBegin(GL_POLYGON);      // ���� ���� �浹 ���� �� ���� �ִ� ������ ������� �ȵ� ��츦 ����� ����.
			glVertex2f(b_line.c[bin * vertex_num], b_line.r[bin * vertex_num]);
			glVertex2f(b_line.c[bin * vertex_num + 1], b_line.r[bin * vertex_num + 1]);
			glVertex2f(b_line.c[bin * vertex_num + 2], b_line.r[bin * vertex_num + 2]);
			glVertex2f(b_line.c[bin * vertex_num + 3], b_line.r[bin * vertex_num + 3]);
			glEnd();
			*/

			b_line.c[bin * vertex_num] = -500; // �ε��� ���� ��ǥ�� -500���� �ξ� ȭ�� ������ ��������
			b_line.r[bin * vertex_num] = -500;
			b_line.c[bin * vertex_num + 1] = -500;
			b_line.r[bin * vertex_num + 1] = -500;
			b_line.c[bin * vertex_num + 2] = -500;
			b_line.r[bin * vertex_num + 2] = -500;
			b_line.c[bin * vertex_num + 3] = -500;
			b_line.r[bin * vertex_num + 3] = -500;
			glFlush();
		}

	}
	

	/*
	�ϴ��� ���� ���� �ε����� �� ���� �߽���ǥ�� -�� �ξ� ȭ�� ������ ǥ���ϰ� �α�
	���� ������ ���� plus_valocity ���� ���̰ų� ball_valocity ���� init() �� ����
	1.0���� �ε��� �ϱ�
	*/
}

void ball_bar_collison() // ���� ���� �浹 ����
{
	float distance_x = ball_center.x;
	float distance_y = ball_center.y;
	float distance;
	float bar_x[3];
	float bar_y;
	float bar_collison;
	
	bar_x[0] = (m_x - bar_col); // ���� ���� ��
	bar_x[1] = (m_x + bar_col); // ���� ������ ��
	bar_x[2] = (m_x); // ���� �߽� ��
	bar_y = m_y + bar_row;
	for (int i = 0; i < 3; i++) // ���� ���κ��� �»�� �߽��� ���ܺκи� ���(��ü ����ϸ� ������ �� ���� �ɸ�)
	{
		distance_x = ball_center.x - bar_x[i];
		distance_y = ball_center.y - bar_y;
		distance = sqrt(distance_x * distance_x + distance_y * distance_y);
		if (distance <= radius)
		{
			ball_velocity.y *= -1.0;
		}
	}
	
}
void Update_ball_location() // ���� ��ġ�� ������Ʈ��
{
	ball_center.x += ball_velocity.x;
	ball_center.y += ball_velocity.y;
}

void check_clear() // ������ ������ �� ȭ��ǥ��
{
	if (count1 >= block_num)
	{
		glClearColor(0.0, 0.0, 0.0, 0.0); // 
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, w_w, 0, w_h);

		glColor3f(1.0, 1.0, 1.0);
		glPointSize(5.0);
		glBegin(GL_LINES);
		glVertex2f(0, w_h / 2);
		glVertex2f(w_w, w_h / 2);
		glVertex2f(w_w/2, 0);
		glVertex2f(w_w/2, w_h);
		glEnd();
		glFlush();
	}
}

void RenderScene(void) {

	glClearColor(1.0, 1.0, 0.0, 0.0); // Set display-window color to Yellow
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//���� ������ ��� �����ؾ� �ϴ°�?
	gluOrtho2D(0,w_w,0,w_h);

	ball_wall_collision(); // ���� ���� �浹
	ball_block_collison(); // ���� ���� �浹
	ball_bar_collison(); // ���� ���� �浹

	Modeling_block(); // �� ��ǥ�� �������
	Modeling_bar_Screen(); // ��� �ٸ� ȭ�鿡 ��Ÿ���� 
	

	Update_ball_location(); // ���� ��ġ ������Ʈ
	Modeling_ball(); // �������
	check_clear();  // ��� �� �ٲ��� ��

	glFlush();
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(w_w, w_h);
	glutCreateWindow("�� Ƣ���");
	init(); // ���� �߽���ǥ�� �ӵ� �����ϱ�
	make_blockindex(); // ���� ��ǥ �����ϱ�
	glutDisplayFunc(RenderScene);
	glutMouseFunc(set_mouse);
	glutMotionFunc(mouse_move);
	glutIdleFunc(RenderScene);
	glutMainLoop();
	return 0;
}
