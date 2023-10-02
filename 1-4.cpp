#include <iostream>
#include <gl/glew.h> // 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <vector>
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse_Move(int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid ConvertXY_OPENGL(int x, int y);
GLvoid Mouse_Click(int button, int state, int x, int y);
GLvoid Timer_event(int value);
std::random_device rd;
std::mt19937 dre(rd());
std::uniform_real_distribution<float> uid{ 0.1, 1 };
std::uniform_real_distribution<float> uid2{ -0.1, 0.3 };

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("Example1"); // 윈도우 생성	(윈도우 이름)
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";
	glutDisplayFunc(drawScene); // 출력 콜백함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 콜백함수 지정
	glutKeyboardFunc(Keyboard);// 키보드 입력 콜백함수 지정
	glutMotionFunc(Mouse_Move);
	glutMouseFunc(Mouse_Click);
	glutTimerFunc(100, Timer_event, 1);// 타이머 지정
	glutMainLoop(); // 이벤트 처리 시작
}


float ox, oy;

std::vector<float> R{  };
std::vector<float> G{  };
std::vector<float> B{  };

std::vector<float> point_x{  };
std::vector<float> endpoint_x{  };
std::vector<float> point_y{  };
std::vector<float> endpoint_y{  };

std::vector<float> temppoint_x{  };
std::vector<float> tempendpoint_x{  };
std::vector<float> temppoint_y{  };
std::vector<float> tempendpoint_y{  };


std::vector<bool> timer_check{ false, false, false, false, false };


GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	//--- 변경된 배경색 설정
	glClearColor(0, 0, 0, 1); //--- 바탕색을 변경
	glClear(GL_COLOR_BUFFER_BIT); //--- 설정된 색으로 전체를 칠하기

	for (int i = 0; i < point_x.size(); ++i) {
		glColor3f(R.at(i), G.at(i), B.at(i));
		glRectf(point_x.at(i), point_y.at(i), endpoint_x.at(i), endpoint_y.at(i));
	}

	glutSwapBuffers(); //--- 화면에 출력하기
}

GLvoid ConvertXY_OPENGL(int x, int y)
{
	int w = 800;
	int h = 600;

	ox = (float)(x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0));
	oy = -(float)(y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0));

}

bool left_button = false;

