#include <iostream>
#include <gl/glew.h> // 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <vector>
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
std::random_device rd;
std::mt19937 dre(rd());
std::uniform_real_distribution<float> uid{ -1, 0.5 };
std::uniform_real_distribution<float> uid2{ -1, 1 };
GLvoid Mouse_Click(int, int, int, int);
GLvoid ConvertXY_OPENGL(int x, int y);
GLvoid Mouse_Move(int x, int y);

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
	// 타이머 지정
	glutMouseFunc(Mouse_Click);
	glutMainLoop(); // 이벤트 처리 시작
}

std::vector<float> R{ 0 };
std::vector<float> B{ 1 };
std::vector<float> G{ 1 };

std::vector<float> point_x{ 0 };
std::vector<float> point_y{ 0 };
GLclampf ox;
GLclampf oy;

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	//--- 변경된 배경색 설정
	glClearColor(1, 1, 1, 1); //--- 바탕색을 변경
	glClear(GL_COLOR_BUFFER_BIT); //--- 설정된 색으로 전체를 칠하기

	for (int i = 0; i < point_x.size(); ++i) {
		glColor3f(R.at(i), G.at(i), B.at(i));
		glRectf(point_x.at(i), point_y.at(i), point_x.at(i) + 0.15, point_y.at(i) + 0.2);
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
int pick_num = 0;

GLvoid Mouse_Click(int button, int state, int x, int y) {
	for (int i = 0; i < point_x.size(); ++i) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			left_button = true;
		}
		else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
			left_button = true;
		}
	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

GLvoid Mouse_Move(int x, int y) {
	ConvertXY_OPENGL(x, y);

	for (int i = 0; i < point_x.size(); ++i) {
		if (left_button) {
			if (ox >= point_x.at(i) && ox <= point_x.at(i) + 0.15 && oy >= point_y.at(i) && oy <= point_y.at(i) + 0.2) {
				point_x.at(i) = ox - 0.075;
				point_y.at(i) = oy - 0.1;
			}
		}
		else {

		}
	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}


GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		if (point_x.size() < 5) {
			point_x.push_back(uid(dre));
			point_y.push_back(uid(dre));
			R.push_back(uid2(dre));
			G.push_back(uid2(dre));
			B.push_back(uid2(dre));
		}
		break;
	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}