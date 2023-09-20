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
GLvoid Make_Food();
std::default_random_engine dre;
std::uniform_real_distribution<float> uid{ -1, 0.8 };
std::uniform_real_distribution<float> uid2{ 0, 0.9 };

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
	Make_Food();
	glutDisplayFunc(drawScene); // 출력 콜백함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 콜백함수 지정
	glutKeyboardFunc(Keyboard);// 키보드 입력 콜백함수 지정
	glutMotionFunc(Mouse_Move);
	glutMouseFunc(Mouse_Click);
	// 타이머 지정
	glutMainLoop(); // 이벤트 처리 시작
}

std::vector<float> food_x{};
std::vector<float> food_y{};
std::vector<float> food_R{};
std::vector<float> food_G{};
std::vector<float> food_B{};

float player_x;
float player_y;

float endplayer_x;
float endplayer_y;
float R = 0;
float G = 0;
float B = 0;
bool left_button = false;

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	//--- 변경된 배경색 설정
	glClearColor(1, 1, 1, 1); //--- 바탕색을 변경
	glClear(GL_COLOR_BUFFER_BIT); //--- 설정된 색으로 전체를 칠하기

	for (int i = 0; i < food_x.size(); ++i) {
		glColor3f(food_R.at(i), food_G.at(i), food_B.at(i));
		glRectf(food_x.at(i), food_y.at(i), food_x.at(i) + 0.075, food_y.at(i) + 0.1);
	}
	if (left_button) {
		glColor3f(R, G, B);
		glRectf(player_x, player_y, endplayer_x, endplayer_y);
	}
	glutSwapBuffers(); //--- 화면에 출력하기
}

float ox, oy;

GLvoid ConvertXY_OPENGL(int x, int y)
{
	int w = 800;
	int h = 600;

	ox = (float)(x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0));
	oy = -(float)(y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0));

}

float up_x = 0.075;
float up_y = 0.1;

GLvoid Mouse_Move(int x, int y)
{
	ConvertXY_OPENGL(x, y);

	if (left_button) {
		
		player_x = ox - up_x;
		player_y = oy - up_y;
		endplayer_x = player_x + up_x * 2;
		endplayer_y = player_y + up_y * 2;

		for (int i = 0; i < food_x.size(); ++i) {
			if (player_x <= food_x.at(i) && endplayer_x >= food_x.at(i) + 0.075
				&& endplayer_y >= food_y.at(i) + 0.1 && player_y <= food_y.at(i)) {
				food_x.at(i) = 100;
				food_y.at(i) = 100;

				up_x += 0.05;
				up_y += 0.05;

				R = food_R.at(i);
				G = food_G.at(i);
				B = food_B.at(i);


			}
		}

	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다



}

GLvoid Mouse_Click(int button, int state, int x, int y)
{
	ConvertXY_OPENGL(x, y);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		left_button = true;
		player_x = ox - 0.075;
		player_y = oy - 0.1;
		endplayer_x = player_x + 0.15;
		endplayer_y = player_y + 0.2;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		left_button = true;
		player_x = 100;
		player_y = 100;
		endplayer_x = 100;
		endplayer_y = 100;
		up_x = 0.075;
		up_y = 0.1;
		R = 0;
		G = 0;
		B = 0;
	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'r':
		food_x.clear();
		food_y.clear();
		food_G.clear();
		food_B.clear();
		food_R.clear();
		Make_Food();
		break;
	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

GLvoid Make_Food() {
	for (int i = 0; i < 20; ++i) {
		food_x.push_back(uid(dre));
		food_y.push_back(uid(dre));
		food_R.push_back(uid2(dre));
		food_G.push_back(uid2(dre));
		food_B.push_back(uid2(dre));
	}
}