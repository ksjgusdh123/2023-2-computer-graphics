#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <gl/glew.h> // 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <vector>
#include <math.h>
char* filetobuf(const char* file);
void make_vertexShaders();
void make_fragmentShaders();
GLvoid make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Mouse_Click(int button, int state, int x, int y);
GLvoid InitBuffer();
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse_Move(int x, int y);
GLvoid Re_Buffer();
GLvoid Timer_event(int value);
GLvoid draw_all();

//--- 필요한 변수 선언
GLint width, height;
GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체
GLuint vbo[2], vao;

std::random_device rd;
std::mt19937 dre(rd());
std::uniform_real_distribution<float> uid{ 0.1, 1 };

GLfloat dot1[5][3]{ {-0.4, 0.8, 0}, {-0.4, 0.8, 0}, {-0.4, 0.8, 0}, {-0.4, 0.8, 0}, {-0.4, 0.8, 0} };
GLfloat dot2[5][3]{ {0.4, 0.8, 0 }, { 0.4, 0.8, 0 }, { 0.4, 0.8, 0 }, { 0.4, 0.8, 0 }, { 0.4, 0.8, 0 } };
GLfloat dot3[5][3]{ {-0.4, -0.8, 0}, {-0.4, -0.8, 0}, {-0.4, -0.8, 0}, {-0.4, -0.8, 0}, {-0.4, -0.8, 0} };
GLfloat dot_colors[5][3]{ {1,0,1},{1,0,1},{1,0,1},{1,0,1},{1,0,1} };

GLfloat line1[5][3]{ {-0.9, 0.3, 0}, {-0.4, 0.8, 0}, {-0.4, 0.8, 0}, {-0.4, 0.8, 0}, {-0.4, 0.8, 0} };
GLfloat line2[5][3]{ {-0.2, 0.3, 0}, {0.4, 0.6,0}, {0.4, 0.6,0}, {0.4, 0.6,0}, {0.4, 0.6,0} };
GLfloat line3[5][3]{ {0.4, -0.5, 0}, {0.8, -0.1, 0}, {0.8, -0.1, 0}, {0.8, -0.1, 0}, {0.8, -0.1, 0} };
GLfloat line_colors[5][3]{ {1,0,0},{1,0,0},{1,0,0},{1,0,0},{1,0,0} };

GLfloat tri1[5][3]{ {0.3, 0.4, 0}, {0.1, 0, 0}, {0.5, 0, 0},{0.5, 0, 0},{0.5, 0, 0} };
GLfloat tri2[5][3]{ {-0.3, 0.2, 0}, {-0.4, 0, 0}, {-0.2, 0, 0}, {-0.2, 0, 0}, {-0.2, 0, 0} };
GLfloat tri3[5][3]{ {0.5, 0.7, 0},{0.4, 0.5, 0}, {0.6, 0.5, 0}, {0.6, 0.5, 0}, {0.6, 0.5, 0} };
GLfloat tri_colors[5][3]{ {1,1,0},{1,1,0},{1,1,0},{1,1,0},{1,1,0} };

GLfloat rec1[5][3]{ {-0.7, -0.1, 0}, {-0.7, -0.4, 0},{-0.4, -0.1, 0}, {-0.4, -0.4, 0}, {-0.4, -0.4, 0} };
GLfloat rec2[5][3]{ {0.5, 0.2, 0},{0.5, 0, 0}, {0.7, 0.2, 0}, {0.7, 0, 0}, {0.7, 0, 0} };
GLfloat rec3[5][3]{ {0, -0.6, 0}, {0, -0.8, 0}, {0.2, -0.6, 0}, {0.2, -0.8, 0}, {0.2, -0.8, 0} };
GLfloat rec_colors[5][3]{ {0,1,0}, {0,1,0}, {0,1,0}, {0,1,0}, {0,1,0} };

GLfloat penta1[5][3]{ {-0.3, 0.05, 0}, {-0.2, -0.1, 0}, {-0.1, 0.15, 0}, {0, -0.1, 0},{0.1, 0.05, 0} };
GLfloat penta2[5][3]{ {0.3, -0.45, 0},{0.4, -0.6, 0},{0.5, -0.35, 0},{0.6, -0.6, 0},{0.7, -0.45, 0} };
GLfloat penta3[5][3]{ {-0.8, -0.75, 0},{-0.7, -0.9, 0},{-0.6, -0.65, 0},{-0.5, -0.9, 0},{-0.4, -0.75, 0} };
GLfloat penta_colors[5][3]{ {0,0,1}, {0,0,1}, {0,0,1}, {0,0,1}, {0,0,1} };

