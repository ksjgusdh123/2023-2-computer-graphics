#define _CRT_SECURE_NO_WARNINGS //--- ���α׷� �� �տ� ������ ��
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <gl/glew.h> // �ʿ��� ������� include
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
GLvoid InitBuffer();
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Make_Tri();
GLvoid Timer_event(int value);
GLvoid change_Tri(int i);
GLvoid Re_Buffer();
GLvoid move_one();
GLvoid move_two();
GLvoid move_three();
GLvoid move_four();
GLvoid rotation_point(int num, int i);

//--- �ʿ��� ���� ����
GLint width, height;
GLuint shaderProgramID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü
GLuint vbo[2], vao;

std::random_device rd;
std::mt19937 dre(rd());
std::uniform_real_distribution<float> uid{ 0.1, 0.4 };

//--- ���� �Լ�
void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example1");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	make_shaderProgram();
	InitBuffer();
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(100, Timer_event, 1);// Ÿ�̸� ����
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	GLfloat rColor = 1.0, gColor = 1.0, bColor = 1.0;
	//--- ����� ���� ����
	glClearColor(rColor, gColor, bColor, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(shaderProgramID);
	//--- ����� VAO �ҷ�����
	glBindVertexArray(vao);
	//--- �ﰢ�� �׸���
	glDrawArrays(GL_TRIANGLES, 4, 12);


	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}

//--- �ٽñ׸��� �ݹ� �Լ�
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

GLint result;
GLchar errorLog[512];

void make_vertexShaders()
{
	GLchar* vertexSource;
	vertexSource = filetobuf("vertex.glsl");
	//--- ���ؽ� ���̴� ��ü �����
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexShader);
	//--- �������� ����� ���� ���� ���: ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	GLchar* fragmentSource;
	fragmentSource = filetobuf("fragment.glsl");
	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentShader);
	//--- �������� ����� ���� ���� ���: ������ ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

GLvoid make_shaderProgram()
{
	GLuint shaderID;
	make_vertexShaders(); //--- ���ؽ� ���̴� �����
	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program ����ϱ�
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

GLfloat shape[100][3]{ {-1, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, -1, 0} };
GLfloat colors[100][3]{ {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };

float ox, oy;

GLvoid ConvertXY_OPENGL(int x, int y)
{
	int w = 800;
	int h = 600;

	ox = (float)(x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0));
	oy = -(float)(y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0));

}

int menu = 0;
int move_point[4]{ 0,1,2,3 };
int move_start[4]{};
float move_stack[4]{ 2, 2, 2, 2 };
int point_x[4] = { 0 }, point_y[4] = { 0 };
float r[4] = { 0.005, 0.005, 0.005, 0.005 };
const double PI = 3.1415926;

GLvoid Timer_event(int value) {
	switch (menu) {
	case 1:
		move_one();
		break;
	case 2:
		move_two();
		break;
	case 3:
		move_three();
		break;
	case 4:
		move_four();
		break;
	}
	Re_Buffer();
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
	glutTimerFunc(100, Timer_event, 4);
}


GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '1':
		for (int i = 0; i < 4; ++i) {
			move_point[i] = i;
		}
		menu = 1;
		break;
	case '2':
		menu = 2;
		break;
	case '3':
		Make_Tri();
		Re_Buffer();
		for (int i = 0; i < 4; ++i) {
			move_stack[i] = 2;
			move_start[i] = 0;
		}
		menu = 3;
		break;
	case '4':
		for (int i = 0; i < 4; ++i) {
			point_x[i] = 0; 
			point_y[i] = 0;
			r[i] = 0.005;
		}
		menu = 4;
		break;
	}
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}


GLvoid InitBuffer()
{
	Make_Tri();
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�
	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//--- ���� diamond ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	//--- triShape �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape), shape, GL_STATIC_DRAW);
	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);
	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	//--- colors �迭�� ������: 9 *float 
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape), colors, GL_STATIC_DRAW);
	//--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	//--- attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);
}

GLvoid Re_Buffer() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//--- ���� diamond ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	//--- triShape �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape), shape, GL_STATIC_DRAW);
	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);
	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	//--- colors �迭�� ������: 9 *float 
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape), colors, GL_STATIC_DRAW);
	//--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	//--- attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);
}

