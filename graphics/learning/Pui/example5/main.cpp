#ifdef WIN32
	#include <windows.h>
#endif

#include <GL/glut.h>
#include <pu.h>


/*
*	These globals will be directly affected by the sliders
*/
float r = 0, g = 0, b = 0;

/*
* Init the user interafce
*/
void InitUI()
{
	puSetDefaultStyle ( PUSTYLE_SMALL_SHADED ) ;
	puSetDefaultColourScheme( 0.5f, 0.7f, 0.9f );


	/* create a new horizontal slider. int min = 0, int max = 100 */
	puSlider *slider = new puSlider( 0, 100, 50 );

	/* 
	 *	this passes a pointer to the global r variable 
	 *	to the slider so it can control the variable,
	 *	ie no need for a callback 
	 */
	slider->setValuator(&r);

	slider->setLabel("R");
	slider->setSize(200,30);
	slider->setPosition(5,5);



	/* create a new horizontal slider. int min = 0, int max = 100 */
	slider = new puSlider( 0, 100, 50 );

	/* 
	 *	this passes a pointer to the global r variable 
	 *	to the slider so it can control the variable,
	 *	ie no need for a callback 
	 */
	slider->setValuator(&g);

	slider->setLabel("G");
	slider->setSize(200,30);
	slider->setPosition(5,40);




	/* create a new horizontal slider. int min = 0, int max = 100 */
	slider = new puSlider( 0, 100, 50 );

	/* 
	 *	this passes a pointer to the global r variable 
	 *	to the slider so it can control the variable,
	 *	ie no need for a callback 
	 */
	slider->setValuator(&b);

	slider->setLabel("B");
	slider->setSize(200,30);
	slider->setPosition(5,75);
}

/* The display function - called when redraw is needed */
void Display()
{
	/* set the background colour using global variables */
	glClearColor(r,g,b,0); 

	glClear(GL_COLOR_BUFFER_BIT);

	/* display the user interface */
	puDisplay();

	glutSwapBuffers();
}

/* called when a mouse button is pressed or released */
void MouseButton(int button,int state,int x,int y)
{
	/* see if the mouse clicked on any of the user interface controls */
	puMouse(button,state,x,y);
	glutPostRedisplay();
}

/* called when the mouse moves */
void MouseMove(int x,int y)
{
	/* Pass information about the position of the mouse to pui */
	puMouse(x,y);
	glutPostRedisplay();
}

int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(300,100);
	glutCreateWindow("pui - example5");

	glutDisplayFunc(Display);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMove);

	puInit();
	InitUI();

	glutMainLoop();

}
