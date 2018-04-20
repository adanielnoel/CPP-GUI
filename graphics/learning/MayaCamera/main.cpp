///-----------------------------------------------------------------------------
/// 
/// \file	main.cpp
/// \author	Rob Bateman
/// \brief	A simple viewer for a simple file format.
/// \note
///
/// 		This sourcefile is provided as-is and no guarentee or warrenty
///			is provided for any damage that may arise from using it. This
///			code is free for your own use, the only thing i ask is a small
///			credit somewhere for my work. An e-mail saying you found it
///			useful would also be much appreciated by myself ;)
///
///			If you find any bugs arising to maya version changes or you
///			have improvements to the source code, then feel free to mail
///			them to me and i'll update the source-code on the website.
///			Before you e-mail bug reports, make sure that it is my code
///			causing the problem, and not some wierd oddity in the maya
///			scene. This code is designed to handle most common model set-ups,
///			it is not designed to replicate perfectly everything that maya
///			does.
///
///			DO NOT send me feature requests, i'm not likely to add to this
///			code myself unless something really new and groovy comes along.
///			This is mainly due to my current time restrictions. If you do
///			have code written that adds something fairly groovy to the
///			exporter, then i will happily update the exporter code and give
///			full credit where due.....
///
///			Robert Bateman
///
///				email code to:    rbateman@bmth.ac.uk
///				contact:          robthebloke@hotmail.com
///
//-------------------------------------------------------------------------------

#include <iostream>
#include <GL/glut.h>


float zoom = 15.0f;
float rotx = 0;
float roty = 0.001f;
float tx = 0;
float ty = 0;
int lastx=0;
int lasty=0;
unsigned char Buttons[3] = {0};


//-------------------------------------------------------------------------------
/// \brief	Initialises the openGL scene
/// 
void Init() 
{
	glEnable(GL_DEPTH_TEST);
}

//-------------------------------------------------------------------------------
/// \brief	Draws the scene
/// 
void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();

	glTranslatef(0,0,-zoom);
	glTranslatef(tx,ty,0);
	glRotatef(rotx,1,0,0);
	glRotatef(roty,0,1,0);	

	// draw grid
	glBegin(GL_LINES);
	for(int i=-10;i<=10;++i) {
		glVertex3f(i,0,-10);
		glVertex3f(i,0,10);

		glVertex3f(10,0,i);
		glVertex3f(-10,0,i);
	}
	glEnd();

	glutSwapBuffers();
}

//-------------------------------------------------------------------------------
/// \brief	Called when the screen gets resized
/// \param	w	-	the new width
/// \param	h	-	the new height
/// 
void reshape(int w, int h)
{
	// prevent divide by 0 error when minimised
	if(w==0) 
		h = 1;

	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,(float)w/h,0.1,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


//-------------------------------------------------------------------------------
//
void Motion(int x,int y)
{
	int diffx=x-lastx;
	int diffy=y-lasty;
	lastx=x;
	lasty=y;

	if( Buttons[0] && Buttons[1] )
	{
		zoom -= (float) 0.05f * diffx;
	}
	else
		if( Buttons[0] )
		{
			rotx += (float) 0.5f * diffy;
			roty += (float) 0.5f * diffx;		
		}
		else
			if( Buttons[1] )
			{
				tx += (float) 0.05f * diffx;
				ty -= (float) 0.05f * diffy;
			}
			glutPostRedisplay();
}

//-------------------------------------------------------------------------------
//
void Mouse(int b,int s,int x,int y)
{
	lastx=x;
	lasty=y;
	switch(b)
	{
	case GLUT_LEFT_BUTTON:
		Buttons[0] = ((GLUT_DOWN==s)?1:0);
		break;
	case GLUT_MIDDLE_BUTTON:
		Buttons[1] = ((GLUT_DOWN==s)?1:0);
		break;
	case GLUT_RIGHT_BUTTON:
		Buttons[2] = ((GLUT_DOWN==s)?1:0);
		break;
	default:
		break;		
	}
	glutPostRedisplay();
}

//-------------------------------------------------------------------------------
///
int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Maya Camera");


	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);

	Init();

	glutMainLoop();
}

























