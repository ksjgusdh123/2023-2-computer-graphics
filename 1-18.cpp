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
bool re_VAO();
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

GLfloat rColor = 0.0, gColor = 1.0, bColor = 1.0;



GLUquadricObj* qobj;

bool square = true;
bool p_button = true;

bool h_button = false;
bool y_button = false;
bool t_button = false;
bool t_check = false;
bool f_button = false;
bool s_button = false;
bool s_check = false;
bool b_button = false;
bool b_check = false;

float b_scale = 1.0;
float s_trans = 0.0;
float f_rotate = 0.0;
float t_rotate = 0.0;
float y_rotate = 30;

bool o_button = false;
bool r_button = false;
bool check = false;

float o1_rotate = 0;
float o2_rotate = 0;
float o3_rotate = 0;
float o4_rotate = 0;
float o_rotate = 0;

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	//--- 변경된 배경색 설정
	glClearColor(rColor, gColor, bColor, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (h_button)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderProgramID);

	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_LINE);
	glm::mat4 projection = glm::mat4(1.0f);
	if(p_button)
		projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f);
	else {
		projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f); //--- 투영 공간 설정: fovy, aspect, near, far
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -5.0));
	}
	unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform"); //--- 투영 변환 값 설정
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	glm::mat4 TR_line = glm::mat4(1.0f);
	glm::mat4 TR = glm::mat4(1.0f);
	glm::mat4 TR_t = glm::mat4(1.0f);
	glm::mat4 TR_f = glm::mat4(1.0f);
	glm::mat4 TR_b = glm::mat4(1.0f);
	glm::mat4 TR_s = glm::mat4(1.0f);
	glm::mat4 TR_o1 = glm::mat4(1.0f);
	glm::mat4 TR_o2 = glm::mat4(1.0f);
	glm::mat4 TR_o3 = glm::mat4(1.0f);
	glm::mat4 TR_o4 = glm::mat4(1.0f);



	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "transform"); //--- 버텍스 세이더에서모델 변환 위치 가져오기
	if(y_button)
		y_rotate += 10;
	if (t_button) {
		if (t_check == false && t_rotate < 90) {
			t_rotate += 10;
		}
		else if (t_check == false && t_rotate >= 90) {
			t_check = true;
		}
		else if (t_check && t_rotate > 0) {
			t_rotate -= 10;
		}
		else if (t_check && t_rotate <= 0) {
			t_check = false;
		}
	}
	if (f_button) {
		f_rotate += 10;
	}
	if(b_button){
		if (b_scale < 2 && b_check == false) {
			b_scale += 0.1;
		}
		else if (b_check) {
			if (b_scale > 0.5) {
				b_scale -= 0.1;
			}
			else {
				b_check = false;
			}
		}
		else if(b_scale > 2){
			b_check = true;
		}
	}
	if (s_button) {
		if (s_check == false && s_trans <= 0.5) {
			s_trans += 0.1;
		}
		else if (s_check) {
			if (s_trans >= -0.5) {
				s_trans -= 0.1;
			}
			else {
				s_check = false;
			}
		}
		else if (s_trans > 0.5) {
			s_check = true;
		}
	}

	// xyz축
	TR_line = glm::rotate(TR_line, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
	TR_line = glm::rotate(TR_line, glm::radians(30.0f), glm::vec3(0.0, 1.0, 0.0));
	TR = glm::rotate(TR, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
	TR = glm::rotate(TR, glm::radians(y_rotate), glm::vec3(0.0, 1.0, 0.0));

	if (square) {
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform 변수에 변환 값 적용하기
		glBindVertexArray(vao);
		TR_b = glm::rotate(TR_b, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
		TR_b = glm::rotate(TR_b, glm::radians(y_rotate), glm::vec3(0.0, 1.0, 0.0));
		TR_b = glm::translate(TR_b, glm::vec3(0, 0.0, -0.25));
		TR_b = glm::scale(TR_b, glm::vec3(b_scale, b_scale, b_scale));
		TR_b = glm::translate(TR_b, glm::vec3(0, 0, 0.25));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_b)); //--- modelTransform 변수에 변환 값 적용하기
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

		TR_f = glm::rotate(TR_f, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
		TR_f = glm::rotate(TR_f, glm::radians(y_rotate), glm::vec3(0.0, 1.0, 0.0));
		TR_f = glm::translate(TR_f, glm::vec3(0, 0.25, 0.0));
		TR_f = glm::rotate(TR_f, glm::radians(f_rotate), glm::vec3(1.0, 0.0, 0.0));
		TR_f = glm::translate(TR_f, glm::vec3(0, -0.25, 0.0));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_f)); //--- modelTransform 변수에 변환 값 적용하기
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)48);


		TR_s = glm::rotate(TR_s, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
		TR_s = glm::rotate(TR_s, glm::radians(y_rotate), glm::vec3(0.0, 1.0, 0.0));
		TR_s = glm::translate(TR_s, glm::vec3(0, s_trans, 0.0));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_s)); //--- modelTransform 변수에 변환 값 적용하기
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)24);  // 왼쪽
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)72);

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform 변수에 변환 값 적용하기
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)96);  // 밑면

		TR_t = glm::rotate(TR_t, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
		TR_t = glm::rotate(TR_t, glm::radians(y_rotate), glm::vec3(0.0, 1.0, 0.0));
		TR_t = glm::translate(TR_t, glm::vec3(0, -0.25, 0.25));
		TR_t = glm::rotate(TR_t, glm::radians(t_rotate), glm::vec3(1.0, 0.0, 0.0));
		TR_t = glm::translate(TR_t, glm::vec3(0, 0.25, -0.25));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_t)); //--- modelTransform 변수에 변환 값 적용하기
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)120);


	}
	else {
		if (o_button) {
			if (check == false && o4_rotate < 235) {
				o1_rotate += 5;
				o2_rotate += 5;
				o3_rotate += 5;
				o4_rotate += 5;
			}
			else if (o4_rotate >= 235 && check == false) {
				check = true;
			}
			else if (check && o4_rotate > 0) {
				o1_rotate -= 5;
				o2_rotate -= 5;
				o3_rotate -= 5;
				o4_rotate -= 5;
			}
			else if (check && o4_rotate <= 0) {
				check = false;
			}
		}
		else if (r_button) {
			if (check == false && o1_rotate < 120) {
				o1_rotate += 10;
			}
			else if(check == false && o2_rotate < 120) {
				o2_rotate += 10;
			}
			else if (check == false && o3_rotate < 120) {
				o3_rotate += 10;
			}
			else if (check == false && o4_rotate < 120) {
				o4_rotate += 10;
			}
			else if (check == false && o4_rotate >= 120) {
				check = true;
			}
			else if (check && o1_rotate > 0) {
				o1_rotate -= 10;
			}
			else if (check && o2_rotate > 0) {
				o2_rotate -= 10;
			}
			else if (check && o3_rotate > 0) {
				o3_rotate -= 10;
			}
			else if (check && o4_rotate > 0) {
				o4_rotate -= 10;
			}
			else if (check && o4_rotate <= 0) {
				check = false;
			}
		}
		glBindVertexArray(vao);   // 0: 앞면 12: 뒤옆 24: 뒷면 36: 앞옆
		TR_o1 = glm::rotate(TR_o1, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
		TR_o1 = glm::rotate(TR_o1, glm::radians(y_rotate), glm::vec3(0.0, 1.0, 0.0));
		TR_o1 = glm::translate(TR_o1, glm::vec3(0, -0.25, 0.25));
		TR_o1 = glm::rotate(TR_o1, glm::radians(o1_rotate), glm::vec3(1.0, 0.0, 0.0));
		TR_o1 = glm::translate(TR_o1, glm::vec3(0, 0.25, -0.25));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_o1));
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);

		TR_o2 = glm::rotate(TR_o2, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
		TR_o2 = glm::rotate(TR_o2, glm::radians(y_rotate), glm::vec3(0.0, 1.0, 0.0));
		TR_o2 = glm::translate(TR_o2, glm::vec3(0.25, -0.25, 0.0));
		TR_o2 = glm::rotate(TR_o2, glm::radians(-1 * o2_rotate), glm::vec3(0.0, 0.0, 1.0));
		TR_o2 = glm::translate(TR_o2, glm::vec3(-0.25, 0.25, 0.0));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_o2));
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)12);


		TR_o3 = glm::rotate(TR_o3, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
		TR_o3 = glm::rotate(TR_o3, glm::radians(y_rotate), glm::vec3(0.0, 1.0, 0.0));
		TR_o3 = glm::translate(TR_o3, glm::vec3(0.0, -0.25, -0.250));
		TR_o3 = glm::rotate(TR_o3, glm::radians(-1 * o3_rotate), glm::vec3(1.0, 0.0, 0.0));
		TR_o3 = glm::translate(TR_o3, glm::vec3(0.0, 0.25, 0.250));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_o3));
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)24);

		TR_o4 = glm::rotate(TR_o4, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
		TR_o4 = glm::rotate(TR_o4, glm::radians(y_rotate), glm::vec3(0.0, 1.0, 0.0));
		TR_o4 = glm::translate(TR_o4, glm::vec3(-0.25, -0.25, 0.0));
		TR_o4 = glm::rotate(TR_o4, glm::radians(o4_rotate), glm::vec3(0.0, 0.0, 1.0));
		TR_o4 = glm::translate(TR_o4, glm::vec3(0.25, 0.25, 0.0));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_o4));
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)36);

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)48);
	}
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_line)); //--- modelTransform 변수에 변환 값 적용하기
	glBindVertexArray(line_vao);
	glDrawArrays(GL_LINES, 0, 6);
	glutSwapBuffers(); //--- 화면에 출력하기
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'h':
		if (h_button)
			h_button = false;
		else
			h_button = true;
		break;
	case 'y':
		if (y_button)
			y_button = false;
		else
			y_button = true;
		break;
	case 't':
		if (t_button)
			t_button = false;
		else
			t_button = true;
		break;
	case 'f':
		if (f_button)
			f_button = false;
		else
			f_button = true;
		break; 
	case 's':
		if (s_button)
			s_button = false;
		else
			s_button = true;
		break;
	case 'b':
		if (b_button)
			b_button = false;
		else
			b_button = true;
		break;
	case 'p':
		if (p_button)
			p_button = false;
		else
			p_button = true;
		break;
	case 'c':
		if (square)
			square = false;
		else
			square = true;
		re_VAO();
		break;
	case 'o':
		if (o_button)
			o_button = false;
		else {
			o_button = true;
			r_button = false;
			o1_rotate = 0;
			o2_rotate = 0;
			o3_rotate = 0;
			o4_rotate = 0;
			check = false;
		}
		break;
	case 'r':
		if (r_button)
			r_button = false;
		else {
			r_button = true;
			o_button = false;
			o1_rotate = 0;
			o2_rotate = 0;
			o3_rotate = 0;
			o4_rotate = 0;
			check = false;
		}
		break;
	}

	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다

}

GLfloat line[6][3]{ {-1, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1} };
GLfloat line_color[6][3]{};

bool re_VAO() {
	square ? ReadObj("C:\\Users\\jhd\\Desktop\\mycube.obj")
		: ReadObj("C:\\Users\\jhd\\Desktop\\mypiramid.obj");
	glBindVertexArray(line_vao);

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

	glBindVertexArray(vao);
	//버텍스 버퍼 오브젝트 (VBO) 이름 생성
	//버퍼 오브젝트를 바인드 한다.
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//버퍼 오브젝트의 데이터를 생성
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	//엘리멘트 버퍼 오브젝트 (EBO) 이름 생성
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

bool Set_VAO() {
	square ? ReadObj("C:\\Users\\jhd\\Desktop\\mycube.obj")
		: ReadObj("C:\\Users\\jhd\\Desktop\\mypiramid.obj");
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
	vertexSource = filetobuf("3Dvertex.glsl");
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