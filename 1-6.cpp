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
GLvoid Make_Square();
GLvoid Separate(int i);
GLvoid Separate2(int i);
GLvoid Timer_event(int value);
GLvoid Move();
GLvoid Move2();
GLvoid Move3();
GLvoid Moving();

std::random_device rd;
std::mt19937 dre(rd());
std::uniform_real_distribution<float> uid{ -0.7, 0.7 };
std::uniform_real_distribution<float> uid2{ 0, 1 };
std::uniform_real_distribution<float> uid3{ 0, 3 };


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
	Make_Square();
	glutDisplayFunc(drawScene); // ��� �ݹ��Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �ݹ��Լ� ����
	// Ű���� �Է� �ݹ��Լ� ����
	glutMotionFunc(Mouse_Move);
	glutMouseFunc(Mouse_Click);
	glutTimerFunc(100, Timer_event, 3);// Ÿ�̸� ����
	// Ÿ�̸� ����
	glutMainLoop(); // �̺�Ʈ ó�� ����
}


std::vector<float> Big_x{};
std::vector<float> Big_y{};

std::vector<float> small_x{};
std::vector<float> small_y{};
std::vector<float> small_end_x{};
std::vector<float> small_end_y{};

float small_R;
float small_G;
float small_B;

std::vector<float> R{};
std::vector<float> G{};
std::vector<float> B{};