float start_ox, start_oy;

int clicking = 100;
int done = 100;
int plus = 0;
class POINT1 {
	GLfloat p[5][3];
	GLfloat color[5][3];
	GLuint vbo[2];

	int state;      // 0: dot, 1: line, 2: triangle
	int id;
	int move;
public:
	POINT1(GLfloat p_[5][3], GLfloat color_[5][3], int state_, int id_) {
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 3; ++j) {
				p[i][j] = p_[i][j];
				color[i][j] = color_[i][j];
			}
		}
		vbo[0] = '\0';
		vbo[1] = '\0';
		state = state_;
		id = id_;
		move = 0;
	}

	GLvoid initBuffer() {
		glGenBuffers(2, vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(p), p, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
	}

	GLvoid Re_Buffer() {
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(p), p, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
	}

	GLvoid pick_draw() {
		switch (state) {
		case 1:
			glPointSize(10);
			glDrawArrays(GL_POINTS, 0, 5); // 설정대로 출력
			break;
		case 2:
			glDrawArrays(GL_LINES, 0, 5); // 설정대로 출력
			break;
		case 3:
		case 4:
		case 5:
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 5); // 설정대로 출력
			break;
		}
	}

	GLvoid draw() {
		int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader의 'layout (location = 0)' 부분
		int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color"); //	: 1


		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // VBO Bind
			glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		}
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // VBO Bind
			glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		}
		pick_draw();
	}

	int wide(float ox, float oy) {
		float x, y;
		int mmm = 100;
		switch (state) {
		case 1:
			return 1;
		case 2:
			for (float i = 0; i <= 1; i += 0.05) {
				x = (1 - i) * p[0][0] + i * p[1][0];
				y = (1 - i) * p[0][1] + i * p[1][1];
				if (x - 0.01 <= ox && x + 0.01 >= ox && y - 0.01 <= oy && y + 0.01 >= oy) {
					return 1;
				}
			}
			break;
		case 3:
			for (float i = 0; i <= 1; i += 0.05) {
				if (p[0][0] >= ox) {
					x = (1 - i) * p[0][0] + i * p[1][0];
					y = (1 - i) * p[0][1] + i * p[1][1];
					if (x - 0.01 < ox && y + 0.01 >= oy) {
						return 1;
					}
				}
				else {
					x = (1 - i) * p[0][0] + i * p[2][0];
					y = (1 - i) * p[0][1] + i * p[2][1];
					if (x + 0.01 > ox && y + 0.01 >= oy) {
						return 1;
					}
				}
			}
			break;
		case 4:
			return 1;
		case 5:
			if (p[2][0] >= ox && p[0][1] <= oy) {
				for (float i = 0; i <= 1; i += 0.05) {
					x = (1 - i) * p[2][0] + i * p[0][0];
					y = (1 - i) * p[2][1] + i * p[0][1];
					if (x - 0.01 < ox && y + 0.01 >= oy) {
						return 1;
					}
				}
			}
			else if (p[2][0] >= ox && p[0][1] > oy) {
				for (float i = 0; i <= 1; i += 0.05) {
					x = (1 - i) * p[0][0] + i * p[1][0];
					y = (1 - i) * p[0][1] + i * p[1][1];
					if (x - 0.01 < ox && y - 0.01 < oy) {
						return 1;
					}
				}
			}
			else if (p[2][0] < ox && p[0][1] <= oy) {
				for (float i = 0; i <= 1; i += 0.05) {
					x = (1 - i) * p[2][0] + i * p[4][0];
					y = (1 - i) * p[2][1] + i * p[4][1];
					if (x + 0.01 > ox && y - 0.01 >= oy) {
						return 1;
					}
				}
			}
			else if (p[2][0] < ox && p[0][1] > oy) {
				for (float i = 0; i <= 1; i += 0.05) {
					x = (1 - i) * p[3][0] + i * p[4][0];
					y = (1 - i) * p[3][1] + i * p[4][1];
					if (x + 0.01 > ox && y + 0.01 < oy) {
						return 1;
					}
				}
			}

			break;
		}
		return 0;
	}

	int click_point(float ox, float oy) {
		switch (state) {
		case 1:
			if (p[0][0] - 0.05 <= ox && p[1][0] + 0.05 >= ox && p[0][1] - 0.05 <= oy && p[1][1] + 0.05 >= oy) {
				clicking = id;
				if (plus == 0)
					plus += state;
				move = 0;
				return 1;
			}
			else
				return 0;
			break;
		case 2:
			if (wide(ox, oy)) {
				clicking = id;
				if (plus == 0)
					plus += state;
				move = 0;
				return 1;
			}
			else
				return 0;
			break;
		case 3:
			if (p[1][0] - 0.05 <= ox && p[2][0] + 0.05 >= ox && p[1][1] - 0.05 <= oy && p[0][1] + 0.05 >= oy) {
				if (wide(ox, oy)) {
					clicking = id;
					if (plus == 0)
						plus += state;
					move = 0;
					return 1;
				}
			}
			else
				return 0;
			break;
		case 4:
			if (p[0][0] - 0.05 <= ox && p[3][0] + 0.05 >= ox && p[3][1] - 0.05 <= oy && p[0][1] + 0.05 >= oy) {
				clicking = id;
				if (plus == 0)
					plus += state;
				move = 0;
				return 1;
			}
			else
				return 0;
			break;
		case 5:
			if (p[0][0] - 0.05 <= ox && p[4][0] + 0.05 >= ox && p[1][1] - 0.05 <= oy && p[2][1] + 0.05 >= oy) {
				if (wide(ox, oy)) {
					clicking = id;
					if (plus == 0)
						plus += state;
					move = 0;
					return 1;
				}
			}
			else
				return 0;
			break;
		}
	}

	GLvoid move_point(float ox, float oy) {
		if (clicking == 100 || clicking == id) {
			if (click_point(ox, oy)) {
				for (int i = 0; i < 5; ++i) {
					if (start_ox > ox) {
						p[i][0] -= start_ox - ox;
						p[i][1] -= start_oy - oy;
					}
					else {
						p[i][0] += ox - start_ox;
						p[i][1] += oy - start_oy;
					}
				}
				start_ox = ox;
				start_oy = oy;
			}
		}
	}

	int done_check(float ox, float oy) {
		if (clicking != id) {
			switch (state) {
			case 1:
				if (p[0][0] - 0.05 <= ox && p[1][0] + 0.05 >= ox && p[0][1] - 0.05 <= oy && p[1][1] + 0.05 >= oy) {
					done = id;
					plus += state;
					return 1;
				}
				else
					return 0;
				break;
			case 2:
				if (wide(ox, oy)) {
					done = id;
					plus += state;
					return 1;
				}
				else
					return 0;
				break;
			case 3:
				if (p[1][0] - 0.05 <= ox && p[2][0] + 0.05 >= ox && p[1][1] - 0.05 <= oy && p[0][1] + 0.05 >= oy) {
					if (wide(ox, oy)) {
						done = id;
						plus += state;
						return 1;
					}
				}
				else
					return 0;
				break;
			case 4:
				if (p[0][0] - 0.05 <= ox && p[3][0] + 0.05 >= ox && p[3][1] - 0.05 <= oy && p[0][1] + 0.05 >= oy) {
					done = id;
					plus += state;
					return 1;
				}
				else
					return 0;
				break;
			case 5:
				if (p[0][0] - 0.05 <= ox && p[4][0] + 0.05 >= ox && p[1][1] - 0.05 <= oy && p[2][1] + 0.05 >= oy) {
					if (wide(ox, oy)) {
						done = id;
						plus += state;
						return 1;
					}
				}
				else
					return 0;
				break;
			}
		}
	}

	GLvoid assemble(float ox, float oy) {
		if (done == id && done != clicking) {
			int menu = plus % 5;
			switch (menu) {
			case 1:
				for (int i = 0; i < 5; ++i) {
					p[i][0] = ox;
					p[i][1] = oy;
					color[i][0] = 1;
					color[i][1] = 0;
					color[i][2] = 1;
				}
				state = 1;
				break;
			case 2:
				for (int i = 0; i < 5; ++i) {
					p[i][0] = ox;
					p[i][1] = oy;
					color[i][0] = 1;
					color[i][1] = 0;
					color[i][2] = 0;
				}
				p[0][0] = ox - 0.2;
				p[0][1] = oy - 0.2;
				state = 2;
				break;
			case 3:
				for (int i = 0; i < 5; ++i) {
					p[i][0] = ox;
					p[i][1] = oy;
					color[i][0] = 1;
					color[i][1] = 1;
					color[i][2] = 0;
				}
				p[0][0] = ox - 0.1;
				p[0][1] = oy + 0.2;
				p[1][0] = ox - 0.2;
				state = 3;
				break;
			case 4:
				for (int i = 0; i < 5; ++i) {
					p[i][0] = ox;
					p[i][1] = oy;
					color[i][0] = 0;
					color[i][1] = 1;
					color[i][2] = 0;
				}
				p[0][0] = ox - 0.3;
				p[0][1] = oy + 0.3;
				p[1][0] = ox - 0.3;
				p[2][1] = oy + 0.3;
				state = 4;
				break;
			case 0:
				for (int i = 0; i < 5; ++i) {
					p[i][0] = ox;
					p[i][1] = oy;
					color[i][0] = 0;
					color[i][1] = 0;
					color[i][2] = 1;
				}
				p[0][0] = ox - 0.2;
				p[0][1] = oy - 0.1;
				p[1][0] = ox - 0.1;
				p[1][1] = oy - 0.25;
				p[3][0] = ox + 0.1;
				p[3][1] = oy - 0.25;
				p[4][0] = ox + 0.2;
				p[4][1] = oy - 0.1;
				state = 5;


				break;
			}
			move = 1;
		}
		else if (clicking == id && done != 100) {
			for (int i = 0; i < 5; ++i) {
				p[i][0] = 100;
				p[i][1] = 100;
			}
		}

	}

	GLvoid moving() {
		if (move) {
			switch (move) {
			case 1:
				for (int i = 0; i < 5; ++i) {
					p[i][0] += 0.05;
					p[i][1] += 0.05;
				}
				for (int i = 0; i < 5; ++i) {
					if (p[i][0] >= 0.9) {
						move = 2;
					}
					else if (p[i][1] >= 0.9) {
						move = 3;
					}
				}


				break;
			case 2:
				for (int i = 0; i < 5; ++i) {
					p[i][0] -= 0.05;
					p[i][1] += 0.05;
				}
				for (int i = 0; i < 5; ++i) {
					if (p[i][1] >= 0.9) {
						move = 3;
					}
					else if (p[i][0] <= -0.9) {
						move = 4;
					}
				}
				break;
			case 3:
				for (int i = 0; i < 5; ++i) {
					p[i][0] -= 0.05;
					p[i][1] -= 0.05;
				}
				for (int i = 0; i < 5; ++i) {
					if (p[i][0] <= -0.9) {
						move = 4;
					}
					else if (p[i][1] <= -0.9) {
						move = 1;
					}
				}
				break;
			case 4:
				for (int i = 0; i < 5; ++i) {
					p[i][0] += 0.05;
					p[i][1] -= 0.05;
				}
				for (int i = 0; i < 5; ++i) {
					if (p[i][1] <= -0.9) {
						move = 1;
					}
					else if (p[i][0] >= 0.9) {
						move = 2;
					}
				}
				break;
			}
		}
	}

};


