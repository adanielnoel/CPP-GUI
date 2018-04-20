/*==========================================================================
**	
**	Filename	:	genTriangles.c
**	Date		:	12/Feb/2002
**	Author 		:	Rob Bateman
**	Notes		:	basic openGL program to generate some primitives
**	
**=========================================================================*/

/*==========================================================================
**	
**	Includes
**	
**=========================================================================*/

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>


/*==========================================================================
**	
**	Global static variables
**	
**=========================================================================*/

static unsigned int g_dwDisplayListNum;
static float 		g_fSpinX 	 = 0.0f,
			 		g_fSpinZ 	 = 0.0f;
static int 			g_iMousePosX = 0;
static int 			g_iMousePosY = 0;
static char			g_bWireframe = 0;
static char			g_bSelected  = 0;



/*==========================================================================
**	Func	:	generateSphere
**	Params	:	iVsections	-	the number of sections around the sphere
**				iHsections	-	the number of slices around the sphere
**				fRadius		-	the radius of the sphere
**	Returns	:	a display list number
**	Notes	:	
**
**=========================================================================*/

unsigned int generateCube(   unsigned int iWsections,
					   		 unsigned int iHsections,
							 unsigned int iDsections,
					   		 float        fDimension )
{
	unsigned int iDisplayListNum;
	float fHalfDimension;
	float fWInc,fHInc,fDInc;
	unsigned int   i,j;
	
	/*
	**	make all params positive to prevent errors
	*/
	fDimension = fabs(fDimension);
	
	/*
	**	calculate some useful values
	*/
	fHalfDimension = 0.5f*fDimension;
	fHInc  = (float)fDimension/iHsections;
	fWInc  = (float)fDimension/iWsections;
	fDInc  = (float)fDimension/iDsections;
	
	/*
	**	enforce a sensible number of divisions
	*/
	if(iWsections < 1)
	{
		iWsections = 1;
	}
	if(iHsections < 1)
	{
		iHsections = 1;
	}
	if(iDsections < 1)
	{
		iDsections = 1;
	}
	
	/*
	**	generate a display list number and start compiling the list
	*/
	iDisplayListNum = glGenLists(1);
	glNewList(iDisplayListNum,GL_COMPILE);
	
		/*
		**	top & bottom faces....
		*/	
		for(i=0;i<iWsections;i++)
		{
			glBegin(GL_QUAD_STRIP);
				glNormal3f(0.0f,1.0f,0.0f);
			
				for(j=0;j<=iDsections;j++)
				{
					glVertex3f( (i+1)*fWInc - fHalfDimension, fHalfDimension, j*fDInc - fHalfDimension );
					glVertex3f(     i*fWInc - fHalfDimension, fHalfDimension, j*fDInc - fHalfDimension );
				}
				
			glEnd();
			
			glBegin(GL_QUAD_STRIP);
				glNormal3f(0.0f,-1.0f,0.0f);
			
				for(j=0;j<=iDsections;j++)
				{
					glVertex3f(     i*fWInc - fHalfDimension, -fHalfDimension, j*fDInc - fHalfDimension);
					glVertex3f( (i+1)*fWInc - fHalfDimension, -fHalfDimension, j*fDInc - fHalfDimension );
				}
				
			glEnd();
		}

		/*
		**	left & right faces....
		*/	
		for(i=0;i<iHsections;i++)
		{
			glBegin(GL_QUAD_STRIP);
				glNormal3f(1.0f,0.0f,0.0f);
			
				for(j=0;j<=iDsections;j++)
				{
					glVertex3f( fHalfDimension,     i*fHInc - fHalfDimension, j*fDInc - fHalfDimension );
					glVertex3f( fHalfDimension, (i+1)*fHInc - fHalfDimension, j*fDInc - fHalfDimension );
				}
				
			glEnd();
			
			glBegin(GL_QUAD_STRIP);
				glNormal3f(-1.0f,0.0f,0.0f);
			
				for(j=0;j<=iDsections;j++)
				{
					glVertex3f( -fHalfDimension, (i+1)*fHInc - fHalfDimension, j*fDInc - fHalfDimension );
					glVertex3f( -fHalfDimension,     i*fHInc - fHalfDimension, j*fDInc - fHalfDimension );
				}
				
			glEnd();
		}

		
		/*
		**	front & back faces....
		*/	
		for(i=0;i<iWsections;i++)
		{
			glBegin(GL_QUAD_STRIP);
				glNormal3f(0.0f,0.0f,1.0f);
			
				for(j=0;j<=iHsections;j++)
				{
					glVertex3f(     i*fWInc  - fHalfDimension, j*fHInc - fHalfDimension, fHalfDimension );
					glVertex3f( (i+1)*fWInc  - fHalfDimension, j*fHInc - fHalfDimension, fHalfDimension );
				}
				
			glEnd();
			
			glBegin(GL_QUAD_STRIP);
				glNormal3f(0.0f,0.0f,-1.0f);
			
				for(j=0;j<=iHsections;j++)
				{
					glVertex3f( (i+1)*fWInc - fHalfDimension, j*fHInc - fHalfDimension, -fHalfDimension );
					glVertex3f(     i*fWInc - fHalfDimension, j*fHInc - fHalfDimension, -fHalfDimension );
				}
				
			glEnd();
		}
		
		
	glEndList();
	
	return iDisplayListNum;
}


