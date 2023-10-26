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
GLvoid Mouse_Move(int x, int y);
GLvoid Timer_event(int value);
GLvoid only_line();
GLvoid draw_rec();

//--- �ʿ��� ���� ����
GLint width, height;
GLuint shaderProgramID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü
GLuint vbo[2], vao;
GLuint rec_vbo[2];


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
	glutMotionFunc(Mouse_Move);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

bool p_check = true;
int dot_num = 0;
int draw_dot = 218;
GLfloat rColor = 1.0, gColor = 1.0, bColor = 1.0;

bool click = false;
int point_num = 0;

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
	only_line();
	draw_rec();
	
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

GLfloat line_background[4][3]{ {0, 1, 0}, {0, -1, 0}, {1, 0, 0}, {-1, 0, 0} };
GLfloat back_colors[4][3]{ {1,1,0},{1,1,0},{1,1,0},{1,1,0} };

GLfloat rec[5][3]{ {-0.8, 0.3, 0}, {-0.8, -0.3, 0}, {0.8, -0.3, 0}, {0.8, 0.3, 0}, {-0.8, 0.3, 0} };
GLfloat rec_colors[5][3]{ {1,0,1},{1,0,1},{1,0,1},{1,0,1},{1,0,1} };
float start_ox;
float start_oy;

GLvoid Mouse_Click(int button, int state, int x, int y) {
	float min_point_x = 0;
	float min_point_y = 0;
	float max_point_x = 0;
	float max_point_y = 0;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		ConvertXY_OPENGL(x, y);
		if (rec[0][0] < rec[1][0])
			min_point_x = rec[0][0];
		else 
			min_point_x = rec[1][0];

		if (rec[2][0] < rec[3][0])
			max_point_x = rec[3][0];
		else
			max_point_x = rec[2][0];

		if (rec[0][1] < rec[1][1])
			min_point_y = rec[0][1];
		else
			min_point_y = rec[1][1];

		if (rec[2][1] < rec[3][1])
			max_point_y = rec[3][1];
		else
			max_point_y = rec[2][1];

		for (int i = 0; i < 5; ++i) {
			if (rec[i][0] - 0.005 <= ox && rec[i][0] + 0.005 >= ox && rec[i][1] - 0.005 <= oy && rec[i][1] + 0.005 >= oy) {
				click = true;
				point_num = i + 1;
			}
			else if (min_point_x <= ox && max_point_x >= ox && min_point_y <= oy && max_point_y >= oy) {
				click = true;
				point_num = 100;
				start_ox = ox;
				start_oy = oy;
			}
		}
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		click = false;
	}
}

GLvoid Mouse_Move(int x, int y)
{
	ConvertXY_OPENGL(x, y);

	if (click) {
		switch (point_num) {
		case 2:
			rec[1][0] = ox;
			rec[1][1] = oy;
			break;
		case 3:
			rec[2][0] = ox;
			rec[2][1] = oy;
			break;
		case 4:
			rec[3][0] = ox;
			rec[3][1] = oy;
			break;
		case 5:
			rec[4][0] = ox;
			rec[4][1] = oy;
			rec[0][0] = ox;
			rec[0][1] = oy;
			break;
		case 100:
			for (int i = 0; i < 5; ++i) {
				if (start_ox > ox) {
					rec[i][0] -= start_ox - ox;
					rec[i][1] -= start_oy - oy;
				}
				else {
					rec[i][0] += ox - start_ox;
					rec[i][1] += oy - start_oy;
				}
			}
			start_ox = ox;
			start_oy = oy;
		}
		Re_Buffer();
	}
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�



}



GLvoid Timer_event(int value) {
	


	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
	glutTimerFunc(100, Timer_event, 4);
}



GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	
	}
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}


GLvoid InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�
	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_background), line_background, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_background), back_colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glGenBuffers(2, rec_vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
	glBindBuffer(GL_ARRAY_BUFFER, rec_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rec), rec, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, rec_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rec), rec_colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	
}

GLvoid Re_Buffer() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_background), line_background, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_background), back_colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, rec_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rec), rec, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, rec_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rec), rec_colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
}

GLvoid only_line() {
	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader�� 'layout (location = 0)' �κ�
	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color"); //	: 1


	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // VBO Bind
		glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		// PosLocation			- Location ��ȣ
		// 3					- VerTex Size (x, y, z �Ӽ��� Vec3�̴� 3) 
		// GL_FLOAT, GL_FALSE	- �ڷ����� Normalize ����
		// sizeof(float) * 3	- VerTex ������ ���� ũ�� (�� �������� �޸� ����)
		//			(0�� ����)	- 0 �� ��� �ڵ����� 2��° ����(3) x 3��° ����(float)�� ����
		// 0					- ������ ���� offset (0�̸� ������ ó������ ����)
	}
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // VBO Bind
		glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	}
	glDrawArrays(GL_LINES, 0, 4); // ������� ���
}

GLvoid draw_rec() {
	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader�� 'layout (location = 0)' �κ�
	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color"); //	: 1


	{
		glBindBuffer(GL_ARRAY_BUFFER, rec_vbo[0]); // VBO Bind
		glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		// PosLocation			- Location ��ȣ
		// 3					- VerTex Size (x, y, z �Ӽ��� Vec3�̴� 3) 
		// GL_FLOAT, GL_FALSE	- �ڷ����� Normalize ����
		// sizeof(float) * 3	- VerTex ������ ���� ũ�� (�� �������� �޸� ����)
		//			(0�� ����)	- 0 �� ��� �ڵ����� 2��° ����(3) x 3��° ����(float)�� ����
		// 0					- ������ ���� offset (0�̸� ������ ó������ ����)
	}
	{
		glBindBuffer(GL_ARRAY_BUFFER, rec_vbo[1]); // VBO Bind
		glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	}
	glDrawArrays(GL_LINE_STRIP, 0, 5); // ������� ���
}