GLvoid Make_Tri() {
	bool third = false;
	for (int i = 4; i < 16; ++i) {
		if (i == 4) {
			shape[i][0] = -0.25;
			shape[i][1] = 0.75;
			shape[i][2] = 0;
			colors[i][0] = 1;
			colors[i][1] = 0;
			colors[i][2] = 0;
			third = true;
		}
		else if (i == 7) {
			shape[i][0] = 0.25;
			shape[i][1] = 0.75;
			shape[i][2] = 0;

			colors[i][0] = 0;
			colors[i][1] = 1;
			colors[i][2] = 0;
			third = true;
		}
		else if (i == 10) {
			shape[i][0] = -0.25;
			shape[i][1] = -0.25;
			shape[i][2] = 0;

			colors[i][0] = 0;
			colors[i][1] = 0;
			colors[i][2] = 1;
			third = true;
		}
		else if (i == 13) {
			shape[i][0] = 0.25;
			shape[i][1] = -0.25;
			shape[i][2] = 0;

			colors[i][0] = 1;
			colors[i][1] = 1;
			colors[i][2] = 0;
			third = true;
		}
		else {
			if (third) {
				shape[i][0] = shape[i - 1][0] - 0.125;
				shape[i][1] = shape[i - 1][1] - 0.125;
				shape[i][2] = 0;

				colors[i][0] = colors[i - 1][0];
				colors[i][1] = colors[i - 1][1];
				colors[i][2] = colors[i - 1][2];
				third = false;
			}
			else {
				shape[i][0] = shape[i - 2][0] + 0.125;
				shape[i][1] = shape[i - 2][1] - 0.125;
				shape[i][2] = 0;

				colors[i][0] = colors[i - 1][0];
				colors[i][1] = colors[i - 1][1];
				colors[i][2] = colors[i - 1][2];
			}
		}
	}

}



