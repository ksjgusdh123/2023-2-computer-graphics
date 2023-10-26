#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <gl/glew.h> // �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <fstream>
#include <random>
#include <vector>
#include <gl\glm\glm\glm.hpp>
#include <gl/glm/glm/ext.hpp>
#include <gl/glm/glm/gtc/matrix_transform.hpp>
char* filetobuf(const char* file);
void make_vertexShaders();
void make_fragmentShaders();
GLvoid make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid InitBuffer();
bool ReadObj(const char* path);
bool Set_VAO();
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Timer_event(int value);
GLvoid make_sp();
using namespace std;
//--- �ʿ��� ���� ����
GLint width, height;
GLuint shaderProgramID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü
GLuint vbo[2], vao;
GLuint line_vao, sp_vao;
GLuint spvbo[2];
std::random_device rd;
std::mt19937 dre(rd());
std::uniform_int_distribution<int> uid{ 0, 5 };
std::uniform_int_distribution<int> uid2{ 0, 3 };

std::vector< glm::vec3 > vertices;
std::vector< glm::vec2 > uvs;
std::vector< glm::vec3 > normals; // ������ �Ⱦ��ſ���.
std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
std::vector< glm::vec3 > temp_vertices;
std::vector< glm::vec2 > temp_uvs;
std::vector< glm::vec3 > temp_normals;
GLuint vertexbuffer;
GLuint EBO;
GLuint uvbuffer;
GLuint colorbuffer;
const double PI = 3.1415926;

GLfloat quadricColor[3] = { 0.0f, 0.0f, 1.0f };

float color[] = {
	   0.0f, 0.0f, 1.0f,//4
	   0.0f, 0.0f, 1.0f,//0
	   0.0f, 0.0f, 1.0f,//3

	   0.0f, 1.0f, 1.0f,//4
	   0.0f, 1.0f, 1.0f,//0
	   0.0f, 1.0f, 1.0f,//3

	   1.0f, 0.0f, 1.0f,//4
	   1.0f, 0.0f, 1.0f,//0
	   1.0f, 0.0f, 1.0f,//3

	   0.0f, 1.0f, 0.0f,//4
	   0.0f, 1.0f, 0.0f,//0
	   0.0f, 1.0f, 0.0f,//3

	   1.0f, 1.0f, 0.0f,//4
	   1.0f, 1.0f, 0.0f,//0
	   1.0f, 1.0f, 0.0f,//3

	   1.0f, 0.0f, 0.0f,//4
	   1.0f, 0.0f, 0.0f,//0
	   1.0f, 0.0f, 0.0f,//3

	   0.0f, 0.50f, 0.50f,//4
	   0.0f, 0.50f, 0.50f,//0
	   0.0f, 0.50f, 0.50f,//3

	   0.50f, 0.0f, 1.0f,//4
	   0.50f, 0.0f, 1.0f,//0
	   0.50f, 0.0f, 1.0f,//3

	   0.0f, 0.50f, 1.0f,//4
	   0.0f, 0.50f, 1.0f,//0
	   0.0f, 0.50f, 1.0f,//3

	   0.50f, 0.50f, 0.50f,//4
	   0.50f, 0.50f, 0.50f,//0
	   0.50f, 0.50f, 0.50f,//3

	   0.50f, 1.0f, 0.0f,//4
	   0.50f, 1.0f, 0.0f,//0
	   0.50f, 1.0f, 0.0f,//3

	   0.0f, 0.0f, 1.0f,//4
	   0.0f, 0.0f, 1.0f,//0
	   0.0f, 0.0f, 1.0f,//3

	   0.0f, 0.0f, 1.0f,//4
	   0.0f, 0.0f, 1.0f,//0
	   0.0f, 0.0f, 1.0f,//3
};

//--- ���� �Լ�
void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(700, 700);
	glutCreateWindow("Example1");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	glutTimerFunc(100, Timer_event, 1);
	make_shaderProgram();
	//InitBuffer();
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	bool res = ReadObj("C:\\Users\\jhd\\Desktop\\four_face.obj");
	Set_VAO();
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLfloat rColor = 0.0, gColor = 1.0, bColor = 1.0;



GLUquadricObj* qobj;
bool p_button = true;
bool m_button = true;
int y_button = 0;
int z_button = 0;
float sib = 0;
int rad = 0;
float x_move = 0;
float y_move = 0;
float z_move = 0;
float z_point = 0;

