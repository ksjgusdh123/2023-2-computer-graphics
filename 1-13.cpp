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
GLvoid Mouse_Move(int x, int y);
GLvoid Timer_event(int value);
GLvoid only_line();
GLvoid draw_rec();

//--- 필요한 변수 선언
GLint width, height;
GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체
GLuint vbo[2], vao;
GLuint rec_vbo[2];


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
	only_line();
	draw_rec();
	
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
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다



}



GLvoid Timer_event(int value) {
	


	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
	glutTimerFunc(100, Timer_event, 4);
}



GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	
	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}


GLvoid InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao); //--- VAO를 바인드하기
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao); //--- VAO를 바인드하기
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_background), line_background, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_background), back_colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glGenBuffers(2, rec_vbo); //--- 2개의 VBO를 지정하고 할당하기
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
	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader의 'layout (location = 0)' 부분
	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color"); //	: 1


	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // VBO Bind
		glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		// PosLocation			- Location 번호
		// 3					- VerTex Size (x, y, z 속성의 Vec3이니 3) 
		// GL_FLOAT, GL_FALSE	- 자료형과 Normalize 여부
		// sizeof(float) * 3	- VerTex 마다의 공백 크기 (한 정점마다 메모리 간격)
		//			(0과 같음)	- 0 일 경우 자동으로 2번째 인자(3) x 3번째 인자(float)로 설정
		// 0					- 데이터 시작 offset (0이면 데이터 처음부터 시작)
	}
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // VBO Bind
		glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	}
	glDrawArrays(GL_LINES, 0, 4); // 설정대로 출력
}

GLvoid draw_rec() {
	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position"); //	: 0  Shader의 'layout (location = 0)' 부분
	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color"); //	: 1


	{
		glBindBuffer(GL_ARRAY_BUFFER, rec_vbo[0]); // VBO Bind
		glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		// PosLocation			- Location 번호
		// 3					- VerTex Size (x, y, z 속성의 Vec3이니 3) 
		// GL_FLOAT, GL_FALSE	- 자료형과 Normalize 여부
		// sizeof(float) * 3	- VerTex 마다의 공백 크기 (한 정점마다 메모리 간격)
		//			(0과 같음)	- 0 일 경우 자동으로 2번째 인자(3) x 3번째 인자(float)로 설정
		// 0					- 데이터 시작 offset (0이면 데이터 처음부터 시작)
	}
	{
		glBindBuffer(GL_ARRAY_BUFFER, rec_vbo[1]); // VBO Bind
		glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	}
	glDrawArrays(GL_LINE_STRIP, 0, 5); // 설정대로 출력
}