GLvoid move_one() {
	int num = 0;

	for (int i = 0; i < 4; ++i) {
		switch (move_point[i]) {
		case 0:
			if (i == 0) {
				num = 4;
				if (shape[num + 2][0] >= 0.95) {
					move_point[i] = 3;
					rotation_point(num, i);
					break;
				}
				else if (shape[num][1] >= 0.9) {
					move_point[i] = 2;
					rotation_point(num, i);
					break;
				}
			}
			else if (i == 1) {
				num = 7;
				if (shape[num + 2][0] >= 0.95) {
					move_point[i] = 3;
					rotation_point(num, i);
					break;
				}
				else if (shape[num][1] >= 0.9) {
					move_point[i] = 2;
					rotation_point(num, i);
					break;
				}
			}
			else if (i == 2) {
				num = 10;
				if (shape[num + 2][0] >= 0.95) {
					move_point[i] = 3;
					rotation_point(num, i);
					break;
				}
				else if (shape[num][1] >= 0.9) {
					move_point[i] = 2;
					rotation_point(num, i);
					break;
				}
			}
			else if (i == 3) {
				num = 13;
				if (shape[num + 2][0] >= 0.95) {
					move_point[i] = 3;
					rotation_point(num, i);
					break;
				}
				else if (shape[num][1] >= 0.9) {
					move_point[i] = 2;
					rotation_point(num, i);
					break;
				}
			}
			
			for (int j = num; j < num + 3; ++j) {
				shape[j][0] += 0.05;
				shape[j][1] += 0.05;
			}

			
			break;
		case 1:
			if (i == 0) {
				num = 4;
				if (shape[num][0] >= 0.95) {
					move_point[i] = 3;
					rotation_point(num, i);
					break;
				}
				else if (shape[num][1] <= -0.9) {
					move_point[i] = 0;
					rotation_point(num, i);
					break;
				}
			}
			else if (i == 1) {
				num = 7;
				if (shape[num][0] >= 0.95) {
					move_point[i] = 3;
					rotation_point(num, i);
					break;
				}
				else if (shape[num][1] <= -0.9) {
					move_point[i] = 0;
					rotation_point(num, i);
					break;
				}
			}
			else if (i == 2) {
				num = 10;
				if (shape[num][0] >= 0.95) {
					move_point[i] = 3;
					rotation_point(num, i);
					break;
				}
				else if (shape[num][1] <= -0.9) {
					move_point[i] = 0;
					rotation_point(num, i);
					break;
				}
			}
			else if (i == 3) {
				num = 13;
				if (shape[num][0] >= 0.95) {
					move_point[i] = 3;
					rotation_point(num, i);
					break;
				}
				else if (shape[num][1] <= -0.9) {
					move_point[i] = 0;
					rotation_point(num, i);
					break;
				}
			}

			for (int j = num; j < num + 3; ++j) {
				shape[j][0] += 0.05;
				shape[j][1] -= 0.05;
			}
			break;
		case 2:
			if (i == 0) {
				num = 4;
				if (shape[num][0] <= -0.95) {
					move_point[i] = 1;
					rotation_point(num, i);
					break;
				}
				else if (shape[num][1] <= -0.9) {
					move_point[i] = 0;
					rotation_point(num, i);
					break;
				}
			}
			else if (i == 1) {
				num = 7;
				if (shape[num][0] <= -0.95) {
					move_point[i] = 1;
					rotation_point(num, i);
					break;
				}
				else if (shape[num][1] <= -0.9) {
					move_point[i] = 0;
					rotation_point(num, i);
					break;
				}
			}
			else if (i == 2) {
				num = 10;
				if (shape[num][0] <= -0.95) {
					move_point[i] = 1;
					rotation_point(num, i);
					break;
				}
				else if (shape[num][1] <= -0.9) {
					move_point[i] = 0;
					rotation_point(num, i);
					break;
				}
			}
			else if (i == 3) {
				num = 13;
				if (shape[num][0] <= -0.95) {
					move_point[i] = 1;
					rotation_point(num, i);
					break;
				}
				else if (shape[num][1] <= -0.9) {
					move_point[i] = 0;
					rotation_point(num, i);
					break;
				}
			}

			for (int j = num; j < num + 3; ++j) {
				shape[j][0] -= 0.05;
				shape[j][1] -= 0.05;
			}
			break;
		case 3:
			if (i == 0) {
				num = 4;
				if (shape[num][0] <= -0.95) {
					move_point[i] = 1;
					rotation_point(num, i);
					break;
				}
				else if (shape[num][1] >= 0.9) {
					move_point[i] = 2;
					rotation_point(num, i);
					break;
				}
			}
			else if (i == 1) {
				num = 7;
				if (shape[num][0] <= -0.95) {
					move_point[i] = 1;
					rotation_point(num, i);
					break;
				}
				else if (shape[num][1] >= 0.9) {
					move_point[i] = 2;
					rotation_point(num, i);
					break;
				}
			}
			else if (i == 2) {
				num = 10;
				if (shape[num][0] <= -0.95) {
					move_point[i] = 1;
					rotation_point(num, i);
					break;
				}
				else if (shape[num][1] >= 0.9) {
					move_point[i] = 2;
					rotation_point(num, i);
					break;
				}
			}
			else if (i == 3) {
				num = 13;
				if (shape[num][0] <= -0.95) {
					move_point[i] = 1;
					rotation_point(num, i);
					break;
				}
				else if (shape[num][1] >= 0.9) {
					move_point[i] = 2;
					rotation_point(num, i);
					break;
				}
			}

			for (int j = num; j < num + 3; ++j) {
				shape[j][0] -= 0.05;
				shape[j][1] += 0.05;
			}
			break;
		}
	}
}