GLvoid drawScene() {
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	glEnable(GL_DEPTH_TEST);
	// Quadric ��ü ���� �� �ʱ�ȭ
	GLUquadricObj* qobj = gluNewQuadric();
	if (m_button) {
		gluQuadricDrawStyle(qobj, GLU_FILL);
	}
	else {
		gluQuadricDrawStyle(qobj, GLU_LINE);
	}
	gluQuadricNormals(qobj, GLU_SMOOTH);
	gluQuadricOrientation(qobj, GLU_OUTSIDE);
	gluQuadricTexture(qobj, GL_FALSE);

	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 TR = glm::mat4(1.0f);             // ���
	glm::mat4 TR_m1 = glm::mat4(1.0f);             // 2�ܰ�
	glm::mat4 TR_m2 = glm::mat4(1.0f);             // 2�ܰ�
	glm::mat4 TR_m3 = glm::mat4(1.0f);             // 2�ܰ�
	glm::mat4 TR_s1 = glm::mat4(1.0f);             // 3�ܰ�
	glm::mat4 TR_s2 = glm::mat4(1.0f);             // 3�ܰ�
	glm::mat4 TR_s3 = glm::mat4(1.0f);             // 3�ܰ�
	glm::mat4 TR_line = glm::mat4(1.0f);
	glm::mat4 TR_line2 = glm::mat4(1.0f);
	glm::mat4 TR_line3 = glm::mat4(1.0f);
	glm::mat4 TR_line_s = glm::mat4(1.0f);
	glm::mat4 TR_line2_s = glm::mat4(1.0f);
	glm::mat4 TR_line3_s = glm::mat4(1.0f);
	if(p_button)
		projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f);
	else {
		projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f); //--- ���� ���� ����: fovy, aspect, near, far
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -5.0));
	}
	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "transform");
	unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform");

	// xyz��
