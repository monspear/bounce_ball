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
float m_x = w_w / 2; // 마우스 기준
float m_y = w_h / 2 - 100;
int bar_row = 10;
int bar_col = 30;
float block_row = 10;
float block_col = 15;
float block_interval_x = 48;
float block_interval_y = 30;
int block_shpae[3] = { 25,50,75 };
int count1 = 0; // 블럭이 깨질때 마다 +1 다 깨면 게임 종료
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
int polygon_num = 18; // 18개 점으로 이루어진 원
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
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) // 마우스 좌클릭
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

void Block_line(block b_line) // 블럭의 테두리를 검은색 선으로 표시
{
	for (int i = 0; i < block_num; i++)
	{
		glColor3f(0.0, 0.0, 0.0);
		glPointSize(2);
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < vertex_num; j++) // 블록 사각형 테두리 검은색깔로     j기준(블록 점찍는 기준)// 0 1
		{                                                                                       // 3 2
			glVertex2f(b_line.c[i * vertex_num + j], b_line.r[i * vertex_num + j]);
		}
		glVertex2f(b_line.c[i * vertex_num], b_line.r[i * vertex_num]); // 제자리(위의 기준 0번을 찍어야 테두리가 색칠됨.)
		glColor3f(0.0, 1.0, 1.0); // 원래 블럭 색깔로 바꾸기
		glEnd();
	}
}

void make_blockindex() // 블럭의 인덱스 지정하기
{
	
	float x = 50; 
	float y = w_h - 50;

	for (int i = 0; i < block_num / 20; i++)
	{
		for (int j = 0; j < 20 * vertex_num; j++) { // 한 줄에 20 개씩 그리기

			if (j % vertex_num == 0) // 점을 x,y 기준으로 4개를 찍어 block 을 만듬.
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

				x += block_interval_x; // block 사이의 간격
			}

		}
		x = block_shpae[i % 2]; //다음 줄에 블럭이 시작할 x 위치
		y -= block_interval_y; // block 사이의 y 간격
	}
}

void Modeling_block(void) // 블럭의 좌표대로 블럭을 생성
{
	glColor3f(0,1.0,1.0);
	
	
	for (int i = 0; i < block_num; i++)
	{
		glBegin(GL_POLYGON);
		for (int j = 0; j < vertex_num; j++) // 점 4개를 하나씩 찍어서 그리기
		{
			glVertex2f(b_line.c[i * vertex_num + j], b_line.r[i * vertex_num + j]);
		}
		glEnd();
		Block_line(b_line); // 블럭 생성후 블럭의 테두리를 검은색 선으로 표시 
	}
}

void Modeling_bar_Screen(void) { // 흰색의 바를 만듦(마우스를 따라감.)
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
		glVertex2f(line.x[i], line.y[i]); // 마우스 기준 블럭 만들기
	}                                     // 화면 중심에서 y만 100 내려간 곳을 중심으로 블럭 만들기
	glEnd();
}



void Modeling_ball() // 공만들기
{
	float x = ball_center.x;
	float y = ball_center.y;

	
	float delta = 2.0 * 3.141592 / polygon_num; // polygon_num이 클수록 원에 가까운 모양 만듬 (현재 18)

	for (int i = 0; i < polygon_num; i++) {
		polygon_vertex[i].x = cos(delta * i);
		polygon_vertex[i].y = sin(delta * i);
	}

	
	glColor3f(0, 255, 0);

	glBegin(GL_POLYGON);
	
	for (int i = 0; i < polygon_num; i++) // 원 그리기
		glVertex3f(x + radius * polygon_vertex[i].x, y + radius * polygon_vertex[i].y, 0.0); // 공의중심 + 반지름 * 몇각형으로 만들지

	glEnd();

}

void ball_wall_collision() // 벽에 공이 부딪힐 때 마다 공의 속도가 올라감.
{
	if (ball_center.y + radius > w_h && ball_velocity.y > 0.0) //상
	{	
		ball_velocity.y *= -1.0 - plus_valocity;
	}

	if (ball_center.y - radius < 0 && ball_velocity.y < 0.0) //하
	{
		ball_velocity.y *= -1.0 - plus_valocity;
	}

	if (ball_center.x - radius < 0 && ball_velocity.x < 0.0) //좌
	{
		ball_velocity.x *= -1.0 - plus_valocity;
	}

	if (ball_center.x + radius > w_w && ball_velocity.x > 0.0) //우
	{
		ball_velocity.x *= -1.0 - plus_valocity;
	}
}

