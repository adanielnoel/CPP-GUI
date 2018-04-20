#ifdef WIN32 
	#include <windows.h>
#endif

#include <GL/glut.h>
#include <pu.h>

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
	puSetDefaultStyle ( PUSTYLE_SMALL_SHADED ) ;
	puSetDefaultColourScheme( 0.5f, 0.7f, 0.9f );	/* set RGB colour scheme */

	/* create a new bit of text */
	puText* Text = new puText( 5, 5 );

	/* set the text in the control */
	Text->setLabel("Text");

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

int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(300,100);
	glutCreateWindow("pui - example1");

	glutDisplayFunc(Display);	/* set Display as the callback function for the window re-draws */

	puInit();	/* initialise pui */
	InitUI();	/* call custom UI initialisation function */
	Init();		/* call custom openGL initialisation function */

	glutMainLoop();

}
