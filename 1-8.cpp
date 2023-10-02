#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <gl/glew.h> // 필요한 헤더파일 include
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
GLvoid Mouse_Click(int button, int state, int x, int y);
GLvoid InitBuffer();
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Make_Tri();
GLvoid change_Tri(int i);
GLvoid Re_Buffer();

//--- 필요한 변수 선언
GLint width, height;
GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체
GLuint vbo[2], vao;

std::random_device rd;
std::mt19937 dre(rd());
std::uniform_real_distribution<float> uid{ 0.1, 0.4 };

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
	glutKeyboardFunc(Keyboard);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

bool p_check = true;

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	GLfloat rColor = 1.0, gColor = 1.0, bColor = 1.0;
	//--- 변경된 배경색 설정
	glClearColor(rColor, gColor, bColor, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderProgramID);
	//--- 사용할 VAO 불러오기
	glBindVertexArray(vao);
	//--- 삼각형 그리기
	glDrawArrays(GL_LINES, 0, 4);
	if(p_check)
		glDrawArrays(GL_TRIANGLES, 4, 12);
	else
		glDrawArrays(GL_LINES, 16, 24);


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

GLvoid Mouse_Click(int button, int state, int x, int y) {
	ConvertXY_OPENGL(x, y);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (ox > 0 && oy > 0) {
			change_Tri(0);
		}
		else if (ox < 0 && oy > 0) {
			change_Tri(1);
		}
		else if (ox > 0 && oy < 0) {
			change_Tri(2);
		}
		else if (ox < 0 && oy < 0) {
			change_Tri(3);
		}
		Re_Buffer();
	}
}



GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
	case 'A':
		p_check = true;
		break;
	case 'b':
	case 'B': 
		p_check = false;
		break;
	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}


