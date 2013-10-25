/*
 *  Assignment 5: Domenic Murtari (domenic.murtari@gmail.com)
 *  Code skeleton from Example 13 (Lighting) from CSCI4229
 *  'p' to switch modes (projections)
 *  'x' to toggle axes
 *  '0' snaps angles to 0,0
 *  arrows to rotate the world
 *  PgUp/PgDn zooms in/out
 *  +/- changes field of view of perspective
 *  F1 toggles smooth/flat shading
 *  F2 toggles local viewer mode
 *  F3 toggles light distance (1/5)
 *  F8 change ball increment
 *  F9 invert bottom normal
 *  'l' toggles lighting
 *  a/A decrease/increase ambient light
 *  d/D decrease/increase diffuse light
 *  s/S decrease/increase specular light
 *  e/E decrease/increase emitted light
 *  n/N decrease/increase shininess
 *  m  toggles light movement
 *  [] lower/rise light
 */
#include "CSCIx229.h"

int mode=1;       //  Projection mode
int move=1;       //  Move light
int th=0;         //  Azimuth of view angle
int ph=10;        //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=3.0;   //  Size of world
// Light values
int one       =   1;     // Unit value
int distance  =   5;     // Light distance
int inc       =  10;     // Ball increment
int smooth    =   1;     // Smooth/Flat shading
int local     =   0;     // Local Viewer Model
int emission  =   0;     // Emission intensity (%)
int ambient   =  30;     // Ambient intensity (%)
int diffuse   = 100;     // Diffuse intensity (%)
int specular  =   0;     // Specular intensity (%)
int shininess =   0;     // Shininess (power of two)
float shinyvec[1];       // Shininess (value)
int zh        =  90;     // Light azimuth
float ylight  =   0;     // Elevation of light
unsigned int texture[9]; // Texture names

/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transformations
   glPopMatrix();
}

/*
 * Draw a house, given position x, y, z and scaling factors
 * dx, dy, dz and rotation about the z-axis th
 *
 */
