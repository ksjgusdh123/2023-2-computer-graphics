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
GLvoid Mouse_Click(int button, int state, int x, int y);
GLvoid InitBuffer();
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Re_Buffer();
GLvoid Timer_event(int value);

//--- �ʿ��� ���� ����
GLint width, height;
GLuint shaderProgramID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü
GLuint vbo[2], vao;

std::random_device rd;
std::mt19937 dre(rd());
std::uniform_real_distribution<float> uid{ 0.1, 1 };

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
	glutMouseFunc(Mouse_Click);
	glutTimerFunc(100, Timer_event, 1);
	glutKeyboardFunc(Keyboard);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

bool p_check = true;  
int dot_num = 0;
int draw_dot = 218;
GLfloat rColor = 1.0, gColor = 1.0, bColor = 1.0;

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	//--- ����� ���� ����
	glClearColor(rColor, gColor, bColor, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(shaderProgramID);
	//--- ����� VAO �ҷ�����
	glBindVertexArray(vao);
	//--- �ﰢ�� �׸���
	glPointSize(2);
	if (p_check && dot_num != 0) {
		for (int i = 0; i < draw_dot / 218; ++i) {
			glDrawArrays(GL_POINTS, i * 218, dot_num);
		}
	}
	else if(dot_num != 0)
		for (int i = 0; i < draw_dot / 218; ++i) {
			glDrawArrays(GL_LINE_STRIP, i * 218, dot_num);
		}

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
	vertexSource = filetobuf("����vertex.glsl");
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
	fragmentSource = filetobuf("����fragment.glsl");
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




float ox, oy;

GLvoid ConvertXY_OPENGL(int x, int y)
{
	int w = 800;
	int h = 600;

	ox = (float)(x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0));
	oy = -(float)(y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0));

}

bool on_click = false;
const double PI = 3.1415926;

GLvoid Mouse_Click(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && on_click == false) {
		ConvertXY_OPENGL(x, y);
		rColor = uid(dre);
		gColor = uid(dre);
		bColor = uid(dre);
		on_click = true;
	}
}

GLfloat shape[1100][3]{};
GLfloat colors[1100][3]{};
float r[1100]{};

GLvoid Timer_event(int value) {
	if (on_click) {
		if (dot_num < 109) {
			r[dot_num] += ((dot_num + 1)) * 0.001;
			shape[dot_num][0] = cos(((10 * dot_num) % 360) * PI / 180) * r[dot_num] + ox;
			shape[dot_num][1] = sin(((10 * dot_num) % 360) * PI / 180) * r[dot_num] + oy;

			r[218 + dot_num] += ((dot_num + 1)) * 0.001;
			shape[218 + dot_num][0] = cos(((10 * dot_num) % 360) * PI / 180) * r[dot_num] + ox + 0.5;
			shape[218 + dot_num][1] = sin(((10 * dot_num) % 360) * PI / 180) * r[dot_num] + oy;

			r[436 + dot_num] += ((dot_num + 1)) * 0.001;
			shape[436 + dot_num][0] = cos(((10 * dot_num) % 360) * PI / 180) * r[dot_num] + ox - 0.5;
			shape[436 + dot_num][1] = sin(((10 * dot_num) % 360) * PI / 180) * r[dot_num] + oy;

			r[654 + dot_num] += ((dot_num + 1)) * 0.001;
			shape[654 + dot_num][0] = cos(((10 * dot_num) % 360) * PI / 180) * r[dot_num] + ox;
			shape[654 + dot_num][1] = sin(((10 * dot_num) % 360) * PI / 180) * r[dot_num] + oy + 0.3;

			r[872 + dot_num] += ((dot_num + 1)) * 0.001;
			shape[872 + dot_num][0] = cos(((10 * dot_num) % 360) * PI / 180) * r[dot_num] + ox;
			shape[872 + dot_num][1] = sin(((10 * dot_num) % 360) * PI / 180) * r[dot_num] + oy - 0.3;

			dot_num++;
			Re_Buffer();

		}
		else if (dot_num < 218) {
			r[dot_num] += ((218 - dot_num + 1)) * 0.001;
			shape[dot_num][0] = cos(((180 - (10 * dot_num) % 360)) * PI / 180) * r[dot_num] + (cos(((10 * 108) % 360) * PI / 180) * r[108]) * 2 + ox;
			shape[dot_num][1] = sin(((180 - (10 * dot_num) % 360)) * PI / 180) * r[dot_num] + oy;

			r[218 + dot_num] += ((218 - dot_num + 1)) * 0.001;
			shape[218 + dot_num][0] = cos(((180 - (10 * dot_num) % 360)) * PI / 180) * r[dot_num] + (cos(((10 * 108) % 360) * PI / 180) * r[108]) * 2 + ox + 0.5;
			shape[218 + dot_num][1] = sin(((180 - (10 * dot_num) % 360)) * PI / 180) * r[dot_num] + oy;

			r[436 + dot_num] += ((218 - dot_num + 1)) * 0.001;
			shape[436 + dot_num][0] = cos(((180 - (10 * dot_num) % 360)) * PI / 180) * r[dot_num] + (cos(((10 * 108) % 360) * PI / 180) * r[108]) * 2 + ox - 0.5;
			shape[436 + dot_num][1] = sin(((180 - (10 * dot_num) % 360)) * PI / 180) * r[dot_num] + oy;

			r[654 + dot_num] += ((218 - dot_num + 1)) * 0.001;
			shape[654 + dot_num][0] = cos(((180 - (10 * dot_num) % 360)) * PI / 180) * r[dot_num] + (cos(((10 * 108) % 360) * PI / 180) * r[108]) * 2 + ox;
			shape[654 + dot_num][1] = sin(((180 - (10 * dot_num) % 360)) * PI / 180) * r[dot_num] + oy + 0.3;

			r[872 + dot_num] += ((218 - dot_num + 1)) * 0.001;
			shape[872 + dot_num][0] = cos(((180 - (10 * dot_num) % 360)) * PI / 180) * r[dot_num] + (cos(((10 * 108) % 360) * PI / 180) * r[108]) * 2 + ox;
			shape[872 + dot_num][1] = sin(((180 - (10 * dot_num) % 360)) * PI / 180) * r[dot_num] + oy - 0.3;

			dot_num++;
			Re_Buffer();

		}
		else {
			on_click = false;
		}
	}


	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
	glutTimerFunc(100, Timer_event, 4);
}



GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'p':
	case 'P':
		p_check = true;
		on_click = false;
		dot_num = 0;
		break;
	case 'l':
	case 'L':
		p_check = false;
		on_click = false;
		dot_num = 0;
	case '1':
		draw_dot = 218;
		on_click = false;
		dot_num = 0;
		break;
	case '2':
		draw_dot = 436;
		on_click = false;
		dot_num = 0;
		break;
	case '3':
		draw_dot = 654;
		on_click = false;
		dot_num = 0;
		break;
	case '4':
		draw_dot = 872;
		on_click = false;
		dot_num = 0;
		break;
	case '5':
		draw_dot = 1090;
		on_click = false;
		dot_num = 0;
		break;
	}
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}


GLvoid InitBuffer()
{
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