bool select = false;

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	//--- ����� ���� ����
	glClearColor(1, 1, 1, 1); //--- �������� ����
	glClear(GL_COLOR_BUFFER_BIT); //--- ������ ������ ��ü�� ĥ�ϱ�

	for (int i = 0; i < Big_x.size(); ++i) {
		glColor3f(R.at(i), G.at(i), B.at(i));
		glRectf(Big_x.at(i), Big_y.at(i), Big_x.at(i) + 0.3, Big_y.at(i) - 0.4);
	}

	for (int i = 0; i < small_x.size(); ++i) {
		glColor3f(small_R, small_G, small_B);
		glRectf(small_x.at(i), small_y.at(i), small_end_x.at(i), small_end_y.at(i));
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

float small_size_x = 0.03;
float small_size_y = 0.04;
int random = 100;

GLvoid Timer_event(int value) {
	if (small_x.size() != 0) {
		if (random == 0) {
			Move();
		}
		else if(random == 1) {
			Move2();
		}
		else if (random == 2) {
			Move3();
		}
	}
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
	glutTimerFunc(100, Timer_event, 4);
}


GLvoid Mouse_Move(int x, int y)
{

}


GLvoid Mouse_Click(int button, int state, int x, int y)
{
	ConvertXY_OPENGL(x, y);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		for (int i = 0; i < Big_x.size(); ++i) {
			if (Big_x.at(i) <= ox && Big_x.at(i) + 0.3 >= ox 
				&& Big_y.at(i) >= oy && Big_y.at(i) - 0.4 <= oy && select == false) {
				Separate(i);
				select = true;
				random = uid3(dre);
			}
		}
	}
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

GLvoid Keyboard(unsigned char key, int x, int y)
{

}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

GLvoid Move() {
	for (int i = 0; i < small_x.size(); ++i) {
		if (i == 0) {
			small_x.at(i) = small_x.at(i) - 0.03;
			small_y.at(i) = small_y.at(i) + 0.04;
			small_end_x.at(i) = small_end_x.at(i) - small_size_x;
			small_end_y.at(i) = small_end_y.at(i) + small_size_y;
		}
		else if (i == 1) {
			small_x.at(i) = small_x.at(i) + small_size_x;
			small_y.at(i) = small_y.at(i) + 0.04;
			small_end_x.at(i) = small_end_x.at(i) + 0.03;
			small_end_y.at(i) = small_end_y.at(i) + small_size_y;
		}
		else if (i == 2) {
			small_x.at(i) = small_x.at(i) - 0.03;
			small_y.at(i) = small_y.at(i) - small_size_y;
			small_end_x.at(i) = small_end_x.at(i) - small_size_x;
			small_end_y.at(i) = small_end_y.at(i) - 0.04;
		}
		else if (i == 3) {
			small_x.at(i) = small_x.at(i) + small_size_x;
			small_y.at(i) = small_y.at(i) - small_size_y;
			small_end_x.at(i) = small_end_x.at(i) + 0.03;
			small_end_y.at(i) = small_end_y.at(i) - 0.04;
		}
	}
	small_size_x += 0.003;
	small_size_y += 0.004;
	if (small_size_x >= 0.05) {
		small_x.clear();
		small_y.clear();
		small_end_x.clear();
		small_end_y.clear();
		small_R = NULL;
		small_G = NULL;
		small_B = NULL;
		small_size_x = 0.03;
		small_size_y = 0.04;
		select = false;
		random = 100;
	}
}

GLvoid Move2() {
	for (int i = 0; i < small_x.size(); ++i) {
		if (i == 0) {
			small_x.at(i) = small_x.at(i) - 0.03/2;
			small_y.at(i) = small_y.at(i) - small_size_y/4;
			small_end_x.at(i) = small_end_x.at(i) - small_size_x;
			small_end_y.at(i) = small_end_y.at(i) + small_size_y/4;
		}
		else if (i == 1) {
			small_x.at(i) = small_x.at(i) + small_size_x/4;
			small_y.at(i) = small_y.at(i) + 0.04/2;
			small_end_x.at(i) = small_end_x.at(i) - 0.03/4;
			small_end_y.at(i) = small_end_y.at(i) + small_size_y;
		}
		else if (i == 2) {
			small_x.at(i) = small_x.at(i) + small_size_x/4;
			small_y.at(i) = small_y.at(i) - small_size_y;
			small_end_x.at(i) = small_end_x.at(i) - small_size_x / 4;
			small_end_y.at(i) = small_end_y.at(i) - small_size_y/2;
		}
		else if (i == 3) {
			small_x.at(i) = small_x.at(i) + small_size_x;
			small_y.at(i) = small_y.at(i) - small_size_y/4;
			small_end_x.at(i) = small_end_x.at(i) + 0.03 / 2;
			small_end_y.at(i) = small_end_y.at(i) + small_size_y/4;
		}
	}
	small_size_x += 0.003;
	small_size_y += 0.004;
	if (small_size_x >= 0.05) {
		small_x.clear();
		small_y.clear();
		small_end_x.clear();
		small_end_y.clear();
		small_R = NULL;
		small_G = NULL;
		small_B = NULL;
		small_size_x = 0.03;
		small_size_y = 0.04;
		select = false;
		random = 100;
	}
}

bool sep_2 = false;

GLvoid Move3() {
	if (sep_2 == false) {
		for (int i = 0; i < small_x.size(); ++i) {
			if (i == 0) {
				small_x.at(i) = small_x.at(i) - 0.03;
				small_y.at(i) = small_y.at(i) + 0.04;
				small_end_x.at(i) = small_end_x.at(i) - small_size_x;
				small_end_y.at(i) = small_end_y.at(i);
			}
			else if (i == 1) {
				small_x.at(i) = small_x.at(i);
				small_y.at(i) = small_y.at(i) + 0.04;
				small_end_x.at(i) = small_end_x.at(i) + 0.03;
				small_end_y.at(i) = small_end_y.at(i) + small_size_y;
			}
			else if (i == 2) {
				small_x.at(i) = small_x.at(i) - 0.03;
				small_y.at(i) = small_y.at(i) - small_size_y;
				small_end_x.at(i) = small_end_x.at(i);
				small_end_y.at(i) = small_end_y.at(i) - 0.04;
			}
			else if (i == 3) {
				small_x.at(i) = small_x.at(i) + small_size_x;
				small_y.at(i) = small_y.at(i);
				small_end_x.at(i) = small_end_x.at(i) + 0.03;
				small_end_y.at(i) = small_end_y.at(i) - 0.04;
			}
		}
		small_size_x += 0.003;
		small_size_y += 0.004;
		if (small_size_y >= 0.05201) {
			for (int i = 0; i < 8; ++i) {
				Separate2(i);
				small_size_x = 0.03;
				small_size_y = 0.04;
			}
			sep_2 = true;
		}
	}
	else {
		Moving();
		small_size_x += 0.0015;
		small_size_y += 0.002;
		if (small_size_x >= 0.035) {
			small_x.clear();
			small_y.clear();
			small_end_x.clear();
			small_end_y.clear();
			small_R = NULL;
			small_G = NULL;
			small_B = NULL;
			small_size_x = 0.03;
			small_size_y = 0.04;
			select = false;
			sep_2 = false;
			random = 100;
		}
	
	}
	
}

GLvoid Make_Square() 
{
	for (int i = 0; i < 5; ++i) {
		R.push_back(uid2(dre));
		G.push_back(uid2(dre));
		B.push_back(uid2(dre));
		Big_x.push_back(uid(dre));
		Big_y.push_back(uid(dre));
	}
}

GLvoid Separate(int i) {
	small_R = R.at(i);
	small_G = G.at(i);
	small_B = B.at(i);

	small_x.push_back(Big_x.at(i));
	small_x.push_back(Big_x.at(i) + 0.15);
	small_x.push_back(Big_x.at(i));
	small_x.push_back(Big_x.at(i) + 0.15);

	small_end_x.push_back(Big_x.at(i) + 0.15);
	small_end_x.push_back(Big_x.at(i) + 0.3);
	small_end_x.push_back(Big_x.at(i) + 0.15);
	small_end_x.push_back(Big_x.at(i) + 0.3);


	small_y.push_back(Big_y.at(i));
	small_y.push_back(Big_y.at(i));
	small_y.push_back(Big_y.at(i) - 0.2);
	small_y.push_back(Big_y.at(i) - 0.2);

	small_end_y.push_back(Big_y.at(i) - 0.2);
	small_end_y.push_back(Big_y.at(i) - 0.2);
	small_end_y.push_back(Big_y.at(i) - 0.4);
	small_end_y.push_back(Big_y.at(i) - 0.4);

	Big_x.at(i) = 100;
	Big_y.at(i) = 100;

}

GLvoid Separate2(int i) {
	switch (i) {
	case 4:
		small_end_y.push_back(small_end_y.at(0));
		small_x.push_back(small_x.at(0));
		small_end_x.push_back(small_end_x.at(0));

		small_end_y.at(0) = small_y.at(0) - (small_y.at(0) - small_end_y.at(0)) / 2;
		small_y.push_back(small_end_y.at(0));
		break;
	case 5:
		small_x.push_back(small_x.at(1));
		if (small_x.at(1) > small_end_x.at(1)) {
			small_x.at(1) = (small_x.at(1) - small_end_x.at(1)) / 2;
		}
		else {
			small_x.at(1) = small_end_x.at(1) - (small_end_x.at(1) - small_x.at(1)) / 2;
		}
		small_end_x.push_back(small_x.at(1));
		small_y.push_back(small_y.at(1));
		small_end_y.push_back(small_end_y.at(1));
		break;
	case 6:
		small_y.push_back(small_y.at(2));
		small_end_y.push_back(small_end_y.at(2));
		small_end_x.push_back(small_end_x.at(2));
		if (small_x.at(2) > small_end_x.at(2)) {
			small_end_x.at(2) = (small_x.at(2) - small_end_x.at(2)) / 2;
		}
		else {
			small_end_x.at(2) = small_end_x.at(2) - (small_end_x.at(2) - small_x.at(2)) / 2;
		}
		small_x.push_back(small_end_x.at(2));
		break;
	case 7:
		small_x.push_back(small_x.at(3));
		small_end_x.push_back(small_end_x.at(3));
		small_y.push_back(small_y.at(3));
		if (small_y.at(3) > small_end_y.at(3)) {
			small_y.at(3) = small_y.at(3) - (small_y.at(3) - small_end_y.at(3))/2;
		}
		else {
			small_end_y.at(3) = (-small_y.at(3) + small_end_y.at(3));
		}
		small_end_y.push_back(small_y.at(3));

		break;
	}
}

GLvoid Moving() {
	for (int i = 0; i < small_x.size(); ++i) {
		switch (i) {
		case 0:
			small_x.at(i) = small_x.at(i) - 0.003;
			small_y.at(i) = small_y.at(i) + 0.004;
			small_end_x.at(i) = small_end_x.at(i) - small_size_x/2;
			small_end_y.at(i) = small_end_y.at(i) + small_size_y/2;
			break;
		case 1:
			small_x.at(i) = small_x.at(i) + small_size_x/2;
			small_y.at(i) = small_y.at(i) + 0.004;
			small_end_x.at(i) = small_end_x.at(i) + 0.003;
			small_end_y.at(i) = small_end_y.at(i) + small_size_y/2;
			break;
		case 2:
			small_x.at(i) = small_x.at(i) - 0.003;
			small_y.at(i) = small_y.at(i) - small_size_y/2;
			small_end_x.at(i) = small_end_x.at(i) - small_size_x/2;
			small_end_y.at(i) = small_end_y.at(i) - 0.004;
			break;
		case 3:
			small_x.at(i) = small_x.at(i) + small_size_x/2;
			small_y.at(i) = small_y.at(i) - small_size_y/2;
			small_end_x.at(i) = small_end_x.at(i) + 0.003;
			small_end_y.at(i) = small_end_y.at(i) - 0.004;
			break;
		case 4:
			small_x.at(i) = small_x.at(i) - 0.03 / 2;
			small_y.at(i) = small_y.at(i) - small_size_y / 4;
			small_end_x.at(i) = small_end_x.at(i) - small_size_x;
			small_end_y.at(i) = small_end_y.at(i) + small_size_y / 4;
			break;
		case 5:
			small_x.at(i) = small_x.at(i) + small_size_x / 4;
			small_y.at(i) = small_y.at(i) + 0.04 / 2;
			small_end_x.at(i) = small_end_x.at(i) - 0.03 / 4;
			small_end_y.at(i) = small_end_y.at(i) + small_size_y;
			break;
		case 6:
			small_x.at(i) = small_x.at(i) + small_size_x / 4;
			small_y.at(i) = small_y.at(i) - small_size_y;
			small_end_x.at(i) = small_end_x.at(i) - small_size_x / 4;
			small_end_y.at(i) = small_end_y.at(i) - small_size_y / 2;
			break;
		case 7:
			small_x.at(i) = small_x.at(i) + small_size_x;
			small_y.at(i) = small_y.at(i) - small_size_y / 4;
			small_end_x.at(i) = small_end_x.at(i) + 0.03 / 2;
			small_end_y.at(i) = small_end_y.at(i) + small_size_y / 4;
			break;
		}
	}
}