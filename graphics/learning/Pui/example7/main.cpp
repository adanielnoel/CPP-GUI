#ifdef WIN32
	#include <windows.h>
#endif
	
#include <GL/glut.h>
#include <pu.h>



/*
*	Globals
*/
puInput *TextBox = NULL;
puInput *IntBox = NULL;
puInput *FloatBox = NULL;

void Text_CB( puObject *pWidget )
{
	printf("Text box's value =\"%s\"\n",pWidget->getStringValue()); 
}

void Int_CB( puObject *pWidget )
{
	int iVal; 
	pWidget->getValue(&iVal); 
	printf("Int box's value = %d\n",iVal); 
}

void Float_CB( puObject *pWidget )
{
	float fVal; 
	pWidget->getValue(&fVal); 
	printf("Float box's value = %f\n",fVal); 
}

void InitUI()
{
	puSetDefaultStyle ( PUSTYLE_SMALL_SHADED ) ;
	puSetDefaultColourScheme( 0.5f, 0.7f, 0.9f );

	/* create a new input control for text */
	TextBox = new puInput( 5, 5, 205, 40 );
	TextBox->setLabel("Text");
	TextBox->setCallback(Text_CB);

	/* create a new input control for integers */
	IntBox = new puInput( 5, 45, 205, 80 );
	IntBox->setLabel("Int");
	IntBox->setCallback(Int_CB);


	/* create a new input control for floats */
	FloatBox = new puInput( 5, 85, 205, 120 );
	FloatBox->setLabel("Float");
	FloatBox->setCallback(Float_CB);
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
	glutCreateWindow("pui - example7");

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
