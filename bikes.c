
#include<unistd.h>
#include<GL/glut.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define   PI            3.14159
#define WIN_WIDTH      600
#define WIN_HEIGHT      600
#define CYCLE_LENGTH   300.3f
#define ROD_RADIUS      0.05f
#define ROD_RADIUS1      0.15f
#define NUM_SPOKES      360
#define SPOKE_ANGLE      1
#define RADIUS_WHEEL   1.0f
#define TUBE_WIDTH      0.15f
#define RIGHT_ROD      1.6f
#define RIGHT_ANGLE     60.0f
#define MIDDLE_ROD      1.5f
#define MIDDLE_ANGLE   106.0f
#define BACK_CONNECTOR   0.5f
#define LEFT_ANGLE      50.0f
#define WHEEL_OFFSET   0.11f
#define WHEEL_LEN      1.1f
#define TOP_LEN         1.5f
#define CRANK_ROD      0.7f
#define CRANK_RODS      1.12f
#define CRANK_ANGLE      8.0f
#define HANDLE_ROD      1.5f
#define FRONT_INCLINE   70.0f
#define HANDLE_LIMIT   70.0f

#define INC_STEERING   2.0f
#define INC_SPEED      0.05f

GLfloat pedalAngle, speed, steering;

GLfloat camx,camy,camz;
GLfloat anglex,angley,anglez;


int prevx,prevy;
GLenum Mouse;


GLfloat xpos,zpos,direction;

void ZCylinder(GLfloat radius,GLfloat length);
void XCylinder(GLfloat radius,GLfloat length);

void drawFrame(void);
void drawFrame1(void);
void gear( GLfloat inner_radius, GLfloat outer_radius,
        GLfloat width,GLint teeth, GLfloat tooth_depth );
void drawChain(void);
void drawPedals(void);
void drawTyre(void);
void drawTyre1(void);

void drawSeat(void);
void help(void);
void init(void);
void reset(void);
void display(void);
void idle(void);
void updateScene(void);
void landmarks(void);
void special(int key,int x,int y);
void keyboard(unsigned char key,int x,int y);
void mouse(int button,int state,int x,int y);
void motion(int x,int y);
void reshape(int w,int h);
void glSetupFuncs(void);
GLfloat Abs(GLfloat);
GLfloat degrees(GLfloat);
GLfloat radians(GLfloat);
GLfloat angleSum(GLfloat, GLfloat);



void ZCylinder(GLfloat radius,GLfloat length)
{
   GLUquadricObj *cylinder;
   cylinder=gluNewQuadric();
   glPushMatrix();
      glTranslatef(0.0f,0.0f,0.0f);
      gluCylinder(cylinder,radius,radius,length,15,5);
   glPopMatrix();
}

void XCylinder(GLfloat radius,GLfloat length)
{
   glPushMatrix();
      glRotatef(90.0f,0.0f,1.0f,0.0f);
      ZCylinder(radius,length);
   glPopMatrix();
}

void drawTyre1(void)
{
   int i;
   //   Draw The Rim
   glColor3f(0.0f,0.0f,0.0f);
   glutSolidTorus(0.12f,0.63f,4,30);
   //   Draw The Central Cylinder
   //   Length of cylinder  0.12f
   glColor3f(0.0f,0.0f,0.0f);
   glPushMatrix();
      glTranslatef(0.0f,0.0f,-0.06f);
      ZCylinder(0.02f,0.12f);
   glPopMatrix();
   glutSolidTorus(0.20f,0.20f,3,20);

   //   Draw The Spokes
   glColor3f(1.0f,3.0f,2.0f);
   for(i=0;i<NUM_SPOKES;++i)
   {
      glPushMatrix();
         glRotatef(i*SPOKE_ANGLE,0.0f,0.0f,1.0f);
         glBegin(GL_LINES);
            glVertex3f(0.0f,0.02f,0.0f);
            glVertex3f(0.0f,0.86f,0.0f);
         glEnd();
      glPopMatrix();
   }

   //   Draw The Tyre
   glColor3f(0.0f,0.0f,0.0f);
   glutSolidTorus(TUBE_WIDTH+0.1,RADIUS_WHEEL,10,30);
   glColor3f(1.0f,0.0f,0.0f);
}

void updateScene()
{
   GLfloat xDelta, zDelta;
   GLfloat rotation;
   GLfloat sin_steering, cos_steering;
   if (-INC_SPEED < speed && speed < INC_SPEED) return;

   if(speed < 0.0f)
         pedalAngle = speed = 0.0f;



   xDelta = speed*cos(radians(direction + steering));
   zDelta = speed*sin(radians(direction + steering));
   xpos += xDelta;
   zpos -= zDelta;
   pedalAngle = degrees(angleSum(radians(pedalAngle), speed/RADIUS_WHEEL));


   sin_steering = sin(radians(steering));
   cos_steering = cos(radians(steering));


   rotation = atan2(speed * sin_steering, CYCLE_LENGTH + speed * cos_steering);
   direction = degrees(angleSum(radians(direction),rotation));
}
GLfloat angleSum(GLfloat a, GLfloat b)
{
  a += b;
  if (a < 0) return a+2*PI;
  else if (a > 2*PI) return a-2*PI;
  else return a;
}