//	TR = glm::rotate(TR, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	glBindVertexArray(line_vao);
	// Quadric ��ü�� �Ӽ��� ����

	if (y_button > 0) {
		sib += 10;
	}
	else if (y_button < 0) {
		sib -= 10;
	}

	quadricColor[0] = 0;
	quadricColor[1] = 0;
	quadricColor[2] = 1;
	unsigned int colorLocation = glGetUniformLocation(shaderProgramID, "colorAttribute");
	TR = glm::translate(TR, glm::vec3(x_move, y_move, z_move));
	TR = glm::rotate(TR, glm::radians(sib), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	// ������ ������ ���ϴ� ��ü�� ���� ����
	glUniform3fv(colorLocation, 1, quadricColor); // ���� ����
	gluSphere(qobj, 0.3, 20, 20);

	if (z_button > 0) {
		z_point += 1;
	}
	else if (z_button < 0) {
		z_point -= 1;
	}
	else
		z_point = 0;

	// 2�ܰ� �༺
	quadricColor[0] = 0;
	quadricColor[1] = 1;
	quadricColor[2] = 0;
	rad+=10;
	TR_m1 = glm::rotate(TR_m1, glm::radians(z_point), glm::vec3(0.0f, 0.0f, 1.0f));
	TR_m1 = glm::translate(TR_m1, glm::vec3(x_move, y_move, z_move));
	TR_m1 = glm::rotate(TR_m1, glm::radians(sib), glm::vec3(0.0f, 1.0f, 0.0f));
	TR_m1 = glm::rotate(TR_m1, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	TR_m1 = glm::translate(TR_m1, glm::vec3(1.5f * sin(rad * 3.14 / 180.0), 0.0f, 1.5f * cos(3.14 * rad / 180 )));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_m1)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	glUniform3fv(colorLocation, 1, quadricColor); // ���� ����
	gluSphere(qobj, 0.2, 20, 20);

	TR_m2 = glm::rotate(TR_m2, glm::radians(z_point), glm::vec3(0.0f, 0.0f, 1.0f));
	TR_m2 = glm::translate(TR_m2, glm::vec3(x_move, y_move, z_move));
	TR_m2 = glm::rotate(TR_m2, glm::radians(sib), glm::vec3(0.0f, 1.0f, 0.0f));
	TR_m2 = glm::rotate(TR_m2, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	TR_m2 = glm::rotate(TR_m2, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	TR_m2 = glm::translate(TR_m2, glm::vec3(1.5f * sin(rad * 3.14 / 360.0), 0.0f, 1.5f * cos(3.14 * rad / 360)));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_m2)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	glUniform3fv(colorLocation, 1, quadricColor); // ���� ����
	gluSphere(qobj, 0.2, 20, 20);

	TR_m3 = glm::rotate(TR_m3, glm::radians(z_point), glm::vec3(0.0f, 0.0f, 1.0f));
	TR_m3 = glm::translate(TR_m3, glm::vec3(x_move, y_move, z_move));
	TR_m3 = glm::rotate(TR_m3, glm::radians(sib), glm::vec3(0.0f, 1.0f, 0.0f));
	TR_m3 = glm::rotate(TR_m3, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	TR_m3 = glm::rotate(TR_m3, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	TR_m3 = glm::translate(TR_m3, glm::vec3(1.5f * sin(rad * 3.14 / 90.0), 0.0f, 1.5f * cos(3.14 * rad / 90)));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_m3)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	glUniform3fv(colorLocation, 1, quadricColor); // ���� ����
	gluSphere(qobj, 0.2, 20, 20);

	// 3�ܰ�
	quadricColor[0] = 1;
	quadricColor[1] = 0;
	quadricColor[2] = 0;
	TR_s1 = glm::rotate(TR_s1, glm::radians(z_point), glm::vec3(0.0f, 0.0f, 1.0f));
	TR_s1 = glm::translate(TR_s1, glm::vec3(x_move, y_move, z_move));
	TR_s1 = glm::rotate(TR_s1, glm::radians(sib), glm::vec3(0.0f, 1.0f, 0.0f));
	TR_s1 = glm::rotate(TR_s1, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	TR_s1 = glm::translate(TR_s1, glm::vec3(0.5f * sin(rad * 3.14 / 180.0), 0.0f, 0.5f * cos(3.14 * rad / 180)));
	TR_s1 = glm::translate(TR_s1, glm::vec3(1.5f * sin(rad * 3.14 / 180.0), 0.0f, 1.5f * cos(3.14 * rad / 180)));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_s1)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	glUniform3fv(colorLocation, 1, quadricColor); // ���� ����
	gluSphere(qobj, 0.1, 20, 20);
	TR_s2 = glm::rotate(TR_s2, glm::radians(z_point), glm::vec3(0.0f, 0.0f, 1.0f));
	TR_s2 = glm::translate(TR_s2, glm::vec3(x_move, y_move, z_move));
	TR_s2 = glm::rotate(TR_s2, glm::radians(sib), glm::vec3(0.0f, 1.0f, 0.0f));
	TR_s2 = glm::rotate(TR_s2, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	TR_s2 = glm::translate(TR_s2, glm::vec3(0.5f * sin(rad * 3.14 / 360.0), 0.0f, 0.5f * cos(3.14 * rad / 360)));
	TR_s2 = glm::rotate(TR_s2, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	TR_s2 = glm::translate(TR_s2, glm::vec3(1.5f * sin(rad * 3.14 / 360.0), 0.0f, 1.5f * cos(3.14 * rad / 360)));
	TR_s2 = glm::rotate(TR_s2, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_s2)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	glUniform3fv(colorLocation, 1, quadricColor); // ���� ����
	gluSphere(qobj, 0.1, 20, 20);
	TR_s3 = glm::rotate(TR_s3, glm::radians(z_point), glm::vec3(0.0f, 0.0f, 1.0f));
	TR_s3 = glm::translate(TR_s3, glm::vec3(x_move, y_move, z_move));
	TR_s3 = glm::rotate(TR_s3, glm::radians(sib), glm::vec3(0.0f, 1.0f, 0.0f));
	TR_s3 = glm::rotate(TR_s3, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	TR_s3 = glm::translate(TR_s3, glm::vec3(0.5f * sin(rad * 3.14 / 90.0), 0.0f, 0.5f * cos(3.14 * rad / 90)));
	TR_s3 = glm::rotate(TR_s3, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	TR_s3 = glm::translate(TR_s3, glm::vec3(1.5f * sin(rad * 3.14 / 90.0), 0.0f, 1.5f * cos(3.14 * rad / 90)));
	TR_s3 = glm::rotate(TR_s3, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_s3)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	glUniform3fv(colorLocation, 1, quadricColor); // ���� ����
	gluSphere(qobj, 0.1, 20, 20);

	// ū���� �׸���
	quadricColor[0] = 0;
	quadricColor[1] = 0;
	quadricColor[2] = 0;
	TR_line = glm::rotate(TR_line, glm::radians(z_point), glm::vec3(0.0f, 0.0f, 1.0f));
	TR_line = glm::translate(TR_line, glm::vec3(x_move, y_move, z_move));
	TR_line = glm::rotate(TR_line, glm::radians(sib), glm::vec3(0.0f, 1.0f, 0.0f));
	TR_line = glm::rotate(TR_line, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_line)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	glUniform3fv(colorLocation, 1, quadricColor); // ���� ����
	glDrawArrays(GL_LINE_STRIP, 0, 365);
	TR_line2 = glm::rotate(TR_line2, glm::radians(z_point), glm::vec3(0.0f, 0.0f, 1.0f));
	TR_line2 = glm::translate(TR_line2, glm::vec3(x_move, y_move, z_move));
	TR_line2 = glm::rotate(TR_line2, glm::radians(sib), glm::vec3(0.0f, 1.0f, 0.0f));
	TR_line2 = glm::rotate(TR_line2, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	TR_line2 = glm::rotate(TR_line2, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_line2)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	glDrawArrays(GL_LINE_STRIP, 365, 365);
	TR_line3 = glm::rotate(TR_line3, glm::radians(z_point), glm::vec3(0.0f, 0.0f, 1.0f));
	TR_line3 = glm::translate(TR_line3, glm::vec3(x_move, y_move, z_move));
	TR_line3 = glm::rotate(TR_line3, glm::radians(sib), glm::vec3(0.0f, 1.0f, 0.0f));
	TR_line3 = glm::rotate(TR_line3, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	TR_line3 = glm::rotate(TR_line3, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_line3)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	glDrawArrays(GL_LINE_STRIP, 730, 365);
	
	// ���� ���� �׸���
	TR_line_s = glm::rotate(TR_line_s, glm::radians(z_point), glm::vec3(0.0f, 0.0f, 1.0f));
	TR_line_s = glm::translate(TR_line_s, glm::vec3(x_move, y_move, z_move));
	TR_line_s = glm::rotate(TR_line_s, glm::radians(sib), glm::vec3(0.0f, 1.0f, 0.0f));
	TR_line_s = glm::rotate(TR_line_s, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	TR_line_s = glm::translate(TR_line_s, glm::vec3(1.5f * sin(rad * 3.14 / 180.0), 0.0f, 1.5f * cos(3.14 * rad / 180)));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_line_s)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	glUniform3fv(colorLocation, 1, quadricColor); // ���� ����
	glDrawArrays(GL_LINE_STRIP, 1095, 365);
	TR_line2_s = glm::rotate(TR_line2_s, glm::radians(z_point), glm::vec3(0.0f, 0.0f, 1.0f));
	TR_line2_s = glm::translate(TR_line2_s, glm::vec3(x_move, y_move, z_move));
	TR_line2_s = glm::rotate(TR_line2_s, glm::radians(sib), glm::vec3(0.0f, 1.0f, 0.0f));
	TR_line2_s = glm::rotate(TR_line2_s, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	TR_line2_s = glm::rotate(TR_line2_s, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	TR_line2_s = glm::translate(TR_line2_s, glm::vec3(1.5f * sin(rad * 3.14 / 360.0), 0.0f, 1.5f * cos(3.14 * rad / 360)));
	TR_line2_s = glm::rotate(TR_line2_s, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_line2_s)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	glDrawArrays(GL_LINE_STRIP, 1460, 365);
	TR_line3_s = glm::rotate(TR_line3_s, glm::radians(z_point), glm::vec3(0.0f, 0.0f, 1.0f));
	TR_line3_s = glm::translate(TR_line3_s, glm::vec3(x_move, y_move, z_move));
	TR_line3_s = glm::rotate(TR_line3_s, glm::radians(sib), glm::vec3(0.0f, 1.0f, 0.0f));
	TR_line3_s = glm::rotate(TR_line3_s, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	TR_line3_s = glm::rotate(TR_line3_s, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	TR_line3_s = glm::translate(TR_line3_s, glm::vec3(1.5f * sin(rad * 3.14 / 90.0), 0.0f, 1.5f * cos(3.14 * rad / 90)));
	TR_line3_s = glm::rotate(TR_line3_s, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_line3_s)); //--- modelTransform ������ ��ȯ �� �����ϱ�
	glDrawArrays(GL_LINE_STRIP, 1825, 365);


	gluDeleteQuadric(qobj); // Quadric ��ü ����

	glutSwapBuffers();
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'p':
		if (p_button)
			p_button = false;
		else
			p_button = true;
		break;
	case 'm':
		if (m_button)
			m_button = false;
		else
			m_button = true;
		break;
	case 'y':
		y_button += 1;
		break;
	case 'Y':
		y_button -= 1;
		break;
	case '+':
		z_move += 0.1;
		break;
	case '-':
		z_move -= 0.1;
		break;
	case 'w':
		y_move += 0.1;
		break;
	case 's':
		y_move -= 0.1;
		break;
	case 'a':
		x_move -= 0.1;
		break;
	case 'd':
		x_move += 0.1;
		break;
	case 'z':
		z_button += 1;
		break;
	case 'Z':
		z_button -= 1;
		break;
	}

	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�

}

GLfloat line[2190][3]{ };

GLfloat line_color[2190][3]{};



bool Set_VAO() {
	for (int i = 0; i < 365; ++i) {
		line[i][0] = 1.5 * sin(i * 3.14 / 180.0);
		line[i][1] = 0;
		line[i][2] = 1.5 * cos(i * 3.14 / 180.0);
		line[i + 365][0] = 1.5 * sin(i * 3.14 / 180.0);
		line[i + 365][1] = 0;
		line[i + 365][2] = 1.5 * cos(i * 3.14 / 180.0);
		line[i + 730][0] = 1.5 * sin(i * 3.14 / 180.0);
		line[i + 730][1] = 0;
		line[i + 730][2] = 1.5 * cos(i * 3.14 / 180.0);
		line[i + 1095][0] = 0.5 * sin(i * 3.14 / 180.0);
		line[i + 1095][1] = 0;
		line[i + 1095][2] = 0.5 * cos(i * 3.14 / 180.0);
		line[i + 1460][0] = 0.5 * sin(i * 3.14 / 180.0);
		line[i + 1460][1] = 0;
		line[i + 1460][2] = 0.5 * cos(i * 3.14 / 180.0);
		line[i + 1825][0] = 0.5 * sin(i * 3.14 / 180.0);
		line[i + 1825][1] = 0;
		line[i + 1825][2] = 0.5 * cos(i * 3.14 / 180.0);
	}
	ReadObj("C:\\Users\\jhd\\Desktop\\mycube.obj");
	glGenVertexArrays(1, &line_vao);
	glBindVertexArray(line_vao);

	glGenBuffers(2, vbo);
	//���� ������Ʈ�� ���ε� �Ѵ�.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//���� ������Ʈ�� �����͸� ����
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);

	//��ġ �������� �Լ�
	GLint lineAttribute = glGetAttribLocation(shaderProgramID, "positionAttribute");
	//���� ������Ʈ�� ���ε� �Ѵ�.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//���ؽ� �Ӽ� �������� �迭�� ����
	glVertexAttribPointer(lineAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�.
	glEnableVertexAttribArray(lineAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//���� ������Ʈ�� �����͸� ����
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_color), line, GL_STATIC_DRAW);

	//��ġ �������� �Լ�
	GLint line_Attribute = glGetAttribLocation(shaderProgramID, "colorAttribute");
	//���� ������Ʈ�� ���ε� �Ѵ�.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//���ؽ� �Ӽ� �������� �迭�� ����
	glVertexAttribPointer(line_Attribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�.
	glEnableVertexAttribArray(line_Attribute);


	return true;
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
	fragmentSource = filetobuf("3Dfragment.glsl");
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



GLvoid InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�
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

GLvoid Timer_event(int value) {





	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
	glutTimerFunc(100, Timer_event, 4);
}

bool ReadObj(const char* path) {
	vertexIndices.clear();
	uvIndices.clear();
	normalIndices.clear();
	vertices.clear();
	uvs.clear();
	normals.clear();

	ifstream in(path);
	if (!in) {
		cerr << path << "���� ��ã��";
		exit(1);
	}

	//vector<char> lineHeader(istream_iterator<char>{in}, {});

	while (in) {
		std::string lineHeader;
		in >> lineHeader;
		if (lineHeader == "v") {
			glm::vec3 vertex;
			in >> vertex.x >> vertex.y >> vertex.z;
			vertices.push_back(vertex);
		}
		else if (lineHeader == "vt") {
			glm::vec2 uv;
			in >> uv.x >> uv.y;
			uvs.push_back(uv);
		}
		else if (lineHeader == "vn") {
			glm::vec3 normal;
			in >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (lineHeader == "f") {
			char a;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

			for (int i = 0; i < 3; i++)
			{
				in >> vertexIndex[i];// >> a >> uvIndex[i] >> a >> normalIndex[i];
				vertexIndices.push_back(vertexIndex[i] - 1);
				//	uvIndices.push_back(uvIndex[i] - 1);
					//normalIndices.push_back(normalIndex[i] - 1);
			}
		}
	}

	return true;
}