float save_point[4]{};
int up_down[4]{};
GLvoid move_two() {
	int num = 0;
	for (int i = 0; i < 4; ++i) {
		if (move_point[i] == 100) {
			for (int j = 0; j < 3; ++j) {
				if (i == 0) {
					num = 4;
					if ((shape[num][1] <= save_point[i] - 0.4 && up_down[i] == 0) || (shape[num][1] >= save_point[i] + 0.4 && up_down[i] == 1)) {
						if (shape[num][0] > 0.8) {
							move_point[i] = 1;
						}
						else {
							move_point[i] = 0;
						}
						rotation_point(num, i);
						break;
					}
					if (up_down[i] == 0) {
						shape[num + j][1] -= 0.05;
					}
					else {
						shape[num + j][1] += 0.05;
					}
				}
				else if (i == 1) {
					num = 7;
					if ((shape[num][1] <= save_point[i] - 0.4 && up_down[i] == 0) || (shape[num][1] >= save_point[i] + 0.4 && up_down[i] == 1)) {
						if (shape[num][0] > 0.8) {
							move_point[i] = 1;
						}
						else {
							move_point[i] = 0;
						}
						rotation_point(num, i);
						break;
					}
					if (up_down[i] == 0) {
						shape[num + j][1] -= 0.05;
					}
					else {
						shape[num + j][1] += 0.05;
					}
				}
				else if (i == 2) {
					num = 10;
					if ((shape[num][1] <= save_point[i] - 0.4 && up_down[i] == 0) || (shape[num][1] >= save_point[i] + 0.4 && up_down[i] == 1)) {
						if (shape[num][0] > 0.8) {
							move_point[i] = 1;
						}
						else {
							move_point[i] = 0;
						}
						rotation_point(num, i);
						break;
					}
					if (up_down[i] == 0) {
						shape[num + j][1] -= 0.05;
					}
					else {
						shape[num + j][1] += 0.05;
					}
				}
				else if (i == 3) {
					num = 13;
					if ((shape[num][1] <= save_point[i] - 0.4 && up_down[i] == 0) || (shape[num][1] >= save_point[i] + 0.4 && up_down[i] == 1)) {
						if (shape[num][0] > 0.8) {
							move_point[i] = 1;
						}
						else {
							move_point[i] = 0;
						}
						rotation_point(num, i);
						break;
					}
					if (up_down[i] == 0) {
						shape[num + j][1] -= 0.05;
					}
					else {
						shape[num + j][1] += 0.05;
					}
				}
			}
		}
		else if (move_point[i] % 2 == 0) {
			for (int j = 0; j < 3; ++j) {
				if (i == 0) {
					num = 4;
					if (shape[num + 2][0] >= 0.95) {
						move_point[i] = 100;
						rotation_point(num, i);
						save_point[i] = shape[num][1];
						if (shape[num][1] >= 0.9) {
							up_down[i] = 0;
						}
						else if (shape[num][1] <= -0.9) {
							up_down[i] = 1;
						}
						break;
					}
					shape[num + j][0] += 0.05;
				}
				else if (i == 1) {
					num = 7;
					if (shape[num + 2][0] >= 0.95) {
						move_point[i] = 100;
						rotation_point(num, i);
						save_point[i] = shape[num][1];
						if (shape[num][1] >= 0.9) {
							up_down[i] = 0;
						}
						else if (shape[num][1] <= -0.9) {
							up_down[i] = 1;
						}
						break;
					}
					shape[num + j][0] += 0.05;
				}
				else if (i == 2) {
					num = 10;
					if (shape[num + 2][0] >= 0.95) {
						move_point[i] = 100;
						rotation_point(num, i);
						save_point[i] = shape[num][1];
						if (shape[num][1] >= 0.9) {
							up_down[i] = 0;
						}
						else if (shape[num][1] <= -0.9) {
							up_down[i] = 1;
						}
						break;
					}
					shape[num + j][0] += 0.05;
				}
				else if (i == 3) {
					num = 13;
					if (shape[num + 2][0] >= 0.95) {
						move_point[i] = 100;
						rotation_point(num, i);
						save_point[i] = shape[num][1];
						if (shape[num][1] >= 0.9) {
							up_down[i] = 0;
						}
						else if (shape[num][1] <= -0.9) {
							up_down[i] = 1;
						}
						break;
					}
					shape[num + j][0] += 0.05;
				}
			}
		}
		else {
			for (int j = 0; j < 3; ++j) {
				if (i == 0) {
					num = 4;
					if (shape[num + 2][0] <= -0.95) {
						move_point[i] = 100;
						rotation_point(num, i);
						save_point[i] = shape[num][1];
						if (shape[num][1] >= 0.9) {
							up_down[i] = 0;
						}
						else if (shape[num][1] <= -0.9) {
							up_down[i] = 1;
						}
						break;
					}
					shape[num + j][0] -= 0.05;
				}
				else if (i == 1) {
					num = 7;
					if (shape[num + 2][0] <= -0.95) {
						move_point[i] = 100;
						rotation_point(num, i);
						save_point[i] = shape[num][1];
						if (shape[num][1] >= 0.9) {
							up_down[i] = 0;
						}
						else if (shape[num][1] <= -0.9) {
							up_down[i] = 1;
						}
						break;
					}
					shape[num + j][0] -= 0.05;
				}
				else if (i == 2) {
					num = 10;
					if (shape[num + 2][0] <= -0.95) {
						move_point[i] = 100;
						rotation_point(num, i);
						save_point[i] = shape[num][1];
						if (shape[num][1] >= 0.9) {
							up_down[i] = 0;
						}
						else if (shape[num][1] <= -0.9) {
							up_down[i] = 1;
						}
						break;
					}
					shape[num + j][0] -= 0.05;
				}
				else if (i == 3) {
					num = 13;
					if (shape[num + 2][0] <= -0.95) {
						move_point[i] = 100;
						rotation_point(num, i);
						save_point[i] = shape[num][1];
						if (shape[num][1] >= 0.9) {
							up_down[i] = 0;
						}
						else if (shape[num][1] <= -0.9) {
							up_down[i] = 1;
						}
						break;
					}
					shape[num + j][0] -= 0.05;
				}
			}
		}
	}
}