void ball_block_collison() // 공과 블럭이 충돌할 때
{
	int bin = 0;
	int num = block_num * vertex_num;
	for (int i = 0; i < num; i++)
	{
		float distance_x = (float)b_line.c[i] - ball_center.x; // 블럭의 중심좌표 , 공의 중심
		float distance_y = (float)b_line.r[i] - ball_center.y;
		//공과 블럭 사이의 x,y 좌표 거리를 구함.
		float distance = sqrt(distance_x * distance_x + distance_y * distance_y);
		float block_distance = sqrt(5 * 5 + 5 * 5);
		if (distance <= radius) // 블럭과 공이 충돌 한 경우
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
			bin = i / vertex_num; // 몇번 블럭에서 충돌했는지 i = 블럭개수(140) * 점의 개수(4) 로 0에서 559까지의 값을 가진다.
								  // bin은 몇번 블럭에서 충돌이 일어났는지를 보는 것이다.
			/*
			glColor3f(1.0, 1.0, 0.0); // 혹시나 해서 만든부분
			glBegin(GL_POLYGON);      // 공과 블럭이 충돌 했을 때 블럭이 있던 공간이 노란색이 안될 경우를 대비해 만듬.
			glVertex2f(b_line.c[bin * vertex_num], b_line.r[bin * vertex_num]);
			glVertex2f(b_line.c[bin * vertex_num + 1], b_line.r[bin * vertex_num + 1]);
			glVertex2f(b_line.c[bin * vertex_num + 2], b_line.r[bin * vertex_num + 2]);
			glVertex2f(b_line.c[bin * vertex_num + 3], b_line.r[bin * vertex_num + 3]);
			glEnd();
			*/

			b_line.c[bin * vertex_num] = -500; // 부딪힌 블럭의 좌표를 -500으로 두어 화면 밖으로 보내버림
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
	일단은 공이 블럭에 부딪혔을 때 블럭의 중심좌표를 -로 두어 화면 밖으로 표현하게 두기
	블럭에 맞을때 마다 plus_valocity 값을 줄이거나 ball_valocity 값을 init() 와 같이
	1.0으로 두도록 하기
	*/
}

void ball_bar_collison() // 공과 바의 충돌 구현
{
	float distance_x = ball_center.x;
	float distance_y = ball_center.y;
	float distance;
	float bar_x[3];
	float bar_y;
	float bar_collison;
	
	bar_x[0] = (m_x - bar_col); // 바의 왼쪽 위
	bar_x[1] = (m_x + bar_col); // 바의 오른쪽 위
	bar_x[2] = (m_x); // 바의 중심 위
	bar_y = m_y + bar_row;
	for (int i = 0; i < 3; i++) // 바의 윗부분중 좌상단 중심위 우상단부분만 계산(전체 계산하면 오히려 더 버그 걸림)
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
void Update_ball_location() // 공의 위치를 업데이트함
{
	ball_center.x += ball_velocity.x;
	ball_center.y += ball_velocity.y;
}

void check_clear() // 게임이 끝났을 때 화면표시
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
	//관측 공간을 어떻게 설정해야 하는가?
	gluOrtho2D(0,w_w,0,w_h);

	ball_wall_collision(); // 공과 벽의 충돌
	ball_block_collison(); // 공과 블럭의 충돌
	ball_bar_collison(); // 공과 바의 충돌

	Modeling_block(); // 블럭 좌표에 블럭만들기
	Modeling_bar_Screen(); // 흰색 바를 화면에 나타내기 
	

	Update_ball_location(); // 공의 위치 업데이트
	Modeling_ball(); // 공만들기
	check_clear();  // 모든 블럭 다깼을 때

	glFlush();
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(w_w, w_h);
	glutCreateWindow("공 튀기기");
	init(); // 공의 중심좌표와 속도 지정하기
	make_blockindex(); // 블럭의 좌표 지정하기
	glutDisplayFunc(RenderScene);
	glutMouseFunc(set_mouse);
	glutMotionFunc(mouse_move);
	glutIdleFunc(RenderScene);
	glutMainLoop();
	return 0;
}
