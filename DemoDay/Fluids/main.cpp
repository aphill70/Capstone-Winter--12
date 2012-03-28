#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>


#include "ParticleSystem.h"

void redraw();
void modelInit();



ParticleSystem * sys;


int main(int argc, char* argv[])
{	
	sys = new ParticleSystem();
	
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(800,600);
    glutCreateWindow("Boid Simulator");
    glutDisplayFunc(redraw);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(8.0);

	sys->initParticles(100, glm::vec4(glm::vec3(0, 0, 0), 0));

	modelInit();


	glutIdleFunc(redraw);
    glutMainLoop();
	

	while(true)

	return 0;
}


void redraw()
{
	sys->updateSimulation();
	

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLfloat pointList[1][3];

	GLfloat color[4];
	color[0] = .7;
	color[1] = .2;
	color[2] = .4;
	color[3] = .5;

	glBegin(GL_POINTS);
	for(int i = 0; i < sys->getNumberOfParticles(); i++)
	{
		float* tmp = sys->getParticlePosition(i);
		pointList[0][0] = tmp[0];
		pointList[0][1] = tmp[1];
		pointList[0][2] = tmp[2];

		glColor4fv(color);
		glVertex3fv(pointList[0]);
	}
	glEnd();
	glutSwapBuffers();	
}

void modelInit()
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}