GLvoid move_three() {
	int num;
	for (int i = 0; i < 4; ++i) {
		if (move_start[i] == 0) {
			for (int j = 0; j < 3; ++j) {
				if (i == 0) {
					num = 4;
					if (save_point[i] - move_stack[i] + 0.2 <= shape[num + 2][0] && shape[num + 2][0] >= -0.8) {
						shape[num + j][0] -= 0.05;
					}
					else {
						move_start[i] = 1;
						rotation_point(num, 0);
						move_stack[i] -= 0.2;
						save_point[i] = shape[num][1];
						break;
					}
				}
				else if (i == 1) {
					num = 7;
					if (save_point[i] - move_stack[i] + 0.2 <= shape[num + 2][0] && shape[num + 2][0] >= -0.8) {
						shape[num + j][0] -= 0.05;
					}
					else {
						move_start[i] = 1;
						rotation_point(num, 0);
						move_stack[i] -= 0.2;
						save_point[i] = shape[num][1];
						break;
					}
				}
				else if (i == 2) {
					num = 10;
					if (save_point[i] - move_stack[i] + 0.2 <= shape[num + 2][0] && shape[num + 2][0] >= -0.8) {
						shape[num + j][0] -= 0.05;
					}
					else {
						move_start[i] = 1;
						rotation_point(num, 0);
						move_stack[i] -= 0.2;
						save_point[i] = shape[num][1];
						break;
					}
				}
				else if (i == 3) {
					num = 13;
					if (save_point[i] - move_stack[i] + 0.2 <= shape[num + 2][0] && shape[num + 2][0] >= -0.8) {
						shape[num + j][0] -= 0.05;
					}
					else {
						move_start[i] = 1;
						rotation_point(num, 0);
						move_stack[i] -= 0.2;
						save_point[i] = shape[num][1];
						break;
					}
				}
			}
		}
		else if(move_start[i] == 1) {
			for (int j = 0; j < 3; ++j) {
				if (i == 0) {
					num = 4;
					if (save_point[i] - move_stack[i] / 1.5 <= shape[num + 2][1] && shape[num + 2][1] >= - 0.8) {
						shape[num + j][1] -= 0.05;
					}
					else {
						move_start[i] = 2;
						rotation_point(num, 0);
						save_point[i] = shape[num][0];
						break;
					}
				}
				else if (i == 1) {
					num = 7;
					if (save_point[i] - move_stack[i] / 1.5 <= shape[num + 2][1] && shape[num + 2][1] >= -0.8) {
						shape[num + j][1] -= 0.05;
					}
					else {
						move_start[i] = 2;
						rotation_point(num, 0);
						save_point[i] = shape[num][0];
						break;
					}
				}
				else if (i == 2) {
					num = 10;
					if (save_point[i] - move_stack[i] / 1.5 <= shape[num + 2][1] && shape[num + 2][1] >= -0.8) {
						shape[num + j][1] -= 0.05;
					}
					else {
						move_start[i] = 2;
						rotation_point(num, 0);
						save_point[i] = shape[num][0];
						break;
					}
				}
				else if (i == 3) {
					num = 13;
					if (save_point[i] - move_stack[i] / 1.5 <= shape[num + 2][1] && shape[num + 2][1] >= -0.8) {
						shape[num + j][1] -= 0.05;
					}
					else {
						move_start[i] = 2;
						rotation_point(num, 0);
						save_point[i] = shape[num][0];
						break;
					}
				}
			}
		}
		else if (move_start[i] == 2) {
			for (int j = 0; j < 3; ++j) {
				if (i == 0) {
					num = 4;
					if (save_point[i] + move_stack[i] >= shape[num + 2][0]) {
						shape[num + j][0] += 0.05;
					}
					else {
						move_start[i] = 3;
						rotation_point(num, 2);
						move_stack[i] -= 0.2;
						save_point[i] = shape[num][1];
						break;
					}
				}
				else if (i == 1) {
					num = 7;
					if (save_point[i] + move_stack[i] >= shape[num + 2][0]) {
						shape[num + j][0] += 0.05;
					}
					else {
						move_start[i] = 3;
						rotation_point(num, 2);
						move_stack[i] -= 0.2;
						save_point[i] = shape[num][1];
						break;
					}
				}
				else if (i == 2) {
					num = 10;
					if (save_point[i] + move_stack[i] >= shape[num + 2][0]) {
						shape[num + j][0] += 0.05;
					}
					else {
						move_start[i] = 3;
						rotation_point(num, 2);
						move_stack[i] -= 0.2;
						save_point[i] = shape[num][1];
						break;
					}
				}
				else if (i == 3) {
					num = 13;
					if (save_point[i] + move_stack[i] >= shape[num + 2][0]) {
						shape[num + j][0] += 0.05;
					}
					else {
						move_start[i] = 3;
						rotation_point(num, 2);
						move_stack[i] -= 0.2;
						save_point[i] = shape[num][1];
						break;
					}
				}
			}
		}
		else if (move_start[i] == 3) {
			for (int j = 0; j < 3; ++j) {
				if (i == 0) {
					num = 4;
					if (save_point[i] + move_stack[i] / 1.5 >= shape[num + 2][1]) {
						shape[num + j][1] += 0.05;
					}
					else {
						move_start[i] = 0;
						rotation_point(num, 3);
						save_point[i] = shape[num][0];
						break;
					}
				}
				else if (i == 1) {
					num = 7;
					if (save_point[i] + move_stack[i] / 1.5 >= shape[num + 2][1]) {
						shape[num + j][1] += 0.05;
					}
					else {
						move_start[i] = 0;
						rotation_point(num, 3);
						save_point[i] = shape[num][0];
						break;
					}
				}
				else if (i == 2) {
					num = 10;
					if (save_point[i] + move_stack[i] / 1.5 >= shape[num + 2][1]) {
						shape[num + j][1] += 0.05;
					}
					else {
						move_start[i] = 0;
						rotation_point(num, 3);
						save_point[i] = shape[num][0];
						break;
					}
				}
				else if (i == 3) {
					num = 13;
					if (save_point[i] + move_stack[i] / 1.5 >= shape[num + 2][1]) {
						shape[num + j][1] += 0.05;
					}
					else {
						move_start[i] = 0;
						rotation_point(num, 3);
						save_point[i] = shape[num][0];
						break;
					}
				}
			}
		}
	}
}



