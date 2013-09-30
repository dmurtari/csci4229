/*
 *  Lorenz Attractor Drawer
 *  Domenic Murtari (domenic.murtari@gmail.com)
 *  Skeleton based on ex6.c, lorenz calculation based on lorenz.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//  Globals
int th=0;       // Azimuth of view angle
int ph=0;       // Elevation of view angle
double z=0;     // Z variable
double w=1;     // W variable
double dim=2;   // Dimension of orthogonal box
char* text[] = {"3D"};  // Dimension display text

float s = 10;
float b = 2.666666;
float r = 28;

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  // Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Display the scene
 */
void display()
{
    //  Clear the image
    glClear(GL_COLOR_BUFFER_BIT);
    
    //  Reset previous transforms
    glLoadIdentity();
    
    // Set Projcetion
    glOrtho(-1, 1, -1, 1, -10, 10);

    //  Set view angle
    glRotated(ph,1,0,0);
    glRotated(th,0,1,0);
    
    // Draw the Lorenz Attractor
    glBegin(GL_LINE_STRIP);
    
    float x = 1, y = 1, z = 1;
    int i;    
    float dt = .001;

    // Iterate and draw Lorenz Attractor, using GL_LINE_STRIP to draw 
    // from one vertex to another. 
    for (i = 0; i < 50000; i++) {    
        
        float dx = s*(y-x);
        float dy = x*(r-z) - y;
        float dz = x*y - b*z;

        x += dt*dx;
        y += dt*dy;
        z += dt*dz;        

        // Fun with colors!
        glColor3ub((int)i/10%255, (int)i/15%255, (int)i/20%255);
        // Multiply by .05 to make attractor fit in default view
        glVertex4f(x*.05, y*.05, z*.05, w); 
    }


    glEnd();

    //  Draw axes in white
    glColor3f(1,1,1);
    glBegin(GL_LINES);
    glVertex3d(0,0,0);
    glVertex3d(1,0,0);
    glVertex3d(0,0,0);
    glVertex3d(0,1,0);
    glVertex3d(0,0,0);
    glVertex3d(0,0,1);
    glEnd();
    
    //  Label axes
    glRasterPos3d(1,0,0);
    Print("X");
    glRasterPos3d(0,1,0);
    Print("Y");
    glRasterPos3d(0,0,1);
    Print("Z");
    
    //  Display parameters
    glWindowPos2i(5,5);
    Print("View Angle=%d,%d",th,ph);
    
    //  Flush and swap
    glFlush();
    glutSwapBuffers();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0'){
      th = ph = 0;
      s = 10;
      b = 2.666666;
      r = 28;
   }
   //  Increase w by 0.1
   else if (ch == '-')
   {
      w += 0.1;
   }
   //  Decrease w by 0.1
   else if (ch == '+')
   {
      w -= 0.1;
   }
   //  Decrease s by 1
   else if (ch == 's'){
      s -= 1;
   }
   //  Increase s by 1
   else if (ch == 'S'){
      s += 1;
   }
   //  Decrease b by .1
   else if (ch == 'b'){
      b -= .1;
   }
   //  Increase b by .1
   else if (ch == 'B'){
      b += .1;
   }
   //  Decrease r by 1
   else if (ch == 'r'){
      r -= 1;
   }
   //  Increase r by 1
   else if (ch == 'R'){
      r += 1;
   }

   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase azimuth by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease azimuth by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   double w2h = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Orthogonal projection box adjusted for the
   //  aspect ratio of the window
   glOrtho(-dim*w2h,+dim*w2h, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
  //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window 
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   //  Request 500 x 500 pixel window
   glutInitWindowSize(500,500);
   //  Create the window
   glutCreateWindow("Assignment 2: Domenic Murtari");
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
  //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   //  Return code
   return 0;
}
