#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include "glm.h"

GLfloat eye[3] = { 0.0, 0.0, 2.0 };
GLfloat at[3]  = { 0.0, 0.0, 0.0 };
GLfloat up[3]  = { 0.0, 1.0, 0.0 };

GLMmodel* pModel = NULL;

void refreshModel(void);
void prepareScene();

GLdouble projection[16], modelview[16], inverse[16];
GLuint window, world, screen, command;
GLuint sub_width = 256, sub_height = 256;
static float angleX=0.0,angleY=0.0;
int rx=0,ry=0,rz=0;
float px=0,py=0,pz=0;
float scale=1;
float lx=1;
float ly=1;


GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat specular[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat shininess = 65.0;  
GLfloat positionLight[] = { 1.0 , 1.0 , 1.0 , 1.0 };

//Pour mouvement/zoom
void inputKey(unsigned char key, int x1, int y1){

	switch (key) {
	//case GLUT_KEY_LEFT : 
	//	angleX--;
	//	if (angleX>360) angleX=0;                 
	//	break;
	//case GLUT_KEY_RIGHT : 
	//	angleX++;
	//	if (angleX<-360) angleX=0; 
	//	break;
	//case GLUT_KEY_UP : 
	//	angleY--;
	//	if (angleY>360) angleY=0; 
	//	break;
	//case GLUT_KEY_DOWN : 
	//	angleY++;
	//	if (angleY<-360) angleY=0; 
	//	break;
	case 'l' :
		px=px+0.3;
		break;
	case 'j' :
		px=px-0.3;
		break;
	case 'i' :
		py=py+0.3;
		break;
	case 'k' :
		py=py-0.3;
		break;
	case 'u' :
		pz=pz+0.3;
		break;
	case 'o' :
		pz=pz-0.3;
		break;
	case 's' :
		scale=1.06;
		glmScale(pModel,scale);
		break;
	case 'x' :
		scale=0.94;
		glmScale(pModel,scale);
		break;
	case 'S':
		if(lx==1) lx=-1;
		else
			lx=1;
		break;
	case 'X':
		if(ly==1) ly=-1;
		else
			ly=1;
		break;

	} refreshModel();
}

//Pour rotation
void inputKey2(int key, int x1, int y1){

	switch (key) {
	case GLUT_KEY_LEFT : 
		angleX-=6;
		if (angleX>360) angleX=0;                 

		break;
	case GLUT_KEY_RIGHT : 
		angleX+=6;
		if (angleX<-360) angleX=0; 

		break;
	case GLUT_KEY_UP : 
		angleY-=6;
		if (angleY>360) angleY=0; 
		break;
	case GLUT_KEY_DOWN : 
		angleY+=6;
		if (angleY<-360) angleY=0; 
		break;
	} refreshModel();
}

void reshape(int width, int height){
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)width/height, 1.0, 8.0);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], up[0], up[1],up[2]);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glClearColor(0.2, 0.2, 0.2, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

}

void prepareScene(){
	float ptSize = 3;
	float flo;
	float xmin=-50;
	float xmax=50;
	float dx=5;
	float x;
	float ymin=-50;
	float ymax=50;
	float dy=5;
	float y;

	for (  flo = -50; flo < 50; flo++ )
	{
		glPointSize(ptSize);
		glBegin(GL_POINTS);
		glVertex3d(flo,0,0);
		glEnd();
	}

	for ( flo = -50; flo < 50; flo++ )
	{
		glPointSize(ptSize);
		glBegin(GL_POINTS);
		glVertex3d(0,flo,0);
		glEnd();
	}

	glBegin(GL_LINES);

	for(x=xmin; x<=xmax; x+=dx)
	{
		for(y=ymin; y<=ymax; y+=dy)
		{
			glVertex3f(x, ymin, 0.0);
			glVertex3f(x, ymax, 0.0);

			glVertex3f(xmin, y, 0.0);
			glVertex3f(xmax, y, 0.0);
		}
	}

	glEnd();/*
	glRotatef(angleX,   rx,  1,   rz);
	glRotatef(angleY,   1,    ry,   rz);
	glTranslatef(px, py, pz);*/
	glFlush ();
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!pModel) {
		pModel = glmReadOBJ("obj/castle.obj");
		if (!pModel) exit(0);
		glmUnitize(pModel);
		glmFacetNormals(pModel);
		glmVertexNormals(pModel, 90.0);
	}
	positionLight[0]=lx;
	positionLight[1]=ly;
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);	

	glLightfv( GL_LIGHT0 , GL_POSITION , positionLight ); 

	glPushMatrix();		
	glRotatef(angleX,   rx,  1,   rz);
	glRotatef(angleY,   1,    ry,   rz);
	glTranslatef(px, py, pz);
	glmDraw(pModel, GLM_SMOOTH | GLM_MATERIAL);
	glPopMatrix();
	glFlush ();

	/* Draw grid */
	prepareScene();

	glutSwapBuffers();
}

//Mouvement de souris
void Load_Obj(int value){
	char* name = 0;
	GLint params[2];
	switch (value) {

	case 1:
		name = "obj/castle.obj";
		break;
	case 2:
		name = "obj/teapot.obj";
		break;
	case 3:
		name = "obj/untitled.obj";
		break;
	case 4:
		name = "obj/suzanne.obj";
		break;
	case 'S':
		glmWriteOBJ(pModel, "nouveauOBJ.obj", GLM_SMOOTH);
		break;
	case 'F':
		glGetIntegerv(GL_POLYGON_MODE, params);
		if (params[0] == GL_FILL)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}

	if (name) {
		pModel = glmReadOBJ(name);
		if (!pModel) exit(0);
		glmUnitize(pModel);
		glmFacetNormals(pModel);
		glmVertexNormals(pModel, 90.0);
	}

	refreshModel();
}



void refreshModel(void){
	glutPostRedisplay();
}


//main function
int main(int argc, char** argv){	
	printf(" ************************************************* \n");
	printf(" ************************************************* \n");
	printf(" ****    Sokaris - OBJ Outil version 0.0.1    **** \n");
	printf(" ************************************************* \n");
	printf(" ************************************************* \n\n");
	printf(" Cliquer sur le bouton droite pour Menu de chargement\n\n");
	printf(" Zoomer                  : s / x \n");
	printf(" Orientation de lumi¨¨re  : S / X \n");
	printf(" Rotation                : Up / Down / Left / Right\n");
	printf(" Axe X                   : l / j \n");
	printf(" Axe Y                   : i / k \n");
	printf(" Axe Z                   : u / o \n");
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(50, 50);
	glutInit(&argc, argv);
	glutCreateWindow("Sokaris - OBJ Model Verifier");
	glutKeyboardFunc(inputKey);
	glutSpecialFunc(inputKey2);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutCreateMenu(Load_Obj);
	glutAddMenuEntry("Enregistrer(Test)", 'S');
	glutAddMenuEntry("Texture On/Off", 'F');
	glutAddMenuEntry("--- Fichiers dans ./obj ---", ' ');
	glutAddMenuEntry("castle", 1);
	glutAddMenuEntry("teapot", 2);
	glutAddMenuEntry("untitled", 3);
	glutAddMenuEntry("suzanne", 4);
	glutAddMenuEntry("---------------------------", ' ');
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	refreshModel();

	glutMainLoop();

	return 0;
}
