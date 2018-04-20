#ifdef WIN32
	#include <windows.h>
#endif

#include <GL/glut.h>
#include <pu.h>


/*
*	This function is called whenever the button is clicked. It is the callback 
*	function we will associate with the button.
*/
void Button_Callback( puObject *pWidget )
{
	printf( "I have been clicked\n" ) ;
}

/*
*	This function is called after the openGL window has been created.
*	It allows you to set up user interface controls within the main
*	window.
*/
void InitUI()
{
	/*
	*	Set a style for the controls. Param can be one of the following :
	*
	*	PUSTYLE_DEFAULT			// =   PUSTYLE_SHADED 
	*	PUSTYLE_NONE			
	*	PUSTYLE_PLAIN      
	*	PUSTYLE_BEVELLED   
	*	PUSTYLE_BOXED      
	*	PUSTYLE_DROPSHADOW 
	*	PUSTYLE_SPECIAL_UNDERLINED 
	*	PUSTYLE_SMALL_BEVELLED     
	*	PUSTYLE_RADIO			// deprecated ! DO NOT USE !
	*	PUSTYLE_SHADED     
	*	PUSTYLE_SMALL_SHADED   
	*/
	puSetDefaultStyle       ( PUSTYLE_SMALL_BEVELLED ) ;
	puSetDefaultColourScheme( 0.5f, 0.7f, 0.9f );

	/*
	* Create a simple button
	*/
	puOneShot *b = new puOneShot( 5, 5, 255, 35 );

	/*
	*	Set the control's text label and callback function
	*/
	b -> setLegend   ( "Click Me" );
	b -> setCallback ( Button_Callback );
}

/* custom OpenGL Initialisation */
void Init()
{
	glClearColor(0.5,0.5,0.5,0.5);
}

/* The display function - called when redraw is needed */
void Display()
{
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
	glutCreateWindow("pui - example2");

	glutDisplayFunc(Display);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMove);

	puInit();
	Init();
	InitUI();

	glutMainLoop();

}