static void drawHouse(double x, double y, double z,
		      double dx, double dy, double dz,
		      double th){
  
  const double rooftop = 2;
  const double roofoff = 1.5;
  const double roofwid = 1.25;
  const double roofbot = .75;

  // Set specular color to white
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

  glPushMatrix();
  
  // Translations
  glTranslated(x, y, z);
  glRotated(th, 0, 1, 0);
  glScaled(dx, dy, dz);

  // Enable Textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

  // Body of house
  glBindTexture(GL_TEXTURE_2D,texture[1]); // Brick
  glBegin(GL_QUADS);
  //Front
  glNormal3f( 0, 0, 1);
  glTexCoord2f(0.0, 0.0); glVertex3f(-1,-1, 1);
  glTexCoord2f(5.0, 0.0); glVertex3f(+1,-1, 1);
  glTexCoord2f(5.0, 5.0); glVertex3f(+1,+1, 1);
  glTexCoord2f(0.0, 5.0); glVertex3f(-1,+1, 1);
  //  Back
  glNormal3f( 0, 0,-1);
  glTexCoord2f(0.0, 0.0); glVertex3f(+1,-1,-1);
  glTexCoord2f(5.0, 0.0); glVertex3f(-1,-1,-1);
  glTexCoord2f(5.0, 5.0); glVertex3f(-1,+1,-1);
  glTexCoord2f(0.0, 5.0); glVertex3f(+1,+1,-1);
  //  Right
  glNormal3f(+1, 0, 0);
  glTexCoord2f(0.0, 0.0); glVertex3f(+1,-1,+1);
  glTexCoord2f(5.0, 0.0); glVertex3f(+1,-1,-1);
  glTexCoord2f(5.0, 5.0); glVertex3f(+1,+1,-1);
  glTexCoord2f(0.0, 5.0); glVertex3f(+1,+1,+1);
  //  Left
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0.0, 0.0); glVertex3f(-1,-1,-1);
  glTexCoord2f(5.0, 0.0); glVertex3f(-1,-1,+1);
  glTexCoord2f(5.0, 5.0); glVertex3f(-1,+1,+1);
  glTexCoord2f(0.0, 5.0); glVertex3f(-1,+1,-1);
  //  Top
  glNormal3f( 0,+1, 0);
  glVertex3f(-1,+1,+1);
  glVertex3f(+1,+1,+1);
  glVertex3f(+1,+1,-1);
  glVertex3f(-1,+1,-1);
  //  Bottom
  glNormal3f( 0,-1, 0);
  glVertex3f(-1,-1,-1);
  glVertex3f(+1,-1,-1);
  glVertex3f(+1,-1,+1);
  glVertex3f(-1,-1,+1);
  glEnd();
  
  // Roof
  glBindTexture(GL_TEXTURE_2D,texture[2]);  // Shingles
  glBegin(GL_TRIANGLES);
  // Front
  glNormal3f(0,       0,       +1      );
  glTexCoord2f(0.0, 0.0); glVertex3f(+roofoff,+roofbot,+roofwid);
  glTexCoord2f(2.0, 0.0); glVertex3f(-roofoff,+roofbot,+roofwid);
  glTexCoord2f(1.0, 2.0); glVertex3f(0,+rooftop,+roofwid/2);
  // Back
  glNormal3f(0,       0,       -1      );
  glTexCoord2f(0.0, 0.0); glVertex3f(+roofoff,+roofbot,-roofwid);
  glTexCoord2f(2.0, 0.0); glVertex3f(-roofoff,+roofbot,-roofwid);
  glTexCoord2f(1.0, 2.0); glVertex3f(0,+rooftop,-roofwid/2);
  glEnd();

  // Sides of Roof
  glBegin(GL_QUADS);
  // Right side
  glNormal3f(+1      ,+1      ,0       );
  glTexCoord2f(0.0, 0.0); glVertex3f(+roofoff,+roofbot,-roofwid);
  glTexCoord2f(2.0, 0.0); glVertex3f(+roofoff,+roofbot,+roofwid);
  glTexCoord2f(2.0, 2.0); glVertex3f(0,+rooftop,+roofwid/2);
  glTexCoord2f(0.0, 2.0); glVertex3f(0,+rooftop,-roofwid/2);
  // Left Side
  glNormal3f(-1      ,-1      ,0       );
  glTexCoord2f(0.0, 0.0); glVertex3f(-roofoff,+roofbot,-roofwid);
  glTexCoord2f(2.0, 0.0); glVertex3f(-roofoff,+roofbot,+roofwid);
  glTexCoord2f(2.0, 2.0); glVertex3f(0,+rooftop,+roofwid/2);
  glTexCoord2f(0.0, 2.0); glVertex3f(0,+rooftop,-roofwid/2);  
  glEnd();

  // Bottom
  glBindTexture(GL_TEXTURE_2D, texture[3]);  // Stucco
  glBegin(GL_QUADS);
  glNormal3f(0       ,+1      ,0       );
  glTexCoord2f(0.0, 0.0); glVertex3f(+roofoff,+roofbot,-roofwid);
  glTexCoord2f(5.0, 0.0); glVertex3f(+roofoff,+roofbot,+roofwid);
  glTexCoord2f(5.0, 5.0); glVertex3f(-roofoff,+roofbot,+roofwid);
  glTexCoord2f(0.0, 5.0); glVertex3f(-roofoff,+roofbot,-roofwid);
  glEnd();
  // Draw the chimney
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  glBegin(GL_QUADS);
  // Left
  glNormal3f( -1, 0,   0);
  glTexCoord2f(0.0, 0.0); glVertex3f(+.5,+1,+.25);
  glTexCoord2f(2.0, 0.0); glVertex3f(+.5,+1,-.25);
  glTexCoord2f(2.0, 2.0); glVertex3f(+.5,+2,-.25);
  glTexCoord2f(0.0, 2.0); glVertex3f(+.5,+2,+.25);
  // Right
  glNormal3f(+1, 0,   0);
  glTexCoord2f(0.0, 0.0); glVertex3f(+1,+1,+.25);
  glTexCoord2f(2.0, 0.0); glVertex3f(+1,+1,-.25);
  glTexCoord2f(2.0, 2.0); glVertex3f(+1,+2,-.25);
  glTexCoord2f(0.0, 2.0); glVertex3f(+1,+2,+.25);
  // Back
  glNormal3f(  0, 0,  -1);
  glTexCoord2f(0.0, 0.0); glVertex3f(+.5,+2,-.25);
  glTexCoord2f(2.0, 0.0); glVertex3f(+.5,+1,-.25);
  glTexCoord2f(2.0, 2.0); glVertex3f(+1,+1,-.25);
  glTexCoord2f(0.0, 2.0); glVertex3f(+1,+2,-.25);
  // Front
  glNormal3f(  0, 0,  +1);
  glTexCoord2f(0.0, 0.0); glVertex3f(+.5,+2,+.25);
  glTexCoord2f(2.0, 0.0); glVertex3f(+.5,+1,+.25);
  glTexCoord2f(2.0, 2.0); glVertex3f(+1,+1,+.25);
  glTexCoord2f(0.0, 2.0); glVertex3f(+1,+2,+.25);
  glEnd();
  glBindTexture(GL_TEXTURE_2D, texture[6]);
  glBegin(GL_QUADS);
  // Top
  glTexCoord2f(0.0, 0.0); glVertex3f(+.5,+2,+.25);
  glTexCoord2f(2.0, 0.0); glVertex3f(+.5,+2,-.25);
  glTexCoord2f(2.0, 2.0); glVertex3f(+1,+2,-.25);
  glTexCoord2f(0.0, 2.0); glVertex3f(+1,+2,+.25);
  glEnd();


  // Draw a Door
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(-1, -1);
  glBindTexture(GL_TEXTURE_2D, texture[4]); // Door
  glBegin(GL_QUADS);
  // Front
  glNormal3f(  0, 0, +1);
  glTexCoord2f(0.0, 0.0); glVertex3f(+.2,-1,1);
  glTexCoord2f(1.0, 0.0); glVertex3f(-.2,-1,1);
  glTexCoord2f(1.0, 1.0); glVertex3f(-.2,0,1);
  glTexCoord2f(0.0, 1.0); glVertex3f(+.2,0,1);
  glEnd();
  // Door
  glBindTexture(GL_TEXTURE_2D, texture[4]); // Door
  glBegin(GL_QUADS);
  glNormal3f(0, 0, -1);
  glTexCoord2f(0.0, 0.0); glVertex3f(+.50,-1,-1);
  glTexCoord2f(1.0, 0.0); glVertex3f(+.75,-1,-1);
  glTexCoord2f(1.0, 1.0); glVertex3f(+.75, 0,-1);
  glTexCoord2f(0.0, 1.0); glVertex3f(+.50, 0,-1);
  glEnd();

  // Windows
  //glColor3ub(173,215,230);
  glBindTexture(GL_TEXTURE_2D, texture[5]); // Window
  glBegin(GL_QUADS);
  glNormal3f(0, 0, -1);
  glTexCoord2f(0.0, 0.0); glVertex3f(-.5,-.5,-1);
  glTexCoord2f(.75, 0.0); glVertex3f(0  ,-.5,-1);
  glTexCoord2f(.75, 1.0); glVertex3f(0  , 0, -1);
  glTexCoord2f(0.0, 1.0); glVertex3f(-.5, 0, -1);
  glEnd();

  glBegin(GL_QUADS);
  // Right window
  glNormal3f(0, 0, +1);
  glTexCoord2f(0.0, 0.0); glVertex3f(+.4,-.5,+1);
  glTexCoord2f(1.0, 0.0); glVertex3f(+.8,-.5,+1);
  glTexCoord2f(1.0, 1.0); glVertex3f(+.8, 0 ,+1);
  glTexCoord2f(0.0, 1.0); glVertex3f(+.4, 0 ,+1);
  // Left windows
  glNormal3f(0, 0, +1);
  glTexCoord2f(0.0, 0.0); glVertex3f(-.4,-.5,+1);
  glTexCoord2f(1.0, 0.0); glVertex3f(-.8,-.5,+1);
  glTexCoord2f(1.0, 1.0); glVertex3f(-.8, 0 ,+1);
  glTexCoord2f(0.0, 1.0); glVertex3f(-.4, 0 ,+1);
  glEnd();


  glDisable(GL_POLYGON_OFFSET_FILL);
  glDisable(GL_TEXTURE_2D); 
  glPopMatrix();
}

