#ifdef WIN32
	#include <windows.h>
#endif

#include <GL/glut.h>
#include <pu.h>

/* the items in the combo box */
char *ComboBoxItems[] =
{
	"bungle",
	"zippy",
	"george",
	NULL 
};

/* the callback for the combo box */
void ComboBox_CB( puObject *pWidget )
{
	char *selected = pWidget->getStringValue();

	printf("CurrentItem = %s\n",selected); 
}

void InitUI()
{
	puSetDefaultStyle ( PUSTYLE_SMALL_SHADED ) ;
	puSetDefaultColourScheme( 0.5f, 0.7f, 0.9f );

	/* create a new combo box */
	puComboBox *box = new puComboBox( 5, 5, 200, 35,ComboBoxItems, FALSE );

	/* set the callback function */
	box->setCallback(ComboBox_CB); 
}

/* custom OpenGL Initialisation */
void Init()
{
	glClearColor(0.5f,0.5f,0.5f,0.5f);
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
	glutCreateWindow("pui - example8");

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
