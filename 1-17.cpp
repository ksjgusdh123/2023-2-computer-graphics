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
const int sp_num = 200;
float r[sp_num]{};

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

GLfloat rColor = 0.0, gColor = 1.0, bColor = 1.0;


bool square = true;

GLUquadricObj* qobj;


float x_movement = -0.8;
float y_movement = 0;
float z_movement = 0;
float scale_size = 0;

float qx_movement = 0.8;
float qy_movement = 0;
float qz_movement = 0;
float qscale_size = 0;

float rotation = 0;

bool select = true;
bool r_button = false;
bool t_button = false;
bool return_obj = false;
bool t_return_obj = false;

bool left_position = true;
bool q_check = false;

int state = 0;
int button = 0;
int sq_size = 0;

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	//--- 변경된 배경색 설정
	glClearColor(rColor, gColor, bColor, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderProgramID);

	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_LINE);
	glm::mat4 TR_iii = glm::mat4(1.0f);
	glm::mat4 TR = glm::mat4(1.0f);
	glm::mat4 TR_sp = glm::mat4(1.0f);
	glm::mat4 TR_line = glm::mat4(1.0f);

	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "transform"); //--- 버텍스 세이더에서모델 변환 위치 가져오기

	// xyz축
	TR_line = glm::rotate(TR_line, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
	TR_line = glm::rotate(TR_line, glm::radians(-30.0f), glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_line)); //--- modelTransform 변수에 변환 값 적용하기
	glBindVertexArray(line_vao);
	glDrawArrays(GL_LINES, 0, 6);
	if (t_button) {
		glBindVertexArray(sp_vao);
		glDrawArrays(GL_LINE_STRIP, 0, sp_num);
	}
	if (r_button) {
		if (left_position) {
			if (return_obj) {
				x_movement -= 0.1;
				qx_movement += 0.1;
				if (x_movement <= -0.8) {
					x_movement = -0.8;
					qx_movement = 0.8;
					return_obj = false;
					r_button = false;
				}
			}
			else {
				x_movement += 0.1;
				qx_movement -= 0.1;
				if (x_movement >= 0) {
					return_obj = true;
				}
			}
		}
		else {
			if (return_obj) {
				x_movement += 0.1;
				qx_movement -= 0.1;
				if (x_movement >= 0.8) {
					x_movement = 0.8;
					qx_movement = -0.8;
					return_obj = false;
					r_button = false;
				}
			}
			else {
				x_movement -= 0.1;
				qx_movement += 0.1;
				if (x_movement <= 0) {
					return_obj = true;
				}
			}
		}
	}
	else if (t_button) {
		if (!sq_size) {
			x_movement = 0;
			qx_movement = 0;
		}
		sq_size += 1;
		if (sq_size >= 200) {
			sq_size = 0;
			t_button = false;
			x_movement = -0.8;
			qx_movement = 0.8;
		}
	}

	if (button == 1) {
		rotation+=10.001;
		int rrr = (int)rotation;
		if(left_position){
			if (rrr % 180 == 0) {
				state = 0;
				button = 0;
				left_position = false;
				x_movement = 0.8;
				qx_movement = -0.8;

				rotation = 0;
			}
		}
		else {
			if (rrr % 180 == 0) {
				state = 0;
				button = 0;
				left_position = true;
				qx_movement = 0.8;
				x_movement = -0.8;
				rotation = 0;
			}
		}
	}
	else if (button == 2) {
		if (left_position) {
			if (state == 0) {
				x_movement += 0.1;
				y_movement += 0.1;
				qx_movement -= 0.1;
				qy_movement -= 0.1;
				if (x_movement >= 0) {
					x_movement = 0;
					y_movement = 0.8;
					qx_movement = 0;
					qy_movement = -0.8;
					state = 1;
				}
			}
			else {
				x_movement += 0.1;
				y_movement -= 0.1;
				qx_movement -= 0.1;
				qy_movement += 0.1;
				if (x_movement >= 0.8) {
					x_movement = 0.8;
					y_movement = 0;
					qx_movement = -0.8;
					qy_movement = 0;
					state = 0;
					left_position = false;
					button = 0;
				}
			}
			
		}
		else {
			if (state == 0) {
				x_movement -= 0.1;
				y_movement -= 0.1;
				qx_movement += 0.1;
				qy_movement += 0.1;
				if (x_movement <= 0) {
					state = 1; 
					x_movement = 0;
					y_movement = -0.8;
					qx_movement = 0.0;
					qy_movement = 0.8;
				}
			}
			else {
				x_movement -= 0.1;
				y_movement += 0.1;
				qx_movement += 0.1;
				qy_movement -= 0.1;
				if (x_movement <= -0.8) {
					state = 0;
					left_position = true;
					button = 0;
					x_movement = -0.8;
					y_movement = 0;
					qx_movement = 0.8;
					qy_movement = 0;
				}
			}
		}
	}
	else if (button == 3) {
		if (left_position) {
			x_movement += 0.1;
			qx_movement -= 0.1;
			if (x_movement >= 0.8) {
				x_movement = 0.8;
				qx_movement = -0.8;
				button = 0;
				left_position = false;
			}
		}
		else {
			x_movement -= 0.1;
			qx_movement += 0.1;
			if (x_movement <= -0.8) {
				x_movement = -0.8;
				qx_movement = 0.8;
				button = 0;
				left_position = true;
			}
		}
	}

	// 육면체
	TR = glm::rotate(TR, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
	TR = glm::rotate(TR, glm::radians(-30.0f), glm::vec3(0.0, 1.0, 0));
	if (t_button) {
		float mmm = cos(((10 * sq_size) % 360) * PI / 180) * r[sq_size];
		float mmm2 = sin(((10 * sq_size) % 360) * PI / 180) * r[sq_size];
		TR = glm::translate(TR, glm::vec3(mmm, 0, mmm2));
	}
	TR = glm::rotate(TR, glm::radians(rotation), glm::vec3(0.0, 1.0, 0));
	if (!q_check) {
		TR = glm::translate(TR, glm::vec3(x_movement, y_movement, z_movement));
		TR = glm::scale(TR, glm::vec3(0.3 + scale_size, 0.3 + scale_size, 0.3 + scale_size));
	}
	else {
		TR = glm::scale(TR, glm::vec3(0.3 + scale_size, 0.3 + scale_size, 0.3 + scale_size));
		TR = glm::translate(TR, glm::vec3(x_movement, y_movement, z_movement));
	}
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform 변수에 변환 값 적용하기
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	// quadric

	TR_iii = glm::rotate(TR_iii, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
	TR_iii = glm::rotate(TR_iii, glm::radians(-30.0f), glm::vec3(0.0, 1.0, 0.0));
	if (t_button) {
		float mmm = cos(((10 * sq_size) % 360) * PI / 180) * r[sq_size];
		float mmm2 = sin(((10 * sq_size) % 360) * PI / 180) * r[sq_size];
		TR_iii = glm::translate(TR_iii, glm::vec3(mmm, 0, mmm2));
	}
	TR_iii = glm::rotate(TR_iii, glm::radians(rotation), glm::vec3(0.0, 1.0, 0));
	if (!q_check) {
		TR_iii = glm::translate(TR_iii, glm::vec3(qx_movement, qy_movement, qz_movement));
		TR_iii = glm::scale(TR_iii, glm::vec3(0.3 + qscale_size, 0.3 + qscale_size, 0.3 + qscale_size));
	}
	else {
		TR_iii = glm::scale(TR_iii, glm::vec3(0.3 + qscale_size, 0.3 + qscale_size, 0.3 + qscale_size));
		TR_iii = glm::translate(TR_iii, glm::vec3(qx_movement, qy_movement, qz_movement));
	}
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR_iii)); //--- modelTransform 변수에 변환 값 적용하기
	gluSphere(qobj, 0.3, 10, 10);


	glutSwapBuffers(); //--- 화면에 출력하기
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'c':
		if (select) {
			select = false;
		}
		else {
			select = true;
		}
		break;
	case 'x':
		if(select)
			x_movement += 0.01;
		else
			qx_movement += 0.01;
		break;
	case 'X':
		if (select)
			x_movement -= 0.01;
		else
			qx_movement -= 0.01;
		break;
	case 'y':
		if (select)
			y_movement += 0.01;
		else
			qy_movement += 0.01;
		break;
	case 'Y':
		if (select)
			y_movement -= 0.01;
		else
			qy_movement -= 0.01;
		break;
	case 'z':
		if (select)
			z_movement += 0.01;
		else
			qz_movement += 0.01;
		1;
		break;
	case 'Z':
		if (select)
		z_movement -= 0.01;
			else
		qz_movement -= 0.01;
		break;
	case 's':
		if (select) {
			scale_size += 0.01;
			q_check = false;
		}
		else {
			qscale_size += 0.01;
			q_check = false;
		}
		break;
	case 'S':
		if (select) {
			scale_size -= 0.01;
			q_check = false;
		}
		else {
			qscale_size -= 0.01;
			q_check = false;
		}
		break;
	case 'q':
		if (select) {
			scale_size += 0.01;
			q_check = true;
		}
		else {
			qscale_size += 0.01;
			q_check = true;
		}
		break;
	case 'Q':
		if (select) {
			scale_size -= 0.01;
			q_check = true;
		}
		else {
			qscale_size -= 0.01;
			q_check = true;
		}
		break;
	case 'r':
		r_button = true;
		t_button = false;
		button = 0;
		break;
	case 't':
		r_button = false;
		t_button = true;
		button = 0;
		break;
	case '1':
		r_button = false;
		t_button = false;
		button = 1;
		break;
	case '2':
		r_button = false;
		t_button = false;
		button = 2;
		break;
	case '3':
		r_button = false;
		t_button = false;
		button = 3;
		break;
	}

	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다

}