GLvoid move_four() {
	int num;
	for (int i = 0; i < 4; ++i) {
		if (i == 0) {
			num = 4;
			for (int j = 0; j < 3; ++j) {
				shape[num + j][0] += cos((point_x[i] % 360) * PI / 180) * r[i];
				shape[num + j][1] += sin((point_y[i] % 360) * PI / 180) * r[i];
			}
			point_x[i] += 30;
			point_y[i] += 30;
			r[i] += 0.0005 * (point_x[i] / 360);
			//r += 0.00005;
		}
		else if (i == 1) {
			num = 7;
			for (int j = 0; j < 3; ++j) {
				shape[num + j][0] += cos((point_x[i] % 360) * PI / 180) * r[i];
				shape[num + j][1] += sin((point_y[i] % 360) * PI / 180) * r[i];
			}
			point_x[i] += 30;
			point_y[i] += 30;
			r[i] += 0.0005 * (point_x[i] / 360);
		}
		else if (i == 2) {
			num = 10;
			for (int j = 0; j < 3; ++j) {
				shape[num + j][0] += cos((point_x[i] % 360) * PI / 180) * r[i];
				shape[num + j][1] += sin((point_y[i] % 360) * PI / 180) * r[i];
			}
			point_x[i] += 30;
			point_y[i] += 30;
			r[i] += 0.0005 * (point_x[i] / 360);
		}
		else if (i == 3) {
			num = 13;
			for (int j = 0; j < 3; ++j) {
				shape[num + j][0] += cos((point_x[i] % 360) * PI / 180) * r[i];
				shape[num + j][1] += sin((point_y[i] % 360) * PI / 180) * r[i];
			}
			point_x[i] += 30;
			point_y[i] += 30;
			r[i] += 0.0005 * (point_x[i] / 360);
		}
	}
}

