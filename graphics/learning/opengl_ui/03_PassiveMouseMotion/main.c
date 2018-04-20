/*----------------------------------------------------------------------------------------
 *	
 *	project:	03_PassiveMouseMotion
 *	author:		Rob Bateman
 *	note:		
 *				In addition to the mouse motion and mouse press callbacks, you can also
 *				register a passive motion function. This is much the same as the motion
 *				function except it is called whenever the mouse button's are NOT HELD.
 *
 *				The passive function can be registered by calling glutPassiveMotionFunc. 
 *
 *				
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
 *	This is the main display callback function. It sets up the drawing for 
 *	The 3D scene first then calls the Draw3D() function. After that it switches to 
 *	an orthographic projection and calls Draw2D().
 */
void Draw(void)
{
	/*
	 *	Clear the background
	 */
	glClear( GL_COLOR_BUFFER_BIT |
			 GL_DEPTH_BUFFER_BIT );

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
 *	\brief	This function is called whenever a mouse button is pressed or released
 *	\param	button	-	GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, or GLUT_MIDDLE_BUTTON
 *	\param	state	-	GLUT_UP or GLUT_DOWN depending on whether the mouse was released
 *						or pressed respectivly. 
 *	\param	x		-	the x-coord of the mouse cursor.
 *	\param	y		-	the y-coord of the mouse cursor.
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
 *	\brief	This function is called whenever the mouse cursor is moved AND A BUTTON IS HELD.
 *	\param	x	-	the new x-coord of the mouse cursor.
 *	\param	y	-	the new y-coord of the mouse cursor.
 */
void MouseMotion(int x, int y)
{
	printf("new mouse pos = (%d,%d)\n",x,y);

	/*
	 *	Force a redraw of the screen
	 */
	glutPostRedisplay();
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function is called whenever the mouse cursor is moved AND NO BUTTONS ARE HELD.
 *	\param	x	-	the new x-coord of the mouse cursor.
 *	\param	y	-	the new y-coord of the mouse cursor.
 */
void MousePassiveMotion(int x, int y)
{
	printf("passive mouse pos = (%d,%d)\n",x,y);
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
	glutCreateWindow("03 - Mouse Motion");

	glutDisplayFunc(Draw);
	glutReshapeFunc(Resize);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);

	glutMainLoop();
}








