#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <gl/glew.h> // 필요한 헤더파일 include
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
//--- 필요한 변수 선언
GLint width, height;
GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체
GLuint vbo[2], vao;
GLuint line_vao, sp_vao;
GLuint spvbo[2];
std::random_device rd;
std::mt19937 dre(rd());
std::uniform_int_distribution<int> uid{ 0, 5 };
std::uniform_int_distribution<int> uid2{ 0, 3 };

std::vector< glm::vec3 > vertices;
std::vector< glm::vec2 > uvs;
std::vector< glm::vec3 > normals; // 지금은 안쓸거에요.
std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
std::vector< glm::vec3 > temp_vertices;
std::vector< glm::vec2 > temp_uvs;
std::vector< glm::vec3 > temp_normals;
GLuint vertexbuffer;
GLuint EBO;
GLuint uvbuffer;
GLuint colorbuffer;
const double PI = 3.1415926;

float color[] = {
	   0.5f, 0.0f, 0.5f,//4
	   0.0f, 0.0f, 1.0f,//0
	   0.0f, 0.0f, 0.0f,//3

	   0.5f, 0.0f, 0.5f,//4
	   0.0f, 0.0f, 0.0f,//3
	   1.0f, 0.0f, 0.0f,//7

	   0.0f, 1.0f, 0.0f,//2
	   0.5f, 0.5f, 0.0f,//6
	   1.0f, 0.0f, 0.0f,//7

	   0.0f, 1.0f, 0.0f,//2
	   1.0f, 0.0f, 0.0f,//7
	   0.0f, 0.0f, 0.0f,//3

	   0.0f, 0.5f, 0.5f,//1
	   1.0f, 1.0f, 1.0f,//5
	   0.0f, 1.0f, 0.0f,//2

	   1.0f, 1.0f, 1.0f,//5
	   0.5f, 0.5f, 0.0f,//6
	   0.0f, 1.0f, 0.0f,//2

	   0.0f, 0.0f, 1.0f,//0
	   0.5f, 0.0f, 0.5f,//4
	   0.0f, 0.5f, 0.5f,//1

	   0.5f, 0.0f, 0.5f,//4
	   1.0f, 1.0f, 1.0f,//5
	   0.0f, 0.5f, 0.5f,//1

	   0.5f, 0.0f, 0.5f,//4
	   1.0f, 0.0f, 0.0f,//7
	   1.0f, 1.0f, 1.0f,//5

	   1.0f, 0.0f, 0.0f,//7
	   0.5f, 0.5f, 0.0f,//6
	   1.0f, 1.0f, 1.0f,//5

	   0.0f, 0.0f, 1.0f,//0
	   0.0f, 0.5f, 0.5f,//1
	   0.0f, 1.0f, 0.0f,//2

	   0.0f, 0.0f, 1.0f,//0
	   0.0f, 1.0f, 0.0f,//2
	   0.0f, 0.0f, 0.0f,//3

	   0.0f, 0.0f, 0.0f,
	   0.0f, 0.0f, 0.0f,
	   0.0f, 0.0f, 0.0f,
	   0.0f, 0.0f, 0.0f
};

//--- 메인 함수
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(700, 700);
	glutCreateWindow("Example1");
	//--- GLEW 초기화하기
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

GLfloat rColor = 0.0, gColor = 0.0, bColor = 0.0;



GLUquadricObj* qobj;


bool b_button = false;
bool B_button = false;
bool m_button = false;
bool M_button = false;
bool f_button = false;
bool F_button = false;
bool e_button = false;
bool E_button = false;
bool t_button = false;
bool T_button = false;
bool r_button = false;

float x_move = 0;
float y_rotate = 0;
float f_y_rotate = 0;
float main_arm_move = 0;
float sub_arm_rotate = 0;
float mul = 1;

float y_y_rad = 0;
float y_rad = 0;
float x_rad = 0;
float z_rad = 0;

GLfloat quadricColor[3] = { 0.0f, 0.0f, 1.0f };


GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	//--- 변경된 배경색 설정
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderProgramID);

	qobj = gluNewQuadric();
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 TR_ground = glm::mat4(1.0f);
	glm::mat4 TR_main = glm::mat4(1.0f);
	glm::mat4 TR_sub = glm::mat4(1.0f);
	glm::mat4 TR_main_arm_L = glm::mat4(1.0f);
	glm::mat4 TR_main_arm_R = glm::mat4(1.0f);
	glm::mat4 TR_sub_arm_L = glm::mat4(1.0f);
	glm::mat4 TR_sub_arm_R = glm::mat4(1.0f);
	glm::mat4 TR_line = glm::mat4(1.0f);

	if (b_button)
		x_move += 0.1;
	else if (B_button)
		x_move -= 0.1;

	if (m_button)
		y_rotate += 10;
	else if (M_button)
		y_rotate -= 10;

	if (e_button) {
		if (f_y_rotate != 0) {
			f_y_rotate += 10;
			if (f_y_rotate >= 360)
				f_y_rotate = 0;
		}
		else {
			f_y_rotate = 0;
			f_button = false;
			if (-0.30 + main_arm_move <= 0)
				main_arm_move += 0.05;
			else {
				e_button = false;
			}
		}
	}
	else if (E_button) {
		if (f_y_rotate != 0) {
			f_y_rotate += 10;
			if (f_y_rotate >= 360)
				f_y_rotate = 0;
		}
		else {
			f_y_rotate = 0;
			f_button = false;
			if (main_arm_move > 0)
				main_arm_move -= 0.05;
			else {
				E_button = false;
			}
		}
	}
	else if (f_button) {
		f_y_rotate += 10;
		if (f_y_rotate >= 360)
			f_y_rotate = 0;
	}

	if (t_button) {
		
		sub_arm_rotate += 10 * mul;
		if (sub_arm_rotate >= 90) {
			mul = -1;
		}
		if (sub_arm_rotate <= -90) {
			mul = 1;
		}
	}
	else if (T_button) {
		sub_arm_rotate -= 10 * mul;
		if (sub_arm_rotate <= -90) {
			mul = -1;
		}
		else if (sub_arm_rotate >= 90) {
			mul = 1;
		}
	}
	if (r_button) {
		y_rad += 10;
	}

	
	unsigned int colorLocation = glGetUniformLocation(shaderProgramID, "colorAttribute");
	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "transform"); //--- 버텍스 세이더에서모델 변환 위치 가져오기
	unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform"); //--- 투영 변환 값 설정
	// 원근 투영
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f); //--- 투영 공간 설정: fovy, aspect, near, far
	projection = glm::rotate(projection, glm::radians(y_y_rad), glm::vec3(0.0, 1.0, 0.0));
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -5.0));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	glBindVertexArray(vao);
	//// 땅바닥
	quadricColor[0] = 0;
	quadricColor[1] = 0;
	quadricColor[2] = 1;
	TR_ground = glm::rotate(TR_ground, glm::radians(z_rad), glm::vec3(0.0, 0.0, 1.0));
	TR_ground = glm::rotate(TR_ground, glm::radians(30.0f + x_rad), glm::vec3(1.0, 0.0, 0.0));
	TR_ground = glm::rotate(TR_ground, glm::radians(15.0f + y_rad), glm::vec3(0.0, 1.0, 0.0));
	TR_ground = glm::translate(TR_ground, glm::vec3(0.0f, -0.25f, 0.0));
	TR_ground = glm::translate(TR_ground, glm::vec3(0.0f, 0.25f, 0.0));
	TR_ground = glm::scale(TR_ground, glm::vec3(50, 50, 50));
	TR_ground = glm::translate(TR_ground, glm::vec3(0.0f, -0.25f, 0.0));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_ground)); //--- modelTransform 변수에 변환 값 적용하기
	glUniform3fv(colorLocation, 1, quadricColor); // 예시 색상
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	 
	// 메인프레임
	quadricColor[0] = 0;
	quadricColor[1] = 1;
	quadricColor[2] = 0;
	TR_main = glm::rotate(TR_main, glm::radians(z_rad), glm::vec3(0.0, 0.0, 1.0));
	TR_main = glm::rotate(TR_main, glm::radians(30.0f + x_rad), glm::vec3(1.0, 0.0, 0.0));
	TR_main = glm::rotate(TR_main, glm::radians(15.0f + y_rad), glm::vec3(0.0, 1.0, 0.0));
	TR_main = glm::translate(TR_main, glm::vec3(0.0f + x_move, 0.25f, 0.0f));
	TR_main = glm::scale(TR_main, glm::vec3(2, 1, 2));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_main)); //--- modelTransform 변수에 변환 값 적용하기
	glUniform3fv(colorLocation, 1, quadricColor); // 예시 색상
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// 메인프레임 왼쪽 팔
	quadricColor[0] = 1;
	quadricColor[1] = 1;
	quadricColor[2] = 0;
	TR_main_arm_L = glm::rotate(TR_main_arm_L, glm::radians(z_rad), glm::vec3(0.0, 0.0, 1.0));
	TR_main_arm_L = glm::rotate(TR_main_arm_L, glm::radians(30.0f + x_rad), glm::vec3(1.0, 0.0, 0.0));
	TR_main_arm_L = glm::rotate(TR_main_arm_L, glm::radians(15.0f + y_rad), glm::vec3(0.0, 1.0, 0.0));
	TR_main_arm_L = glm::translate(TR_main_arm_L, glm::vec3(0.0f + x_move, 0.25f, 0.0f));
	TR_main_arm_L = glm::rotate(TR_main_arm_L, glm::radians(-f_y_rotate), glm::vec3(0.0, 1.0, 0.0));
	TR_main_arm_L = glm::translate(TR_main_arm_L, glm::vec3(0.0f - x_move, -0.25f, 0.0f));
	TR_main_arm_L = glm::translate(TR_main_arm_L, glm::vec3(-0.30f + x_move + main_arm_move, 0.25f, 0.50f));
	TR_main_arm_L = glm::scale(TR_main_arm_L, glm::vec3(0.4, 0.2, 1.5));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_main_arm_L)); //--- modelTransform 변수에 변환 값 적용하기
	glUniform3fv(colorLocation, 1, quadricColor); // 예시 색상
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// 메인프레임 오른 팔
	quadricColor[0] = 0;
	quadricColor[1] = 1;
	quadricColor[2] = 1; TR_main_arm_R = glm::rotate(TR_main_arm_R, glm::radians(z_rad), glm::vec3(0.0, 0.0, 1.0));
	TR_main_arm_R = glm::rotate(TR_main_arm_R, glm::radians(30.0f + x_rad), glm::vec3(1.0, 0.0, 0.0));
	TR_main_arm_R = glm::rotate(TR_main_arm_R, glm::radians(15.0f + y_rad), glm::vec3(0.0, 1.0, 0.0));
	TR_main_arm_R = glm::translate(TR_main_arm_R, glm::vec3(0.0f + x_move, 0.25f, 0.0f));
	TR_main_arm_R = glm::rotate(TR_main_arm_R, glm::radians(f_y_rotate), glm::vec3(0.0, 1.0, 0.0));
	TR_main_arm_R = glm::translate(TR_main_arm_R, glm::vec3(0.0f - x_move, -0.25f, 0.0f));
	TR_main_arm_R = glm::translate(TR_main_arm_R, glm::vec3(0.30f + x_move - main_arm_move, 0.25f, 0.50f));
	TR_main_arm_R = glm::scale(TR_main_arm_R, glm::vec3(0.4, 0.2, 1.5));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_main_arm_R)); //--- modelTransform 변수에 변환 값 적용하기
	glUniform3fv(colorLocation, 1, quadricColor); // 예시 색상
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// 서브프레임
	quadricColor[0] = 1;
	quadricColor[1] = 0;
	quadricColor[2] = 0; TR_sub = glm::rotate(TR_sub, glm::radians(z_rad), glm::vec3(0.0, 0.0, 1.0));
	TR_sub = glm::rotate(TR_sub, glm::radians(30.0f + x_rad), glm::vec3(1.0, 0.0, 0.0));
	TR_sub = glm::rotate(TR_sub, glm::radians(15.0f + y_rad), glm::vec3(0.0, 1.0, 0.0));
	TR_sub = glm::translate(TR_sub, glm::vec3(0.0f + x_move, 0.625, 0.0f));
	TR_sub = glm::rotate(TR_sub, glm::radians(y_rotate), glm::vec3(0.0, 1.0, 0.0));
	TR_sub = glm::scale(TR_sub, glm::vec3(1.5, 0.5, 1.5));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_sub)); //--- modelTransform 변수에 변환 값 적용하기
	glUniform3fv(colorLocation, 1, quadricColor); // 예시 색상
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// 서브프레임 왼쪽 팔
	quadricColor[0] = 1;
	quadricColor[1] = 0;
	quadricColor[2] = 1; TR_sub_arm_L = glm::rotate(TR_sub_arm_L, glm::radians(z_rad), glm::vec3(0.0, 0.0, 1.0));
	TR_sub_arm_L = glm::rotate(TR_sub_arm_L, glm::radians(30.0f + x_rad), glm::vec3(1.0, 0.0, 0.0));
	TR_sub_arm_L = glm::rotate(TR_sub_arm_L, glm::radians(15.0f + y_rad), glm::vec3(0.0, 1.0, 0.0));
	TR_sub_arm_L = glm::translate(TR_sub_arm_L, glm::vec3(-0.250f + x_move, 0.8750f, 0.0f));
	TR_sub_arm_L = glm::rotate(TR_sub_arm_L, glm::radians(sub_arm_rotate), glm::vec3(0.0, 0.0, 1.0));
	TR_sub_arm_L = glm::rotate(TR_sub_arm_L, glm::radians(y_rotate), glm::vec3(0.0, 1.0, 0.0));
	TR_sub_arm_L = glm::scale(TR_sub_arm_L, glm::vec3(0.3, 0.5, 0.2));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_sub_arm_L)); //--- modelTransform 변수에 변환 값 적용하기
	glUniform3fv(colorLocation, 1, quadricColor); // 예시 색상
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// 서브프레임 왼쪽 팔
	quadricColor[0] = 1;
	quadricColor[1] = 1;
	quadricColor[2] = 1; TR_sub_arm_R = glm::rotate(TR_sub_arm_R, glm::radians(z_rad), glm::vec3(0.0, 0.0, 1.0));
	TR_sub_arm_R = glm::rotate(TR_sub_arm_R, glm::radians(30.0f + x_rad), glm::vec3(1.0, 0.0, 0.0));
	TR_sub_arm_R = glm::rotate(TR_sub_arm_R, glm::radians(15.0f + y_rad), glm::vec3(0.0, 1.0, 0.0));
	TR_sub_arm_R = glm::translate(TR_sub_arm_R, glm::vec3(0.250f + x_move, 0.8750f, 0.0f));
	TR_sub_arm_R = glm::rotate(TR_sub_arm_R, glm::radians(sub_arm_rotate * -1.0f), glm::vec3(0.0, 0.0, 1.0));
	TR_sub_arm_R = glm::rotate(TR_sub_arm_R, glm::radians(y_rotate), glm::vec3(0.0, 1.0, 0.0));
	TR_sub_arm_R = glm::scale(TR_sub_arm_R, glm::vec3(0.3, 0.5, 0.2));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_sub_arm_R)); //--- modelTransform 변수에 변환 값 적용하기
	glUniform3fv(colorLocation, 1, quadricColor); // 예시 색상
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// xyz축
	quadricColor[0] = 0;
	quadricColor[1] = 1;
	quadricColor[2] = 0; TR_line = glm::rotate(TR_line, glm::radians(z_rad), glm::vec3(0.0, 0.0, 1.0));
	TR_line = glm::rotate(TR_line, glm::radians(30.0f + x_rad), glm::vec3(1.0, 0.0, 0.0));
	TR_line = glm::rotate(TR_line, glm::radians(15.0f + y_rad), glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_line)); //--- modelTransform 변수에 변환 값 적용하기
	glUniform3fv(colorLocation, 1, quadricColor); // 예시 색상
	glBindVertexArray(line_vao);
	glDrawArrays(GL_LINES, 0, 6);



	glutSwapBuffers(); //--- 화면에 출력하기
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'y':
		y_y_rad += 5;
		break;
	case 'Y':
		y_y_rad -= 5;
		break;
	case 'x':
		x_rad += 1;
		break;
	case 'X':
		x_rad -= 1;
		break;
	case 'z':
		z_rad += 1;
		break;
	case 'Z':
		z_rad -= 1;
		break;
	case 'b':
		if (b_button) {
			b_button = false;
		}
		else {
			b_button = true;
			B_button = false;
		}
		break;
	case 'B':
		if (B_button) {
			B_button = false;
		}
		else {
			B_button = true;
			b_button = false;
		}
		break;
	case 'm':
		if (m_button) {
			m_button = false;
		}
		else {
			m_button = true;
			M_button = false;
		}
		break;
	case 'M':
		if (M_button) {
			M_button = false;
		}
		else {
			M_button = true;
			m_button = false;
		}
		break;
	case 'e':
		if (e_button) {
			e_button = false;
		}
		else {
			e_button = true;
			E_button = false;
		}
		break;
	case 'E':
		if (E_button) {
			E_button = false;
		}
		else {
			E_button = true;
			e_button = false;
		}
		break;
	case 't':
		if (t_button) {
			t_button = false;
		}
		else {
			t_button = true;
			T_button = false;
		}
		break;
	case 'T':
		if (T_button) {
			T_button = false;
		}
		else {
			T_button = true;
			t_button = false;
		}
		break;
	case 'f':
		if (f_button) {
			f_button = false;
		}
		else {
			f_button = true;
			F_button = false;
		}
		break;
	case 'F':
		if (F_button) {
			F_button = false;
		}
		else {
			F_button = true;
			f_button = false;
		}
		break;
	case 'r':
	case 'R':
		if (r_button) {
			r_button = false;
		}
		else {
			r_button = true;
		}
		break;
	case 'a':
	case 'A':
		y_rad += 10;
		break;
	case 's':
	case 'S':
		b_button = false;
		B_button = false;
		m_button = false;
		M_button = false;
		f_button = false;
		F_button = false;
		e_button = false;
		E_button = false;
		t_button = false;
		T_button = false;
		r_button = false;
		break;
	case 'c':
	case 'C':
		b_button = false;
		B_button = false;
		m_button = false;
		M_button = false;
		f_button = false;
		F_button = false;
		e_button = false;
		E_button = false;
		t_button = false;
		T_button = false;
		r_button = false;

		x_move = 0;
		y_rotate = 0;
		f_y_rotate = 0;
		main_arm_move = 0;
		sub_arm_rotate = 0;
		mul = 1;

		y_rad = 0;
		x_rad = 0;
		z_rad = 0;

		break;
	}

	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다

}