GLvoid rotation_point(int num, int i) {
	switch (move_point[i]) {
	case 1:
		for (int j = 0; j < 2; ++j) {
			if (j == 0) {
				shape[num][j] = shape[num + 2][j] - 0.125;
			}
			else {
				shape[num][j] = shape[num + 2][j];
			}
		}
		shape[num + 1][0] = shape[num][0] - 0.125;
		shape[num + 1][1] = shape[num][1] - 0.125;
		shape[num + 2][0] = shape[num][0] - 0.125;
		shape[num + 2][1] = shape[num][1] + 0.125;

		break;
	case 0:
		for (int j = 0; j < 2; ++j) {
			if (j == 0) {
				shape[num][j] = shape[num + 2][j] - 0.125;
			}
			else {
				shape[num][j] = shape[num + 2][j];
			}
		}
		shape[num + 1][0] = shape[num][0] - 0.125;
		shape[num + 1][1] = shape[num][1] - 0.125;
		shape[num + 2][0] = shape[num][0] + 0.125;
		shape[num + 2][1] = shape[num][1] - 0.125;
		break;
	case 3:
		for (int j = 0; j < 2; ++j) {
			if (j == 0) {
				shape[num][j] = shape[num + 2][j] - 0.125;
			}
			else {
				shape[num][j] = shape[num + 2][j];
			}
		}
		shape[num + 1][0] = shape[num][0] + 0.125;
		shape[num + 1][1] = shape[num][1] - 0.125;
		shape[num + 2][0] = shape[num][0] + 0.125;
		shape[num + 2][1] = shape[num][1] + 0.125;
		break;
	case 2:
		for (int j = 0; j < 2; ++j) {
			if (j == 0) {
				shape[num][j] = shape[num + 2][j] - 0.125;
			}
			else {
				shape[num][j] = shape[num + 2][j];
			}
		}
		shape[num + 1][0] = shape[num][0] - 0.125;
		shape[num + 1][1] = shape[num][1] + 0.125;
		shape[num + 2][0] = shape[num][0] + 0.125;
		shape[num + 2][1] = shape[num][1] + 0.125;
		break;
	case 100:
		for (int j = 0; j < 2; ++j) {
			if (j == 0) {
				shape[num][j] = shape[num + 2][j] - 0.125;
			}
			else {
				shape[num][j] = shape[num + 2][j];
			}
		}
		shape[num + 1][0] = shape[num][0] + 0.125;
		shape[num + 1][1] = shape[num][1] - 0.125;
		shape[num + 2][0] = shape[num][0] + 0.125;
		shape[num + 2][1] = shape[num][1] + 0.125;
		break;
	}
	
}