#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#include <gl\glm\glm\glm.hpp>
#include <gl/glm/glm/ext.hpp>
#include <gl/glm/glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <random>
#include <fstream>
#include <iterator>

using namespace std;

const int WIN_X = 10, WIN_Y = 10;
const int WIN_W = 800, WIN_H = 800;

const glm::vec3 background_rgb = glm::vec3(1.0f, 1.0f, 1.0f);

bool isCulling = true;
bool isFill = true;
double xMove = 0.0, yMove = 0.0, zMove = 0.0;
float xRotateAni = 0.0f;
float yRotateAni = 0.0f;
int rotateKey = 0;

GLfloat mx = 0.0f;
GLfloat my = 0.0f;

int framebufferWidth, framebufferHeight;
GLuint triangleVertexArrayObject;
GLuint shaderProgramID;
GLuint trianglePositionVertexBufferObjectID, triangleColorVertexBufferObjectID;
GLuint trianglePositionElementBufferObject;

std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;

std::vector< glm::vec3 > vertices;
std::vector< glm::vec2 > uvs;
std::vector< glm::vec3 > normals;

bool isCube = true;

char* File_To_Buf(const char* file)
{
	ifstream in(file, ios_base::binary);

	if (!in) {
		cerr << file << "���� ��ã��";
		exit(1);
	}

	in.seekg(0, ios_base::end);
	long len = in.tellg();
	char* buf = new char[len + 1];
	in.seekg(0, ios_base::beg);

	int cnt = -1;
	while (in >> noskipws >> buf[++cnt]) {}
	buf[len] = 0;

	return buf;
}

//������Ʈ �ε��ϴ� �͸� �׽�Ʈ�ϰ� �������� �˾Ƽ� �ذ��غ���
bool  Load_Object(const char* path) {
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
		string lineHeader;
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
				in >> vertexIndex[i] >> a >> uvIndex[i] >> a >> normalIndex[i];
				vertexIndices.push_back(vertexIndex[i] - 1);
				uvIndices.push_back(uvIndex[i] - 1);
				normalIndices.push_back(normalIndex[i] - 1);
			}
		}
	}

	return true;
}

bool Make_Shader_Program() {
	//���̴� �ڵ� ���� �ҷ�����
	const GLchar* vertexShaderSource = File_To_Buf("3Dvertex.glsl");
	const GLchar* fragmentShaderSource = File_To_Buf("3Dfragment.glsl");

	//���̴� ��ü �����
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//���̴� ��ü�� ���̴� �ڵ� ���̱�
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//���̴� ��ü �������ϱ�
	glCompileShader(vertexShader);

	GLint result;
	GLchar errorLog[512];

	//���̴� ���� ��������
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader ������ ����\n" << errorLog << endl;
		return false;
	}

	//���̴� ��ü �����
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//���̴� ��ü�� ���̴� �ڵ� ���̱�
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//���̴� ��ü �������ϱ�
	glCompileShader(fragmentShader);
	//���̴� ���� ��������
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader ������ ����\n" << errorLog << endl;
		return false;
	}

	//���̴� ���α׷� ����
	shaderProgramID = glCreateProgram();
	//���̴� ���α׷��� ���̴� ��ü���� ���̱�
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	//���̴� ���α׷� ��ũ
	glLinkProgram(shaderProgramID);

	//���̴� ��ü �����ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//���α׷� ���� ��������
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderProgramID, 512, NULL, errorLog);
		cerr << "ERROR: shader program ���� ����\n" << errorLog << endl;
		return false;
	}
	//���̴� ���α׷� Ȱ��ȭ
	glUseProgram(shaderProgramID);

	return true;
}

bool Set_VAO() {
	//�ﰢ���� �����ϴ� vertex ������ - position�� color

	isCube ? Load_Object("C:\\Users\\jhd\\Desktop\\cube.obj") : Load_Object("Resources/piramid.obj");

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

	//���ؽ� �迭 ������Ʈ (VAO) �̸� ����
	glGenVertexArrays(1, &triangleVertexArrayObject);
	//VAO�� ���ε��Ѵ�.
	glBindVertexArray(triangleVertexArrayObject);

	//Vertex Buffer Object(VBO)�� �����Ͽ� vertex �����͸� �����Ѵ�.

	//���ؽ� ���� ������Ʈ (VBO) �̸� ����
	glGenBuffers(1, &trianglePositionVertexBufferObjectID);
	//���� ������Ʈ�� ���ε� �Ѵ�.
	glBindBuffer(GL_ARRAY_BUFFER, trianglePositionVertexBufferObjectID);
	//���� ������Ʈ�� �����͸� ����
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	//������Ʈ ���� ������Ʈ (EBO) �̸� ����
	glGenBuffers(1, &trianglePositionElementBufferObject);
	//���� ������Ʈ�� ���ε� �Ѵ�.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trianglePositionElementBufferObject);
	//���� ������Ʈ�� �����͸� ����
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(unsigned int), &vertexIndices[0], GL_STATIC_DRAW);

	//��ġ �������� �Լ�
	GLint positionAttribute = glGetAttribLocation(shaderProgramID, "positionAttribute");
	if (positionAttribute == -1) {
		cerr << "position �Ӽ� ���� ����" << endl;
		return false;
	}
	//���ؽ� �Ӽ� �������� �迭�� ����
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�.
	glEnableVertexAttribArray(positionAttribute);

	//Į�� ���� ������Ʈ (VBO) �̸� ����
	glGenBuffers(1, &triangleColorVertexBufferObjectID);
	//���� ������Ʈ�� ���ε� �Ѵ�.
	glBindBuffer(GL_ARRAY_BUFFER, triangleColorVertexBufferObjectID);
	//���� ������Ʈ�� �����͸� ����
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

	//��ġ �������� �Լ�
	GLint colorAttribute = glGetAttribLocation(shaderProgramID, "colorAttribute");
	if (colorAttribute == -1) {
		cerr << "color �Ӽ� ���� ����" << endl;
		return false;
	}
	//���� ������Ʈ�� ���ε� �Ѵ�.
	glBindBuffer(GL_ARRAY_BUFFER, triangleColorVertexBufferObjectID);
	//���ؽ� �Ӽ� �������� �迭�� ����
	glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�.
	glEnableVertexAttribArray(colorAttribute);


	glBindVertexArray(0);


	return true;
}

