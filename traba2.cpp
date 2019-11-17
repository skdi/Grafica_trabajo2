#include<GL/glut.h>
#include<iostream>
#include<math.h>
using namespace std;

void display(void);

void reshape(int, int);
void func_mouse(int button, int state, int x, int y);
void func_teclado(unsigned char c, int x, int y);
void special_teclado(int key, int x, int y);
float angle = 0;
bool camDefault = true, p_ortho = true;

float iX =0  , iY = -0.8, iZ = 0, lX = 0, lY = -0.8, lZ = -1, uX = 0, uY = 1, uZ = 0; 
float camAngle = 0;
//VALORES DE POSICION
GLfloat xi = 0.0;
GLfloat yi = 0.0;
//Valor inicial de escalamiento
GLint escala = 1;
//Angulo inicial para la rotacion
GLdouble angulo = 0;
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(512, 512);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("Camarita");
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0,0,0,1);
	glutMouseFunc(func_mouse);
	glutKeyboardFunc(func_teclado);
	glutSpecialFunc(special_teclado);
	glutMainLoop();

	return 0;
}


void drawEarth() {

    glColor3f(0,255,0);
    glBegin(GL_QUADS);
    glVertex3f(-3,-3,-3);
    glVertex3f(-3,-3,3);
    glVertex3f(3,-3,3);
    glVertex3f(3,-3,-3);
    glEnd();
}

void drawWalls() {

	glColor3f(0, 222, 222);
	glBegin(GL_QUADS);
	glTranslatef( 0.0, 0.0, 0.0);
	glutSolidCube(0.5);
	glEnd();


}

float unitVector(float dir, float x, float y, float z) {
    float sumSqr = pow(x,2) + pow(y,2) + pow(z,2);
    return dir / (sqrt(sumSqr));
}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(255,0,0);

    glPushMatrix();
    glTranslatef(iX,iY,iZ); // translacion del centro de camara
    glRotatef(-camAngle , 0,1,0); // rotar hacia la camara
    glTranslatef(0.05,0,-0.05); // Offset del objetp
   	glScaled(escala, escala, 1.0);
	glRotated(angulo, 0.0, 0.0, 1.0);
    glPopMatrix();
    drawWalls();
    drawEarth();
    glutSwapBuffers();
}



void reshape(int width, int height)
{

    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90,1, 0.001, 1000);
    gluLookAt(iX,iY,iZ,iX+lX,lY,iZ+lZ,uX,uY,uZ);
    glMatrixMode(GL_MODELVIEW);

}

void updateLookAt() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (90, 1 ,0.001, 1000);
    gluLookAt(iX,iY,iZ,iX+lX,lY,iZ+lZ,uX,uY,uZ);
    glMatrixMode(GL_MODELVIEW);
}


void func_teclado(unsigned char c, int x, int y) {
    switch(c) {
    case 'a':  // izq
        iX-=0.01;
        lX -= 0.01;
        break;

    case 'd':  // der
        iX += 0.01;
        lX += 0.01;
        break;

    case 'w': // arriba
        iZ += 0.01;
        lZ += 0.01;
        break;

    case 's': // abajo
        iZ -= 0.01;
        lZ -= 0.01;
        break;
        //Escalamiento
	case GLUT_KEY_HOME:
		escala = escala + 1;
		break;
	case GLUT_KEY_PAGE_UP:
		if (escala <=1)
			escala = 1;
		else
			escala = escala - 1;
		break;
	case GLUT_KEY_PAGE_DOWN:
		angulo +=5;
		break;
	case GLUT_KEY_END:
		angulo -=5;
		break;
    default:
        break;
    }

    updateLookAt();

}

void func_mouse(int button, int state, int x, int y) {
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        if(camDefault) {
            glLoadIdentity();
            gluLookAt(-1,0,0,0.0,0.0,0.0,0.0,1,0);
            camDefault = false;
        }

        else {
            glLoadIdentity();
            gluLookAt(0,0,0,0.0,0.0,0.0,0.0,1,0);
            camDefault = true;
        }
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { 
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if(p_ortho) {
            gluPerspective (90, 1 ,0.00001, 1000);
            p_ortho = false;
        }

        else {

            glOrtho(-1,1,-1,1, -1,100);
            p_ortho = true;

        }
        glMatrixMode(GL_MODELVIEW);

        glLoadIdentity();
        if(camDefault)
                gluLookAt(0,0,1,0.0,0.0,0.0,0.0,1,0);
            else 
                gluLookAt(-1,0,0,0.0,0.0,0.0,0.0,1,0);


    }
}



void special_teclado(int key, int x, int y) {

    float fraction = 0.05f;

	switch (key) {
	    case GLUT_KEY_LEFT :
	        camAngle -= 0.01f;
	        lX = sin(camAngle);
	        lZ = -cos(camAngle);
	        break;
	    case GLUT_KEY_RIGHT :
	        camAngle += 0.01f;
	        lX = sin(camAngle);
	        lZ = -cos(camAngle);
	        break;
	    case GLUT_KEY_UP :
	        iX += lX * fraction;
	        iZ += lZ * fraction;
	        break;
	    case GLUT_KEY_DOWN :
	        iX -= lX * fraction;
	        iZ -= lZ * fraction;
	        break;

	    default:
	        break;
	    }
    updateLookAt();
 }