GLfloat line[6][3]{ {-1, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1} };
GLfloat line_color[6][3]{};

bool Set_VAO() {
	ReadObj("C:\\Users\\jhd\\Desktop\\mycube.obj");
	glGenVertexArrays(1, &line_vao);
	glBindVertexArray(line_vao);

	glGenBuffers(2, vbo);
	//버퍼 오브젝트를 바인드 한다.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//버퍼 오브젝트의 데이터를 생성
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);

	//위치 가져오기 함수
	GLint lineAttribute = glGetAttribLocation(shaderProgramID, "positionAttribute");
	//버퍼 오브젝트를 바인드 한다.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//버텍스 속성 데이터의 배열을 정의
	glVertexAttribPointer(lineAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//버텍스 속성 배열을 사용하도록 한다.
	glEnableVertexAttribArray(lineAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//버퍼 오브젝트의 데이터를 생성
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_color), line, GL_STATIC_DRAW);

	//위치 가져오기 함수
	GLint line_Attribute = glGetAttribLocation(shaderProgramID, "colorAttribute");
	//버퍼 오브젝트를 바인드 한다.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//버텍스 속성 데이터의 배열을 정의
	glVertexAttribPointer(line_Attribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//버텍스 속성 배열을 사용하도록 한다.
	glEnableVertexAttribArray(line_Attribute);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//버텍스 버퍼 오브젝트 (VBO) 이름 생성
	glGenBuffers(1, &vertexbuffer);
	//버퍼 오브젝트를 바인드 한다.
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//버퍼 오브젝트의 데이터를 생성
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	//엘리멘트 버퍼 오브젝트 (EBO) 이름 생성
	glGenBuffers(1, &EBO);
	//버퍼 오브젝트를 바인드 한다.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//버퍼 오브젝트의 데이터를 생성
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(unsigned int), &vertexIndices[0], GL_STATIC_DRAW);

	//위치 가져오기 함수
	GLint positionAttribute = glGetAttribLocation(shaderProgramID, "positionAttribute");
	if (positionAttribute == -1) {
		std::cerr << "position 속성 설정 실패" << std::endl;
		return false;
	}
	//버텍스 속성 데이터의 배열을 정의
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//버텍스 속성 배열을 사용하도록 한다.
	glEnableVertexAttribArray(positionAttribute);



	glGenBuffers(1, &colorbuffer);
	//버퍼 오브젝트를 바인드 한다.
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	//버퍼 오브젝트의 데이터를 생성
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

	//위치 가져오기 함수
	GLint colorAttribute = glGetAttribLocation(shaderProgramID, "colorAttribute");
	//버퍼 오브젝트를 바인드 한다.
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	//버텍스 속성 데이터의 배열을 정의
	glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//버텍스 속성 배열을 사용하도록 한다.
	glEnableVertexAttribArray(colorAttribute);
	return true;
}

//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

GLint result;
GLchar errorLog[512];

void make_vertexShaders()
{
	GLchar* vertexSource;
	vertexSource = filetobuf("예시vertex.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexShader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	GLchar* fragmentSource;
	fragmentSource = filetobuf("3Dfragment.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentShader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

GLvoid make_shaderProgram()
{
	GLuint shaderID;
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
	glUseProgram(shaderProgramID);

}



GLvoid InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao); //--- VAO를 바인드하기
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





	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
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
		cerr << path << "파일 못찾음";
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