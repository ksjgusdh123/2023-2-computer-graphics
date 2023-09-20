#include <iostream>
#include <gl/glew.h> // �ʿ��� ������� include
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
	glutDisplayFunc(drawScene); // ��� �ݹ��Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �ݹ��Լ� ����
	glutKeyboardFunc(Keyboard);// Ű���� �Է� �ݹ��Լ� ����
	glutMotionFunc(Mouse_Move);
	// Ÿ�̸� ����
	glutMouseFunc(Mouse_Click);
	glutMainLoop(); // �̺�Ʈ ó�� ����
}

std::vector<float> R{ 0 };
std::vector<float> B{ 1 };
std::vector<float> G{ 1 };

std::vector<float> point_x{ 0 };
std::vector<float> point_y{ 0 };
GLclampf ox;
GLclampf oy;

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	//--- ����� ���� ����
	glClearColor(1, 1, 1, 1); //--- �������� ����
	glClear(GL_COLOR_BUFFER_BIT); //--- ������ ������ ��ü�� ĥ�ϱ�

	for (int i = 0; i < point_x.size(); ++i) {
		glColor3f(R.at(i), G.at(i), B.at(i));
		glRectf(point_x.at(i), point_y.at(i), point_x.at(i) + 0.15, point_y.at(i) + 0.2);
	}

	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
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
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
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
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
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
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}