POINT1 p1(dot1, dot_colors, 1, 0);
POINT1 p2(dot2, dot_colors, 1, 1);
POINT1 p3(dot3, dot_colors, 1, 2);
POINT1 l1(line1, line_colors, 2, 3);
POINT1 l2(line2, line_colors, 2, 4);
POINT1 l3(line3, line_colors, 2, 5);
POINT1 t1(tri1, tri_colors, 3, 6);
POINT1 t2(tri2, tri_colors, 3, 7);
POINT1 t3(tri3, tri_colors, 3, 8);
POINT1 r1(rec1, rec_colors, 4, 9);
POINT1 r2(rec2, rec_colors, 4, 10);
POINT1 r3(rec3, rec_colors, 4, 11);
POINT1 pt1(penta1, penta_colors, 5, 12);
POINT1 pt2(penta2, penta_colors, 5, 13);
POINT1 pt3(penta3, penta_colors, 5, 14);

//--- 메인 함수
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example1");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	make_shaderProgram();
	InitBuffer();
	glutDisplayFunc(drawScene);
	glutMouseFunc(Mouse_Click);
	glutTimerFunc(100, Timer_event, 1);
	glutMotionFunc(Mouse_Move);
	glutKeyboardFunc(Keyboard);
	p1.initBuffer();
	p2.initBuffer();
	p3.initBuffer();
	l1.initBuffer();
	l2.initBuffer();
	l3.initBuffer();
	t1.initBuffer();
	t2.initBuffer();
	t3.initBuffer();
	r1.initBuffer();
	r2.initBuffer();
	r3.initBuffer();
	pt1.initBuffer();
	pt2.initBuffer();
	pt3.initBuffer();
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