void drawFrame()
{
   glColor3f(1.0f,0.0f,0.0f);

    glTranslatef(0,0,-2.5);

   glPushMatrix();

      glPushMatrix();

         glColor3f(0.0f,1.0f,0.0f);


         glPushMatrix();
            glTranslatef(0.0f,0.0f,0.06f);
            glRotatef(-2*pedalAngle,0.0f,0.0f,1.0f);
            gear(0.08f,0.3f,0.03f,30,0.03f);
         glPopMatrix();

         glColor3f(1.0f,0.0f,0.0f);
         glTranslatef(0.0f,0.0f,-0.2f);
         ZCylinder(0.08f,0.32f);
      glPopMatrix();

      glRotatef(RIGHT_ANGLE,0.0f,0.0f,1.0f);
      glTranslatef(0,0,0);

      XCylinder(ROD_RADIUS,RIGHT_ROD);


      glRotatef(MIDDLE_ANGLE-RIGHT_ANGLE,0.0f,0.0f,1.0f);
      XCylinder(ROD_RADIUS,MIDDLE_ROD);

      glColor3f(1.0f,1.0f,0.0f);
      glTranslatef(MIDDLE_ROD,0.0f,0.0f);
      glRotatef(-MIDDLE_ANGLE,0.0f,0.0f,1.0f);
      glScalef(0.3f,ROD_RADIUS,0.25f);
      drawSeat();

      glColor3f(1.0f,0.0f,0.0f);
   glPopMatrix();

   glPushMatrix();
      glRotatef(-180.0f,0.0f,1.0f,0.0f);
      XCylinder(ROD_RADIUS1,BACK_CONNECTOR);



      glPushMatrix();
         glTranslatef(0.5f,0.0f,WHEEL_OFFSET+0.2);
         glRotatef(15,0,0,1);
         XCylinder(ROD_RADIUS1,RADIUS_WHEEL+TUBE_WIDTH+1);
      glPopMatrix();
      glPushMatrix();
         glTranslatef(0.5f,0.0f,-WHEEL_OFFSET-0.2);
           glRotatef(15,0,0,1);
         XCylinder(ROD_RADIUS1,RADIUS_WHEEL+TUBE_WIDTH+1);
      glPopMatrix();
//carrier ha

            glPushMatrix();
         glTranslatef(0.5f,1.36f,WHEEL_OFFSET);
         glRotatef(7,0,0,1);
         XCylinder(ROD_RADIUS1-0.05,RADIUS_WHEEL+TUBE_WIDTH);
      glPopMatrix();
            glPushMatrix();
         glTranslatef(1.5f,1.5f,WHEEL_OFFSET);
         glRotatef(-8,0,0,1);
         XCylinder(ROD_RADIUS1-0.05,RADIUS_WHEEL+TUBE_WIDTH);
      glPopMatrix();



      glPushMatrix();
         glTranslatef(0.5f,0.0f,WHEEL_OFFSET);
//
         XCylinder(ROD_RADIUS,RADIUS_WHEEL+TUBE_WIDTH);
      glPopMatrix();
      glPushMatrix();
         glTranslatef(0.5f,0.0f,-WHEEL_OFFSET);
  //         glRotatef(15,0,0,1);
         XCylinder(ROD_RADIUS,RADIUS_WHEEL+TUBE_WIDTH);
      glPopMatrix();

   glPopMatrix();


   glPushMatrix();
      glTranslatef(-(BACK_CONNECTOR+RADIUS_WHEEL+TUBE_WIDTH),0.0f,0.0f);

      glPushMatrix();
         glRotatef(-2*pedalAngle,0.0f,0.0f,1.0f);
         drawTyre();

         glColor3f(0.0f,1.0f,0.0f);
            gear(0.03f,0.15f,0.03f,20,0.03f);
         glColor3f(1.0f,0.0f,0.0f);
      glPopMatrix();
      glRotatef(LEFT_ANGLE,0.0f,0.0f,1.0f);


      glPushMatrix();
         glTranslatef(0.0f,0.0f,-WHEEL_OFFSET);
         XCylinder(ROD_RADIUS,WHEEL_LEN);
      glPopMatrix();
      glPushMatrix();
         glTranslatef(0.0f,0.0f,WHEEL_OFFSET);
         XCylinder(ROD_RADIUS,WHEEL_LEN);
      glPopMatrix();


      glTranslatef(WHEEL_LEN,0.0f,0.0f);
      XCylinder(ROD_RADIUS,CRANK_ROD);


      glTranslatef(CRANK_ROD,0.0f,0.0f);
      glRotatef(-LEFT_ANGLE,0.0f,0.0f,1.0f);
      XCylinder(ROD_RADIUS,TOP_LEN);


      glTranslatef(TOP_LEN,0.0f,0.0f);
      glRotatef(-FRONT_INCLINE,0.0f,0.0f,1.0f);


      glPushMatrix();
         glTranslatef(-0.1f,0.0f,0.0f);
         XCylinder(ROD_RADIUS,0.45f);
      glPopMatrix();


      glPushMatrix();
         glRotatef(-steering,1.0f,0.0f,0.0f);

         glTranslatef(-0.3f,0.0f,0.0f);


         glPushMatrix();
            glRotatef(FRONT_INCLINE,0.0f,0.0f,1.0f);

            glPushMatrix();
               glTranslatef(0.0f,0.0f,-HANDLE_ROD/2);
               ZCylinder(ROD_RADIUS,HANDLE_ROD);
            glPopMatrix();

            glPushMatrix();
               glColor3f(1.0f,1.0f,0.0f);
               glTranslatef(0.0f,0.0f,-HANDLE_ROD/2);
               ZCylinder(0.07f,HANDLE_ROD/4);
               glTranslatef(0.0f,0.0f,HANDLE_ROD*3/4);
               ZCylinder(0.07f,HANDLE_ROD/4);
               glColor3f(1.0f,0.0f,0.0f);
            glPopMatrix();
         glPopMatrix();

         glPushMatrix();

            XCylinder(ROD_RADIUS,CRANK_ROD);


            glTranslatef(CRANK_ROD,0.0f,0.0f);
            glRotatef(CRANK_ANGLE,0.0f,0.0f,1.0f);


            glPushMatrix();
               glTranslatef(0.0f,0.0f,WHEEL_OFFSET);
               XCylinder(ROD_RADIUS,CRANK_RODS);
            glPopMatrix();
            glPushMatrix();
               glTranslatef(0.0f,0.0f,-WHEEL_OFFSET);
               XCylinder(ROD_RADIUS,CRANK_RODS);
            glPopMatrix();

            glTranslatef(CRANK_RODS,0.0f,0.0f);
            glRotatef(-2*pedalAngle,0.0f,0.0f,1.0f);
            drawTyre();
         glPopMatrix();
      glPopMatrix();
   glPopMatrix();







   glPushMatrix();
    glTranslatef(0,0,0);
    glRotatef(180,0,1,1);
glColor3f(1,0,0);
    glutSolidTeapot(0.4);
    glPopMatrix();
       glPushMatrix();
    glTranslatef(0.6,1.1,0);
    glRotatef(180,1,0,0);
    glutSolidTeapot(0.6);
    glPopMatrix();
    //real 2 tank





       glPushMatrix();
       glColor3f(1,0,0);
    glTranslatef(0.2,0.8,0);
    //glRotatef(180,1,0,0);
    glutSolidCube(0.75);
    glPopMatrix();



       glPushMatrix();
       glColor3f(1,0,0);
    glTranslatef(1.25,1.35,0);
    //glRotatef(180,1,0,0);
    glutSolidSphere(0.4,10,10);
    glPopMatrix();



       glPushMatrix();
       glColor3f(1,1,0);
    glTranslatef(1.65,1.4,0);
    glRotatef(270,0,1,0);
    glutSolidCone(0.25,0.5,10,10);
    glPopMatrix();





}