static void drawFencePost(double x, double y, double z,
		      double dx, double dy, double dz,
		      double th){
  // Set specular color to white
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

  glPushMatrix();
  
  // Translations
  glTranslated(x, y, z);
  glRotated(th, 0, 1, 0);
  glScaled(dx, dy, dz);
  
  // Begin drawing body of fencepost
  glBegin(GL_QUADS);
  // Front
  glColor3ub(200,200,200);
  glNormal3f( 0, 0, 1);
  glVertex3f(-1, 0, .5);
  glVertex3f(+1, 0, .5);
  glVertex3f(+1,+6, .5);
  glVertex3f(-1,+6, .5);
  // Back
  glNormal3f( 0, 0,-1);
  glVertex3f(+1, 0,-.5);
  glVertex3f(-1, 0,-.5);
  glVertex3f(-1,+6,-.5);
  glVertex3f(+1,+6,-.5);
  // Right
  glNormal3f(+1, 0, 0);
  glVertex3f(+1, 0,+.5);
  glVertex3f(+1, 0,-.5);
  glVertex3f(+1,+6,-.5);
  glVertex3f(+1,+6,+.5);
  // Left
  glNormal3f(-1, 0, 0);
  glVertex3f(-1, 0,-.5);
  glVertex3f(-1, 0,+.5);
  glVertex3f(-1,+6,+.5);
  glVertex3f(-1,+6,-.5);
  glEnd(); // End drawing main body

  // Begin drawing top of fencepost
  glBegin(GL_QUADS);
  // Right
  glNormal3f(+1, 0, 0);
  glVertex3f(+1,+6,+.5);
  glVertex3f(+1,+6,-.5);
  glVertex3f(0 ,+7,-.5);
  glVertex3f(0 ,+7,+.5);
  // Left
  glNormal3f(-1, 0, 0);
  glVertex3f(-1,+6,-.5);
  glVertex3f(-1,+6,+.5);
  glVertex3f(0 ,+7,+.5);
  glVertex3f(0 ,+7,-.5);
  glEnd();

  glBegin(GL_TRIANGLES);
  // Front
  glNormal3f(0, 0, 1);
  glVertex3f(+1,+6,+.5);
  glVertex3f(-1,+6,+.5);
  glVertex3f(0 ,+7,+.5);
  // Back
  glNormal3f(0, 0,-1);
  glVertex3f(+1,+6,-.5);
  glVertex3f(-1,+6,-.5);
  glVertex3f(0, +7,-.5);
  glEnd();
  // End drawing top of fencepost

  glPopMatrix();
}