GLvoid Mouse_Click(int button, int state, int x, int y)
{
	ConvertXY_OPENGL(x, y);
	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		left_button = true;
		if (point_x.size() < 5) {
			R.push_back(uid(dre));
			G.push_back(uid(dre));
			B.push_back(uid(dre));
			point_x.push_back(ox - 0.075);
			point_y.push_back(oy - 0.1);
			endpoint_x.push_back(point_x.at(point_x.size() - 1) + 0.15);
			endpoint_y.push_back(point_y.at(point_y.size() - 1) + 0.2);

			temppoint_x.push_back(point_x.at(point_x.size() - 1));
			tempendpoint_x.push_back(endpoint_x.at(point_x.size() - 1));
			temppoint_y.push_back(point_y.at(point_x.size() - 1));
			tempendpoint_y.push_back(endpoint_y.at(point_x.size() - 1));

		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		left_button = true;
	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

GLvoid Mouse_Move(int x, int y) 
{

}

int size_count = -1;
float size[5] = { 0.1, 0.2, 0, -0.1, -0.2 };
bool p_check[5] = { 0 };
bool i_check[5] = { 0 };

GLvoid Timer_event(int value)
{
	int timer_num = 100;
	for (int i = 0; i < timer_check.size(); ++i) {
		if (timer_check.at(i) == true) {
			timer_num = i;
			break;
		}
	}
	size_count++;
	if (size_count >= 5) {
		size_count = 0;
	}

	bool zigzag = false;
	switch (timer_num) {
	case 0:
		for (int i = 0; i < point_x.size(); ++i) {
			if (point_x.at(i) <= -1 || point_y.at(i) >= 1 || endpoint_x.at(i) >= 1 || endpoint_y.at(i) <= -1) {
				if (p_check[i]) {
					p_check[i] = false;
				}
				else {
					p_check[i] = true;
				}
			}
		}
		for (int i = 0; i < point_x.size(); ++i) {
			if (p_check[i]) {
				point_x.at(i) += 0.1;
				endpoint_x.at(i) += 0.1;
				point_y.at(i) -= 0.1;
				endpoint_y.at(i) -= 0.1;
			}
			else {
				point_x.at(i) -= 0.1;
				endpoint_x.at(i) -= 0.1;
				point_y.at(i) += 0.1;
				endpoint_y.at(i) += 0.1;
			}
		}
		break;
	case 1:
		for (int i = 0; i < point_x.size(); ++i) {
			if (point_x.at(i) <= -1 || endpoint_x.at(i) >= 1) {
				if (i_check[i]) {
					i_check[i] = false;
				}
				else {
					i_check[i] = true;
				}
			}
		}
		for (int i = 0; i < point_x.size(); ++i) {
			if (i_check[i]) {
				point_x.at(i) = point_x.at(i) + 0.1;
				endpoint_x.at(i) = endpoint_x.at(i) + 0.1;
			}
			else {
				point_x.at(i) = point_x.at(i) - 0.1;
				endpoint_x.at(i) = endpoint_x.at(i) - 0.1;
			}
		}
		timer_check.at(1) = false;
		timer_check.at(4) = true;
		break;
	case 2:
		for (int i = 0; i < point_x.size(); ++i) {
			point_x.at(i) = point_x.at(i) - size[size_count];
			endpoint_x.at(i) = endpoint_x.at(i) + size[size_count];
			point_y.at(i) = point_y.at(i) - size[size_count];
			endpoint_y.at(i) = endpoint_y.at(i) + size[size_count];
		}
		break;
	case 3:
		for (int i = 0; i < point_x.size(); ++i) {
			R.at(i) = uid(dre);
			G.at(i) = uid(dre);
			B.at(i) = uid(dre);
		}
		break;
	case 4:
		for (int i = 0; i < point_x.size(); ++i) {
			if (point_y.at(i) >= 1 || endpoint_y.at(i) <= -1) {
				if (i_check[i]) {
					i_check[i] = false;
				}
				else {
					i_check[i] = true;
				}
			}
		}
		for (int i = 0; i < point_x.size(); ++i) {
			if (i_check[i]) {
				point_y.at(i) = point_y.at(i) + 0.1;
				endpoint_y.at(i) = endpoint_y.at(i) + 0.1;
			}
			else {
				point_y.at(i) = point_y.at(i) - 0.1;
				endpoint_y.at(i) = endpoint_y.at(i) - 0.1;
			}
		}
		timer_check.at(1) = true;
		timer_check.at(4) = false;
		break;
	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
	glutTimerFunc(100, Timer_event, 4);
}

GLvoid Keyboard(unsigned char key, int x, int y) 
{
	switch (key) {
	case 'a':
	case 'A':
		for (int i = 0; i < timer_check.size(); ++i) {
			if (i != 0) {
				timer_check.at(i) = false;
			}
		}
		if (timer_check.at(0) == true) {
			timer_check.at(0) = false;
		}
		else {
			timer_check.at(0) = true;
		}
		break;
	case 'i':
	case 'I':
		for (int i = 0; i < timer_check.size(); ++i) {
			if (i != 1) {
				timer_check.at(i) = false;
			}
		}
		if (timer_check.at(1) == true) {
			timer_check.at(1) = false;
		}
		else {
			timer_check.at(1) = true;
		}
		break;
	case 'c':
	case 'C':
		for (int i = 0; i < timer_check.size(); ++i) {
			if (i != 2) {
				timer_check.at(i) = false;
			}
		}
		if (timer_check.at(2) == true) {
			timer_check.at(2) = false;
		}
		else {
			timer_check.at(2) = true;
		}
		break;
	case 'o':
	case 'O':
		for (int i = 0; i < timer_check.size(); ++i) {
			if (i != 3) {
				timer_check.at(i) = false;
			}
		}
		if (timer_check.at(3) == true) {
			timer_check.at(3) = false;
		}
		else {
			timer_check.at(3) = true;
		}
		break;
	case 's':
	case 'S':
		for (int i = 0; i < timer_check.size(); ++i) {
			timer_check.at(i) = false;
		}
		break;
	case 'm':
	case 'M':
		for (int i = 0; i < point_x.size(); ++i) {
			point_x.at(i) = temppoint_x.at(i);
			endpoint_x.at(i) = tempendpoint_x.at(i);
			point_y.at(i) = temppoint_y.at(i);
			endpoint_y.at(i) = tempendpoint_y.at(i);
		}
		break;
	case 'r':
	case 'R':
		point_x.clear();
		point_y.clear();
		endpoint_y.clear();
		endpoint_x.clear();
		R.clear();
		G.clear();
		B.clear();
		break;
	case 'q':
	case 'Q':
		break;
	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}