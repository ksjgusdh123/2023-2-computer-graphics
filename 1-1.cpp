#include <iostream>
#include <gl/glew.h> // �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Random_Color(int value);

bool timer_check = false;

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
	glutKeyboardFunc(Keyboard); // Ű���� �Է� �ݹ��Լ� ����
	glutTimerFunc(100, Random_Color, 1);
	glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLclampf r = 0;
GLclampf g = 0;
GLclampf b = 0;
std::default_random_engine dre;
std::uniform_real_distribution<float> uid{ 0, 1 };

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	//--- ����� ���� ����
	glClearColor(r,g,b,1); //--- �������� ����
	glClear(GL_COLOR_BUFFER_BIT); //--- ������ ������ ��ü�� ĥ�ϱ�
	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

GLvoid Random_Color(int value)
{
	if (timer_check) {
		r = uid(dre); g = uid(dre); b = uid(dre);
		glutPostRedisplay();
	}
	glutTimerFunc(100, Random_Color, 1);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c': 
		r = 0; g = 1; b = 1;
		break; //--- ������ û�ϻ����� ����
	case 'm':
		r = 1; g = 0; b = 1;
		break; //--- ������ ��ȫ������ ����
	case 'y': 
		r = 1; g = 1; b = 0;
		break; //--- ������ ��������� ����
	case 'a':
		r = uid(dre); g = uid(dre); b = uid(dre);
		break;
	case 'w':
		r = 1; g = 1; b = 1;
		break;
	case 'k':
		r = 0; g = 0; b = 0;
		break;
	case 't':
		timer_check = true;
		break;
	case 's':
		timer_check = false;
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}