void drawFrame1()
{
   glColor3f(1.0f,0.0f,0.0f);
   glTranslatef(0,0,2.5);


   glPushMatrix();

      glPushMatrix();

         glColor3f(0.0f,1.0f,0.0f);


         //hacksaw 1
         glPushMatrix();
            glTranslatef(-0.2f,0.90f,0.06f);//hacksaw like struct
            glRotatef(90,0,1,0);
            glColor3f(0,1,0);
            gear(0.18f,0.23f,0.53f,30,0.03f);
         glPopMatrix();

          //hacksaw 2
         glPushMatrix();
            glTranslatef(-0.2f,0.50f,0.06f);//hacksaw like struct
            glRotatef(90,1,0,0);
            glColor3f(0,1,0);
            gear(0.18f,0.23f,0.53f,30,0.03f);
         glPopMatrix();

         glColor3f(0.0f,0.0f,0.0f);
         glTranslatef(0.0f,0.0f,-0.2f);
         ZCylinder(0.08f,0.32f);
      glPopMatrix();

      glRotatef(RIGHT_ANGLE,0.0f,0.0f,1.0f);
      glTranslatef(0,0,0);
      //glutSolidTeapot(1);
      XCylinder(ROD_RADIUS,RIGHT_ROD);


      glRotatef(MIDDLE_ANGLE-RIGHT_ANGLE,0.0f,0.0f,1.0f);
      XCylinder(ROD_RADIUS,MIDDLE_ROD);

      glColor3f(0.0f,0.0f,0.0f);
      glTranslatef(MIDDLE_ROD,0.0f,0.0f);
      glRotatef(-MIDDLE_ANGLE,0.0f,0.0f,1.0f);
      glScalef(0.3f,ROD_RADIUS,0.25f);
      drawSeat();

      glColor3f(0.0f,0.0f,0.0f);
   glPopMatrix();

   glPushMatrix();
      glRotatef(-180.0f,0.0f,1.0f,0.0f);
	glColor3f(0,1,0);
      XCylinder(ROD_RADIUS1+0.1,BACK_CONNECTOR);







      // new silencer
      glPushMatrix();
         glTranslatef(-1,-0.3f,-WHEEL_OFFSET-0.2);

glColor3f(1,1,1);
         XCylinder(ROD_RADIUS1,RADIUS_WHEEL+TUBE_WIDTH+1);
      glPopMatrix();

      glPushMatrix();
         glTranslatef(-1,-0.3,-WHEEL_OFFSET-0.2);

         glutSolidSphere(0.15,20,20);
               glPopMatrix();

               glPushMatrix();
         glTranslatef(0.4,-0.3f,-WHEEL_OFFSET-0.2);
glColor3f(1,1,1);

         XCylinder(ROD_RADIUS1-0.05,RADIUS_WHEEL+TUBE_WIDTH);
      glPopMatrix();


               glPushMatrix();
         glTranslatef(1.5,-0.325f,-WHEEL_OFFSET-0.2);
           glRotatef(30,0,0,1);
         XCylinder(ROD_RADIUS1-0.08,RADIUS_WHEEL+TUBE_WIDTH-0.8);
      glPopMatrix();

//silence mirror
      glPushMatrix();
         glTranslatef(-1,-0.3f,-WHEEL_OFFSET+0.4);

         XCylinder(ROD_RADIUS1,RADIUS_WHEEL+TUBE_WIDTH+1);
      glPopMatrix();

      glPushMatrix();
         glTranslatef(-1,-0.3,-WHEEL_OFFSET+0.4);

         glutSolidSphere(0.15,20,20);
               glPopMatrix();

               glPushMatrix();
         glTranslatef(0.4,-0.3f,-WHEEL_OFFSET+0.4);

         XCylinder(ROD_RADIUS1-0.05,RADIUS_WHEEL+TUBE_WIDTH);
      glPopMatrix();


               glPushMatrix();
         glTranslatef(1.5,-0.325f,-WHEEL_OFFSET+0.4);
           glRotatef(30,0,0,1);
         XCylinder(ROD_RADIUS1-0.08,RADIUS_WHEEL+TUBE_WIDTH-0.8);
      glPopMatrix();


//carrier ha neeche



      glPushMatrix();
         glTranslatef(0.5f,0.0f,WHEEL_OFFSET);

         XCylinder(ROD_RADIUS,RADIUS_WHEEL+TUBE_WIDTH);
      glPopMatrix();
      glPushMatrix();
         glTranslatef(0.5f,0.0f,-WHEEL_OFFSET);

         XCylinder(ROD_RADIUS,RADIUS_WHEEL+TUBE_WIDTH);
      glPopMatrix();

   glPopMatrix();


   glPushMatrix();
    glColor3f(0,0,0);
      glTranslatef(-(BACK_CONNECTOR+RADIUS_WHEEL+TUBE_WIDTH),0.0f,0.0f);

      glPushMatrix();
         glRotatef(-2*pedalAngle,0.0f,0.0f,1.0f);

       drawTyre1();
         glColor3f(0.0f,1.0f,0.0f);
            gear(0.03f,0.15f,0.03f,20,0.03f);
         glColor3f(1.0f,0.0f,0.0f);
      glPopMatrix();
      glRotatef(LEFT_ANGLE,0.0f,0.0f,1.0f);

      glPushMatrix();
        glColor3f(0,0,0);
         glTranslatef(0.0f,0.0f,-WHEEL_OFFSET);
         XCylinder(ROD_RADIUS,WHEEL_LEN);
      glPopMatrix();
      glPushMatrix();
        glColor3f(0,0,0);
         glTranslatef(0.0f,0.0f,WHEEL_OFFSET);
         XCylinder(ROD_RADIUS,WHEEL_LEN);
      glPopMatrix();


      glTranslatef(WHEEL_LEN,0.0f,0.0f);
      glColor3f(0,0,0);
      XCylinder(ROD_RADIUS,CRANK_ROD);

      glTranslatef(CRANK_ROD,0.0f,0.0f);
      glRotatef(-LEFT_ANGLE,0.0f,0.0f,1.0f);
      XCylinder(ROD_RADIUS,TOP_LEN);
  glTranslatef(TOP_LEN,0.0f,0.0f);
      glRotatef(-FRONT_INCLINE,0.0f,0.0f,1.0f);

      glPushMatrix();
        glColor3f(1,0,0);
         glTranslatef(-0.1f,0.0f,0.0f);
         XCylinder(ROD_RADIUS,0.45f);
      glPopMatrix();

      glPushMatrix();
         glRotatef(-steering,1.0f,0.0f,0.0f);
         glTranslatef(-0.3f,0.0f,0.0f);
         glPushMatrix();
            glRotatef(FRONT_INCLINE,0.0f,0.0f,1.0f);

            glPushMatrix();
               glTranslatef(0.0f,0.0f,-HANDLE_ROD/2);
               ZCylinder(ROD_RADIUS,HANDLE_ROD);
            glPopMatrix();

            glPushMatrix();
               glColor3f(0.0f,0.0f,0.0f);
               glTranslatef(0.0f,0.0f,-HANDLE_ROD/2);
               ZCylinder(0.07f,HANDLE_ROD/4);
               glTranslatef(0.0f,0.0f,HANDLE_ROD*3/4);
               ZCylinder(0.07f,HANDLE_ROD/4);
               glColor3f(1.0f,0.0f,0.0f);
            glPopMatrix();
         glPopMatrix();

         glPushMatrix();
            glColor3f(0,0,0);
            XCylinder(ROD_RADIUS+0.25,CRANK_ROD);

            glTranslatef(CRANK_ROD,0.0f,0.0f);
            glRotatef(CRANK_ANGLE,0.0f,0.0f,1.0f);

            glPushMatrix();
               glTranslatef(0.0f,0.0f,WHEEL_OFFSET);
               XCylinder(ROD_RADIUS,CRANK_RODS);
            glPopMatrix();
            glPushMatrix();
               glTranslatef(0.0f,0.0f,-WHEEL_OFFSET);
               XCylinder(ROD_RADIUS,CRANK_RODS);
            glPopMatrix();

            glTranslatef(CRANK_RODS,0.0f,0.0f);
            glRotatef(-2*pedalAngle,0.0f,0.0f,1.0f);
            drawTyre1();
         glPopMatrix();
      glPopMatrix();
      glPopMatrix();






    //tank
    glPushMatrix();
    glTranslatef(0,0,0);
    glRotatef(180,0,1,1);
    glColor3f(0,1,0);
    glutSolidTeapot(0.4);
    glPopMatrix();

    //cube

       glPushMatrix();
       glColor3f(0,0,0);
    glTranslatef(0.7,0.4,0);

    glutSolidCube(1.1);

    glPopMatrix();

    //main engine
    GLUquadricObj *cylinder;
    cylinder=gluNewQuadric();

       glPushMatrix();
       glColor3f(0,0,0);
    glTranslatef(0.2,0.4,0);
    glRotatef(90,0,1,0);
    gluCylinder(cylinder,0.7,0.7,0.2,15,5);

    glPopMatrix();


   glPushMatrix();
       glColor3f(0,0,0);
    glTranslatef(0.45,0.4,0);
    glRotatef(90,0,1,0);
    gluCylinder(cylinder,0.7,0.7,0.2,15,5);

    glPopMatrix();


   glPushMatrix();
       glColor3f(0,0,0);
    glTranslatef(0.7,0.4,0);
    glRotatef(90,0,1,0);
    gluCylinder(cylinder,0.7,0.7,0.2,15,5);

    glPopMatrix();


   glPushMatrix();
       glColor3f(0,0,0);
    glTranslatef(0.95,0.4,0);
    glRotatef(90,0,1,0);
    gluCylinder(cylinder,0.7,0.7,0.2,15,5);
    glPopMatrix();


    //engine ends


       glPushMatrix();
       glColor3f(0,1,0);
    glTranslatef(1.5,1.35,0);

    glutSolidSphere(0.44,10,10);
    glPopMatrix();


       glPushMatrix();
       glColor3f(0,1,0);
    glTranslatef(-0.1,1,0);
   glutSolidSphere(0.29,10,10);
    glPopMatrix();



//petrol tank final

  glPushMatrix();
       glColor3f(0,1,0);
    glTranslatef(-0.1,1,0);
    glRotatef(90,0,1,0);
    glRotatef(-12.5,1,0,0);

    glRotatef(90,0,0,1);
    gluCylinder(cylinder,0.3,0.45,1.7,15,5);

    glPopMatrix();


    //ends


       glPushMatrix();
       glColor3f(1,1,0);
    glTranslatef(1.9,1.5,0.15);
    glRotatef(-90,0,1,0);
    glRotatef(90,0,0,1);

    glutSolidCone(0.15,0.5,10,10);
    glPopMatrix();

       glPushMatrix();
       glColor3f(1,1,0);
    glTranslatef(1.9,1.5,-0.15);
    glRotatef(-90,0,1,0);
    glRotatef(90,0,0,1);
    glutSolidCone(0.15,0.5,10,10);
    glPopMatrix();








}




