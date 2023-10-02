#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <gl/glew.h> // 필요한 헤더파일 include
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

//--- 필요한 변수 선언
GLint width, height;
GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체
GLuint vbo[2], vao;

std::random_device rd;
std::mt19937 dre(rd());
std::uniform_real_distribution<float> uid{ 0.1, 1 };

//--- 메인 함수
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example1");
	//--- GLEW 초기화하기
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

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	//--- 변경된 배경색 설정
	glClearColor(rColor, gColor, bColor, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderProgramID);
	//--- 사용할 VAO 불러오기
	glBindVertexArray(vao);
	//--- 삼각형 그리기
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

	glutSwapBuffers(); //--- 화면에 출력하기
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
	fragmentSource = filetobuf("예시fragment.glsl");
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


	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
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
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}


GLvoid InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao); //--- VAO를 바인드하기
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기
	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.
	//--- triShape 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape), shape, GL_STATIC_DRAW);
	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);
	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- 변수 colors에서 버텍스 색상을 복사한다.
	//--- colors 배열의 사이즈: 9 *float 
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape), colors, GL_STATIC_DRAW);
	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	//--- attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);
}

GLvoid Re_Buffer() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.
	//--- triShape 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape), shape, GL_STATIC_DRAW);
	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);
	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- 변수 colors에서 버텍스 색상을 복사한다.
	//--- colors 배열의 사이즈: 9 *float 
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape), colors, GL_STATIC_DRAW);
	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	//--- attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);
}

