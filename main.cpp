#include "GL/freeglut.h"
#include <math.h>
#include <stdio.h>

// kut rotacije za smjer kamere
float angle = 0.0;
float angley = 0.0;
// trenutni smjer kamere
float lx = 0.0f, lz = -1.0f;
float ly = 0.0f;
// XZY pozicije kamere
float x = 0.0f, z = 5.0f, y=2.5f;
//flag vrata
int doorNumber;
bool doorUlazna = false;
bool doorKuhinja = false;
bool doorDnevna = false;
bool doorSobaDolje = false;
bool doorKupatilo = false;
bool doorGym = false;
bool doorKupatiloGore = false;
bool doorSoba1 = false;
bool doorSoba2 = false;
bool doorRadnaSoba = false;

GLuint texture;

int CheckDoor() {
	if (x<4.7f && x>3.55f && y > 1.05f && y < 3.05f && z<2.0f && z>-2.0f && lx<0.5f && lx>-0.5f) {
		return 1;
	}
	else if (x > 1.25f && x<5.25f && y>1.05f && y < 3.05f && z<-2.5f && z>-3.2f && lz<0.5f && lz>-0.5f) {
		return 2;
	}
	else if (x > 3.75f && x<4.5f && y>1.05f && y<3.05f && z>-6.7f && z < -2.4f && lx<0.5f && lx>-0.5f) {
		return 3;
	}
	else if (x > 4.8f && x<5.5f && y>1.05f && y<3.05f && z>-6.7f && z < -2.4f && lx<0.5f && lx>-0.5f) {
		return 4;
	}
	else if (x > 3.7f && x<7.7f && y>1.05f && y<3.05f && z>-4.4f && z < -3.6f && lz<0.5f && lz>-0.5f) {
		return 5;
	}
	else if (x > 1.15f && x < 5.15f && y>4.025f && y<6.025f && z > -2.7f && z < -2.0f && lz<0.5f && lz>-0.5f) {
		return 6;
	}
	else if (x > 0.65f && x < 3.8f && y>4.025f && y<6.025f && z > -3.8f && z < -3.1f && lz<0.5f && lz>-0.5f) {
		return 7;
	}
	else if (x > 2.95f && x < 3.65f && y>4.025f && y<6.025f && z > -6.85f && z < -2.85f && lx<0.5f && lx>-0.5f) {
		return 8;
	}
	else if (x > 4.05f && x < 4.75f && y>4.025f && y<6.025f && z > -6.85f && z < -2.85f && lx<0.5f && lx>-0.5f) {
		return 9;
	}
	else if (x > 3.8f && x < 7.0f && y>4.025f && y<6.025f && z > -4.3f && z < -3.6f && lz<0.5f && lz>-0.5f) {
		return 10;
	}
	return 0;
}

GLuint LoadTexture(const char* filename){
	GLuint texture;
	int width, height;
	unsigned char* data;
	FILE* file;
	file = fopen(filename, "rb");

	if (file == NULL) return 0;
	width = 256;
	height = 256;
	data = (unsigned char*)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);
	for (int i = 0; i < width * height; ++i){		
		int index = i * 3;
		unsigned char B, R;
		B = data[index];
		R = data[index + 2];
		data[index] = R;
		data[index + 2] = B;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);
	return texture;
}