void gear( GLfloat inner_radius, GLfloat outer_radius, GLfloat width,
        GLint teeth, GLfloat tooth_depth )
{
    GLint i;
    GLfloat r0, r1, r2;
    GLfloat angle, da;
    GLfloat u, v, len;
    const double pi = 3.14159264;

    r0 = inner_radius;
    r1 = outer_radius - tooth_depth/2.0;
    r2 = outer_radius + tooth_depth/2.0;

    da = 2.0*pi / teeth / 4.0;

    glShadeModel( GL_FLAT );

    glNormal3f( 0.0, 0.0, 1.0 );


    glBegin( GL_QUAD_STRIP );
    for (i=0;i<=teeth;i++) {
   angle = i * 2.0*pi / teeth;
   glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 );
   glVertex3f( r1*cos(angle), r1*sin(angle), width*0.5 );
   glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 );
   glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), width*0.5 );
    }
    glEnd();

    glBegin( GL_QUADS );
    da = 2.0*pi / teeth / 4.0;
    for (i=0;i<teeth;i++) {
   angle = i * 2.0*pi / teeth;

   glVertex3f( r1*cos(angle),      r1*sin(angle),     width*0.5 );
   glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),     width*0.5 );
   glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), width*0.5 );
   glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), width*0.5 );
    }
    glEnd();


    glNormal3f( 0.0, 0.0, -1.0 );

    glBegin( GL_QUAD_STRIP );
    for (i=0;i<=teeth;i++) {
   angle = i * 2.0*pi / teeth;
   glVertex3f( r1*cos(angle), r1*sin(angle), -width*0.5 );
   glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 );
   glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 );
   glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 );
    }
    glEnd();

    glBegin( GL_QUADS );
    da = 2.0*pi / teeth / 4.0;
    for (i=0;i<teeth;i++) {
   angle = i * 2.0*pi / teeth;

   glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 );
   glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), -width*0.5 );
   glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),     -width*0.5 );
   glVertex3f( r1*cos(angle),      r1*sin(angle),     -width*0.5 );
    }
    glEnd();


    glBegin( GL_QUAD_STRIP );
    for (i=0;i<teeth;i++) {
   angle = i * 2.0*pi / teeth;

   glVertex3f( r1*cos(angle),      r1*sin(angle),      width*0.5 );
   glVertex3f( r1*cos(angle),      r1*sin(angle),     -width*0.5 );
   u = r2*cos(angle+da) - r1*cos(angle);
   v = r2*sin(angle+da) - r1*sin(angle);
   len = sqrt( u*u + v*v );
   u /= len;
   v /= len;
   glNormal3f( v, -u, 0.0 );
   glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),      width*0.5 );
   glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),     -width*0.5 );
   glNormal3f( cos(angle), sin(angle), 0.0 );
   glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da),  width*0.5 );
   glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), -width*0.5 );
   u = r1*cos(angle+3*da) - r2*cos(angle+2*da);
   v = r1*sin(angle+3*da) - r2*sin(angle+2*da);
   glNormal3f( v, -u, 0.0 );
   glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da),  width*0.5 );
   glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 );
   glNormal3f( cos(angle), sin(angle), 0.0 );
    }

    glVertex3f( r1*cos(0.0), r1*sin(0.0), width*0.5 );
    glVertex3f( r1*cos(0.0), r1*sin(0.0), -width*0.5 );

    glEnd();


    glShadeModel( GL_SMOOTH );

    glBegin( GL_QUAD_STRIP );
    for (i=0;i<=teeth;i++) {
   angle = i * 2.0*pi / teeth;
   glNormal3f( -cos(angle), -sin(angle), 0.0 );
   glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 );
   glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 );
    }
    glEnd();

}

