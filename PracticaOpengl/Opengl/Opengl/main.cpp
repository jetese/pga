#include <cstdio>
#include <Windows.h>
#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/freeglut.h>
#include <GL\GL.h>
#include<vector>

GLuint vao;
GLuint vbo;
float vertices[] = { 0.0, 0.5, 0.0, 0.5, -0.5, 0.0, -0.5, -0.5, 0.0 };
float colores[] = { 0.0, 0.5, 0.0, 0.5, -0.5, 0.0, 0.0, 0.5, 0.5 };
float vertexcolor[] = { 0.0, 0.5, 0.0, 1.0, 0.0, 0.0, 0.5, -0.5, 0.0, 0.0, 1.0, 0.0, -0.5, -0.5, 0.0, 0.0, 0.0, 1.0 };
GLuint vbo2[2];
std::vector<std::string> lines;
int size;
const char** content;
GLuint vs,fs,program;


void init(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Practica 1");

	glewExperimental = GL_TRUE; GLenum err = glewInit(); if (GLEW_OK != err) { printf("Error: %s\n", glewGetErrorString(err)); exit(-1); }
	printf("This system supports OpenGL Version: %s\n", glGetString(GL_VERSION));
}
void initDataColorSimple() {
	glGenVertexArrays(1, &vao); 
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
}
void initDataDobleBuffer() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbo2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo2[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo2[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colores), colores, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
}
void initData() {
	glGenVertexArrays(1, &vao); glBindVertexArray(vao);
	glGenBuffers(1, &vbo); glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexcolor), vertexcolor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
	glBindVertexArray(0);
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(vao); glDrawArrays(GL_TRIANGLES, 0, 3); glBindVertexArray(0);
	glutSwapBuffers();
}
void reshape(int width, int height) { glViewport(0, 0, width, height); glutPostRedisplay(); }
void idle() { glutPostRedisplay(); }
void readFile(const char* filename) {
	FILE* f;
	fopen_s(&f, filename, "rt");
	if (!f) {
		printf("ERROR: Can't open filename: %s\n", filename);
	}
	char line[256];
	while (fgets(line, 256, f))
		lines.push_back(line); 
	fclose(f);
	size = (int)lines.size();
	content = new const char*[size];
	for (int i = 0; i < size; i++) {
		content[i] = lines[i].c_str();
	}
}
void checkCompilling(GLuint shader) {
	GLint isCompiled = 0;
	GLint length;
	GLchar* log;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (!isCompiled) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		log = new GLchar[length];
		glGetShaderInfoLog(shader, length, &length, log);
		printf("ERROR: Compile error: %s\n", log);
		delete log;
	}
}
void initShader() {
	vs = glCreateShader(GL_VERTEX_SHADER);
	readFile("../shaders/ShaderVertices.vs");
	glShaderSource(vs, size, content, NULL);
	glCompileShader(vs);
	delete[] content;
	lines.clear();
	checkCompilling(vs);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	readFile("../shaders/ShaderFragmentos.fsh");
	glShaderSource(fs, size, content, NULL);
	glCompileShader(fs);
	delete[] content;
	lines.clear();
	checkCompilling(fs);

	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glUseProgram(program);
}


int main(int argc, char *argv[]) {
	printf("Hola Mundo!\n");
	init(argc, argv);
	

	glClearColor(0.0, 0.0, 0.8, 0.0);
	initShader();
	initDataDobleBuffer();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMainLoop();
	
	

	getchar(); 
	return (0);


}