void changeSize(int w, int h)
{

	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	
	gluLookAt(	x, y, z,
				x + lx, y+ly, z + lz,
				0.0f, 1.0f, 0.0f);

	//vanjski pod
	glColor3f(0.3f,0.8f,0.3f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f, 100.0f);
		glVertex3f(100.0f, 0.0f, 100.0f);
		glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();

	glBegin(GL_TRIANGLES); //za ispod stepenica
		glColor3f(0.6f, 0.6f, 0.6f);
		for (int i = 0; i < 8; i++) {
			glVertex3f(4.8f + 0.275 * i, 4.025f - 0.175 * (i+1), -1.15f);
			glVertex3f(4.8f + 0.275 * (i + 1), 4.025f - 0.175 *(i+1), -1.15f);
			glVertex3f(4.8f + 0.275 * (i + 1), 4.025f - 0.175 * (i + 2), -1.15f);
		}
	glEnd();

	//prvi kat
	glBegin(GL_QUADS);
		glColor3f(0.25f, 0.1f, 0.0f);

		//smedji zid jug
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.05f, 0.0f);
		glVertex3f(8.1f, 1.05f, 0.0f);
		glVertex3f(8.1f, 0.0f, 0.0f);
		//smedji zid zapad
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.05f, 0.0f);
		glVertex3f(0.0f, 1.05f, -9.1f);
		glVertex3f(0.0f, 0.0f, -9.1f);
		//smedji zid sjever
		glVertex3f(0.0f, 0.0f, -9.1f);
		glVertex3f(0.0f, 1.05f, -9.1f);
		glVertex3f(8.1f, 1.05f, -9.1f);
		glVertex3f(8.1f, 0.0f, -9.1f);
		//smedji zid istok
		glVertex3f(8.1f, 0.0f, -9.1f);
		glVertex3f(8.1f, 1.05f, -9.1f);
		glVertex3f(8.1f, 1.05f, 0.0f);
		glVertex3f(8.1f, 0.0f, 0.0f);

		//pod 1.kat
		glColor3f(0.75f, 0.6f, 0.38f);
		glVertex3f(0.0f, 1.05f, 0.0f);
		glVertex3f(8.1f, 1.05f, 0.0f);
		glVertex3f(8.1f, 1.05f, -9.1f);
		glVertex3f(0.0f, 1.05f, -9.1f);

		//plafon 1.kat  
		glColor3f(0.3f, 0.3f, 0.3f);
		glVertex3f(0.0f, 3.65f, 0.0f);
		glVertex3f(0.0f, 3.65f, -9.1f);
		glVertex3f(4.8f, 3.65f, -9.1f);
		glVertex3f(4.8f, 3.65f, -0.0f);

		glColor3f(0.3f, 0.3f, 0.3f);
		glVertex3f(4.8f, 3.65f, -9.1f);
		glVertex3f(4.8f, 3.65f, -1.9f);
		glVertex3f(8.1f, 3.65f, -1.9f);
		glVertex3f(8.1f, 3.65f, -9.1f);

		glColor3f(0.3f, 0.3f, 0.3f);
		glVertex3f(7.7f, 3.65f, -1.9f);
		glVertex3f(7.7f, 3.65f, 0.0f);
		glVertex3f(8.1f, 3.65f, 0.0f);
		glVertex3f(8.1f, 3.65f, -1.9f);

		glColor3f(0.3f, 0.3f, 0.3f);
		glVertex3f(4.8f, 3.65f, -0.4f);
		glVertex3f(4.8f, 3.65f, 0.0f);
		glVertex3f(8.1f, 3.65f, 0.0f);
		glVertex3f(8.1f, 3.65f, -0.4f); 

		//vanjski zidovi sjever
		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(8.1f, 1.05f, -9.1f);//od prozora prema istoku
		glVertex3f(8.1f, 3.65f, -9.1f);
		glVertex3f(6.6f, 3.65f, -9.1f);
		glVertex3f(6.6f, 1.05f, -9.1f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(4.7f, 1.05f, -9.1f);//zid do prozora prema istoku
		glVertex3f(4.7f, 3.65f, -9.1f);
		glVertex3f(4.9f, 3.65f, -9.1f);
		glVertex3f(4.9f, 1.05f, -9.1f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(6.6f, 1.05f, -9.1f);//zid od prozora 
		glVertex3f(6.6f, 3.65f, -9.1f);
		glVertex3f(7.7f, 3.65f, -9.1f);
		glVertex3f(7.7f, 1.05f, -9.1f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(4.9f, 1.05f, -9.1f);//zid ispod prozora
		glVertex3f(4.9f, 1.95f, -9.1f);
		glVertex3f(6.6f, 1.95f, -9.1f);
		glVertex3f(6.6f, 1.05f, -9.1f);
				
		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(4.9f, 3.35f, -9.1f);//zid iznad prozora
		glVertex3f(4.9f, 3.65f, -9.1f);
		glVertex3f(6.6f, 3.65f, -9.1f);
		glVertex3f(6.6f, 3.35f, -9.1f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(4.9f, 1.95f, -9.1f);//zidovi oko prozora do vanjskog zida
		glVertex3f(6.6f, 1.95f, -9.1f);
		glVertex3f(6.6f, 1.95f, -8.7f);
		glVertex3f(4.9f, 1.95f, -8.7f);
		
		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(6.6f, 1.95f, -9.1f);
		glVertex3f(6.6f, 3.35f, -9.1f);
		glVertex3f(6.6f, 3.35f, -8.7f);
		glVertex3f(6.6f, 1.95f, -8.7f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(4.9f, 1.95f, -9.1f);
		glVertex3f(4.9f, 3.35f, -9.1f);
		glVertex3f(4.9f, 3.35f, -8.7f);
		glVertex3f(4.9f, 1.95f, -8.7f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(4.9f, 3.35f, -9.1f);
		glVertex3f(6.6f, 3.35f, -9.1f);
		glVertex3f(6.6f, 3.35f, -8.7f);
		glVertex3f(4.9f, 3.35f, -8.7f);// 
		
		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(4.7f, 1.05f, -9.1f);//zid izmeðu dva prozora
		glVertex3f(4.7f, 3.65f, -9.1f);
		glVertex3f(3.2f, 3.65f, -9.1f);
		glVertex3f(3.2f, 1.05f, -9.1f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(0.0f, 1.05f, -9.1f);//zid do prozora prema istoku
		glVertex3f(0.0f, 3.65f, -9.1f);
		glVertex3f(1.5f, 3.65f, -9.1f);
		glVertex3f(1.5f, 1.05f, -9.1f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(1.5f, 1.05f, -9.1f);//zid ispod prozora
		glVertex3f(1.5f, 1.95f, -9.1f);
		glVertex3f(3.2f, 1.95f, -9.1f);
		glVertex3f(3.2f, 1.05f, -9.1f);
						
		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(1.5f, 3.35f, -9.1f);//zid iznad prozora
		glVertex3f(1.5f, 3.65f, -9.1f);
		glVertex3f(3.2f, 3.65f, -9.1f);
		glVertex3f(3.2f, 3.35f, -9.1f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(1.5f, 1.95f, -9.1f);//zidovi oko prozora do vanjskog zida
		glVertex3f(1.5f, 1.95f, -8.7f);
		glVertex3f(3.2f, 1.95f, -8.7f);
		glVertex3f(3.2f, 1.95f, -9.1f);
		
		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(1.5f, 3.35f, -9.1f);
		glVertex3f(1.5f, 3.35f, -8.7f);
		glVertex3f(3.2f, 3.35f, -8.7f);
		glVertex3f(3.2f, 3.35f, -9.1f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(1.5f, 1.95f, -9.1f);
		glVertex3f(1.5f, 1.95f, -8.7f);
		glVertex3f(1.5f, 3.35f, -8.7f);
		glVertex3f(1.5f, 3.35f, -9.1f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(3.2f, 1.95f, -9.1f);
		glVertex3f(3.2f, 1.95f, -8.7f);
		glVertex3f(3.2f, 3.35f, -8.7f);
		glVertex3f(3.2f, 3.35f, -9.1f);//

		//vanjski zidovi zapad
		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(0.0f, 1.05f, -9.1f);//od prozora prema sjeveru
		glVertex3f(0.0f, 3.65f, -9.1f);
		glVertex3f(0.0f, 3.65f, -3.9f);
		glVertex3f(0.0f, 1.05f, -3.9f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(0.0f, 1.05f, -2.7f);//zid do prozora
		glVertex3f(0.0f, 3.65f, -2.7f);
		glVertex3f(0.0f, 3.65f, 0.0f);
		glVertex3f(0.0f, 1.05f, 0.0f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(0.0f, 1.05f, -3.9f);//zid ispod prozora
		glVertex3f(0.0f, 1.95f, -3.9f);
		glVertex3f(0.0f, 1.95f, -2.7f);
		glVertex3f(0.0f, 1.05f, -2.7f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(0.0f, 3.35f, -3.9f);//zid iznad prozora
		glVertex3f(0.0f, 3.65f, -3.9f);
		glVertex3f(0.0f, 3.65f, -2.7f);
		glVertex3f(0.0f, 3.35f, -2.7f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(0.0f, 1.95f, -3.9f);//zidovi oko prozora do vanjskog zida
		glVertex3f(0.0f, 1.95f, -2.7f);
		glVertex3f(0.4f, 1.95f, -2.7f);
		glVertex3f(0.4f, 1.95f, -3.9f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(0.0f, 1.95f, -3.9f);
		glVertex3f(0.0f, 3.35f, -3.9f);
		glVertex3f(0.4f, 3.35f, -3.9f);
		glVertex3f(0.4f, 1.95f, -3.9f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(0.0f, 1.95f, -2.7f);
		glVertex3f(0.0f, 3.35f, -2.7f);
		glVertex3f(0.4f, 3.35f, -2.7f);
		glVertex3f(0.4f, 1.95f, -2.7f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(0.0f, 3.35f, -3.9f);
		glVertex3f(0.0f, 3.35f, -2.7f);
		glVertex3f(0.4f, 3.35f, -2.7f);
		glVertex3f(0.4f, 3.35f, -3.9f);//

		//vanjski zidovi jug
		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(0.0f, 1.05f, 0.0f);//zid do vrata
		glVertex3f(0.0f, 3.65f, 0.0f);
		glVertex3f(3.55f, 3.65f, 0.0f);
		glVertex3f(3.55f, 1.05f, 0.0f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(4.7f, 1.05f, 0.0f);//zid od vrata prema istoku
		glVertex3f(4.7f, 3.65f, 0.0f);
		glVertex3f(8.1f, 3.65f, 0.0f);
		glVertex3f(8.1f, 1.05f, 0.0f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(3.55f, 3.05f, 0.0f);//zid iznad ulaznih vrata
		glVertex3f(3.55f, 3.65f, 0.0f);
		glVertex3f(4.7f, 3.65f, 0.0f);
		glVertex3f(4.7f, 3.05f, 0.0f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(4.7f, 1.05f, 0.0f);//zidovi kod vrata izmeðu vanjskih i unutarnjih zidova
		glVertex3f(4.7f, 3.65f, 0.0f);
		glVertex3f(4.7f, 3.65f, -0.4f);
		glVertex3f(4.7f, 1.05f, -0.4f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(3.55f, 3.05f, 0.0f);
		glVertex3f(3.55f, 3.05f, -0.4f);
		glVertex3f(4.7f, 3.05f, -0.4f);
		glVertex3f(4.7f, 3.05f, 0.0f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(3.55f, 1.05f, 0.0f);
		glVertex3f(3.55f, 3.05f, 0.0f);
		glVertex3f(3.55f, 3.05f, -0.4f);
		glVertex3f(3.55f, 1.05f, -0.4f);

		//vanjski zidovi istok
		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(8.1f, 1.05f, 0.0f);//do prozora prema sjeveru
		glVertex3f(8.1f, 3.65f, 0.0f);
		glVertex3f(8.1f, 3.65f, -3.0f);
		glVertex3f(8.1f, 1.05f, -3.0f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(8.1f, 1.05f, -3.6f);//od prozora prema sjeveru
		glVertex3f(8.1f, 3.65f, -3.6);
		glVertex3f(8.1f, 3.65f, -9.1f);
		glVertex3f(8.1f, 1.05f, -9.1f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(8.1f, 1.05f, -3.0f);//zid ispod prozora
		glVertex3f(8.1f, 2.75f, -3.0f);
		glVertex3f(8.1f, 2.75f, -3.6f);
		glVertex3f(8.1f, 1.05f, -3.6f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(8.1f, 3.35f, -3.0f);//zid iznad prozora
		glVertex3f(8.1f, 3.65f, -3.0f);
		glVertex3f(8.1f, 3.65f, -3.6f);
		glVertex3f(8.1f, 3.35f, -3.6f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(7.7f, 2.75f, -3.0f);//zidovi oko prozora izmeðu vanjskih i unutarnjih zidova
		glVertex3f(7.7f, 2.75f, -3.6f);
		glVertex3f(8.1f, 2.75f, -3.6f);
		glVertex3f(8.1f, 2.75f, -3.0f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(7.7f, 3.35f, -3.0f);
		glVertex3f(7.7f, 3.35f, -3.6f);
		glVertex3f(8.1f, 3.35f, -3.6f);
		glVertex3f(8.1f, 3.35f, -3.0f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(7.7f, 2.75f, -3.0f);
		glVertex3f(7.7f, 3.35f, -3.0f);
		glVertex3f(8.1f, 3.35f, -3.0f);
		glVertex3f(8.1f, 2.75f, -3.0f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(7.7f, 2.75f, -3.6f);
		glVertex3f(7.7f, 3.35f, -3.6f);
		glVertex3f(8.1f, 3.35f, -3.6f);
		glVertex3f(8.1f, 2.75f, -3.6f);


		//zid izmedu sjever
		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(4.5f, 1.05f, -8.7f);
		glVertex3f(4.5f, 3.65f, -8.7f);
		glVertex3f(4.5f, 3.65f, -4.7f);
		glVertex3f(4.5f, 1.05f, -4.7f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(4.8f, 1.05f, -8.7f);
		glVertex3f(4.8f, 3.65f, -8.7f);
		glVertex3f(4.8f, 3.65f, -4.7f);
		glVertex3f(4.8f, 1.05f, -4.7f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(4.5f, 1.05f, -4.4f);
		glVertex3f(4.5f, 3.65f, -4.4f);
		glVertex3f(4.8f, 3.65f, -4.4f);
		glVertex3f(4.8f, 1.05f, -4.4f);
			
		//zid izmedu istok
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(7.7f, 1.05f, -4.4f);
		glVertex3f(7.7f, 3.65f, -4.4f);
		glVertex3f(5.5f, 3.65f, -4.4f);
		glVertex3f(5.5f, 1.05f, -4.4f);

		glVertex3f(7.7f, 1.05f, -4.7f);
		glVertex3f(7.7f, 3.65f, -4.7f);
		glVertex3f(5.5f, 3.65f, -4.7f);
		glVertex3f(5.5f, 1.05f, -4.7f); 

		//zid izmedu istok kupatilo-stepenice
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(7.7f, 1.05f, -2.2f);
		glVertex3f(7.7f, 3.65f, -2.2f);
		glVertex3f(5.4f, 3.65f, -2.2f);
		glVertex3f(5.4f, 1.05f, -2.2f);  

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(7.7f, 1.05f, -1.9f);
		glVertex3f(7.7f, 3.65f, -1.9f);
		glVertex3f(4.7f, 3.65f, -1.9f);
		glVertex3f(4.7f, 1.05f, -1.9f); 

		//kupatilo
		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(5.4f, 1.05f, -2.2f);
		glVertex3f(5.4f, 3.65f, -2.2f);
		glVertex3f(5.4f, 3.65f, -3.6f);
		glVertex3f(5.4f, 1.05f, -3.6f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(5.4f, 1.05f, -3.3f);
		glVertex3f(5.4f, 3.65f, -3.3f);
		glVertex3f(5.7f, 3.65f, -3.3f);
		glVertex3f(5.7f, 1.05f, -3.3f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(5.7f, 1.05f, -3.3f);
		glVertex3f(5.7f, 3.65f, -3.3f);
		glVertex3f(5.7f, 3.65f, -3.6f);
		glVertex3f(5.7f, 1.05f, -3.6f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(5.7f, 1.05f, -3.6f);
		glVertex3f(5.7f, 3.65f, -3.6f);
		glVertex3f(5.05f, 3.65f, -3.6f);
		glVertex3f(5.05f, 1.05f, -3.6f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(7.7f, 1.05f, -4.4f);//zid istok do prozora
		glVertex3f(7.7f, 3.65f, -4.4f);
		glVertex3f(7.7f, 3.65f, -3.6f);
		glVertex3f(7.7f, 1.05f, -3.6f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(7.7f, 1.05f, -3.0f);//zid istok od prozora
		glVertex3f(7.7f, 3.65f, -3.0f);
		glVertex3f(7.7f, 3.65f, -2.2f);
		glVertex3f(7.7f, 1.05f, -2.2f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(7.7f, 1.05f, -3.6f);//zid ispod prozora
		glVertex3f(7.7f, 2.75f, -3.6f);
		glVertex3f(7.7f, 2.75f, -3.0f);
		glVertex3f(7.7f, 1.05f, -3.0f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(7.7f, 3.35f, -3.6f);//zid iznad prozora
		glVertex3f(7.7f, 3.65f, -3.6f);
		glVertex3f(7.7f, 3.65f, -3.0f);
		glVertex3f(7.7f, 3.35f, -3.0f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(5.7f, 3.05f, -4.4f);//zid iznad vrata strana kupatilo
		glVertex3f(5.7f, 3.65f, -4.4f);
		glVertex3f(5.7f, 3.65f, -3.6f);
		glVertex3f(5.7f, 3.05f, -3.6f);
		
		glVertex3f(5.5f, 3.05f, -4.4f);//zid iznad vrata strana hodnik
		glVertex3f(5.5f, 3.65f, -4.4f);
		glVertex3f(5.5f, 3.65f, -3.6f);
		glVertex3f(5.5f, 3.05f, -3.6f);

		glVertex3f(5.5f, 3.05f, -4.4f);
		glVertex3f(5.5f, 3.05f, -3.6f);
		glVertex3f(5.7f, 3.05f, -3.6f);
		glVertex3f(5.7f, 3.05f, -4.4f);

		//hodnik-kupatilo
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(4.7f, 1.05f, -1.9f);
		glVertex3f(4.7f, 3.65f, -1.9f);
		glVertex3f(4.7f, 3.65f, -2.55f);
		glVertex3f(4.7f, 1.05f, -2.55f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(4.7f, 1.05f, -2.55f);
		glVertex3f(4.7f, 3.65f, -2.55f);
		glVertex3f(5.05f, 3.65f, -2.55f);
		glVertex3f(5.05f, 1.05f, -2.55f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(5.05f, 1.05f, -2.55f);
		glVertex3f(5.05f, 3.65f, -2.55f);
		glVertex3f(5.05f, 3.65f, -3.6f);
		glVertex3f(5.05f, 1.05f, -3.6f);

		//hodnik
		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(3.55f, 1.05f, -4.4f);//onaj mali dio
		glVertex3f(3.55f, 3.65f, -4.4f);
		glVertex3f(3.75f, 3.65f, -4.4f);
		glVertex3f(3.75f, 1.05f, -4.4f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(4.5f, 3.05f, -4.4f);//zid iznad vrata dnevne
		glVertex3f(4.5f, 3.65f, -4.4f);
		glVertex3f(3.75f, 3.65f, -4.4f);
		glVertex3f(3.75f, 3.05f, -4.4f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(4.8f, 3.05f, -4.4f);//zid iznad vrata sobe
		glVertex3f(4.8f, 3.65f, -4.4f);
		glVertex3f(5.5f, 3.65f, -4.4f);
		glVertex3f(5.5f, 3.05f, -4.4f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(3.55f, 1.05f, -4.4f);//zid izmeðu blagavaonice
		glVertex3f(3.55f, 3.65f, -4.4f);
		glVertex3f(3.55f, 3.65f, -3.2f);
		glVertex3f(3.55f, 1.05f, -3.2f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(3.55f, 1.05f, -0.4f);//zid strana hodnik prema ulazu
		glVertex3f(3.55f, 3.65f, -0.4f);
		glVertex3f(3.55f, 3.65f, -2.5f);
		glVertex3f(3.55f, 1.05f, -2.5f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(3.55f, 3.05f, -2.5f);//zid iznad vrata strana hodnik
		glVertex3f(3.55f, 3.65f, -2.5f);
		glVertex3f(3.55f, 3.65f, -3.2f);
		glVertex3f(3.55f, 3.05f, -3.2f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(3.25f, 1.05f, -2.5f);//zidovi oko vrata kuhinja
		glVertex3f(3.25f, 3.65f, -2.5f);
		glVertex3f(3.55f, 3.65f, -2.5f);
		glVertex3f(3.55f, 1.05f, -2.5f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(3.25f, 1.05f, -3.2f);
		glVertex3f(3.25f, 3.65f, -3.2f);
		glVertex3f(3.55f, 3.65f, -3.2f);
		glVertex3f(3.55f, 1.05f, -3.2f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(3.25f, 3.05f, -2.5f);
		glVertex3f(3.25f, 3.05f, -3.2f);
		glVertex3f(3.55f, 3.05f, -3.2f);
		glVertex3f(3.55f, 3.05f, -2.5f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(3.55f, 3.05f, -0.4f);//zid iznad ulaznih vrata
		glVertex3f(3.55f, 3.65f, -0.4f);
		glVertex3f(4.7f, 3.65f, -0.4f);
		glVertex3f(4.7f, 3.05f, -0.4f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(4.7f, 1.05f, -0.4f);//zid od vrata prema istoku
		glVertex3f(4.7f, 3.65f, -0.4f);
		glVertex3f(7.7f, 3.65f, -0.4f);
		glVertex3f(7.7f, 1.05f, -0.4f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(7.7f, 1.05f, -0.4f);//zid istok hodnik
		glVertex3f(7.7f, 3.65f, -0.4f);
		glVertex3f(7.7f, 3.65f, -1.9f);
		glVertex3f(7.7f, 1.05f, -1.9f);

		//stepenice hodnik 
		for (int i = 0; i < 8; i++) {
			glColor3f(0.75f, 0.35f, 0.1f);
			glVertex3f(6.725f - 0.275 * i, 2.45f - 0.175 * i, -0.4f);
			glVertex3f(6.725f - 0.275 * i, 2.45f - 0.175 * i, -1.15f);
			glVertex3f(6.725f - 0.275 * i, 2.45f - 0.175 * (i + 1), -1.15f);
			glVertex3f(6.725f - 0.275 * i, 2.45f - 0.175 * (i + 1), -0.4f);

			glColor3f(0.65f, 0.55f, 0.2f);
			glVertex3f(7.0f - 0.275 * i, 2.45f - 0.175 * i, -1.15f);
			glVertex3f(7.0f - 0.275 * i, 2.45f - 0.175 * i, -0.4f);
			glVertex3f(7.0f - 0.275 * (i + 1), 2.45f - 0.175 * i, -0.4f);
			glVertex3f(7.0f - 0.275 * (i + 1), 2.45f - 0.175 * i, -1.15f);

			glColor3f(0.6f, 0.6f, 0.6f);
			glVertex3f(7.0f - 0.275 * i, 2.45f - 0.175 * i, -1.15f);
			glVertex3f(7.0f - 0.275 * i, 1.05f, -1.15f);
			glVertex3f(7.0f - 0.275 * (i + 1), 1.05f, -1.15f);
			glVertex3f(7.0f - 0.275 * (i + 1), 2.45f - 0.175 * i, -1.15f);
		}

		for (int i = 0; i < 9; i++) {
			glColor3f(0.75f, 0.35f, 0.1f);
			glVertex3f(5.075f + 0.275 * i, 4.025f - 0.175 * i, -1.15f);
			glVertex3f(5.075f + 0.275 * i, 4.025f - 0.175 * i, -1.9f);
			glVertex3f(5.075f + 0.275 * i, 4.025f - 0.175 * (i + 1), -1.9f);
			glVertex3f(5.075f + 0.275 * i, 4.025f - 0.175 * (i + 1), -1.15f);

			glColor3f(0.65f, 0.55f, 0.2f);
			glVertex3f(4.8f + 0.275 * i, 4.025f - 0.175 * i, -1.9f);
			glVertex3f(4.8f + 0.275 * i, 4.025f - 0.175 * i, -1.15f);
			glVertex3f(4.8f + 0.275 * (i + 1), 4.025f - 0.175 * i, -1.15f);
			glVertex3f(4.8f + 0.275 * (i + 1), 4.025f - 0.175 * i, -1.9f);

			glColor3f(0.6f, 0.6f, 0.6f);
			glVertex3f(4.8f + 0.275 * i, 4.025f - 0.175 * i, -1.15f);
			glVertex3f(4.8f + 0.275 * i, 4.025f - 0.175 * (i + 1), -1.15f);
			glVertex3f(4.8f + 0.275 * (i + 1), 4.025f - 0.175 * (i + 1), -1.15f);
			glVertex3f(4.8f + 0.275 * (i + 1), 4.025f - 0.175 * i, -1.15f);

		}

		glColor3f(0.6f, 0.6f, 0.6f);
		glVertex3f(7.275f, 2.45f, -1.15f);
		glVertex3f(7.275f, 2.45f, -1.9f);
		glVertex3f(7.0f, 2.45f, -1.9f);
		glVertex3f(7.0f, 2.45f, -1.15f);

		glVertex3f(7.0f, 2.45f, -1.15f);
		glVertex3f(7.0f, 2.45f, -1.9f);
		glVertex3f(4.8, 3.85f, -1.9f);
		glVertex3f(4.8f, 3.85f, -1.15f);

		glVertex3f(7.0f, 2.275f, -0.4f);
		glVertex3f(7.0f, 2.275f, -1.9f);
		glVertex3f(7.7f, 2.275f, -1.9f);
		glVertex3f(7.7f, 2.275f, -0.4f);

		glVertex3f(7.0f, 2.275f, -1.15f);
		glVertex3f(7.0f, 2.275f, -1.9f);
		glVertex3f(7.0f, 2.45f, -1.9f);
		glVertex3f(7.0f, 2.45f, -1.15f);

		glColor3f(0.65f, 0.55f, 0.2f);
		glVertex3f(7.0f, 2.45f, -0.4f);//dio kod stepenica
		glVertex3f(7.0f, 2.45f, -1.15f);
		glVertex3f(7.7f, 2.45f, -1.15f);
		glVertex3f(7.7f, 2.45f, -0.4f);

		glVertex3f(7.275f, 2.45f, -0.4f);
		glVertex3f(7.275f, 2.45f, -1.9f);
		glVertex3f(7.7f, 2.45f, -1.9f);
		glVertex3f(7.7f, 2.45f, -0.4f);

		//dnevna
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(3.75f, 1.05f, -4.7f);//zid do connectora
		glVertex3f(3.75f, 3.65f, -4.7f);
		glVertex3f(2.85f, 3.65f, -4.7f);
		glVertex3f(2.85f, 1.05f, -4.7f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(1.15f, 1.05f, -4.7f);//od connectora
		glVertex3f(1.15f, 3.65f, -4.7f);
		glVertex3f(0.4f, 3.65f, -4.7f);
		glVertex3f(0.4f, 1.05f, -4.7f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(2.85f, 3.0f, -4.7f);//zid iznad connectora
		glVertex3f(2.85f, 3.65f, -4.7f);
		glVertex3f(1.15f, 3.65f, -4.7f);
		glVertex3f(1.15f, 3.0f, -4.7f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(0.4f, 1.05f, -4.7f);//zid prema zapadu
		glVertex3f(0.4f, 3.65f, -4.7f);
		glVertex3f(0.4f, 3.65f, -8.7f);
		glVertex3f(0.4f, 1.05f, -8.7f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(0.4f, 1.05f, -8.7f);//zid do prozora prema istoku
		glVertex3f(0.4f, 3.65f, -8.7f);
		glVertex3f(1.5f, 3.65f, -8.7f);
		glVertex3f(1.5f, 1.05f, -8.7f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(3.2f, 1.05f, -8.7f);//zid od prozora 
		glVertex3f(3.2f, 3.65f, -8.7f);
		glVertex3f(4.5f, 3.65f, -8.7f);
		glVertex3f(4.5f, 1.05f, -8.7f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(1.5f, 1.05f, -8.7f);//zid ispod prozora
		glVertex3f(1.5f, 1.95f, -8.7f);
		glVertex3f(3.2f, 1.95f, -8.7f);
		glVertex3f(3.2f, 1.05f, -8.7f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(1.5f, 3.35f, -8.7f);//zid iznad prozora
		glVertex3f(1.5f, 3.65f, -8.7f);
		glVertex3f(3.2f, 3.65f, -8.7f);
		glVertex3f(3.2f, 3.35f, -8.7f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(4.5f, 3.05f, -4.7f);//zid iznad vrata strana dnevna
		glVertex3f(4.5f, 3.65f, -4.7f);
		glVertex3f(3.75f, 3.65f, -4.7f);
		glVertex3f(3.75f, 3.05f, -4.7f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(4.5f, 3.05f, -4.4f);//zidovi oko vrata
		glVertex3f(4.5f, 3.05f, -4.7f);
		glVertex3f(3.75f, 3.05f, -4.7f);
		glVertex3f(3.75f, 3.05f, -4.4f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(4.5f, 1.05f, -4.7f);
		glVertex3f(4.5f, 3.05f, -4.7f);
		glVertex3f(4.5f, 3.05f, -4.4f);
		glVertex3f(4.5f, 1.05f, -4.4f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(3.75f, 1.05f, -4.7f);
		glVertex3f(3.75f, 3.65f, -4.7f);
		glVertex3f(3.75f, 3.65f, -4.4f);
		glVertex3f(3.75f, 1.05f, -4.4f);

		//soba dolje
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(4.7f, 1.05f, -8.7f);//zid do prozora prema istoku
		glVertex3f(4.7f, 3.65f, -8.7f);
		glVertex3f(4.9f, 3.65f, -8.7f);
		glVertex3f(4.9f, 1.05f, -8.7f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(6.6f, 1.05f, -8.7f);//zid od prozora 
		glVertex3f(6.6f, 3.65f, -8.7f);
		glVertex3f(7.7f, 3.65f, -8.7f);
		glVertex3f(7.7f, 1.05f, -8.7f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(4.9f, 1.05f, -8.7f);//zid ispod prozora
		glVertex3f(4.9f, 1.95f, -8.7f);
		glVertex3f(6.6f, 1.95f, -8.7f);
		glVertex3f(6.6f, 1.05f, -8.7f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(4.9f, 3.35f, -8.7f);//zid iznad prozora
		glVertex3f(4.9f, 3.65f, -8.7f);
		glVertex3f(6.6f, 3.65f, -8.7f);
		glVertex3f(6.6f, 3.35f, -8.7f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(7.7f, 1.05f, -8.7f);//zid istok
		glVertex3f(7.7f, 3.65f, -8.7f);
		glVertex3f(7.7f, 3.65f, -4.7f);
		glVertex3f(7.7f, 1.05f, -4.7f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(4.8f, 3.05f, -4.7f);//zid iznad vrata sobe 
		glVertex3f(4.8f, 3.65f, -4.7f);
		glVertex3f(5.5f, 3.65f, -4.7f);
		glVertex3f(5.5f, 3.05f, -4.7f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(4.8f, 3.05f, -4.4f);//zid oko vrata
		glVertex3f(4.8f, 3.05f, -4.7f);
		glVertex3f(5.5f, 3.05f, -4.7f);
		glVertex3f(5.5f, 3.05f, -4.4f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(4.8f, 1.05f, -4.7f);
		glVertex3f(4.8f, 3.05f, -4.7f);
		glVertex3f(4.8f, 3.05f, -4.4f);
		glVertex3f(4.8f, 1.05f, -4.4f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(5.5f, 1.05f, -4.7f);
		glVertex3f(5.5f, 3.65f, -4.7f);
		glVertex3f(5.5f, 3.65f, -4.4f);
		glVertex3f(5.5f, 1.05f, -4.4f);

		//connector
		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(2.85f, 1.05f, -4.7f);
		glVertex3f(2.85f, 3.0f, -4.7f);
		glVertex3f(2.85f, 3.0f, -4.4f);
		glVertex3f(2.85f, 1.05f, -4.4f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(1.15f, 1.05f, -4.7f);
		glVertex3f(1.15f, 3.0f, -4.7f);
		glVertex3f(1.15f, 3.0f, -4.4f);
		glVertex3f(1.15f, 1.05f, -4.4f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(1.15f, 3.0f, -4.7f);
		glVertex3f(1.15f, 3.0f, -4.4f);
		glVertex3f(2.85f, 3.0f, -4.4f);
		glVertex3f(2.85f, 3.0f, -4.7f);

		//blagavaonica-kuhinja
		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(3.25f, 1.05f, -4.4f);//zid izmeðu hodnika
		glVertex3f(3.25f, 3.65f, -4.4f);
		glVertex3f(3.25f, 3.65f, -3.2f);
		glVertex3f(3.25f, 1.05f, -3.2f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(2.85f, 1.05f, -4.4f);
		glVertex3f(2.85f, 3.65f, -4.4f);
		glVertex3f(3.25f, 3.65f, -4.4f);
		glVertex3f(3.25f, 1.05f, -4.4f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(1.15f, 1.05f, -4.4f);
		glVertex3f(1.15f, 3.65f, -4.4f);
		glVertex3f(0.4f, 3.65f, -4.4f);
		glVertex3f(0.4f, 1.05f, -4.4f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(2.85f, 3.0f, -4.4f);//zid iznad connectora
		glVertex3f(2.85f, 3.65f, -4.4f);
		glVertex3f(1.15f, 3.65f, -4.4f);
		glVertex3f(1.15f, 3.0f, -4.4f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(3.25f, 1.05f, -2.5f);//kuhinja stup
		glVertex3f(3.25f, 3.65f, -2.5f);
		glVertex3f(2.65f, 3.65f, -2.5f);
		glVertex3f(2.65f, 1.05f, -2.5f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(2.65f, 1.05f, -2.5f);
		glVertex3f(2.65f, 3.65f, -2.5f);
		glVertex3f(2.65f, 3.65f, -2.2f);
		glVertex3f(2.65f, 1.05f, -2.2f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(3.25f, 1.05f, -2.2f);
		glVertex3f(3.25f, 3.65f, -2.2f);
		glVertex3f(2.65f, 3.65f, -2.2f);
		glVertex3f(2.65f, 1.05f, -2.2f);
		
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(2.65f, 1.05f, -2.5f);//šank
		glVertex3f(2.65f, 2.15f, -2.5f);
		glVertex3f(1.35f, 2.15f, -2.5f);
		glVertex3f(1.35f, 1.05f, -2.5f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(1.35f, 1.05f, -2.5f);
		glVertex3f(1.35f, 2.15f, -2.5f);
		glVertex3f(1.35f, 2.15f, -2.2f);
		glVertex3f(1.35f, 1.05f, -2.2f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(2.65f, 1.05f, -2.2f);
		glVertex3f(2.65f, 2.15f, -2.2f);
		glVertex3f(1.35f, 2.15f, -2.2f);
		glVertex3f(1.35f, 1.05f, -2.2f);

		glColor3f(0.55f, 0.4f, 0.18);
		glVertex3f(2.65f, 2.15f, -2.2f);
		glVertex3f(2.65f, 2.15, -2.5f);
		glVertex3f(1.35f, 2.15f, -2.5f);
		glVertex3f(1.35f, 2.15f, -2.2f);//kraj šanka

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(3.25f, 1.05f, -2.5f);//strana kuhinja prema ulazu
		glVertex3f(3.25f, 3.65f, -2.5f);
		glVertex3f(3.25f, 3.65f, -0.4f);
		glVertex3f(3.25f, 1.05f, -0.4f);
		
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(3.25f, 1.05f, -0.4f);//zid kuhinja jug
		glVertex3f(3.25f, 3.65f, -0.4f);
		glVertex3f(0.4f, 3.65f, -0.4f);
		glVertex3f(0.4f, 1.05f, -0.4f);
		
		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(0.4f, 1.05f, -0.4f);//zid kuhinja do prozora
		glVertex3f(0.4f, 3.65f, -0.4f);
		glVertex3f(0.4f, 3.65f, -2.7f);
		glVertex3f(0.4f, 1.05f, -2.7f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(0.4f, 1.05f, -4.4f);//zid od prozora
		glVertex3f(0.4f, 3.65f, -4.4f);
		glVertex3f(0.4f, 3.65f, -3.9f);
		glVertex3f(0.4f, 1.05f, -3.9f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(0.4f, 1.05f, -3.9f);//zid ispod prozora
		glVertex3f(0.4f, 1.95f, -3.9f);
		glVertex3f(0.4f, 1.95f, -2.7f);
		glVertex3f(0.4f, 1.05f, -2.7f);
		
		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(0.4f, 3.35f, -3.9f);//zid iznad prozora
		glVertex3f(0.4f, 3.65f, -3.9f);
		glVertex3f(0.4f, 3.65f, -2.7f);
		glVertex3f(0.4f, 3.35f, -2.7f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(3.25f, 3.05f, -2.5f);//zid iznad vrata strana blagavaonica
		glVertex3f(3.25f, 3.65f, -2.5f);
		glVertex3f(3.25f, 3.65f, -3.2f);
		glVertex3f(3.25f, 3.05f, -3.2f);


	glEnd();
	
	//stepenice vani
	glBegin(GL_QUADS);
		glColor3f(0.65f, 0.55f, 0.2f);
		glVertex3f(3.2f, 1.05f, 0.0f);
		glVertex3f(3.2f, 1.05f, 1.0f);
		glVertex3f(5.1f, 1.05f, 1.0f);
		glVertex3f(5.1f, 1.05f, 0.0f);

		glColor3f(0.25f, 0.1f, 0.0f);
		glVertex3f(3.2f, 1.05f, 1.0f);
		glVertex3f(3.2f, 0.0f, 1.0f);
		glVertex3f(5.1f, 0.0f, 1.0f);
		glVertex3f(5.1f, 1.05f, 1.0f);

		glVertex3f(5.1f, 1.05f, 1.0f);
		glVertex3f(5.1f, 0.0f, 1.0f);
		glVertex3f(5.1f, 0.0f, 0.0f);
		glVertex3f(5.1f, 1.05f, 0.0f);

		for (int i = 0; i < 6; i++) {
			glColor3f(0.75f, 0.35f, 0.1f);
			glVertex3f(3.2f - 0.3 * i, 1.05f - 0.175 * i, 0.0f);
			glVertex3f(3.2f - 0.3 * i, 1.05f - 0.175 * i, 1.0f);
			glVertex3f(3.2f - 0.3 * i, 1.05f - 0.175 * (i + 1), 1.0f);
			glVertex3f(3.2f - 0.3 * i, 1.05f - 0.175 * (i + 1), 0.0f);

			glColor3f(0.65f, 0.55f, 0.2f);
			glVertex3f(3.5f - 0.3 * i, 1.05f - 0.175 * i, 0.0f);
			glVertex3f(3.5f - 0.3 * i, 1.05f - 0.175 * i, 1.0f);
			glVertex3f(3.5f - 0.3 * (i + 1), 1.05f - 0.175 * i, 1.0f);
			glVertex3f(3.5f - 0.3 * (i + 1), 1.05f - 0.175 * i, 0.0f);

			glColor3f(0.25f, 0.1f, 0.0f);
			glVertex3f(3.5f - 0.3 * i, 1.05f - 0.175 * i, 1.0f);
			glVertex3f(3.5f - 0.3 * i, 0.0f, 1.0f);
			glVertex3f(3.5f - 0.3 * (i + 1), 0.0f, 1.0f);
			glVertex3f(3.5f - 0.3 * (i + 1), 1.05f - 0.175 * i, 1.0f);
		}
	glEnd();

	//drugi kat
	glBegin(GL_QUADS);
		//pod 2.kat
		glColor3f(0.75f, 0.6f, 0.38f);
		glVertex3f(0.0f, 4.025f, 0.0f);
		glVertex3f(0.0f, 4.025f, -9.1f);
		glVertex3f(4.8f, 4.025f, -9.1f);
		glVertex3f(4.8f, 4.025f, 0.0f);

		glVertex3f(4.8f, 4.025f, -1.9f);
		glVertex3f(4.8f, 4.025f, -9.1f);
		glVertex3f(8.1f, 4.025f, -9.1f);
		glVertex3f(8.1f, 4.025f, -1.9f);

		//plafon 2.kat
		glColor3f(0.3f, 0.3f, 0.3f);
		glVertex3f(0.0f, 6.475f, 0.0f);
		glVertex3f(8.1f, 6.475f, 0.0f);
		glVertex3f(8.1f, 6.475f, -9.1f);
		glVertex3f(0.0f, 6.475f, -9.1f);

		//mali dio kod stepenica
		glColor3f(0.6f, 0.6f, 0.6f);
		glVertex3f(4.8f, 3.65f, -0.4f);
		glVertex3f(4.8f, 4.025f, -0.4f);
		glVertex3f(4.8f, 4.025f, -1.9f);
		glVertex3f(4.8f, 3.65f, -1.9f);

		//hodnik
		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(4.7f, 3.65f, -1.9f);//zid radna soba-stepenice
		glVertex3f(4.7f, 6.475f, -1.9f);
		glVertex3f(7.7f, 6.475f, -1.9f);
		glVertex3f(7.7f, 3.65f, -1.9f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(7.7f, 3.65f, -1.9f);//zid stepenice prema istoku
		glVertex3f(7.7f, 6.475f, -1.9f);
		glVertex3f(7.7f, 6.475f, -0.4f);
		glVertex3f(7.7f, 3.65f, -0.4f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(4.8f, 3.65f, -0.4f);//zid stepenice prema jugu
		glVertex3f(4.8f, 6.475f, -0.4f);
		glVertex3f(7.7f, 6.475f, -0.4f);
		glVertex3f(7.7f, 3.65f, -0.4f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(4.7f, 4.025, -1.9f);//noseæi stup
		glVertex3f(4.7f, 6.475f, -1.9f);
		glVertex3f(4.7f, 6.475f, -2.7f);
		glVertex3f(4.7f, 4.025f, -2.7f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(4.7f, 4.025, -2.7f);
		glVertex3f(4.7f, 6.475f, -2.7f);
		glVertex3f(4.8f, 6.475f, -2.7f);
		glVertex3f(4.8f, 4.025f, -2.7f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(4.8f, 4.025, -2.7f);//zid prema radnoj sobi do vrata
		glVertex3f(4.8f, 6.475f, -2.7f);
		glVertex3f(4.8f, 6.475f, -3.6f);
		glVertex3f(4.8f, 4.025f, -3.6f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(4.8f, 4.025, -4.3f);//zid od vrata radne sobe
		glVertex3f(4.8f, 6.475f, -4.3f);
		glVertex3f(4.8f, 6.475f, -4.55f);
		glVertex3f(4.8f, 4.025f, -4.55f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(4.8f, 6.025, -4.3f);//zid iznad vrata radne sobe
		glVertex3f(4.8f, 6.475f, -4.3f);
		glVertex3f(4.8f, 6.475f, -3.6f);
		glVertex3f(4.8f, 6.025f, -3.6f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(4.8f, 4.025, -0.4f);//zid do balkonskih vrata
		glVertex3f(4.8f, 6.475f, -0.4f);
		glVertex3f(4.4f, 6.475f, -0.4f);
		glVertex3f(4.4f, 4.025f, -0.4f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(3.65f, 4.025, -0.4f);//zid od balkonskih vrata
		glVertex3f(3.65f, 6.475f, -0.4f);
		glVertex3f(3.35f, 6.475f, -0.4f);
		glVertex3f(3.35f, 4.025f, -0.4f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(4.4f, 6.025, -0.4f);//zid iznad balkonskih vrata
		glVertex3f(4.4f, 6.475f, -0.4f);
		glVertex3f(3.65f, 6.475f, -0.4f);
		glVertex3f(3.65f, 6.025f, -0.4f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(4.4f, 6.025f, -0.4f);//zidovi oko balkonskih vrata
		glVertex3f(4.4f, 6.025f, 0.0f);
		glVertex3f(3.65f, 6.025f, 0.0f);
		glVertex3f(3.65f, 6.025f, -0.4f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(4.4f, 4.025f, 0.0f);
		glVertex3f(4.4f, 6.025f, 0.0f);
		glVertex3f(4.4f, 6.025f, -0.4f);
		glVertex3f(4.4f, 4.025f, -0.4f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(3.65f, 4.025f, 0.0f);
		glVertex3f(3.65f, 6.025f, 0.0f);
		glVertex3f(3.65f, 6.025f, -0.4f);
		glVertex3f(3.65f, 4.025f, -0.4f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(3.35f, 4.025f, -0.4f);//zid do gym vrata
		glVertex3f(3.35f, 6.475f, -0.4f);
		glVertex3f(3.35f, 6.475f, -2.0f);
		glVertex3f(3.35f, 4.025f, -2.0f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(3.35f, 4.025f, -2.7f);//zid od gym vrata
		glVertex3f(3.35f, 6.475f, -2.7f);
		glVertex3f(3.35f, 6.475f, -3.0f);
		glVertex3f(3.35f, 4.025f, -3.0f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(3.35f, 6.025f, -2.0f);//zid iznad gym vrata
		glVertex3f(3.35f, 6.475f, -2.0f);
		glVertex3f(3.35f, 6.475f, -2.7f);
		glVertex3f(3.35f, 6.025f, -2.7f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(3.35f, 4.025f, -3.0f);//zid do kupatila
		glVertex3f(3.35f, 6.475f, -3.0f);
		glVertex3f(2.85f, 6.475f, -3.0f);
		glVertex3f(2.85f, 4.025f, -3.0f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(2.85f, 4.025f, -3.0f);//zid do vrata kupatila
		glVertex3f(2.85f, 6.475f, -3.0f);
		glVertex3f(2.85f, 6.475f, -3.1f);
		glVertex3f(2.85f, 4.025f, -3.1f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(2.85f, 4.025f, -3.8f);//zid od vrata kupatila do sobe 1
		glVertex3f(2.85f, 6.475f, -3.8f);
		glVertex3f(2.85f, 6.475f, -4.55f);
		glVertex3f(2.85f, 4.025f, -4.55f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(2.85f, 6.025f, -3.1f);//zid iznad vrata kupatila
		glVertex3f(2.85f, 6.475f, -3.1f);
		glVertex3f(2.85f, 6.475f, -3.8f);
		glVertex3f(2.85f, 6.025f, -3.8f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(2.85f, 4.025f, -4.55f);//mali zid do vrata sobe 1
		glVertex3f(2.85f, 6.475f, -4.55f);
		glVertex3f(2.95f, 6.475f, -4.55f);
		glVertex3f(2.95f, 4.025f, -4.55f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(3.65f, 4.025f, -4.55f);//zid izmeðu vrata dvaju soba
		glVertex3f(3.65f, 6.475f, -4.55f);
		glVertex3f(4.05f, 6.475f, -4.55f);
		glVertex3f(4.05f, 4.025f, -4.55f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(2.95f, 6.025f, -4.55f);//zid iznad vrata dvaju soba
		glVertex3f(2.95f, 6.475f, -4.55f);
		glVertex3f(4.75f, 6.475f, -4.55f);
		glVertex3f(4.75f, 6.025f, -4.55f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(4.75f, 4.025f, -4.55f);//mali zid izmeðu sobe 2 i radne sobe
		glVertex3f(4.75f, 6.475f, -4.55f);
		glVertex3f(4.8f, 6.475f, -4.55f);
		glVertex3f(4.8f, 4.025f, -4.55f);

		//gym
		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(3.15f, 4.025f, -2.7f);//zid strana kupatilo
		glVertex3f(3.15f, 6.475f, -2.7f);
		glVertex3f(0.4f, 6.475f, -2.7f);
		glVertex3f(0.4f, 4.025f, -2.7f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(0.4f, 4.025f, -2.7f);//zid prema zapadu
		glVertex3f(0.4f, 6.475f, -2.7f);
		glVertex3f(0.4f, 6.475f, -0.4f);
		glVertex3f(0.4f, 4.025f, -0.4f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(0.4f, 4.025f, -0.4f);//zid prema jugu do prozora
		glVertex3f(0.4f, 6.475f, -0.4f);
		glVertex3f(1.7f, 6.475f, -0.4f);
		glVertex3f(1.7f, 4.025f, -0.4f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(2.8f, 4.025f, -0.4f);//zid prema jugu od prozora
		glVertex3f(2.8f, 6.475f, -0.4f);
		glVertex3f(3.15f, 6.475f, -0.4f);
		glVertex3f(3.15f, 4.025f, -0.4f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(1.7f, 4.025f, -0.4f);//zid ispod prozora
		glVertex3f(1.7f, 4.825f, -0.4f);
		glVertex3f(2.8f, 4.825f, -0.4f);
		glVertex3f(2.8f, 4.025f, -0.4f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(1.7f, 6.025f, -0.4f);//zid iznad prozora
		glVertex3f(1.7f, 6.475f, -0.4f);
		glVertex3f(2.8f, 6.475f, -0.4f);
		glVertex3f(2.8f, 6.025f, -0.4f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(1.7f, 4.825f, -0.4f);//zidovi oko prozora
		glVertex3f(1.7f, 4.825f, 0.0f);
		glVertex3f(2.8f, 4.825f, 0.0f);
		glVertex3f(2.8f, 4.825f, -0.4f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(1.7f, 6.025f, -0.4f);
		glVertex3f(1.7f, 6.025f, 0.0f);
		glVertex3f(2.8f, 6.025f, 0.0f);
		glVertex3f(2.8f, 6.025f, -0.4f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(1.7f, 4.825f, -0.4f);
		glVertex3f(1.7f, 4.825f, 0.0f);
		glVertex3f(1.7f, 6.025f, 0.0f);
		glVertex3f(1.7f, 6.025f, -0.4f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(2.8f, 4.825f, -0.4f);
		glVertex3f(2.8f, 4.825f, 0.0f);
		glVertex3f(2.8f, 6.025f, 0.0f);
		glVertex3f(2.8f, 6.025f, -0.4f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(3.15f, 4.025f, -0.4f);//zid prema istoku do vrata
		glVertex3f(3.15f, 6.475f, -0.4f);
		glVertex3f(3.15f, 6.475f, -2.0f);
		glVertex3f(3.15f, 4.025f, -2.0f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(3.15f, 6.025f, -2.0f);//zid iznad vrata
		glVertex3f(3.15f, 6.475f, -2.0f);
		glVertex3f(3.15f, 6.475f, -2.7f);
		glVertex3f(3.15f, 6.025f, -2.7f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(3.15f, 6.025f, -2.0f);//zidovi oko vrata
		glVertex3f(3.35f, 6.025f, -2.0f);
		glVertex3f(3.35f, 6.025f, -2.7f);
		glVertex3f(3.15f, 6.025f, -2.7f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(3.15f, 4.025f, -2.0f);
		glVertex3f(3.35f, 4.025f, -2.0f);
		glVertex3f(3.35f, 6.025f, -2.0f);
		glVertex3f(3.15f, 6.025f, -2.0f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(3.15f, 4.025f, -2.7f);
		glVertex3f(3.35f, 4.025f, -2.7f);
		glVertex3f(3.35f, 6.025f, -2.7f);
		glVertex3f(3.15f, 6.025f, -2.7f);

		//kupatilo
		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(2.85f, 4.025f, -3.0f);//zid do gym
		glVertex3f(2.85f, 6.475f, -3.0f);
		glVertex3f(0.4f, 6.475f, -3.0f);
		glVertex3f(0.4f, 4.025f, -3.0f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(2.65f, 4.025f, -3.0f);//zid do vrata
		glVertex3f(2.65f, 6.475f, -3.0f);
		glVertex3f(2.65f, 6.475f, -3.1f);
		glVertex3f(2.65f, 4.025f, -3.1f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(2.65f, 4.025f, -3.8f);//zid od vrata
		glVertex3f(2.65f, 6.475f, -3.8f);
		glVertex3f(2.65f, 6.475f, -4.55f);
		glVertex3f(2.65f, 4.025f, -4.55f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(2.65f, 6.025f, -3.1f);//zid iznad vrata
		glVertex3f(2.65f, 6.475f, -3.1f);
		glVertex3f(2.65f, 6.475f, -3.8f);
		glVertex3f(2.65f, 6.025f, -3.8f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(2.65f, 6.025f, -3.1f);//zidovi oko vrata
		glVertex3f(2.85f, 6.025f, -3.1f);
		glVertex3f(2.85f, 6.025f, -3.8f);
		glVertex3f(2.65f, 6.025f, -3.8f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(2.65f, 4.025f, -3.1f);
		glVertex3f(2.85f, 4.025f, -3.1f);
		glVertex3f(2.85f, 6.025f, -3.1f);
		glVertex3f(2.65f, 6.025f, -3.1f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(2.65f, 4.025f, -3.8f);
		glVertex3f(2.85f, 4.025f, -3.8f);
		glVertex3f(2.85f, 6.025f, -3.8f);
		glVertex3f(2.65f, 6.025f, -3.8f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(2.65f, 4.025f, -4.55f);//zid do soba 1
		glVertex3f(2.65f, 6.475f, -4.55f);
		glVertex3f(0.4f, 6.475f, -4.55f);
		glVertex3f(0.4f, 4.025f, -4.55f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(0.4f, 4.025f, -4.55f);//zid zapad
		glVertex3f(0.4f, 6.475f, -4.55f);
		glVertex3f(0.4f, 6.475f, -3.0f);
		glVertex3f(0.4f, 4.025f, -3.0f);

		//radna soba
		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(5.0f, 4.025f, -4.3f);//mali zid iza vrata
		glVertex3f(5.0f, 6.475f, -4.3f);
		glVertex3f(5.0f, 6.475f, -4.55f);
		glVertex3f(5.0f, 4.025f, -4.55f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(5.0f, 4.025f, -4.55f);//zid izmeðu sobe 2
		glVertex3f(5.0f, 6.475f, -4.55f);
		glVertex3f(7.7f, 6.475f, -4.55f);
		glVertex3f(7.7f, 4.025f, -4.55f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(7.7f, 4.025f, -4.55f);//zid istok
		glVertex3f(7.7f, 6.475f, -4.55f);
		glVertex3f(7.7f, 6.475f, -2.25f);
		glVertex3f(7.7f, 4.025f, -2.25f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(5.0f, 4.025f, -2.25f);//zid izmeðu stepenica
		glVertex3f(5.0f, 6.475f, -2.25f);
		glVertex3f(7.7f, 6.475f, -2.25f);
		glVertex3f(7.7f, 4.025f, -2.25f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(5.0f, 4.025f, -2.25f);//zid izmeðu hodnika do vrata
		glVertex3f(5.0f, 6.475f, -2.25f);
		glVertex3f(5.0f, 6.475f, -3.6f);
		glVertex3f(5.0f, 4.025f, -3.6f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(5.0f, 6.025f, -3.6f);//zid iznad vrata
		glVertex3f(5.0f, 6.475f, -3.6f);
		glVertex3f(5.0f, 6.475f, -4.3f);
		glVertex3f(5.0f, 6.025f, -4.3f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(5.0f, 6.025f, -3.6f);//zidovi oko vrata
		glVertex3f(4.8f, 6.025f, -3.6f);
		glVertex3f(4.8f, 6.025f, -4.3f);
		glVertex3f(5.0f, 6.025f, -4.3f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(5.0f, 4.025f, -3.6f);
		glVertex3f(4.8f, 4.025f, -3.6f);
		glVertex3f(4.8f, 6.025f, -3.6f);
		glVertex3f(5.0f, 6.025f, -3.6f);


		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(5.0f, 4.025f, -4.3f);
		glVertex3f(4.8f, 4.025f, -4.3f);
		glVertex3f(4.8f, 6.025f, -4.3f);
		glVertex3f(5.0f, 6.025f, -4.3f);

		//soba 1
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(0.4f, 4.025f, -4.85f);//zid izmeðu kupatila do vrata
		glVertex3f(0.4f, 6.475f, -4.85f);
		glVertex3f(2.95f, 6.475f, -4.85f);
		glVertex3f(2.95f, 4.025f, -4.85f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(0.4f, 4.025f, -4.85f);//zid zapad
		glVertex3f(0.4f, 6.475f, -4.85f);
		glVertex3f(0.4f, 6.475f, -8.7f);
		glVertex3f(0.4f, 4.025f, -8.7f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(0.4f, 4.025f, -8.7f);//zid sjever do prozora
		glVertex3f(0.4f, 6.475f, -8.7f);
		glVertex3f(1.95f, 6.475f, -8.7);
		glVertex3f(1.95f, 4.025f, -8.7);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(1.95f, 6.125f, -8.7f);//zid iznad prozora
		glVertex3f(1.95f, 6.475f, -8.7f);
		glVertex3f(3.75f, 6.475f, -8.7f);
		glVertex3f(3.75f, 6.125f, -8.7f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(1.95f, 4.025f, -8.7f);//zid ispod prozora do balkonskih vrata
		glVertex3f(1.95f, 4.725f, -8.7f);
		glVertex3f(2.95f, 4.725f, -8.7f);
		glVertex3f(2.95f, 4.025f, -8.7f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(3.75f, 4.025f, -8.7f);//zid od balkonskih vrata do zida
		glVertex3f(3.75f, 6.475f, -8.7f);
		glVertex3f(3.9f, 6.475f, -8.7f);
		glVertex3f(3.9f, 4.025f, -8.7f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(1.95f, 4.725f, -8.7f);//zidovi oko prozora i balk. vrata
		glVertex3f(1.95f, 6.125f, -8.7f);
		glVertex3f(1.95f, 6.125f, -9.1f);
		glVertex3f(1.95f, 4.725f, -9.1f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(1.95f, 6.125f, -8.7f);
		glVertex3f(3.75f, 6.125f, -8.7f);
		glVertex3f(3.75f, 6.125f, -9.1f);
		glVertex3f(1.95f, 6.125f, -9.1f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(1.95f, 4.725f, -8.7f);
		glVertex3f(2.95f, 4.725f, -8.7f);
		glVertex3f(2.95f, 4.725f, -9.1f);
		glVertex3f(1.95f, 4.725f, -9.1f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(2.95f, 4.025f, -8.7f);
		glVertex3f(2.95f, 4.725f, -8.7f);
		glVertex3f(2.95f, 4.725f, -9.1f);
		glVertex3f(2.95f, 4.025f, -9.1f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(3.75f, 4.025f, -8.7f);
		glVertex3f(3.75f, 6.125f, -8.7f);
		glVertex3f(3.75f, 6.125f, -9.1f);
		glVertex3f(3.75f, 4.025f, -9.1f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(3.9f, 4.025f, -8.7f);//zid izmeðu sobe 2
		glVertex3f(3.9f, 6.475f, -8.7f);
		glVertex3f(3.9f, 6.475f, -4.85f);
		glVertex3f(3.9f, 4.025f, -4.85f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(3.9f, 4.025f, -4.85f);//mali zid do vrata
		glVertex3f(3.9f, 6.475f, -4.85f);
		glVertex3f(3.65f, 6.475f, -4.85f);
		glVertex3f(3.65f, 4.025f, -4.85f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(3.65f, 6.025f, -4.85f);//mali zid iznad vrata
		glVertex3f(3.65f, 6.475f, -4.85f);
		glVertex3f(2.95f, 6.475f, -4.85f);
		glVertex3f(2.95f, 6.025f, -4.85f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(2.95f, 6.025f, -4.85f);//zidovi oko vrata
		glVertex3f(2.95f, 6.025f, -4.55f);
		glVertex3f(3.65f, 6.025f, -4.55f);
		glVertex3f(3.65f, 6.025f, -4.85f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(3.65f, 4.025f, -4.85f);
		glVertex3f(3.65f, 4.025f, -4.55f);
		glVertex3f(3.65f, 6.025f, -4.55f);
		glVertex3f(3.65f, 6.025f, -4.85f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(2.95f, 4.025f, -4.85f);
		glVertex3f(2.95f, 4.025f, -4.55f);
		glVertex3f(2.95f, 6.025f, -4.55f);
		glVertex3f(2.95f, 6.025f, -4.85f);

		//soba2
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(4.75f, 4.025f, -4.85f);//zid izmeðu radne sobe
		glVertex3f(4.75f, 6.475f, -4.85f);
		glVertex3f(7.7f, 6.475f, -4.85f);
		glVertex3f(7.7f, 4.025f, -4.85f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(7.7f, 4.025f, -4.85f);//zid istok
		glVertex3f(7.7f, 6.475f, -4.85f);
		glVertex3f(7.7f, 6.475f, -8.7f);
		glVertex3f(7.7f, 4.025f, -8.7f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(7.7f, 4.025f, -8.7f);//zid sjever do prozora
		glVertex3f(7.7f, 6.475f, -8.7f);
		glVertex3f(6.25f, 6.475f, -8.7f);
		glVertex3f(6.25f, 4.025f, -8.7f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(6.25f, 6.125f, -8.7f);//zid iznad prozora i balk. vrata
		glVertex3f(6.25f, 6.475f, -8.7f);
		glVertex3f(4.45f, 6.475f, -8.7f);
		glVertex3f(4.45f, 6.125f, -8.7f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(6.25f, 4.025f, -8.7f);//zid ispod prozora do balk. vrata
		glVertex3f(6.25f, 4.725f, -8.7f);
		glVertex3f(5.25f, 4.725f, -8.7f);
		glVertex3f(5.25f, 4.025f, -8.7f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(6.25f, 6.125f, -8.7f);//zidovi oko prozora i balk.vrata
		glVertex3f(6.25f, 6.125f, -9.1f);
		glVertex3f(4.45f, 6.125f, -9.1f);
		glVertex3f(4.45f, 6.125f, -8.7f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(6.25f, 4.725f, -8.7f); 
		glVertex3f(6.25f, 4.725f, -9.1f);
		glVertex3f(5.25f, 4.725f, -9.1f);
		glVertex3f(5.25f, 4.725f, -8.7f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(6.25f, 4.725f, -8.7f);
		glVertex3f(6.25f, 4.725f, -9.1f);
		glVertex3f(6.25f, 6.125f, -9.1f);
		glVertex3f(6.25f, 6.125f, -8.7f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(4.45f, 4.025f, -8.7f);
		glVertex3f(4.45f, 4.025f, -9.1f);
		glVertex3f(4.45f, 6.125f, -9.1f);
		glVertex3f(4.45f, 6.125f, -8.7f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(5.25f, 4.725f, -8.7f);
		glVertex3f(5.25f, 4.725f, -9.1f);
		glVertex3f(5.25f, 4.025f, -9.1f);
		glVertex3f(5.25f, 4.025f, -8.7f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(4.45f, 4.025f, -8.7f);//zid od balk. vrata do zida sobe 1
		glVertex3f(4.45f, 6.475f, -8.7f);
		glVertex3f(4.0f, 6.475f, -8.7f);
		glVertex3f(4.0f, 4.025f, -8.7f);

		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(4.0f, 4.025f, -8.7f);//zid izmeðu sobe 1
		glVertex3f(4.0f, 6.475f, -8.7f);
		glVertex3f(4.0f, 6.475f, -4.85f);
		glVertex3f(4.0f, 4.025f, -4.85f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(4.0f, 4.025f, -4.85f);//mali zid do vrata
		glVertex3f(4.0f, 6.475f, -4.85f);
		glVertex3f(4.05f, 6.475f, -4.85f);
		glVertex3f(4.05f, 4.025f, -4.85f);

		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(4.05f, 6.025f, -4.85f);//zid iznad vrata
		glVertex3f(4.05f, 6.475f, -4.85f);
		glVertex3f(4.75f, 6.475f, -4.85f);
		glVertex3f(4.75f, 6.025f, -4.85f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(4.05f, 6.025f, -4.85f);//zidovi oko vrata
		glVertex3f(4.05f, 6.025f, -4.55f);
		glVertex3f(4.75f, 6.025f, -4.55f);
		glVertex3f(4.75f, 6.025f, -4.85f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(4.05f, 6.025f, -4.85f);
		glVertex3f(4.05f, 6.025f, -4.55f);
		glVertex3f(4.05f, 4.025f, -4.55f);
		glVertex3f(4.05f, 4.025f, -4.85f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(4.75f, 6.025f, -4.85f);
		glVertex3f(4.75f, 6.025f, -4.55f);
		glVertex3f(4.75f, 4.025f, -4.55f);
		glVertex3f(4.75f, 4.025f, -4.85f);

		//vanjski zidovi
		//jug
		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(0.0f, 3.65f, 0.0f);//do prozora
		glVertex3f(0.0f, 6.475f, 0.0f);
		glVertex3f(1.7f, 6.475f, 0.0f);
		glVertex3f(1.7f, 3.65f, 0.0f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(1.7f, 6.025f, 0.0f);//iznad prozora
		glVertex3f(1.7f, 6.475f, 0.0f);
		glVertex3f(2.8f, 6.475f, 0.0f);
		glVertex3f(2.8f, 6.025f, 0.0f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(1.7f, 3.65f, 0.0f);//ispod prozora
		glVertex3f(1.7f, 4.825f, 0.0f);
		glVertex3f(2.8f, 4.825f, 0.0f);
		glVertex3f(2.8f, 3.65f, 0.0f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(2.8f, 3.65f, 0.0f);//od prozora do balk. vrata
		glVertex3f(2.8f, 6.475f, 0.0f);
		glVertex3f(3.65f, 6.475f, 0.0f);
		glVertex3f(3.65f, 3.65f, 0.0f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(3.65f, 6.025f, 0.0f);//iznad balk. vrata
		glVertex3f(3.65f, 6.475f, 0.0f);
		glVertex3f(4.4f, 6.475f, 0.0f);
		glVertex3f(4.4f, 6.025f, 0.0f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(3.65f, 3.65f, 0.0f);//ispod balk. vrata
		glVertex3f(3.65f, 4.025f, 0.0f);
		glVertex3f(4.4f, 4.025f, 0.0f);
		glVertex3f(4.4f, 3.65f, 0.0f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(4.4f, 3.65f, 0.0f);//od balk. vrata prema istoku
		glVertex3f(4.4f, 6.475f, 0.0f);
		glVertex3f(8.1f, 6.475f, 0.0f);
		glVertex3f(8.1f, 3.65f, 0.0f);

		//istok
		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(8.1f, 3.65f, 0.0f);
		glVertex3f(8.1f, 6.475f, 0.0f);
		glVertex3f(8.1f, 6.475f, -9.1f);
		glVertex3f(8.1f, 3.65f, -9.1f);

		//sjever
		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(8.1f, 3.65f, -9.1f);//do prozora
		glVertex3f(8.1f, 6.475f, -9.1f);
		glVertex3f(6.25f, 6.475f, -9.1f);
		glVertex3f(6.25f, 3.65f, -9.1f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(6.25f, 6.125f, -9.1f);//iznad prozora i balk. vrata 
		glVertex3f(6.25f, 6.475f, -9.1f);
		glVertex3f(4.45f, 6.475f, -9.1f);
		glVertex3f(4.45f, 6.125f, -9.1f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(6.25f, 3.65f, -9.1f);//ispod prozora do balk. vrata
		glVertex3f(6.25f, 4.725f, -9.1f);
		glVertex3f(5.25f, 4.725f, -9.1f);
		glVertex3f(5.25f, 3.65f, -9.1f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(5.25f, 3.65f, -9.1f);//ispod balk. vrata
		glVertex3f(5.25f, 4.025f, -9.1f);
		glVertex3f(4.45f, 4.025f, -9.1f);
		glVertex3f(4.45f, 3.65f, -9.1f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(4.45f, 3.65f, -9.1f);//izmeðu balk. vrata
		glVertex3f(4.45f, 6.475f, -9.1f);
		glVertex3f(3.75f, 6.475f, -9.1f);
		glVertex3f(3.75f, 3.65f, -9.1f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(3.75f, 6.125f, -9.1f);//iznad balk. vrata i prozora
		glVertex3f(3.75f, 6.475f, -9.1f);
		glVertex3f(1.95f, 6.475f, -9.1f);
		glVertex3f(1.95f, 6.125f, -9.1f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(3.75f, 3.65f, -9.1f);//ispod balk. vrata
		glVertex3f(3.75f, 4.025f, -9.1f);
		glVertex3f(2.95f, 4.025f, -9.1f);
		glVertex3f(2.95f, 3.65f, -9.1f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(2.95f, 3.65f, -9.1f);//ispod prozora od balk. vrata
		glVertex3f(2.95f, 4.725f, -9.1f);
		glVertex3f(1.95f, 4.725f, -9.1f);
		glVertex3f(1.95f, 3.65f, -9.1f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(1.95f, 3.65f, -9.1f);//od prozora prema zapadu
		glVertex3f(1.95f, 6.475f, -9.1f);
		glVertex3f(0.0f, 6.475f, -9.1f);
		glVertex3f(0.0f, 3.65f, -9.1f);

		//zapad
		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(0.0f, 3.65f, -9.1f);
		glVertex3f(0.0f, 6.475f, -9.1f);
		glVertex3f(0.0f, 6.475f, 0.0f);
		glVertex3f(0.0f, 3.65f, 0.0f);

		//balkon jug

		glColor3f(0.75f, 0.6f, 0.38f);
		glVertex3f(2.65f, 4.025f, 0.0f);//pod balk.
		glVertex3f(2.65f, 4.025f, 1.0f);
		glVertex3f(5.2f, 4.025f, 1.0f);
		glVertex3f(5.2f, 4.025f, 0.0f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(2.65f, 3.925f, 0.0f);
		glVertex3f(2.65f, 3.925f, 1.0f);
		glVertex3f(5.2f, 3.925f, 1.0f);
		glVertex3f(5.2f, 3.925f, 0.0f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(2.65f, 3.925f, 1.0f);
		glVertex3f(2.65f, 4.025f, 1.0f);
		glVertex3f(5.2f, 4.025f, 1.0f);
		glVertex3f(5.2f, 3.925f, 1.0f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(2.65f, 3.925f, 1.0f);
		glVertex3f(2.65f, 4.025f, 1.0f);
		glVertex3f(2.65f, 4.025f, 0.0f);
		glVertex3f(2.65f, 3.925f, 0.0f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(5.2f, 3.925f, 1.0f);
		glVertex3f(5.2f, 4.025f, 1.0f);
		glVertex3f(5.2f, 4.025f, 0.0f);
		glVertex3f(5.2f, 3.925f, 0.0f);

		glColor3f(0.87f, 0.87f, 0.87f);
		glVertex3f(2.65f, 4.025f, 0.0f);//ograda vanjska strana
		glVertex3f(2.65f, 4.8f, 0.0f);
		glVertex3f(2.65f, 4.8f, 1.0f);
		glVertex3f(2.65f, 4.025f, 1.0f);

		glColor3f(0.87f, 0.87f, 0.87f);
		glVertex3f(2.65f, 4.025f, 1.0f);
		glVertex3f(2.65f, 4.8f, 1.0f);
		glVertex3f(5.2f, 4.8f, 1.0f);
		glVertex3f(5.2f, 4.025f, 1.0f);

		glColor3f(0.87f, 0.87f, 0.87f);
		glVertex3f(5.2f, 4.025f, 1.0f);
		glVertex3f(5.2f, 4.8f, 1.0f);
		glVertex3f(5.2f, 4.8f, 0.0f);
		glVertex3f(5.2f, 4.025f, 0.0f);

		glColor3f(0.92f, 0.92f, 0.92f);
		glVertex3f(2.65f, 4.8f, 0.0f);//ograda iznad
		glVertex3f(2.7f, 4.8f, 0.0f);
		glVertex3f(2.7f, 4.8f, 1.0f);
		glVertex3f(2.65f, 4.8f, 1.0f);

		glColor3f(0.92f, 0.92f, 0.92f);
		glVertex3f(2.65f, 4.8f, 0.95f);
		glVertex3f(2.65f, 4.8f, 1.0f);
		glVertex3f(5.2f, 4.8f, 1.0f);
		glVertex3f(5.2f, 4.8f, 0.95f);

		glColor3f(0.92f, 0.92f, 0.92f);
		glVertex3f(5.15f, 4.8f, 0.0f);
		glVertex3f(5.15f, 4.8f, 1.0f);
		glVertex3f(5.2f, 4.8f, 1.0f);
		glVertex3f(5.2f, 4.8f, 0.0f);

		glColor3f(0.87f, 0.87f, 0.87f);
		glVertex3f(2.7f, 4.025f, 0.0f);//ograda unutarnja strana
		glVertex3f(2.7f, 4.8f, 0.0f);
		glVertex3f(2.7f, 4.8f, 0.95f);
		glVertex3f(2.7f, 4.025f, 0.95f);

		glColor3f(0.87f, 0.87f, 0.87f);
		glVertex3f(2.7f, 4.025f, 0.95f);
		glVertex3f(2.7f, 4.8f, 0.95f);
		glVertex3f(5.15f, 4.8f, 0.95f);
		glVertex3f(5.15f, 4.025f, 0.95f);

		glColor3f(0.87f, 0.87f, 0.87f);
		glVertex3f(5.15f, 4.025f, 0.0f);
		glVertex3f(5.15f, 4.8f, 0.0f);
		glVertex3f(5.15f, 4.8f, 0.95f);
		glVertex3f(5.15f, 4.025f, 0.95f);

		//balkon sjever
		glColor3f(0.75f, 0.6f, 0.38f);
		glVertex3f(0.0f, 4.025f, -9.1f);//pod
		glVertex3f(0.0f, 4.025f, -10.3f);
		glVertex3f(8.1f, 4.025f, -10.3f);
		glVertex3f(8.1f, 4.025f, -9.1f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(0.0f, 3.925f, -9.1f);//ispod
		glVertex3f(0.0f, 3.925f, -10.3f);
		glVertex3f(8.1f, 3.925f, -10.3f);
		glVertex3f(8.1f, 3.925f, -9.1f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(0.0f, 3.925f, -10.3f);//strane
		glVertex3f(0.0f, 4.025f, -10.3f);
		glVertex3f(8.1f, 4.025f, -10.3f);
		glVertex3f(8.1f, 3.925f, -10.3f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(8.1f, 3.925f, -9.1f);
		glVertex3f(8.1f, 4.025f, -9.1f);
		glVertex3f(8.1f, 4.025f, -10.3f);
		glVertex3f(8.1f, 3.925f, -10.3f);

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(0.0f, 3.925f, -9.1f);
		glVertex3f(0.0f, 4.025f, -9.1f);
		glVertex3f(0.0f, 4.025f, -10.3f);
		glVertex3f(0.0f, 3.925f, -10.3f);

		glColor3f(0.87f, 0.87f, 0.87f);
		glVertex3f(0.0f, 4.025f, -9.1f);//ograda vanjska strana
		glVertex3f(0.0f, 5.025f, -9.1f);
		glVertex3f(0.0f, 5.025f, -10.3f);
		glVertex3f(0.0f, 4.025f, -10.3f);

		glColor3f(0.87f, 0.87f, 0.87f);
		glVertex3f(0.0f, 4.025f, -10.3f);
		glVertex3f(0.0f, 5.025f, -10.3f);
		glVertex3f(8.1f, 5.025f, -10.3f);
		glVertex3f(8.1f, 4.025f, -10.3f);

		glColor3f(0.87f, 0.87f, 0.87f);
		glVertex3f(8.1f, 4.025f, -10.3f);
		glVertex3f(8.1f, 5.025f, -10.3f);
		glVertex3f(8.1f, 5.025f, -9.1f);
		glVertex3f(8.1f, 4.025f, -9.1f);

		glColor3f(0.92f, 0.92f, 0.92f);
		glVertex3f(0.0f, 5.025f, -10.3f);//ograda iznad strana
		glVertex3f(0.05f, 5.025f, -10.3f);
		glVertex3f(0.05f, 5.025f, -9.1f);
		glVertex3f(0.0f, 5.025f, -9.1f);

		glColor3f(0.92f, 0.92f, 0.92f);
		glVertex3f(0.0f, 5.025f, -10.3f);
		glVertex3f(0.0f, 5.025f, -10.25f);
		glVertex3f(8.1f, 5.025f, -10.25f);
		glVertex3f(8.1f, 5.025f, -10.3f);

		glColor3f(0.92f, 0.92f, 0.92f);
		glVertex3f(8.1f, 5.025f, -10.3f);
		glVertex3f(8.1f, 5.025f, -9.1f);
		glVertex3f(8.05f, 5.025f, -9.1f);
		glVertex3f(8.05f, 5.025f, -10.3f);

		glColor3f(0.87f, 0.87f, 0.87f);
		glVertex3f(0.05f, 4.025f, -9.1f);//ograda unutarnja strana
		glVertex3f(0.05f, 5.025f, -9.1f);
		glVertex3f(0.05f, 5.025f, -10.3f);
		glVertex3f(0.05f, 4.025f, -10.3f);

		glColor3f(0.87f, 0.87f, 0.87f);
		glVertex3f(0.05f, 4.025f, -10.25f);
		glVertex3f(0.05f, 5.025f, -10.25f);
		glVertex3f(8.05f, 5.025f, -10.25f);
		glVertex3f(8.05f, 4.025f, -10.25f);

		glColor3f(0.87f, 0.87f, 0.87f);
		glVertex3f(8.05f, 4.025f, -10.25f);
		glVertex3f(8.05f, 5.025f, -10.25f);
		glVertex3f(8.05f, 5.025f, -9.1f);
		glVertex3f(8.05f, 4.025f, -9.1f);

	glEnd();

	glBegin(GL_TRIANGLES);
		//zid do krova
		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(0.0f, 6.475f, 0.0f);
		glVertex3f(8.1f, 6.475f, 0.0f);
		glVertex3f(4.05f, 7.975f, 0.0f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glVertex3f(0.0f, 6.475f, -9.1f);
		glVertex3f(8.1f, 6.475f, -9.1f);
		glVertex3f(4.05f, 7.975f, -9.1f);
	glEnd();

	glBegin(GL_QUADS);
		//krov
		glColor3f(0.65f, 0.05f, 0.1f);
		glVertex3f(4.05f, 7.975f, 0.5f);//prema zapadu dolje
		glVertex3f(4.05f, 7.975f, -9.6f);
		glVertex3f(-0.5f, 6.29f, -9.6f);
		glVertex3f(-0.5f, 6.29f, 0.5f);

		glColor3f(0.65f, 0.05f, 0.1f);
		glVertex3f(4.05f, 7.975f, 0.5f);//prema istoku dolje
		glVertex3f(4.05f, 7.975f, -9.6f);
		glVertex3f(8.6f, 6.29f, -9.6f);
		glVertex3f(8.6f, 6.29f, 0.5f);

		glColor3f(0.75f, 0.05f, 0.05f);
		glVertex3f(4.05f, 8.175f, 0.5f);//prema zapadu gore
		glVertex3f(4.05f, 8.175f, -9.6f);
		glVertex3f(-0.5f, 6.49f, -9.6f);
		glVertex3f(-0.5f, 6.49f, 0.5f);

		glColor3f(0.75f, 0.05f, 0.05f);
		glVertex3f(4.05f, 8.175f, 0.5f);//prema zapadu gore
		glVertex3f(4.05f, 8.175f, -9.6f);
		glVertex3f(8.6f, 6.49f, -9.6f);
		glVertex3f(8.6f, 6.49f, 0.5f);

		glColor3f(0.7f, 0.1f, 0.0f);
		glVertex3f(4.05f, 8.175f, 0.5f);
		glVertex3f(4.05f, 7.975f, 0.5f);
		glVertex3f(-0.5f, 6.29f, 0.5f);
		glVertex3f(-0.5f, 6.49f, 0.5f);

		glColor3f(0.7f, 0.1f, 0.0f);
		glVertex3f(4.05f, 8.175f, 0.5f);
		glVertex3f(4.05f, 7.975f, 0.5f);
		glVertex3f(8.6f, 6.29f, 0.5f);
		glVertex3f(8.6f, 6.49f, 0.5f);

		glColor3f(0.7f, 0.1f, 0.0f);
		glVertex3f(4.05f, 8.175f, -9.6f);
		glVertex3f(4.05f, 7.975f, -9.6f);
		glVertex3f(8.6f, 6.29f, -9.6f);
		glVertex3f(8.6f, 6.49f, -9.6f);

		glColor3f(0.7f, 0.1f, 0.0f);
		glVertex3f(4.05f, 8.175f, -9.6f);
		glVertex3f(4.05f, 7.975f, -9.6f);
		glVertex3f(-0.5f, 6.29f, -9.6f);
		glVertex3f(-0.5f, 6.49f, -9.6f);

		glColor3f(0.7f, 0.1f, 0.0f);
		glVertex3f(-0.5f, 6.49f, 0.5f);
		glVertex3f(-0.5f, 6.29f, 0.5f);
		glVertex3f(-0.5f, 6.29f, -9.6f);
		glVertex3f(-0.5f, 6.49f, -9.6f);

		glColor3f(0.7f, 0.1f, 0.0f);
		glVertex3f(8.6f, 6.49f, 0.5f);
		glVertex3f(8.6f, 6.29f, 0.5f);
		glVertex3f(8.6f, 6.29f, -9.6f);
		glVertex3f(8.6f, 6.49f, -9.6f);

	glEnd();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glPushMatrix();//ulazna vrata
	if (doorUlazna) {
		glTranslatef(3.55f, 0.0f, -0.05f);
		glRotatef(90, 0.0f, 1.0f, 0.0f);
		glTranslatef(-3.55f, 0.0f, 0.05f);
	}
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(3.55f, 1.05f, -0.05f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(3.55f, 3.05f, -0.05f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(4.7f, 3.05f, -0.05f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(4.7f, 1.05f, -0.05f);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(3.55f, 1.05f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(3.55f, 3.05f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(4.7f, 3.05f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(4.7f, 1.05f, 0.0f);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(3.55f, 3.05f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(3.55f, 3.05f, -0.05f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(4.7f, 3.05f, -0.05f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(4.7f, 3.05f, 0.0f);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(3.55f, 1.05f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(3.55f, 3.05f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(3.55f, 3.05f, -0.05f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(3.55f, 1.05f, -0.05f);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(4.7f, 1.05f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(4.7f, 3.05f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(4.7f, 3.05f, -0.05f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(4.7f, 1.05f, -0.05f);
	glEnd();
	glPopMatrix();
	
	glPushMatrix();//kuhinjska vrata
	if (doorKuhinja) {
		glTranslatef(3.25f, 0.0f, -2.5f);
		glRotatef(90, 0.0f, 1.0f, 0.0f);
		glTranslatef(-3.25f, 0.0f, 2.5f);
	}
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(3.25f, 1.05f, -2.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(3.25f, 3.05f, -2.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(3.25f, 3.05f, -3.2f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(3.25f, 1.05f, -3.2f);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(3.3f, 1.05f, -2.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(3.3f, 3.05f, -2.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(3.3f, 3.05f, -3.2f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(3.3f, 1.05f, -3.2f);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(3.25f, 3.05f, -2.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(3.3f, 3.05f, -2.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(3.3f, 3.05f, -3.2f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(3.25f, 3.05f, -3.2f);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(3.3f, 1.05f, -3.2f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(3.3f, 3.05f, -3.2f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(3.25f, 3.05f, -3.2f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(3.25f, 1.05f, -3.2f);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(3.3f, 1.05f, -2.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(3.3f, 3.05f, -2.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(3.25f, 3.05f, -2.5f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(3.25f, 1.05f, -2.5f);
	glEnd();
	glPopMatrix();

	glPushMatrix();//vrata dnevna
	if (doorDnevna) {
		glTranslatef(4.5f, 0.0f, -4.7f);
		glRotatef(-90, 0.0f, 1.0f, 0.0f);
		glTranslatef(-4.5f, 0.0f, 4.7f);
	}
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(3.75f, 1.05f, -4.7f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(3.75f, 3.05f, -4.7f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(4.5f, 3.05f, -4.7f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(4.5f, 1.05f, -4.7f);
			   
		glTexCoord2f(0.0f, 0.0f); glVertex3f(3.75f, 1.05f, -4.65f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(3.75f, 3.05f, -4.65f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(4.5f, 3.05f, -4.65f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(4.5f, 1.05f, -4.65f);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(3.75f, 3.05f, -4.65f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(3.75f, 3.05f, -4.7f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(4.5f, 3.05f, -4.7f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(4.5f, 3.05f, -4.65f);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(3.75f, 1.05f, -4.65f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(3.75f, 3.05f, -4.65f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(3.75f, 3.05f, -4.7f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(3.75f, 1.05f, -4.7f);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(4.5f, 1.05f, -4.65f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(4.5f, 3.05f, -4.65f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(4.5f, 3.05f, -4.7f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(4.5f, 1.05f, -4.7f);
	glEnd();
	glPopMatrix();

	glPushMatrix();//vrata soba dolje
	if (doorSobaDolje) {
		glTranslatef(4.8f, 0.0f, -4.7f);
		glRotatef(90, 0.0f, 1.0f, 0.0f);
		glTranslatef(-4.8f, 0.0f, 4.7f);
	}
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(4.8f, 1.05f, -4.7f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(4.8f, 3.05f, -4.7f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(5.5f, 3.05f, -4.7f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(5.5f, 1.05f, -4.7f);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(4.8f, 1.05f, -4.65f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(4.8f, 3.05f, -4.65f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(5.5f, 3.05f, -4.65f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(5.5f, 1.05f, -4.65f);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(4.8f, 3.05f, -4.65f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(4.8f, 3.05f, -4.7f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(5.5f, 3.05f, -4.7f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(5.5f, 3.05f, -4.65f);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(4.8f, 1.05f, -4.65f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(4.8f, 3.05f, -4.65f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(4.8f, 3.05f, -4.7f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(4.8f, 1.05f, -4.7f);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(5.5f, 1.05f, -4.65f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(5.5f, 3.05f, -4.65f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(5.5f, 3.05f, -4.7f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(5.5f, 1.05f, -4.7f);
	glEnd();
	glPopMatrix();

	glPushMatrix();//vrata kupatilo
	if (doorKupatilo) {
		glTranslatef(5.7f, 0.0f, -4.4f);
		glRotatef(90, 0.0f, 1.0f, 0.0f);
		glTranslatef(-5.7f, 0.0f, 4.4f);
	}
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(5.7f, 1.05f, -4.4f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(5.7f, 3.05f, -4.4f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(5.7f, 3.05f, -3.6f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(5.7f, 1.05f, -3.6f);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(5.65f, 1.05f, -4.4f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(5.65f, 3.05f, -4.4f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(5.65f, 3.05f, -3.6f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(5.65f, 1.05f, -3.6f);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(5.7f, 3.05f, -4.4f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(5.65f, 3.05f, -4.4f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(5.65f, 3.05f, -3.6f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(5.7f, 3.05f, -3.6f);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(5.7f, 1.05f, -4.4f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(5.7f, 3.05f, -4.4f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(5.65f, 3.05f, -4.4f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(5.65f, 1.05f, -4.4f);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(5.7f, 1.05f, -3.6);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(5.7f, 3.05f, -3.6f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(5.65f, 3.05f, -3.6f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(5.65f, 1.05f, -3.6f);

	glEnd();
	glPopMatrix();

	glPushMatrix();//vrata gym
	if (doorGym) {
		glTranslatef(3.15f, 0.0f, -2.7f);
		glRotatef(-90, 0.0f, 1.0f, 0.0f);
		glTranslatef(-3.15f, 0.0f, 2.7f);
	}
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(3.15f, 4.025f, -2.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(3.15f, 6.025f, -2.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(3.15f, 6.025f, -2.7f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(3.15f, 4.025f, -2.7f);
	
	glTexCoord2f(0.0f, 0.0f); glVertex3f(3.2f, 4.025f, -2.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(3.2f, 6.025f, -2.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(3.2f, 6.025f, -2.7f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(3.2f, 4.025f, -2.7f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(3.2f, 6.025f, -2.7f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(3.15f, 6.025f, -2.7f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(3.15f, 6.025f, -2.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(3.2f, 6.025f, -2.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(3.2f, 4.025f, -2.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(3.2f, 6.025f, -2.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(3.15f, 6.025f, -2.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(3.15f, 4.025f, -2.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(3.2f, 4.025f, -2.7f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(3.2f, 6.025f, -2.7f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(3.15f, 6.025f, -2.7f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(3.15f, 4.025f, -2.7f);
	glEnd();
	glPopMatrix();

	glPushMatrix();//vrata kupatilo gore
	if (doorKupatiloGore) {
		glTranslatef(2.65f, 0.0f, -3.1f);
		glRotatef(90, 0.0f, 1.0f, 0.0f);
		glTranslatef(-2.65f, 0.0f, 3.1f);
	}
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(2.65f, 4.025f, -3.1f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(2.65f, 6.025f, -3.1f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2.65f, 6.025f, -3.8f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(2.65f, 4.025f, -3.8f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(2.7f, 4.025f, -3.1f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(2.7f, 6.025f, -3.1f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2.7f, 6.025f, -3.8f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(2.7f, 4.025f, -3.8f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(2.65f, 6.025f, -3.1f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(2.65f, 6.025f, -3.8f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2.7f, 6.025f, -3.8f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(2.7f, 6.025f, -3.1f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(2.65f, 4.025f, -3.1f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(2.65f, 6.025f, -3.1f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2.7f, 6.025f, -3.1f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(2.7f, 4.025f, -3.1f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(2.65f, 4.025f, -3.8f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(2.65f, 6.025f, -3.8f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2.7f, 6.025f, -3.8f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(2.7f, 4.025f, -3.8f);

	glEnd();
	glPopMatrix();

	glPushMatrix();//vrata soba 1
	if (doorSoba1) {
		glTranslatef(3.65f, 0.0f, -4.85f);
		glRotatef(-90, 0.0f, 1.0f, 0.0f);
		glTranslatef(-3.65f, 0.0f, 4.85f);
	}
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(2.95f, 4.025f, -4.85f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(2.95f, 6.025f, -4.85f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(3.65f, 6.025f, -4.85f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(3.65f, 4.025f, -4.85f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(2.95f, 4.025f, -4.8f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(2.95f, 6.025f, -4.8f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(3.65f, 6.025f, -4.8f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(3.65f, 4.025f, -4.8f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(2.95f, 6.025f, -4.8f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(2.95f, 6.025f, -4.85f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(3.65f, 6.025f, -4.85f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(3.65f, 6.025f, -4.8f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(3.65f, 4.025f, -4.85f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(3.65f, 6.025f, -4.85f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(3.65f, 6.025f, -4.8f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(3.65f, 4.025f, -4.8f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(2.95f, 4.025f, -4.85f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(2.95f, 6.025f, -4.85f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2.95f, 6.025f, -4.8f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(2.95f, 4.025f, -4.8f);

	glEnd();
	glPopMatrix();

	glPushMatrix();//vrata soba 2
	if (doorSoba2) {
		glTranslatef(4.05f, 0.0f, -4.85f);
		glRotatef(90, 0.0f, 1.0f, 0.0f);
		glTranslatef(-4.05f, 0.0f, 4.85f);
	}
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(4.05f, 4.025f, -4.85f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(4.05f, 6.025f, -4.85f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(4.75f, 6.025f, -4.85f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(4.75f, 4.025f, -4.85f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(4.05f, 4.025f, -4.8f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(4.05f, 6.025f, -4.8f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(4.75f, 6.025f, -4.8f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(4.75f, 4.025f, -4.8f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(4.05f, 6.025f, -4.8f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(4.05f, 6.025f, -4.85f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(4.75f, 6.025f, -4.85f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(4.75f, 6.025f, -4.8f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(4.75f, 4.025f, -4.8f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(4.75f, 6.025f, -4.8f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(4.75f, 6.025f, -4.85f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(4.75f, 4.025f, -4.85f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(4.05f, 4.025f, -4.8f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(4.05f, 6.025f, -4.8f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(4.05f, 6.025f, -4.85f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(4.05f, 4.025f, -4.85f);

	glEnd();
	glPopMatrix();

	glPushMatrix();//vrata radna soba
	if (doorRadnaSoba) {
		glTranslatef(5.0f, 0.0f, -4.3f);
		glRotatef(90, 0.0f, 1.0f, 0.0f);
		glTranslatef(-5.0f, 0.0f, 4.3f);
	}
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0f, 4.025f, -3.6f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0f, 6.025f, -3.6f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(5.0f, 6.025f, -4.3f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0f, 4.025f, -4.3f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(4.95f, 4.025f, -3.6f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(4.95f, 6.025f, -3.6f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(4.95f, 6.025f, -4.3f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(4.95f, 4.025f, -4.3f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0f, 6.025f, -3.6f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0f, 6.025f, -4.3f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(4.95f, 6.025f, -4.3f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(4.95f, 6.025f, -3.6f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0f, 4.025f, -3.6f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0f, 6.025f, -3.6f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(4.95f, 6.025f, -3.6f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(4.95f, 4.025f, -3.6f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0f, 4.025f, -4.3f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0f, 6.025f, -4.3f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(4.95f, 6.025f, -4.3f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(4.95f, 4.025f, -4.3f);

	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27) {
		exit(0);
	}
	else if (key == 13) {
		doorNumber = CheckDoor();
		switch (doorNumber) {
		case 1:
			doorUlazna = !doorUlazna;
			break;
		case 2:
			doorKuhinja = !doorKuhinja;
			break;
		case 3:
			doorDnevna = !doorDnevna;
			break;
		case 4:
			doorSobaDolje = !doorSobaDolje;
			break;
		case 5:
			doorKupatilo = !doorKupatilo;
			break;
		case 6:
			doorGym = !doorGym;
			break;
		case 7:
			doorKupatiloGore = !doorKupatiloGore;
			break;
		case 8:
			doorSoba1 = !doorSoba1;
			break;
		case 9:
			doorSoba2 = !doorSoba2;
			break;
		case 10:
			doorRadnaSoba = !doorRadnaSoba;
			break;
		}
	}

	glutPostRedisplay();
}

void processSpecialKeys(int key, int xx, int yy) {

	float fraction = 0.25f;

	switch (key) {
	case GLUT_KEY_LEFT:
		angle -= 0.08f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_RIGHT:
		angle += 0.08f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_UP:
		x += lx * fraction;
		z += lz * fraction;
		break;
	case GLUT_KEY_DOWN:
		x -= lx * fraction;
		z -= lz * fraction;
		break;
	case GLUT_KEY_PAGE_UP:
		y += 0.25;
		break;
	case GLUT_KEY_PAGE_DOWN:
		y -= 0.25;
		break;
	case GLUT_KEY_END:
		angley -= 0.08f;
		ly = tan(angley);
		break;
	case GLUT_KEY_HOME:
		angley += 0.08f;
		ly = tan(angley);
		break;
	case GLUT_KEY_INSERT:
		doorNumber = CheckDoor();
		switch (doorNumber) {
		case 1:
			doorUlazna = !doorUlazna;
			break;
		case 2:
			doorKuhinja = !doorKuhinja;
			break;
		case 3:
			doorDnevna = !doorDnevna;
			break;
		case 4:
			doorSobaDolje = !doorSobaDolje;
			break;
		case 5:
			doorKupatilo = !doorKupatilo;
			break;
		case 6:
			doorGym = !doorGym;
			break;
		case 7:
			doorKupatiloGore = !doorKupatiloGore;
			break;
		case 8:
			doorSoba1 = !doorSoba1;
			break;
		case 9:
			doorSoba2 = !doorSoba2;
			break;
		case 10:
			doorRadnaSoba = !doorRadnaSoba;
			break;
		}
	}

	glutPostRedisplay();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("3D Model Kuce");
	glEnable(GL_DEPTH_TEST);

	texture = LoadTexture("door.bmp");

	glClearDepth(1);
	glutReshapeFunc(changeSize);
	glutDisplayFunc(render);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);

	glutMainLoop();
	return 0;
}