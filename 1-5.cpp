#include <iostream>
#include <gl/glew.h> // �ʿ��� ������� include
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

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(800, 600); // �������� ũ�� ����
	glutCreateWindow("Example1"); // ������ ����	(������ �̸�)
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";
	Make_Food();
	glutDisplayFunc(drawScene); // ��� �ݹ��Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �ݹ��Լ� ����
	glutKeyboardFunc(Keyboard);// Ű���� �Է� �ݹ��Լ� ����
	glutMotionFunc(Mouse_Move);
	glutMouseFunc(Mouse_Click);
	// Ÿ�̸� ����
	glutMainLoop(); // �̺�Ʈ ó�� ����
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

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	//--- ����� ���� ����
	glClearColor(1, 1, 1, 1); //--- �������� ����
	glClear(GL_COLOR_BUFFER_BIT); //--- ������ ������ ��ü�� ĥ�ϱ�

	for (int i = 0; i < food_x.size(); ++i) {
		glColor3f(food_R.at(i), food_G.at(i), food_B.at(i));
		glRectf(food_x.at(i), food_y.at(i), food_x.at(i) + 0.075, food_y.at(i) + 0.1);
	}
	if (left_button) {
		glColor3f(R, G, B);
		glRectf(player_x, player_y, endplayer_x, endplayer_y);
	}
	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
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
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�



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
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
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
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
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