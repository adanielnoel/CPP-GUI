/*----------------------------------------------------------------------------------------
 *	
 *	project:	01_MouseBasics
 *	author:		Rob Bateman
 *	note:		This file is designed to show you the basics of how to set up your glut
 *				apps to recieve mouse input. 
 *
 *				The first stage of this is to provide a button callback function that 
 *				will be called whenever a mouse button is pressed or released. In this
 *				sourcefile It is the function "MouseButton" that is set as the glut
 *				callback for button events. 
 *
 *				The function prototype must always be (though the name can change!):
 *
 *				void MouseButton( int button, int state, int x, int y );
 *
 *				The parameters are :
 *
 *				button -	values can be GLUT_LEFT_BUTTON,GLUT_RIGHT_BUTTON,GLUT_MIDDLE_BUTTON
 *				state  -	was the button pressed or released, specified with GLUT_DOWN & GLUT_UP respectively.
 *				x      -	the x-position of the mouse click
 *				y      -	the y-position of the mouse click
 */


#include <stdlib.h>
#include <stdio.h>

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif

#include <GL/glut.h>

int winw = 640;
int winh = 480;

/*----------------------------------------------------------------------------------------
 *	This function will be used to draw the 3D scene
 */
void Draw3D()
{
}

/*----------------------------------------------------------------------------------------
 *	This function will be used to draw an overlay over the 3D scene.
 *	This will be used to draw our fonts, buttons etc......
 */
void Draw2D()
{

}

/*----------------------------------------------------------------------------------------
 *	This is the main display callback function. It sets up the drawing for 
 *	The 3D scene first then calls the Draw3D() function. After that it switches to 
 *	an orthographic projection and calls Draw2D().
 */
void Draw()
{
	/*
	 *	Clear the background
	 */
	glClear( GL_COLOR_BUFFER_BIT |
			 GL_DEPTH_BUFFER_BIT );

	/*
	 *	Enable lighting and the z-buffer
	 */
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	/*
	 *	Set perspective viewing transformation
	 */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,1,1,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*
	 *	Draw the 3D elements in the scene
	 */
	Draw3D();

	/*
	 *	Disable depth test and lighting for 2D elements
	 */
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	/*
	 *	Set the orthographic viewing transformation
	 */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,winw,0,winh,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*
	 *	Draw the 2D overlay
	 */
	Draw2D();

	/*
	 *	Bring the back buffer to the front and vice-versa.
	 */
	glutSwapBuffers();
}

/*----------------------------------------------------------------------------------------
 *	This function is called when the window is resized. All this does is simply 
 *	store the new width and height of the window which are then referenced by
 *	the draw function to set the correct viewing transforms 
 */
void Resize(int w, int h)
{
	winw = w;
	winh = h;

	/*
	 *	Allow drawing in full region of the screen
	 */
	glViewport(0,0,w,h);
}


/*----------------------------------------------------------------------------------------
 *
 */
void MouseButton(int button,int state,int x, int y)
{
	/*
	 *	has the button been pressed or released?
	 */
	if (state == GLUT_DOWN) 
	{
		/*
		 *	Which button was pressed?
		 */
		switch(button) 
		{
		case GLUT_LEFT_BUTTON:
			printf("left ");
			break;
		case GLUT_MIDDLE_BUTTON:
			printf("middle ");
			break;
		case GLUT_RIGHT_BUTTON:
			printf("right ");
			break;
		}
		printf("button pressed at (%d,%d)\n",x,y);
	}
	else 
	{
		/*
		 *	Which button was released?
		 */
		switch(button) 
		{
		case GLUT_LEFT_BUTTON:
			printf("left ");
			break;
		case GLUT_MIDDLE_BUTTON:
			printf("middle ");
			break;
		case GLUT_RIGHT_BUTTON:
			printf("right ");
			break;
		}
		printf("button released at (%d,%d)\n",x,y);
	}

	/*
	 *	Force a redraw of the screen
	 */
	glutPostRedisplay();
}


/*----------------------------------------------------------------------------------------
 *
 */
int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitWindowSize(winw,winh);
	glutInitWindowPosition(200,100);
	glutCreateWindow("01 - Mouse Basics");

	glutDisplayFunc(Draw);
	glutReshapeFunc(Resize);
	glutMouseFunc(MouseButton);

	glutMainLoop();
}