bool p_check = true;
int dot_num = 0;
int draw_dot = 218;
GLfloat rColor = 1.0, gColor = 1.0, bColor = 1.0;

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	//--- 변경된 배경색 설정
	glClearColor(rColor, gColor, bColor, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderProgramID);
	//--- 사용할 VAO 불러오기
	glBindVertexArray(vao);
	//--- 삼각형 그리기
	draw_all();

	glutSwapBuffers(); //--- 화면에 출력하기
}

//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

GLint result;
GLchar errorLog[512];

void make_vertexShaders()
{
	GLchar* vertexSource;
	vertexSource = filetobuf("예시vertex.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexShader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	GLchar* fragmentSource;
	fragmentSource = filetobuf("예시fragment.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentShader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

GLvoid make_shaderProgram()
{
	GLuint shaderID;
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
	glUseProgram(shaderProgramID);

}

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading 
	if (!fptr) // Return NULL on failure 
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file 
	length = ftell(fptr); // Find out how many bytes into the file we are 
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator 
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file 
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer 
	fclose(fptr); // Close the file 
	buf[length] = 0; // Null terminator 
	return buf; // Return the buffer 
}




float ox, oy;

GLvoid ConvertXY_OPENGL(int x, int y)
{
	int w = 800;
	int h = 600;

	ox = (float)(x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0));
	oy = -(float)(y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0));

}