GLfloat line[6][3]{ {-1, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1} };
GLfloat line_color[6][3]{};

GLfloat shape[sp_num][3]{};
GLfloat shape_c[sp_num][3]{};
int dot_num = 0;

GLvoid make_sp() {
	while (dot_num < sp_num) {
		r[dot_num] += ((dot_num + 1)) * 0.005;
		shape[dot_num][0] = cos(((10 * dot_num) % 360) * PI / 180) * r[dot_num];
		shape[dot_num][2] = sin(((10 * dot_num) % 360) * PI / 180) * r[dot_num];
		dot_num++;
	}
}

bool Set_VAO() {
	make_sp();
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

	glGenVertexArrays(1, &sp_vao);
	glBindVertexArray(sp_vao);

	glGenBuffers(2, spvbo);
	//버퍼 오브젝트를 바인드 한다.
	glBindBuffer(GL_ARRAY_BUFFER, spvbo[0]);
	//버퍼 오브젝트의 데이터를 생성
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape), shape, GL_STATIC_DRAW);
	
	//위치 가져오기 함수
	GLint spAttribute = glGetAttribLocation(shaderProgramID, "positionAttribute");
	glBindBuffer(GL_ARRAY_BUFFER, spvbo[0]);

	//버퍼 오브젝트를 바인드 한다.
	glVertexAttribPointer(spAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//버텍스 속성 배열을 사용하도록 한다.
	glEnableVertexAttribArray(spAttribute);
	
	glBindBuffer(GL_ARRAY_BUFFER, spvbo[1]);
	//버퍼 오브젝트의 데이터를 생성
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape_c), shape_c, GL_STATIC_DRAW);
	
	//위치 가져오기 함수
	GLint sp_Attribute = glGetAttribLocation(shaderProgramID, "colorAttribute");
	glBindBuffer(GL_ARRAY_BUFFER, spvbo[1]);

	//버퍼 오브젝트를 바인드 한다.
	glBindBuffer(GL_ARRAY_BUFFER, spvbo[1]);
	//버텍스 속성 데이터의 배열을 정의
	glVertexAttribPointer(sp_Attribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//버텍스 속성 배열을 사용하도록 한다.
	glEnableVertexAttribArray(sp_Attribute);


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

			if (square) {
				for (int i = 0; i < 3; i++)
				{
					in >> vertexIndex[i];// >> a >> uvIndex[i] >> a >> normalIndex[i];
					vertexIndices.push_back(vertexIndex[i] - 1);
					//	uvIndices.push_back(uvIndex[i] - 1);
						//normalIndices.push_back(normalIndex[i] - 1);
				}
			}
			else {
				for (int i = 0; i < 3; i++)
				{
					in >> vertexIndex[i];
					vertexIndices.push_back(vertexIndex[i] - 1);
				}
			}
		}
	}

	return true;
}