/*==========================================================================
**	Func	:	generateCylinder
**	Params	:	iVsections	-	the number of sections up the cylinder
**				iHsections	-	the number of sections around the circumference
**				fHeight		-	the height of the cylinder
**				fRadius		-	the radius of the circular part	
**	Returns	:	a display list number
**	Notes	:	
**
**=========================================================================*/

unsigned int generateCylinder(  unsigned int iVsections,
					   			unsigned int iHsections,
					   			float        fHeight   ,
					   			float        fRadius   )
{
	unsigned int iDisplayListNum;
	float fHalfHeight;
	float fHeightInc;
	float fAngle;
	unsigned int   i,j;
	
	/*
	**	make all params positive to prevent errors
	*/
	fHeight = fabs(fHeight);
	fRadius = fabs(fRadius);
	
	/*
	**	calculate some useful values
	*/
	fHalfHeight = 0.5f*fHeight;
	fHeightInc  = (float)fHeight/iHsections;

	/*
	**	enforce a sensible number of vertical sections
	*/
	if(iVsections < 5)
	{
		iVsections = 5;
	}
	
	/*
	**	generate a display list number and start compiling the list
	*/
	iDisplayListNum = glGenLists(1);
	glNewList(iDisplayListNum,GL_COMPILE);
	

		/*
		**	Draw Top Circle as a triangle fan
		*/
		
		glBegin( GL_TRIANGLE_FAN );
			glNormal3f(0.0f,1.0f,0.0f);
			glVertex3f(0.0f,fHalfHeight,0.0f);
			for(i=0;i<=iVsections;i++)
			{
				fAngle = (float)i*(-6.2831853)/iVsections;
				glVertex3f(fRadius*cos(fAngle),fHalfHeight,fRadius*sin(fAngle));
			}
		
		glEnd();
		
		
		/*
		**	Draw Bottom Circle as a triangle fan
		*/
		
		glBegin( GL_TRIANGLE_FAN );
			glNormal3f(0.0f,-1.0f,0.0f);
			glVertex3f(0.0f,-fHalfHeight,0.0f);
			for(i=0;i<=iVsections;i++)
			{
				fAngle = (float)i*(6.2831853)/iVsections;
				glVertex3f(fRadius*cos(fAngle),-fHalfHeight,fRadius*sin(fAngle));
			}
		
		glEnd();
		
		
		
		/*
		**	Draw a number of sections that will make up the sides as quad strips
		*/

		for(i=0;i<iHsections;i++)
		{
			float fCurrMin = -fHalfHeight + fHeightInc*i,
				  fCurrMax = fCurrMin + fHeightInc;
			glBegin( GL_QUAD_STRIP );
				for(j=0;j<=iVsections;j++)
				{
					float ca,sa,sc;
					fAngle = (float)j*(6.2831853)/iVsections;
					ca = fRadius*cos(fAngle);
					sa = fRadius*sin(fAngle);
					sc = 1.0f/(sqrt(ca*ca +sa*sa));
					glNormal3f(ca,0.0f,sa);
					glVertex3f(ca,fCurrMin,sa);
					glVertex3f(ca,fCurrMax,sa);
				}			
			glEnd();
		}
	
	glEndList();
	
	return iDisplayListNum;
}



/*==========================================================================
**	Func	:	init
**	Params	:	
**	Returns	:	
**	Notes	:	Initialise Function to set default openGL parameters
**
**=========================================================================*/

void init( void )
{
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	/*
	**	Lighting setup
	*/
	{
		float fpAmbient[]  = {0.15f,0.15f,0.15f,0.0f};
		float fpDiffuse[]  = {0.8f,0.8f,0.8f,0.0f};
		float fpSpecular[] = {0.8f,0.8f,0.8f,0.0f};
		float fpPosition[] = {1.0f,16.0f,10.0f,1.0f};
		
		glLightfv(GL_LIGHT0,GL_AMBIENT,fpAmbient);
		glLightfv(GL_LIGHT0,GL_DIFFUSE,fpDiffuse);
		glLightfv(GL_LIGHT0,GL_SPECULAR,fpSpecular);
		glLightfv(GL_LIGHT0,GL_POSITION,fpPosition);
		
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0); 
	}
	
	/*
	**	Material Properties setup
	*/
	{
		float fpAmbient[]  = {0.3f,0.3f,0.3f,0.0f};
		float fpDiffuse[]  = {0.7f,0.7f,0.7f,0.0f};
		float fpSpecular[] = {1.0f,1.0f,1.0f,0.0f};
		
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,fpAmbient);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,fpDiffuse);
		glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,fpSpecular);
		glMaterialf (GL_FRONT_AND_BACK,GL_SHININESS,128);
	}
	
	/*
	**	Call function to generate the cylinder
	*/
	g_dwDisplayListNum = generateCube(50,50,50,4);/* generateCylinder(45,45,-6.6f,-1.25f);*/
}