bool click = false;

GLvoid Mouse_Click(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		ConvertXY_OPENGL(x, y);
		click = true;
		start_ox = ox;
		start_oy = oy;
		done = 100;
		plus = 0;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && clicking != 100) {
		p1.done_check(ox, oy);
		p2.done_check(ox, oy);
		p3.done_check(ox, oy);
		t1.done_check(ox, oy);
		t2.done_check(ox, oy);
		t3.done_check(ox, oy);
		l1.done_check(ox, oy);
		l2.done_check(ox, oy);
		l3.done_check(ox, oy);
		r1.done_check(ox, oy);
		r2.done_check(ox, oy);
		r3.done_check(ox, oy);
		pt1.done_check(ox, oy);
		pt2.done_check(ox, oy);
		pt3.done_check(ox, oy);

		if (done != 100) {
			p1.assemble(ox, oy);
			p2.assemble(ox, oy);
			p3.assemble(ox, oy);
			t1.assemble(ox, oy);
			t2.assemble(ox, oy);
			t3.assemble(ox, oy);
			l1.assemble(ox, oy);
			l2.assemble(ox, oy);
			l3.assemble(ox, oy);
			r1.assemble(ox, oy);
			r2.assemble(ox, oy);
			r3.assemble(ox, oy);
			pt1.assemble(ox, oy);
			pt2.assemble(ox, oy);
			pt3.assemble(ox, oy);
		}
		click = false;
		clicking = 100;
	}
}

GLvoid Mouse_Move(int x, int y)
{
	ConvertXY_OPENGL(x, y);
	if (click) {
		p1.move_point(ox, oy);
		p2.move_point(ox, oy);
		p3.move_point(ox, oy);
		t1.move_point(ox, oy);
		t2.move_point(ox, oy);
		t3.move_point(ox, oy);
		l1.move_point(ox, oy);
		l2.move_point(ox, oy);
		l3.move_point(ox, oy);
		r1.move_point(ox, oy);
		r2.move_point(ox, oy);
		r3.move_point(ox, oy);
		pt1.move_point(ox, oy);
		pt2.move_point(ox, oy);
		pt3.move_point(ox, oy);
	}


	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}


GLvoid Timer_event(int value) {

	p1.moving();
	p2.moving();
	p3.moving();
	t1.moving();
	t2.moving();
	t3.moving();
	l1.moving();
	l2.moving();
	l3.moving();
	r1.moving();
	r2.moving();
	r3.moving();
	pt1.moving();
	pt2.moving();
	pt3.moving();

	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
	glutTimerFunc(100, Timer_event, 4);
}



GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {

	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}


GLvoid InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao); //--- VAO를 바인드하기
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

GLvoid draw_all() {
	p1.draw();
	p1.Re_Buffer();
	p2.draw();
	p2.Re_Buffer();
	p3.draw();
	p3.Re_Buffer();

	l1.draw();
	l1.Re_Buffer();
	l2.draw();
	l2.Re_Buffer();
	l3.draw();
	l3.Re_Buffer();

	t1.draw();
	t1.Re_Buffer();
	t2.draw();
	t2.Re_Buffer();
	t3.draw();
	t3.Re_Buffer();

	r1.draw();
	r1.Re_Buffer();
	r2.draw();
	r2.Re_Buffer();
	r3.draw();
	r3.Re_Buffer();

	pt1.draw();
	pt1.Re_Buffer();
	pt2.draw();
	pt2.Re_Buffer();
	pt3.draw();
	pt3.Re_Buffer();
}