#define _CRT_SECURE_NO_WARNINGS //--- ���α׷� �� �տ� ������ ��
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <gl/glew.h> // �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <vector>

char* filetobuf(const char* file);
void make_vertexShaders();
void make_fragmentShaders();
GLvoid make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid InitBuffer();
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse_Click(int button, int state, int x, int y);
GLvoid ConvertXY_OPENGL(int x, int y);
GLvoid Timer_event(int value);
GLvoid re_buffer();

//--- �ʿ��� ���� ����
GLint width, height;
GLuint shaderProgramID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü
GLuint vao, vbo[2];

GLfloat point[60][3]{};

GLfloat point_color[60][3]{};

int point_arr[10]{};

int point_num = 0;

int shape_num = 0;

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
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse_Click);
	glutTimerFunc(100, Timer_event, 1);// Ÿ�̸� ����
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
	glPointSize(10);

	int start_num = 0;

	for (int i = 0; i < shape_num; ++i) {
		switch (point_arr[i]) {
		case 1:
			glDrawArrays(GL_POINTS, start_num, 1);
			start_num++;
			break;
		case 2:
			glDrawArrays(GL_LINES, start_num, 2);
			start_num += 2;
			break;
		case 3:
			glDrawArrays(GL_TRIANGLES, start_num, 3);
			start_num += 3;
			break;
		case 6:
			glDrawArrays(GL_TRIANGLES, start_num, 6);
			start_num += 6;
			break;
		}
	}

	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}

//--- �ٽñ׸��� �ݹ� �Լ�
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}




float ox, oy;

char input_key;

GLvoid ConvertXY_OPENGL(int x, int y)
{
	int w = 800;
	int h = 600;

	ox = (float)(x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0));
	oy = -(float)(y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0));

}

GLvoid Mouse_Click(int button, int state, int x, int y) {
	ConvertXY_OPENGL(x, y);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (shape_num < 10) {
			switch (input_key) {
			case 'p':
				point[point_num][0] = ox;
				point[point_num][1] = oy;
				point[point_num][2] = 0;
				point_color[point_num][0] = 1;
				point_color[point_num][1] = 0;
				point_color[point_num][2] = 0;
				point_arr[shape_num] = 1;
				re_buffer();
				point_num++;
				shape_num++;
				break;
			case 'l':
				point[point_num][0] = ox;
				point[point_num][1] = oy;
				point[point_num][2] = 0;
				point[point_num + 1][0] = ox + 0.2;
				point[point_num + 1][1] = oy;
				point[point_num + 1][2] = 0;
				point_color[point_num][0] = 0;
				point_color[point_num][1] = 1;
				point_color[point_num][2] = 0;
				point_color[point_num + 1][0] = 0;
				point_color[point_num + 1][1] = 1;
				point_color[point_num + 1][2] = 0;
				point_arr[shape_num] = 2;
				re_buffer();
				point_num += 2;
				shape_num++;
				break;
			case 't':
				point[point_num][0] = ox;
				point[point_num][1] = oy;
				point[point_num][2] = 0;
				point[point_num + 1][0] = ox - 0.125;
				point[point_num + 1][1] = oy - 0.25;
				point[point_num + 1][2] = 0;
				point[point_num + 2][0] = ox + 0.125;
				point[point_num + 2][1] = oy - 0.25;
				point[point_num + 2][2] = 0;
				point_color[point_num][0] = 0;
				point_color[point_num][1] = 0;
				point_color[point_num][2] = 1;
				point_color[point_num + 1][0] = 0;
				point_color[point_num + 1][1] = 0;
				point_color[point_num + 1][2] = 1;
				point_color[point_num + 2][0] = 0;
				point_color[point_num + 2][1] = 0;
				point_color[point_num + 2][2] = 1;
				point_arr[shape_num] = 3;
				re_buffer();
				point_num += 3;
				shape_num++;
				break;
			case 'r':
				point[point_num][0] = ox;
				point[point_num][1] = oy;
				point[point_num][2] = 0;
				point[point_num + 1][0] = ox;
				point[point_num + 1][1] = oy - 0.25;
				point[point_num + 1][2] = 0;
				point[point_num + 2][0] = ox + 0.125;
				point[point_num + 2][1] = oy - 0.25;
				point[point_num + 2][2] = 0;

				point[point_num + 3][0] = ox + 0.125;
				point[point_num + 3][1] = oy - 0.25;
				point[point_num + 3][2] = 0;
				point[point_num + 4][0] = ox + 0.125;
				point[point_num + 4][1] = oy;
				point[point_num + 4][2] = 0;
				point[point_num + 5][0] = ox;
				point[point_num + 5][1] = oy;
				point[point_num + 5][2] = 0;

				point_color[point_num][0] = 0;
				point_color[point_num][1] = 1;
				point_color[point_num][2] = 1;
				point_color[point_num + 1][0] = 0;
				point_color[point_num + 1][1] = 1;
				point_color[point_num + 1][2] = 1;
				point_color[point_num + 2][0] = 0;
				point_color[point_num + 2][1] = 1;
				point_color[point_num + 2][2] = 1;

				point_color[point_num + 3][0] = 0;
				point_color[point_num + 3][1] = 1;
				point_color[point_num + 3][2] = 1;
				point_color[point_num + 4][0] = 0;
				point_color[point_num + 4][1] = 1;
				point_color[point_num + 4][2] = 1;
				point_color[point_num + 5][0] = 0;
				point_color[point_num + 5][1] = 1;
				point_color[point_num + 5][2] = 1;
				point_arr[shape_num] = 6;
				re_buffer();
				point_num += 6;
				shape_num++;
				break;
			}
		}
	}
}


GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'p':
	case 'P':
		input_key = 'p';
		break;
	case 'l':
	case 'L':
		input_key = 'l';
		break;
	case 't':
	case 'T':
		input_key = 't';
		break;
	case 'r':
	case 'R':
		input_key = 'r';
		break;
	case 'w':
	case 'W':
		input_key = 'w';
		break;
	case 'a':
	case 'A':
		input_key = 'a';
		break;
	case 's':
	case 'S':
		input_key = 's';
		break;
	case 'd':
	case 'D':
		input_key = 'd';
		break;
	case 'q':
	case 'Q':
		input_key = 'q';
		break;
	case 'c':
	case 'C':
		/*for (int i = 0; i < 60; ++i) {
			for (int j = 0; j < 3; ++j) {
				point_color[i][j] = NULL;
				point[i][j] = NULL;
			}
		}
		
		point_arr[10]{};*/

		point_num = 0;

		shape_num = 0;
		break;
	}
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

GLvoid Timer_event(int value) {
	switch (input_key) {
	case 'w':
		for (int i = 0; i < point_num; ++i) {
			point[i][1] += 0.05;
		}
		re_buffer();
		break;
	case 'a':
		for (int i = 0; i < point_num; ++i) {
			point[i][0] -= 0.05;
		}
		re_buffer();
		break;
	case 's':
		for (int i = 0; i < point_num; ++i) {
			point[i][1] -= 0.05;
		}
		re_buffer();
		break;
	case 'd':
		for (int i = 0; i < point_num; ++i) {
			point[i][0] += 0.05;
		}
		re_buffer();
		break;
	}
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
	glutTimerFunc(100, Timer_event, 4);
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



GLvoid InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�
	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	////--- ���� diamond ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	////--- triShape �迭�� ������: 9 * float
	//glBufferData(GL_ARRAY_BUFFER, sizeof(triShape), triShape, GL_STATIC_DRAW);
	////--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	////--- attribute �ε��� 0���� ��밡���ϰ� ��
	//glEnableVertexAttribArray(0);
	////--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	////--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	////--- colors �迭�� ������: 9 *float 
	//glBufferData(GL_ARRAY_BUFFER, sizeof(triShape), colors, GL_STATIC_DRAW);
	////--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float 
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	////--- attribute �ε��� 1���� ��� �����ϰ� ��.
	//glEnableVertexAttribArray(1);

}

GLvoid re_buffer() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point), point_color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

}