void drawChain()
{
   GLfloat depth;
   static int mode=0;

   glColor3f(0.0f,1.0f,0.0f);
   glEnable(GL_LINE_STIPPLE);
   mode=(mode+1)%2;

   if(mode==0 && speed>0)
      glLineStipple(1,0x1c47);
   else if(mode==1 && speed>0)
      glLineStipple(1,0x00FF);

   glBegin(GL_LINES);
   for(depth=0.06f;depth<=0.12f;depth+=0.01f)
   {
      glVertex3f(-1.6f,0.15f,ROD_RADIUS);
      glVertex3f(0.0f,0.3f,depth);

      glVertex3f(-1.6f,-0.15f,ROD_RADIUS);
      glVertex3f(0.0f,-0.3f,depth);
   }
   glEnd();
   glDisable(GL_LINE_STIPPLE);
}

void drawSeat()
{

   glBegin(GL_POLYGON);
      glVertex3f(-0.2f, 1.0f, -0.5f);
      glVertex3f(   2.0f, 1.0f, -0.3f);
      glVertex3f( 2.0f, 1.0f,  0.3f);
      glVertex3f(-0.2f, 1.0f,  0.5f);
      glVertex3f(-1.0f, 1.0f,  1.0f);
      glVertex3f(-2.0f, 1.0f,  1.0f);
      glVertex3f(-2.0f, 1.0f, -1.0f);
      glVertex3f(-01.0f, 1.0f, -1.0f);
   glEnd();


   glBegin(GL_POLYGON);
      glVertex3f(-0.2f, -1.0f, -0.5f);
      glVertex3f(   2.0f, -1.0f, -0.3f);
      glVertex3f( 2.0f, -1.0f,  0.3f);
      glVertex3f(-0.2f, -1.0f,  0.5f);
      glVertex3f(-1.0f, -1.0f,  1.0f);
      glVertex3f(-2.0f, -1.0f,  1.0f);
      glVertex3f(-2.0f, -1.0f, -1.0f);
      glVertex3f(-2.0f, -1.0f, -1.0f);
   glEnd();

   glBegin(GL_QUADS);
      glVertex3f(2.0f,1.0f,-0.3f);
      glVertex3f(2.0f,1.0f,0.3f);
      glVertex3f(2.0f,-1.0f,0.3f);
      glVertex3f(2.0f,-1.0f,-0.3f);

      glVertex3f(2.0f,1.0f,0.3f);
      glVertex3f(-0.2f,1.0f,0.5f);
      glVertex3f(-0.2f,-1.0f,0.5f);
      glVertex3f(2.0f,-1.0f,0.3f);

      glVertex3f(2.0f,1.0f,-0.3f);
      glVertex3f(-0.2f,1.0f,-0.5f);
      glVertex3f(-0.2f,-1.0f,-0.5f);
      glVertex3f(2.0f,-1.0f,-0.3f);

      glVertex3f(-0.2f,1.0f,0.5f);
      glVertex3f(-1.0f,1.0f,1.0f);
      glVertex3f(-1.0f,-1.0f,1.0f);
      glVertex3f(-0.2f,-1.0f,0.5f);

      glVertex3f(-0.2f,1.0f,-0.5f);
      glVertex3f(-1.0f,1.0f,-1.0f);
      glVertex3f(-1.0f,-1.0f,-1.0f);
      glVertex3f(-0.2f,-1.0f,-0.5f);

      glVertex3f(-1.0f,1.0f,1.0f);
      glVertex3f(-2.0f,1.0f,1.0f);
      glVertex3f(-2.0f,-1.0f,1.0f);
      glVertex3f(-1.0f,-1.0f,1.0f);

      glVertex3f(-1.0f,1.0f,-1.0f);
      glVertex3f(-2.0f,1.0f,-1.0f);
      glVertex3f(-2.0f,-1.0f,-1.0f);
      glVertex3f(-1.0f,-1.0f,-1.0f);

      glVertex3f(-2.0f,1.0f,1.0f);
      glVertex3f(-2.0f,1.0f,-1.0f);
      glVertex3f(-2.0f,-1.0f,-1.0f);
      glVertex3f(-2.0f,-1.0f,1.0f);

   glEnd();


}
void drawTyre(void)
{
   int i;
   glColor3f(1.0f,1.0f,1.0f);
   glutSolidTorus(0.06f,0.83f,4,30);
   // Central Cylinder

   glColor3f(1.0f,1.0f,0.5f);
   glPushMatrix();
      glTranslatef(0.0f,0.0f,-0.06f);
      ZCylinder(0.02f,0.12f);
   glPopMatrix();
   glutSolidTorus(0.02f,0.02f,3,20);

   //   Spokes
   glColor3f(1.0f,1.0f,0.0f);
   for(i=0;i<NUM_SPOKES;++i)
   {
      glPushMatrix();
         glRotatef(i*SPOKE_ANGLE,0.0f,0.0f,1.0f);
         glBegin(GL_LINES);
            glVertex3f(0.0f,0.02f,0.0f);
            glVertex3f(0.0f,0.86f,0.0f);
         glEnd();
      glPopMatrix();
   }

   //    Tyre
   glColor3f(0.0f,0.0f,0.0f);
   glutSolidTorus(TUBE_WIDTH,RADIUS_WHEEL,10,30);
   glColor3f(1.0f,0.0f,0.0f);
}

