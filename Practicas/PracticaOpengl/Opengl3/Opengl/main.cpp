#include <cstdio>
#include <Windows.h>
#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/freeglut.h>
#include <GL\GL.h>
#include<vector>

GLuint vao;
GLuint vbo;
float vertices[] = {-1.0, 1.0, 0.0,1.0, 1.0, 0.0,-1.0, -1.0, 0.0, 1.0,-1.0,0.0 };
float colores[] = { 0.0, 0.5, 0.0, 0.5, -0.5, 0.0, 0.0, 0.5, 0.5 };
float vertexcolor[] = { 0.0, 0.5, 0.0, 1.0, 0.0, 0.0, 0.5, -0.5, 0.0, 0.0, 1.0, 0.0, -0.5, -0.5, 0.0, 0.0, 0.0, 1.0 };
float texcoord[] = {	0.0, 1.0,
						1.0, 1.0,
						0.0, 0.0,
						1.0, 0.0};
GLuint vbo2[2];
std::vector<std::string> lines;
int size;
const char** content;
GLuint vs,fs,program;
unsigned int utex;

//Inicialización de openGl y glew
void init(int argc, char *argv[]) {
	//Inicializamos el contexto de opengl, la version 3.3 y
	// utilizamos las opciones CORE
	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	//Creamos la ventana con un doble buffer de cuatro
	// canales, definimos el tamaño de la ventana y el lugar en el 
	// que se genera, con el nombre "plantilla"
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Practica 1");

	//Acivamos GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		printf("Error: %s\n", glewGetErrorString(err));
		exit(-1);
	}
	printf("This system supports OpenGL Version: %s\n", glGetString(GL_VERSION));

	glClearColor(0.0, 0.0, 0.8, 0.0);
}

//Inicialización del buffer de vertices
void initDataColorSimple() {
	//Generamos un vao
	glGenVertexArrays(1, &vao); 
	glBindVertexArray(vao);

	//Generamos un buffer que contiene los vertices
	glGenBuffers(1, &vbo);
	//Activamos el buffer y le asignamos los vertices
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices, GL_STATIC_DRAW);
	//Activamos el array de vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//Desactivamos el vao
	glBindVertexArray(0);
}

//Inicialización del buffer de vertices y color en dos buffer
void initDataDobleBuffer() {
	//Generamos el vao y lo activamos
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Generamos 2 vbos
	glGenBuffers(2, vbo2);
	//Activamos el primero y le asignamos la información del array de vertices
	glBindBuffer(GL_ARRAY_BUFFER, vbo2[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//Activamos el buffer de vertices y lo configuramos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Activamos el segundo vbo y le cargamos la información del array de coordenadas de textura
	glBindBuffer(GL_ARRAY_BUFFER, vbo2[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoord), texcoord, GL_STATIC_DRAW);
	//Activamos el buffer de coordenadas de textura para configurarlo
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	//Desactivamos el vao
	glBindVertexArray(0);
}

//Inicialización del buffer de vertices y de color con un solo buffer
void initData() {
	//Generamos un vao y lo activamos
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Generamos un vbo y lo activamos
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Le asignamos el array de vertices y color (todo en uno)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexcolor), vertexcolor, GL_STATIC_DRAW);
	//Activamos el atributo de vertices para configurarlo
	glEnableVertexAttribArray(0);
	//Le decimos desde donde empieza los vertices y donde acaba dentro del array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	//Activamos el otro atributo para configurarlo
	glEnableVertexAttribArray(1);
	//Le decimos donde empieza y acaba dentro del array
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
	glBindVertexArray(0);
}

//Función de pintado
void display() {
	//Limpiamos el framebuffer activo
	glClear(GL_COLOR_BUFFER_BIT);
	//Activamos el vao
	glBindVertexArray(vao);
	//Pintamos el vao con la configuración (triangulos concatenados)
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//Desactivamos el vao
	glBindVertexArray(0);
	//Camniamos el buffer de pintado
	glutSwapBuffers();
}

//Función para rescalar la pantalla con la ventana
void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	glutPostRedisplay();
}

//Función que llamamos en cada frame
void idle() {
	glutPostRedisplay();
}

//Función para leer los shaders
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

//Función que comprueba si los shaders se compilaron bien
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
//Función de inicialización de shaders
void initShader() {
	//Creamos el shader de vertices y leemos el archivo que lo contiene
	vs = glCreateShader(GL_VERTEX_SHADER);
	readFile("../shaders/ShaderVertices.vs");
	glShaderSource(vs, size, content, NULL);

	//Compilamos el shader, limpiamos los arrays y comprobamos si se ha compilado correctamente
	glCompileShader(vs);
	delete[] content;
	lines.clear();
	checkCompilling(vs);

	//Creamos el shader de fragmentos y leemos el archivo que lo contiene
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	readFile("../shaders/ShaderFragmentos.fsh");
	glShaderSource(fs, size, content, NULL);
	//Compilamos el shader de fragmentos, limpiamos los arrays y comprobamos si se ha compilado correctamente
	glCompileShader(fs);
	delete[] content;
	lines.clear();
	checkCompilling(fs);

	//Creamos el programa que contiene los shaders
	program = glCreateProgram();
	//Lincamos los shaders al programa
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	//Linkamos el programa a la ejecución y lo usamos
	glLinkProgram(program);
	glUseProgram(program);
}

//Leer archivos bmp para las texturas
unsigned char* readBMP2(const char* filename, int* width, int* height)
{
	FILE* f;
	fopen_s(&f, filename, "rb");

	if (!f)
	{
		printf("ERROR: Can't load filename: %s\n", filename);
	}

	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f);

	*width = *(int*)&info[18];
	*height = *(int*)&info[22];

	int row_padded = (*width * 3 + 3) & (~3);
	int size = *width * row_padded;
	unsigned char* data = new unsigned char[size];

	fread(data, sizeof(unsigned char), size, f);

	fclose(f);

	return data;
}

//Función que inicializa las texturas
void InitTextures(const char* filename, int id, GLenum texture)
{

	GLuint texture_id = id;

	int texture_width = 0;
	int texture_height = 0;

	//Generamos una textura y la activamos
	glGenTextures(1, &texture_id);
	glActiveTexture(texture);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	//Configuramos la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Cargamos la imagen en la textura creada
	unsigned char* texture_data = readBMP2(filename, &texture_width, &texture_height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_BGR, GL_UNSIGNED_BYTE, texture_data);

	//Subimos una uniform que contiene la textura
	glUniform1i(glGetUniformLocation(program, "tex" + id), texture_id);
}



//Función que sube el alpha como uniform
void InitUniforms()
{
	glUniform1f(glGetUniformLocation(program, "alpha"), 0.5);
}


int main(int argc, char *argv[]) {
	//Incicializamos OpenGl
	init(argc, argv);
	
	//Inicializamos los shader y los datos
	initShader();
	initDataDobleBuffer();

	//Incicializamos las texturas
	InitTextures("hola.bmp", 0, GL_TEXTURE0);
	InitTextures("tiles.bmp", 1, GL_TEXTURE1);
	//Subimos el alpha como uniform
	InitUniforms();
	
	//Llamamos a la función de pintado
	glutDisplayFunc(display);
	//LLamamos a la función de reescalar la pantalla
	glutReshapeFunc(reshape);
	//Llamamos al limpiado del buffer
	glutIdleFunc(idle);
	//LLamamos al loop principal
	glutMainLoop();
	
	

	//getchar(); 
	return (0);


}
