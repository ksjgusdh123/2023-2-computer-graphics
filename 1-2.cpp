#include <iostream>
#include <gl/glew.h> // 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <array>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
std::default_random_engine dre;
std::uniform_real_distribution<float> uid{ 0, 1 };
GLvoid Mouse_Click(int, int, int, int);
GLvoid ConvertXY_OPENGL(int x, int y);
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("Example1"); // 윈도우 생성	(윈도우 이름)
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";
	glutDisplayFunc(drawScene); // 출력 콜백함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 콜백함수 지정
	// 키보드 입력 콜백함수 지정
	// 타이머 지정
	glutMouseFunc(Mouse_Click);
	glutMainLoop(); // 이벤트 처리 시작
}

std::array<GLclampf, 5> R{ 1, 0, 0, 0, 1 };
std::array<GLclampf, 5> G{ 1, 1, 1, 0, 1 };
std::array<GLclampf, 5> B{ 1, 1, 0, 1, 0 };

std::array<GLclampf, 5> point_x{ 0, -1, -1, 0, 0 };
std::array<GLclampf, 5> point_y{ 0, 0, -1, 0, -1 };
std::array<GLclampf, 5> point_endx{ 0, 0, 0, 1, 1 };
std::array<GLclampf, 5> point_endy{ 0, 1, 0, 1, 0 };


int array_num = 0;
GLclampf ox;
GLclampf oy;


GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	//--- 변경된 배경색 설정
	glClearColor(R.at(array_num), G.at(array_num), B.at(array_num), 1); //--- 바탕색을 변경
	glClear(GL_COLOR_BUFFER_BIT); //--- 설정된 색으로 전체를 칠하기

	array_num++;


	for(int i = 0; i < R.size() - 1; ++i){
		glColor3f(R.at(array_num), G.at(array_num), B.at(array_num));
		glRectf(point_x.at(array_num), point_y.at(array_num), point_endx.at(array_num), point_endy.at(array_num));
		array_num++;
	}
	array_num = 0;
	glutSwapBuffers(); //--- 화면에 출력하기
}

GLvoid ConvertXY_OPENGL(int x, int y)
{
	int w = 800;
	int h = 600;

	ox = (float)(x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0));
	oy = -(float)(y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0));

}

int x_num = 0;

GLvoid Mouse_Click(int button, int state, int x, int y) 
{
	ConvertXY_OPENGL(x, y);

	for (int i = 1; i < R.size(); ++i) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			if ((GLclampf)ox >= (point_x.at(i)) && (GLclampf)ox <= (point_endx.at(i))
				&& (GLclampf)oy >= (point_y.at(i)) && (GLclampf)oy <= (point_endy.at(i))) {
				R.at(i) = uid(dre); G.at(i) = uid(dre); B.at(i) = uid(dre);
			}
			else {
				x_num++;
			}
		}
		else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
			if ((GLclampf)ox >= (point_x.at(i)) && (GLclampf)ox <= (point_endx.at(i))
				&& (GLclampf)oy >= (point_y.at(i)) && (GLclampf)oy <= (point_endy.at(i))) {
				if (i == 1) {
					if (point_x.at(i) < -0.7) {
						point_x.at(i) += 0.1;
						point_y.at(i) += 0.1;
						point_endx.at(i) -= 0.1;
						point_endy.at(i) -= 0.1;
						break;
					}
				}
				else if (i == 2) {
					if (point_x.at(i) < -0.7) {
						point_x.at(i) += 0.1;
						point_y.at(i) += 0.1;
						point_endx.at(i) -= 0.1;
						point_endy.at(i) -= 0.1;
						break;
					}
				}
				else if (i == 3) {
					if (point_endx.at(i) >  0.7) {
						point_x.at(i) += 0.1;
						point_y.at(i) += 0.1;
						point_endx.at(i) -= 0.1;
						point_endy.at(i) -= 0.1;
						break;
					}
				}
				else if (i == 4) {
					if (point_endx.at(i) > 0.7) {
						point_x.at(i) += 0.1;
						point_y.at(i) += 0.1;
						point_endx.at(i) -= 0.1;
						point_endy.at(i) -= 0.1;
						break;
					}
				}
			}
			else {
				if (i == 1) {
					if (point_x.at(i) > -1 && ox >= -1 && ox <= 0 && oy >= 0 && oy <= 1) {
						if (point_x.at(i) <= -0.8) {
							point_x.at(i) = -1;
						}
						else {
							point_x.at(i) -= 0.1;
						}
						if (point_y.at(i) <= 0.15) {
							point_y.at(i) = 0;
						}
						else {
							point_y.at(i) -= 0.1;
						}
						if (point_endx.at(i) >= -0.13) {
							point_endx.at(i) = 0;
						}
						else {
							point_endx.at(i) += 0.1;
						}
						if (point_endy.at(i) >= 0.8) {
							point_endy.at(i) = 1;
						}
						else {
							point_endy.at(i) += 0.1;
						}
						break;
					}
				}
				else if (i == 2) {
					if (point_x.at(i) > -1 && ox >= -1 && ox <= 0 && oy >= -1 && oy <= 0) {
						if (point_x.at(i) <= -0.8) {
							point_x.at(i) = -1;
						}
						else {
							point_x.at(i) -= 0.1;
						}
						if (point_y.at(i) <= -0.9) {
							point_y.at(i) = -1;
						}
						else {
							point_y.at(i) -= 0.1;
						}
						if (point_endx.at(i) >= -0.1) {
							point_endx.at(i) = 0;
						}
						else {
							point_endx.at(i) += 0.1;
						}
						if (point_endy.at(i) >= -0.1) {
							point_endy.at(i) = 0;
						}
						else {
							point_endy.at(i) += 0.1;
						}
						break;
					}
				}
				else if (i == 3) {
					if (point_x.at(i) > 0 && ox >= 0 && ox <= 1 && oy >= 0 && oy <= 1) {
						if (point_x.at(i) <= 0.1) {
							point_x.at(i) = 0;
						}
						else {
							point_x.at(i) -= 0.1;
						}
						if (point_y.at(i) <= 0.15) {
							point_y.at(i) = 0;
						}
						else {
							point_y.at(i) -= 0.1;
						}
						if (point_endx.at(i) >= 0.8) {
							point_endx.at(i) = 1;
						}
						else {
							point_endx.at(i) += 0.1;
						}
						if (point_endy.at(i) >= 0.8) {
							point_endy.at(i) = 1;
						}
						else {
							point_endy.at(i) += 0.1;
						}
						break;
					}
				}
				else if (i == 4) {
					if (point_x.at(i) > 0 && point_x.at(i) > 0 && ox >= 0 && ox <= 1 && oy >= -1 && oy <= 0) {
						if (point_x.at(i) <= 0.1) {
							point_x.at(i) = 0;
						}
						else {
							point_x.at(i) -= 0.1;
						}
						if (point_y.at(i) <= -0.9) {
							point_y.at(i) = -1;
						}
						else {
							point_y.at(i) -= 0.1;
						}
						if (point_endx.at(i) >= 0.8) {
							point_endx.at(i) = 1;
						}
						else {
							point_endx.at(i) += 0.1;
						}
						if (point_endy.at(i) >= -0.1) {
							point_endy.at(i) = 0;
						}
						else {
							point_endy.at(i) += 0.1;
						}
						break;
					}
				}
			}
		}
	}
	if (x_num == 4) {
		R.at(0) = uid(dre); G.at(0) = uid(dre); B.at(0) = uid(dre);
	}
	x_num = 0;
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다

}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}