void init()
{
   GLfloat mat_specular[]={1.0,1.0,1.0,1.0};
   GLfloat mat_shininess[]={100.0};
   GLfloat light_directional[]={1.0,1.0,1.0,1.0};
   GLfloat light_positional[]={1.0,1.0,1.0,0.0};
   GLfloat light_diffuse[]={1.0,1.0,1.0};

   reset();

   glShadeModel(GL_SMOOTH);

   glLightfv(GL_LIGHT0,GL_POSITION,light_directional);
   glLightfv(GL_LIGHT0,GL_AMBIENT,light_diffuse);
   glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
   glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
   glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
   glColorMaterial(GL_FRONT,GL_DIFFUSE);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_COLOR_MATERIAL);
   glEnable(GL_DEPTH_TEST);
}

void landmarks(void)
{
      GLfloat i;
      glColor3f(0.0f,1.0f,0.0f);


      glBegin(GL_LINES);
      for(i=-100.0f ; i<100.0f ; i += 0.10f)
      {
         glVertex3f(-100.0f,-RADIUS_WHEEL,i);
         glVertex3f( 100.0f,-RADIUS_WHEEL,i);
         glVertex3f(i,-RADIUS_WHEEL,-100.0f);
         glVertex3f(i,-RADIUS_WHEEL,100.0f);
      }
      glEnd();
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_NORMALIZE);

   glPushMatrix();

      glRotatef(angley,1.0f,0.0f,0.0f);
      glRotatef(anglex,0.0f,1.0f,0.0f);
      glRotatef(anglez,0.0f,0.0f,1.0f);



      landmarks();

    glPushMatrix();
         glTranslatef(xpos,0.0f,zpos);
         glRotatef(direction,0.0f,1.0f,0.0f);
         drawFrame();
         drawFrame1();
         drawChain();
    glPopMatrix();

   glPopMatrix();
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(camx,camy,camz,  camx,0.0,0.0,  0.0,1.0,0.0);

   glutSwapBuffers();
}

