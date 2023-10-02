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
GLvoid InitBuffer();
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse_Click(int button, int state, int x, int y);
GLvoid ConvertXY_OPENGL(int x, int y);
GLvoid Timer_event(int value);
GLvoid re_buffer();

//--- 필요한 변수 선언
GLint width, height;
GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체
GLuint vao, vbo[2];

GLfloat point[60][3]{};

GLfloat point_color[60][3]{};

int point_arr[10]{};

int point_num = 0;

int shape_num = 0;

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
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse_Click);
	glutTimerFunc(100, Timer_event, 1);// 타이머 지정
	glutMainLoop();
}
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

	glutSwapBuffers(); //--- 화면에 출력하기
}

//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
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
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
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
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
	glutTimerFunc(100, Timer_event, 4);
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



GLvoid InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao); //--- VAO를 바인드하기
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기
	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	////--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.
	////--- triShape 배열의 사이즈: 9 * float
	//glBufferData(GL_ARRAY_BUFFER, sizeof(triShape), triShape, GL_STATIC_DRAW);
	////--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	////--- attribute 인덱스 0번을 사용가능하게 함
	//glEnableVertexAttribArray(0);
	////--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	////--- 변수 colors에서 버텍스 색상을 복사한다.
	////--- colors 배열의 사이즈: 9 *float 
	//glBufferData(GL_ARRAY_BUFFER, sizeof(triShape), colors, GL_STATIC_DRAW);
	////--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float 
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	////--- attribute 인덱스 1번을 사용 가능하게 함.
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