//--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
GLvoid drawScene()
{
	// �������� ����
	glClearColor(background_rgb.x, background_rgb.y, background_rgb.z, 1.0f);
	// ������ ������ ��ü�� ĥ�ϱ�
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	isCulling ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	isCulling ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
	isFill ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//������� ���̴� ���α׷� ����ϱ�
	glUseProgram(shaderProgramID);

	glm::mat4 TR = glm::mat4(1.0f);
	TR = glm::translate(TR, glm::vec3(xMove, yMove, zMove));
	TR = glm::rotate(TR, glm::radians(30.0f + xRotateAni), glm::vec3(1.0, 0.0, 0.0));
	TR = glm::rotate(TR, glm::radians(-30.0f + yRotateAni), glm::vec3(0.0, 1.0, 0.0));
	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "transform"); //--- ���ؽ� ���̴������� ��ȯ ��ġ ��������
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	// ����� VAO �ҷ�����
	glBindVertexArray(triangleVertexArrayObject);

	//�迭 �����ͷκ��� ������Ƽ�� ������ �ϱ�
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 12);
	// ȭ�鿡 ����ϱ�
	glutSwapBuffers();
}

//--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid TimerFunction1(int value)
{
	glutPostRedisplay(); // ȭ�� �� ���
	if (rotateKey == 1)
		xRotateAni += 0.5f;
	if (rotateKey == 2)
		xRotateAni -= 0.5f;
	if (rotateKey == 3)
		yRotateAni += 0.5f;
	if (rotateKey == 4)
		yRotateAni -= 0.5f;
	glutTimerFunc(100, TimerFunction1, 1);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c':
		isCube = true;
		Set_VAO();
		break;
	case 'p':
		isCube = false;
		Set_VAO();
		break;
	case 'h':
		isCulling = 1 - isCulling;
		break;
	case 'x':
		rotateKey = 1;
		glutTimerFunc(100, TimerFunction1, 1);
		break;
	case 'X':
		rotateKey = 2;
		glutTimerFunc(100, TimerFunction1, 1);
		break;
	case 'y':
		rotateKey = 3;
		glutTimerFunc(100, TimerFunction1, 1);
		break;
	case 'Y':
		rotateKey = 4;
		glutTimerFunc(100, TimerFunction1, 1);
		break;
	case 'w':
		isFill = false;
		break;
	case 'W':
		isFill = true;
		break;
	case 'j':
		xMove -= 0.1;
		break; 
	case 'l':
		xMove += 0.1;
		break;
	case 'i':
		yMove += 0.1;
		break;
	case 'k':
		yMove -= 0.1;
		break;
	case 'S':
		rotateKey = 0;
		xMove = 0.0, yMove = 0.0f, zMove = 0.0f;
		xRotateAni = 0.0f;
		yRotateAni = 0.0f;
		break;
	}
	glutPostRedisplay(); //--- ������ �ٲ𶧸��� ��� �ݹ��Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}


void Mouse(int button, int state, int x, int y)
{
	GLfloat half_w = WIN_W / 2.0f;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mx = (x - half_w) / half_w;
		my = (half_w - y) / half_w;
	}
	Set_VAO();
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	//������ ����
	glutInit(&argc, argv);																// glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);				// ���÷��� ��� ����
	glutInitWindowPosition(WIN_X, WIN_Y);								// �������� ��ġ ����
	glutInitWindowSize(WIN_W, WIN_H);									// �������� ũ�� ����
	glutCreateWindow("Example1");												// ������ ����(������ �̸�)

	//GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	if (!Make_Shader_Program()) {
		cerr << "Error: Shader Program ���� ����" << endl;
		std::exit(EXIT_FAILURE);
	}

	if (!Set_VAO()) {
		cerr << "Error: VAO ���� ����" << endl;
		std::exit(EXIT_FAILURE);
	}

	glutDisplayFunc(drawScene);													// ��� �Լ��� ����
	glutReshapeFunc(Reshape);														// �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMainLoop();																			// �̺�Ʈ ó�� ����
}