GLfloat Abs(GLfloat a)
{
   if(a < 0.0f)
      return -a;
   else
      return a;
}


GLfloat degrees(GLfloat a)
{
   return a*180.0f/PI;
}


GLfloat radians(GLfloat a)
{
   return a*PI/180.0f;
}


void idle(void)
{
   updateScene();
   glutPostRedisplay();
}

void special(int key,int x,int y)
{
   switch(key)
   {
      case GLUT_KEY_UP:
         camz -= 0.1f;
         break;
      case GLUT_KEY_DOWN:
         camz += 0.1f;
         break;
      case GLUT_KEY_LEFT:
         camx -= 0.1f;
         break;
      case GLUT_KEY_RIGHT:
         camx += 0.1f;
         break;
   }
   glutPostRedisplay();
}



void reset()
{
   anglex=angley=anglez=0.0f;
   pedalAngle=steering=0.0f;
   Mouse=GLUT_UP;
   pedalAngle=speed=steering=0.0f;
   camx=camy=0.0f;
   camz=5.0f;
   xpos=zpos=0.0f;
   direction=0.0f;
}

void keyboard(unsigned char key,int x,int y)
{
   GLfloat r=0.0f;

   switch(key)
   {
      case 's':
      case 'S':
         reset();
         break;
      case 'z':
         if(steering < HANDLE_LIMIT)
               steering += INC_STEERING;
         break;
      case 'b':
         if(steering > -HANDLE_LIMIT)
            steering -= INC_STEERING;
         break;
      case '+':
         speed += INC_SPEED;
         break;
      case '-':
         speed -= INC_SPEED;
         break;
      case 27:
         exit(1);
   }



   pedalAngle += speed;
   if(speed < 0.0f)
      speed = 0.0f;
   if(pedalAngle < 0.0f)
      pedalAngle = 0.0f;
   if(pedalAngle >= 360.0f)
      pedalAngle -= 360.0f;


   glutPostRedisplay();
}