GLvoid InitBuffer()
{
	Make_Tri();
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

GLvoid change_Tri(int i) {
	float rand_x, rand_y;
	switch (i) {
	case 0:
		shape[7][0] = ox;
		shape[7][1] = oy;
		rand_x = uid(dre);
		rand_y = uid(dre);
		shape[8][0] = ox - rand_x;
		shape[8][1] = oy - rand_y; 
		shape[9][0] = ox + rand_x;
		shape[9][1] = oy - rand_y;
		break;
	case 1:
		shape[4][0] = ox;
		shape[4][1] = oy;
		rand_x = uid(dre);
		rand_y = uid(dre);
		shape[5][0] = ox - rand_x;
		shape[5][1] = oy - rand_y;
		shape[6][0] = ox + rand_x;
		shape[6][1] = oy - rand_y;
		break;
	case 2:
		shape[13][0] = ox;
		shape[13][1] = oy;
		rand_x = uid(dre);
		rand_y = uid(dre);
		shape[14][0] = ox - rand_x;
		shape[14][1] = oy - rand_y;
		shape[15][0] = ox + rand_x;
		shape[15][1] = oy - rand_y;
		break;
	case 3:
		shape[10][0] = ox;
		shape[10][1] = oy;
		rand_x = uid(dre);
		rand_y = uid(dre);
		shape[11][0] = ox - rand_x;
		shape[11][1] = oy - rand_y;
		shape[12][0] = ox + rand_x;
		shape[12][1] = oy - rand_y;
		break;
	}
	for (int i = 16; i < 40; i += 6) {
		if (i < 22) {
			shape[i][0] = shape[4][0];
			shape[i][1] = shape[4][1];
			shape[i][2] = shape[4][2];
			shape[i + 1][0] = shape[5][0];
			shape[i + 1][1] = shape[5][1];
			shape[i + 1][2] = shape[5][2];
			shape[i + 2][0] = shape[5][0];
			shape[i + 2][1] = shape[5][1];
			shape[i + 2][2] = shape[5][2];
			shape[i + 3][0] = shape[6][0];
			shape[i + 3][1] = shape[6][1];
			shape[i + 3][2] = shape[6][2];
			shape[i + 4][0] = shape[6][0];
			shape[i + 4][1] = shape[6][1];
			shape[i + 4][2] = shape[6][2];
			shape[i + 5][0] = shape[4][0];
			shape[i + 5][1] = shape[4][1];
			shape[i + 5][2] = shape[4][2];
		}
		else if (i < 28) {
			shape[i][0] = shape[7][0];
			shape[i][1] = shape[7][1];
			shape[i][2] = shape[7][2];
			shape[i + 1][0] = shape[8][0];
			shape[i + 1][1] = shape[8][1];
			shape[i + 1][2] = shape[8][2];
			shape[i + 2][0] = shape[8][0];
			shape[i + 2][1] = shape[8][1];
			shape[i + 2][2] = shape[8][2];
			shape[i + 3][0] = shape[9][0];
			shape[i + 3][1] = shape[9][1];
			shape[i + 3][2] = shape[9][2];
			shape[i + 4][0] = shape[9][0];
			shape[i + 4][1] = shape[9][1];
			shape[i + 4][2] = shape[9][2];
			shape[i + 5][0] = shape[7][0];
			shape[i + 5][1] = shape[7][1];
			shape[i + 5][2] = shape[7][2];
		}
		else if (i < 34) {
			shape[i][0] = shape[10][0];
			shape[i][1] = shape[10][1];
			shape[i][2] = shape[10][2];
			shape[i + 1][0] = shape[11][0];
			shape[i + 1][1] = shape[11][1];
			shape[i + 1][2] = shape[11][2];
			shape[i + 2][0] = shape[11][0];
			shape[i + 2][1] = shape[11][1];
			shape[i + 2][2] = shape[11][2];
			shape[i + 3][0] = shape[12][0];
			shape[i + 3][1] = shape[12][1];
			shape[i + 3][2] = shape[12][2];
			shape[i + 4][0] = shape[12][0];
			shape[i + 4][1] = shape[12][1];
			shape[i + 4][2] = shape[12][2];
			shape[i + 5][0] = shape[10][0];
			shape[i + 5][1] = shape[10][1];
			shape[i + 5][2] = shape[10][2];
		}
		else {
			shape[i][0] = shape[13][0];
			shape[i][1] = shape[13][1];
			shape[i][2] = shape[13][2];
			shape[i + 1][0] = shape[14][0];
			shape[i + 1][1] = shape[14][1];
			shape[i + 1][2] = shape[14][2];
			shape[i + 2][0] = shape[14][0];
			shape[i + 2][1] = shape[14][1];
			shape[i + 2][2] = shape[14][2];
			shape[i + 3][0] = shape[15][0];
			shape[i + 3][1] = shape[15][1];
			shape[i + 3][2] = shape[15][2];
			shape[i + 4][0] = shape[15][0];
			shape[i + 4][1] = shape[15][1];
			shape[i + 4][2] = shape[15][2];
			shape[i + 5][0] = shape[13][0];
			shape[i + 5][1] = shape[13][1];
			shape[i + 5][2] = shape[13][2];
		}
	}
}

GLvoid Make_Tri() {
	bool third = false;
	for (int i = 4; i < 16; ++i) {
		if (i == 4) {
			shape[i][0] = -0.5;
			shape[i][1] = 0.75;
			shape[i][2] = 0;
			colors[i][0] = 1;
			colors[i][1] = 0;
			colors[i][2] = 0;
			third = true;
		}
		else if (i == 7) {
			shape[i][0] = 0.5;
			shape[i][1] = 0.75;
			shape[i][2] = 0;

			colors[i][0] = 0;
			colors[i][1] = 1;
			colors[i][2] = 0;
			third = true;
		}
		else if (i == 10) {
			shape[i][0] = -0.5;
			shape[i][1] = -0.25;
			shape[i][2] = 0;

			colors[i][0] = 0;
			colors[i][1] = 0;
			colors[i][2] = 1;
			third = true;
		}
		else if (i == 13) {
			shape[i][0] = 0.5;
			shape[i][1] = -0.25;
			shape[i][2] = 0;

			colors[i][0] = 1;
			colors[i][1] = 1;
			colors[i][2] = 0;
			third = true;
		}
		else {
			if (third) {
				shape[i][0] = shape[i - 1][0] - 0.25;
				shape[i][1] = shape[i - 1][1] - 0.5;
				shape[i][2] = 0;

				colors[i][0] = colors[i - 1][0];
				colors[i][1] = colors[i - 1][1];
				colors[i][2] = colors[i - 1][2];
				third = false;
			}
			else {
				shape[i][0] = shape[i - 2][0] + 0.25;
				shape[i][1] = shape[i - 2][1] - 0.5;
				shape[i][2] = 0;

				colors[i][0] = colors[i - 1][0];
				colors[i][1] = colors[i - 1][1];
				colors[i][2] = colors[i - 1][2];
			}
		}
	}

	for (int i = 16; i < 40; i += 6) {
		if (i < 22) {
			shape[i][0] = shape[4][0];
			shape[i][1] = shape[4][1];
			shape[i][2] = shape[4][2];
			shape[i + 1][0] = shape[5][0];
			shape[i + 1][1] = shape[5][1];
			shape[i + 1][2] = shape[5][2];
			shape[i + 2][0] = shape[5][0];
			shape[i + 2][1] = shape[5][1];
			shape[i + 2][2] = shape[5][2];
			shape[i + 3][0] = shape[6][0];
			shape[i + 3][1] = shape[6][1];
			shape[i + 3][2] = shape[6][2];
			shape[i + 4][0] = shape[6][0];
			shape[i + 4][1] = shape[6][1];
			shape[i + 4][2] = shape[6][2];
			shape[i + 5][0] = shape[4][0];
			shape[i + 5][1] = shape[4][1];
			shape[i + 5][2] = shape[4][2];
			for (int j = 16; j < 16 + 6; ++j) {
				colors[j][0] = colors[4][0];
				colors[j][1] = colors[4][1];
				colors[j][2] = colors[4][2];
			}
		}
		else if (i < 28) {
			shape[i][0] = shape[7][0];
			shape[i][1] = shape[7][1];
			shape[i][2] = shape[7][2];
			shape[i + 1][0] = shape[8][0];
			shape[i + 1][1] = shape[8][1];
			shape[i + 1][2] = shape[8][2];
			shape[i + 2][0] = shape[8][0];
			shape[i + 2][1] = shape[8][1];
			shape[i + 2][2] = shape[8][2];
			shape[i + 3][0] = shape[9][0];
			shape[i + 3][1] = shape[9][1];
			shape[i + 3][2] = shape[9][2];
			shape[i + 4][0] = shape[9][0];
			shape[i + 4][1] = shape[9][1];
			shape[i + 4][2] = shape[9][2];
			shape[i + 5][0] = shape[7][0];
			shape[i + 5][1] = shape[7][1];
			shape[i + 5][2] = shape[7][2];
			for (int j = 22; j < 22 + 6; ++j) {
				colors[j][0] = colors[7][0];
				colors[j][1] = colors[7][1];
				colors[j][2] = colors[7][2];
			}
		}
		else if (i < 34) {
			shape[i][0] = shape[10][0];
			shape[i][1] = shape[10][1];
			shape[i][2] = shape[10][2];
			shape[i + 1][0] = shape[11][0];
			shape[i + 1][1] = shape[11][1];
			shape[i + 1][2] = shape[11][2];
			shape[i + 2][0] = shape[11][0];
			shape[i + 2][1] = shape[11][1];
			shape[i + 2][2] = shape[11][2];
			shape[i + 3][0] = shape[12][0];
			shape[i + 3][1] = shape[12][1];
			shape[i + 3][2] = shape[12][2];
			shape[i + 4][0] = shape[12][0];
			shape[i + 4][1] = shape[12][1];
			shape[i + 4][2] = shape[12][2];
			shape[i + 5][0] = shape[10][0];
			shape[i + 5][1] = shape[10][1];
			shape[i + 5][2] = shape[10][2];
			for (int j = 28; j < 28 + 6; ++j) {
				colors[j][0] = colors[10][0];
				colors[j][1] = colors[10][1];
				colors[j][2] = colors[10][2];
			}

		}
		else {
			shape[i][0] = shape[13][0];
			shape[i][1] = shape[13][1];
			shape[i][2] = shape[13][2];
			shape[i + 1][0] = shape[14][0];
			shape[i + 1][1] = shape[14][1];
			shape[i + 1][2] = shape[14][2];
			shape[i + 2][0] = shape[14][0];
			shape[i + 2][1] = shape[14][1];
			shape[i + 2][2] = shape[14][2];
			shape[i + 3][0] = shape[15][0];
			shape[i + 3][1] = shape[15][1];
			shape[i + 3][2] = shape[15][2];
			shape[i + 4][0] = shape[15][0];
			shape[i + 4][1] = shape[15][1];
			shape[i + 4][2] = shape[15][2];
			shape[i + 5][0] = shape[13][0];
			shape[i + 5][1] = shape[13][1];
			shape[i + 5][2] = shape[13][2];
			for (int j = 34; j < 34 + 6; ++j) {
				colors[j][0] = colors[13][0];
				colors[j][1] = colors[13][1];
				colors[j][2] = colors[13][2];
			}
		}
	}
}