static void drawScene(){
  // drawFencePost(-2,0,-1.2, .1,.1,.1, 90);
  int i;

  for(i = 0; i<=30; i+=2){
    drawFencePost(-2,0,-1-(double)i/10, .05,.1,.05, 90);
    drawFencePost( 2,0,-1-(double)i/10, .05,.1,.05, 90);
  }
  for(i = -18; i<=18; i+=2){
    drawFencePost((double)i/10,0,-4, .05,.1,.05, 0);
  }
  
  drawHouse(0,1,0 , 2,1,1, 0);

  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);


  // Draw Ground
  glBindTexture(GL_TEXTURE_2D,texture[0]);
  glBegin(GL_QUADS);
  glNormal3f(0, 1, 0);
  glTexCoord2f(0.0,  0.0);   glVertex3f(-100,0,-100);
  glTexCoord2f(200.0,0.0);   glVertex3f(-100,0,100);
  glTexCoord2f(200.0,200.0); glVertex3f(100,0,100);
  glTexCoord2f(0.0,  200.0); glVertex3f(100,0,-100);
  glEnd();
 
  // Sidewalk
  glEnable(GL_POLYGON_OFFSET_FILL);
  glBindTexture(GL_TEXTURE_2D,texture[7]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0); glVertex3f(.4,0,0);
  glTexCoord2f(4.0, 0.0); glVertex3f(.4,0,5);
  glTexCoord2f(4.0, 4.0); glVertex3f(-.4,0,5);
  glTexCoord2f(0.0, 4.0); glVertex3f(-.4,0,0);
  glEnd();

  // Road
  glBindTexture(GL_TEXTURE_2D,texture[8]);
  glBegin(GL_QUADS);
  glTexCoord2f(4.0, 0.0); glVertex3f(40,0,5);
  glTexCoord2f(4.0, 1.0); glVertex3f(40,0,7);
  glTexCoord2f(0.0, 1.0); glVertex3f(-40,0,7);
  glTexCoord2f(0.0, 0.0); glVertex3f(-40,0,5);
  glEnd(); 
  glDisable(GL_POLYGON_OFFSET_FILL);
 
  glDisable(GL_TEXTURE_2D);



}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (mode)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }

   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light)
   {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light position
        float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        ball(Position[0],Position[1],Position[2] , 0.1);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
     glDisable(GL_LIGHTING);
   // Draw scene
   drawScene();

   //  no lighting from here on
   glDisable(GL_LIGHTING);

   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
     th,ph,dim,fov,mode?"Perspective":"Orthogonal",light?"On":"Off");
   if (light)
   {
      glWindowPos2i(5,45);
      Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shinyvec[0]);
   }

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th -= 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th += 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;

   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(mode?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
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
   else if (ch == '0')
      th = ph = 0;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Switch projection mode
   else if (ch == 'p' || ch == 'P')
      mode = 1-mode;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Translate shininess power to value (-1 => 0)
   shinyvec[0] = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(mode?fov:0,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(mode?fov:0,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Assignment 7: Domenic Murtari");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Load textures
   texture[0] = LoadTexBMP("ground.bmp");
   texture[1] = LoadTexBMP("wall.bmp");
   texture[2] = LoadTexBMP("shingle.bmp");
   texture[3] = LoadTexBMP("stucco.bmp");
   texture[4] = LoadTexBMP("door.bmp");
   texture[5] = LoadTexBMP("window.bmp");
   texture[6] = LoadTexBMP("dark.bmp");
   texture[7] = LoadTexBMP("sidewalk.bmp");
   texture[8] = LoadTexBMP("road.bmp");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