void mouse(int button,int state,int x,int y)
{
   switch(button)
   {
      case GLUT_LEFT_BUTTON:
         if(state==GLUT_DOWN)
         {
            Mouse=GLUT_DOWN;
            prevx=x;
            prevy=y;
         }
         if(state==GLUT_UP)
         {
            Mouse=GLUT_UP;
         }
         break;
      case GLUT_RIGHT_BUTTON:
         /*   DO NOTHING   */
         break;
   }
   glutPostRedisplay();
}

void passive(int x,int y)
{
}

void motion(int x,int y)
{
   if(Mouse==GLUT_DOWN)
   {
      int deltax,deltay;
      deltax=prevx-x;
      deltay=prevy-y;
      anglex += 0.5*deltax;
      angley += 0.5*deltay;
      if(deltax!=0 && deltay!=0)
         anglez += 0.5*sqrt(deltax*deltax + deltay*deltay);

      if(anglex < 0)
         anglex+=360.0;
      if(angley < 0)
         angley+=360.0;
      if(anglez < 0)
         anglez += 360.0;

      if(anglex > 360.0)
         anglex-=360.0;
      if(angley > 360.0)
         angley-=360.0;
      if(anglez > 360.0)
         anglez-=360.0;
   }
   else
   {
      Mouse=GLUT_UP;
   }
   prevx=x;
   prevy=y;
   glutPostRedisplay();
}

void reshape(int w,int h)
{
   glViewport(0,0,(GLsizei)w,(GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0,(GLfloat)w/(GLfloat)h,0.1,100.0);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(camx,camy,camz,  0.0,0.0,0.0,  0.0,1.0,0.0);
}

void glSetupFuncs(void)
{
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutIdleFunc(idle);
   glutSpecialFunc(special);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutPassiveMotionFunc(passive);
   glutSetCursor(GLUT_CURSOR_CROSSHAIR);
}

void help(void)
{
   printf("3D Model of Exotic Bikes\n");
   printf("Group Project made By:Nishrutha T(USN 1JS20CS104) and Rajeev Gubbi(1JS20CS126)\n\n");
   printf("'+' to increase the speed\n");
   printf("'-' to decrease the speed\n");
   printf("'s' or 'S' to reset the scene\n");
   printf("Arrow keys to move the camera\n");
   printf("Mouse to move the scene\n");
}

int main(int argc,char *argv[])
{
   help();
   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowPosition(200,200);
   glutInitWindowSize(WIN_WIDTH,WIN_HEIGHT);
   glutCreateWindow(" 3D Exotic Bikes");
   init();
   glSetupFuncs();
   glutMainLoop();
}
