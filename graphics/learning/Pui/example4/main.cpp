#ifdef WIN32
	#include <windows.h>
#endif

#include <GL/glut.h>
#include <pu.h>


/*
 * This function will be called whenever we click 
 * OK or Cancel on the file selector.
 */
void File_Selector_Callback( puObject *pWidget )
{
	/* get the filename held in the widget */
	char *filename = pWidget->getStringValue(); 

	/* if cancel was pressed then filename will be "", or "\0" */
	if( filename[0] == NULL ) 
	{
		printf("Cancel was specified\n"); 
	}
	else
	{
		printf("File to load = \"%s\"\n", filename ) ; 
	}

	/* delete the file selector widget */
	puDeleteObject(pWidget);
}

/*
 * This function is called whenever the button is clicked.
 * This creates the file dialog.
 */
void Button_Callback( puObject *pWidget )
{
	/*
	 * This odd bit of C++ syntax creates a puFileSelector specifying -
	 * - The x coord of the bottom left corner
	 * - The y coord of the bottom left corner
	 * - The width
	 * - The height
	 * - The start file
	 */
	puFileSelector* fs = new puFileSelector( 5, 40, 250, 430, "" );

	fs->setCallback(File_Selector_Callback); /* set callback for when user clicks OK or cancel */
}

/*
 * Init the user interafce
 */
void InitUI()
{
	puSetDefaultStyle ( PUSTYLE_SMALL_SHADED ) ;
	puSetDefaultColourScheme( 0.5f, 0.7f, 0.9f );

	/* Create a simple button to trigger creation of file selector*/
	puOneShot *b = new puOneShot( 5, 5, 255, 35 ) ;

	b->setCallback( Button_Callback );	/* set callback function */
	b->setLegend( "Open File" );			/* set text label */
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

/* called when a mouse button is pressed or released */
void KeyBoard(unsigned char key,int x,int y)
{
	puKeyboard(key,PU_DOWN);
	glutPostRedisplay();
}

/* called when the mouse moves */
void SpecialKeyBoard(int key,int x,int y)
{
	puKeyboard(key+PU_KEY_GLUT_SPECIAL_OFFSET,PU_DOWN);
	glutPostRedisplay();
}

int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(300,100);
	glutCreateWindow("pui - example4");

	glutDisplayFunc(Display);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMove);
	glutKeyboardFunc(KeyBoard);
	glutSpecialFunc(SpecialKeyBoard);

	puInit();
	Init();
	InitUI();

	glutMainLoop();

}