/*==========================================================================
**	Func	:	display
**	Params	:	
**	Returns	:
**	Notes	:	Display callback Function for screen re-draw
**
**=========================================================================*/

void display( void )
{
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
	glLoadIdentity();
	
	gluLookAt ( 0.0f,0.0f,10.0f,	/* Eye position */
				0.0f,0.0f,0.0f,		/* Center of interest */
				0.0f,1.0f,0.0f );	/* Camera Up Vector */
	
	glRotatef( g_fSpinZ, 1,0,0);			
	glRotatef( -g_fSpinX,0,0,1);
	
	/*
	**	set up display mode (is it wireframe?)
	*/
	glPolygonMode( GL_FRONT_AND_BACK, (g_bWireframe) ? GL_LINE : GL_FILL );
	(g_bWireframe)? glDisable(GL_CULL_FACE): glEnable(GL_CULL_FACE);
	(g_bWireframe)? glDisable(GL_LIGHTING) : glEnable(GL_LIGHTING);	
	
	/* 
	**	draw geometry
	*/
	glPushMatrix();
		glCallList( g_dwDisplayListNum );
	glPopMatrix();
	
	glutSwapBuffers();
}



/*==========================================================================
**	Func	:	reshape
**	Params	:	w	-	new screen width
**				h	-	new screen height
**	Returns	:	
**	Notes	:	Reshape callback function to resize gl display
**
**=========================================================================*/

void reshape( int w, int h )
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(52.0f,(h==0)?1.0f:(float)w/h,1,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}





/*==========================================================================
**	Func	:	mouseClick
**	Params	:	button		-	id of button pressed
**				state		-	whether button has been pressed or released
**				posX		-	x coord of screen where event happened
**				posY		-	y coord of screen where event happened
**	Returns	:	
**	Notes	:	mouse click callback function to register mouse presses
**
**=========================================================================*/

void mouseClick( int button, int state, int posX, int posY )
{
	/*
	**	store positions of the mouse cursor for later
	*/
	if( state == GLUT_DOWN )
	{
		g_iMousePosX = posX;
		g_iMousePosY = posY;
	}
	glutPostRedisplay();
}




/*==========================================================================
**	Func	:	mouseMotion
**	Params	:	x	-	the x coord of new mouse position
**				y	-	the y coord of new mouse position
**	Returns	:	
**	Notes	:	motion function for mouse movement
**
**=========================================================================*/

void mouseMotion( int x,int y)
{
	/*
	**	move the global rotation values when the mouse is moved
	*/
	g_fSpinX += (float)(x-g_iMousePosX)*0.1f;
	g_fSpinZ += (float)(y-g_iMousePosY)*0.1f;
	g_iMousePosX = x;
	g_iMousePosY = y;
	
	glutPostRedisplay();
}




/*==========================================================================
**	Func	:	mouseClick
**	Params	:	key			-	id of button pressed
**				posX		-	x coord of screen where event happened
**				posY		-	y coord of screen where event happened
**	Returns	:	
**	Notes	:	mouse click callback function to register mouse presses
**
**=========================================================================*/

void keyboard(unsigned char key, int posX, int posY )
{
	switch( key )
	{
	case 'w' : case 'W' :
		g_bWireframe = !g_bWireframe;
		break;
	case 'c' : case 'C' :
		glDeleteLists(g_dwDisplayListNum,1);
		g_bSelected = !g_bSelected;
		g_dwDisplayListNum = (g_bSelected)? generateCube(15,15,15,4) : generateCylinder(35,35,5.4f,1.65f);
		break;
	default:
		break;
	}
			
	glutPostRedisplay();
}

/*==========================================================================
**	Func	:	main
**	Params	:	argc	-	the number of command line arguments recieved
**				argv	-	an array of strings holding the arguments
**	Returns	:	
**	Notes	:	main function to set the ball rolling
**
**=========================================================================*/

void main( int argc, char** argv )
{
     glutInit(&argc, argv);
     glutInitDisplayMode   ( GLUT_DOUBLE |
                             GLUT_RGB    |
                             GLUT_DEPTH  );
							 
     glutInitWindowSize    ( 800 , 600 );
     glutInitWindowPosition( 100 , 100 );
	 
     glutCreateWindow( "Press 'c' to toggle object" );
     glutDisplayFunc(display);
     glutReshapeFunc(reshape);
	 glutMouseFunc(mouseClick);
	 glutMotionFunc(mouseMotion);
	 glutKeyboardFunc(keyboard);
	 
     init();

	 glutMainLoop();
}


	
	
