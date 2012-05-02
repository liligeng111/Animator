// The sample model.  You should build a file
// very similar to this for when you make your model.
#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "modelerglobals.h"
#include "particlesystem.h"

#include <FL/gl.h>
#include "mat.h"
#include <math.h>

Mat4f getModelViewMatrix();
void SpawnParticles( Mat4f cameraTransform );

// To make a SampleModel, we inherit off of ModelerView
class SampleModel : public ModelerView 
{
public:
    SampleModel(int x, int y, int w, int h, char *label) 
        : ModelerView(x,y,w,h,label) { }

    virtual void draw();
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createSampleModel(int x, int y, int w, int h, char *label)
{ 
    return new SampleModel(x,y,w,h,label); 
}

void drawRoundCylinder(double h, double r1, double r2)
{
	glPushMatrix();
	glTranslated(0, 0, r1);
	drawSphere(r1);
	if (h - r1 - r2 > 0) drawCylinder(h - r1 - r2, r1, r2);
	glTranslated(0, 0, h - r1 - r2);
	drawSphere(r2);
	glPopMatrix();
}

/*
void drawTail()
{
	float gap = 0.025;
	float y = -1 / VAL(TAIL_X) * VAL(TAIL_X) * VAL(TAIL_X);
	float x = 1 / VAL(TAIL_Y) * VAL(TAIL_Y) * VAL(TAIL_Y);
	float u = 360 * gap / (2 * 3.1415 * sqrt(x * x + y * y));
	glRotated(180, 1, 0, 0);
	for (int i = 0; i < VAL(TAIL_LENGTH) / gap; i++)
	{
		drawCylinder(gap, 0.1, 0.1);
		glRotated(u, x, y, 0);
		glTranslated(0, 0 , gap);
	}
}
*/

void drawWedge() {
}


void SampleModel::draw()
{

  // This call takes care of a lot of the nasty projection 
  // matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...
  ModelerView::draw();
	
  Mat4f CameraMatrix = getModelViewMatrix();

	// draw the floor
  // let's make it big !!!
	setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(COLOR_BLUE);
	glPushMatrix();
	glScaled(VAL(CARPET_SIZE), VAL(CARPET_SIZE), VAL(CARPET_SIZE));
	drawSierpinskiTriangle(0, 0, 1,
							0.866025404, 0, -0.5,
							-0.866025404, 0, -0.5,
							VAL(CARPET_DEPTH));

  glTranslated(0, -0.05, 0);
	setDiffuseColor(COLOR_RED);
	drawPolygon(7, 2);
	glPopMatrix();


	glPushMatrix();
	// draw the sample model
	GLfloat maambient[]={0.79225f,0.79225f,0.79225f,1.0f};
	GLfloat madiffuse[]={0.50754f,0.50754f,0.50754f,1.0f};
	GLfloat maspecular[]={0.508273f,0.508273f,0.508273f,0.508273f};
	GLfloat maemi[4]={0.0f,0.0f,0.0f,1.0f};
  // what do these magic numbers mean?

	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,maambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,madiffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,maspecular);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,51.2f);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,maemi);

	glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));
	glScaled(VAL(XSCALE), VAL(YSCALE), VAL(ZSCALE));
		// Torus
    if (VAL(DETAILS) > 1) {
      glPushMatrix();
      glTranslated(.0f, 6, .0f);
      drawTorus(VAL(TORUS_R), VAL(TORUS_r));
      glPopMatrix();
    }

		//head
		glPushMatrix();
		glTranslated(0, VAL(LEG_LENGTH) + 0.05 + VAL(HEIGHT) + 0.05 + VAL(HEAD_SIZE), 0);
		glRotated(VAL(HEAD_ROTATE), 0.0, 1.0, 0.0);
		drawSphere(VAL(HEAD_SIZE));
			glPushMatrix();			
			glTranslated(0, VAL(HEAD_SIZE), 0);
				SpawnParticles( CameraMatrix );
			glPopMatrix();
		if (VAL(DETAILS) > 2) {
			drawRoundCylinder(VAL(HEAD_SIZE) * 1.1, 0.2, 0.2);
		glPushMatrix();
			glTranslated(-0.5, 0.5, 0);
			drawRoundCylinder(VAL(HEAD_SIZE) * 0.9, 0.2, 0.2);
		glPopMatrix();
		glPushMatrix();
			glTranslated(0.5, 0.5, 0);
			drawRoundCylinder(VAL(HEAD_SIZE) * 0.9, 0.2, 0.2);
		glPopMatrix();
		glPopMatrix();
		}

		//body
    // a.k.a. torso/trunk
		glPushMatrix();
		glTranslated(0, 0.05 + VAL(LEG_LENGTH), 0);
		glRotated(-90, 1.0, 0.0, 0.0);
		drawRoundCylinder(VAL(HEIGHT), 0.7, 0.6);
      glPushMatrix();
        glTranslated(-0.8, 0, VAL(HEIGHT) - 0.4);
        glRotated(90, 0, 1, 0);
        // the shoulder
        if (VAL(DETAILS) > 1) {
			drawRoundCylinder(1.6, 0.2, 0.2);
		}
      glPopMatrix();

      // the waist?
      if (VAL(DETAILS) > 2) {
        glPushMatrix();
          glTranslated(0, 0, 0.5);
          glRotated(90, 1, 0, 0);
          drawTorus(0.7, 0.08);
        glPopMatrix();
      }
    // the hip ? 
    glTranslated(0, 0, 0.3);
    if (VAL(DETAILS) > 1)
    drawTorus(0.45, 0.2);

		glPopMatrix();
		
		//right arm
		glPushMatrix();
      glTranslated(-0.7 - 0.20, VAL(LEG_LENGTH) + 0.05 + VAL(HEIGHT) * 0.9f, 0);
      glTranslated(0.15, 0, 0);
      glRotated(VAL(RIGHT_UPPER_ARM_ROTATE_X), 1.0, 0.0, 0.0);
      glRotated(VAL(RIGHT_UPPER_ARM_ROTATE_Y), 0.0, 1.0, 0.0);
      glTranslated(-0.15, 0, 0);
      drawRoundCylinder(VAL(UPPER_ARM_LENGTH), 0.22, 0.15);

        // lower arm
        glTranslated(0, 0, VAL(UPPER_ARM_LENGTH) - 0.1);
        glRotated(VAL(RIGHT_LOWER_ARM_ROTATE) - 180, 1, 0, 0);
        drawRoundCylinder(VAL(LOWER_ARM_LENGTH), 0.15, 0.20);

          // hand, basically clamp
          glPushMatrix();
            glTranslated(-0.03, -0.15, VAL(LOWER_ARM_LENGTH) - 0.1);
            glRotated(VAL(RIGHT_HAND_ANGLE), 0, 1, 0);
            drawBox(0.03, 0.25, 0.5);
          glPopMatrix();

          glPushMatrix();
            glTranslated(0.03, -0.15, VAL(LOWER_ARM_LENGTH) - 0.1);
            glRotated(-VAL(RIGHT_HAND_ANGLE), 0, 1, 0);
            drawBox(0.03, 0.25, 0.5);
          glPopMatrix();

		glPopMatrix();

		//left arm
		glPushMatrix();
		glTranslated(0.7 + 0.20, VAL(LEG_LENGTH) + 0.05 + VAL(HEIGHT) * 0.9f, 0);
    glTranslated(-0.15, 0, 0);
		glRotated(VAL(LEFT_UPPER_ARM_ROTATE_X), 1.0, 0.0, 0.0);
		glRotated(VAL(LEFT_UPPER_ARM_ROTATE_Y), 0.0, 1.0, 0.0);
    glTranslated(0.15, 0, 0);
		drawRoundCylinder(VAL(UPPER_ARM_LENGTH), 0.22, 0.15);

    glTranslated(0, 0, VAL(UPPER_ARM_LENGTH) - 0.1);
    glRotated(VAL(LEFT_LOWER_ARM_ROTATE) - 180, 1, 0, 0);
		drawRoundCylinder(VAL(LOWER_ARM_LENGTH), 0.15, 0.20);

          // hand, basically clamp
          glPushMatrix();
            glTranslated(-0.03, 0, VAL(LOWER_ARM_LENGTH) - 0.1);
            glRotated(VAL(LEFT_HAND_ANGLE), 0, 1, 0);
            drawCylinder(0.8, 0.15, 0.0001);
          glPopMatrix();

          glPushMatrix();
            glTranslated(0.03, 0, VAL(LOWER_ARM_LENGTH) - 0.1);
            glRotated(-VAL(LEFT_HAND_ANGLE), 0, 1, 0);
            drawCylinder(0.8, 0.15, 0.0001);
          glPopMatrix();

		glPopMatrix();

		//right leg
		glPushMatrix();
      glTranslated(-0.5, VAL(LEG_LENGTH), 0);
      glRotated(VAL(RIGHT_LEG_ROTATE_X), 1.0, 0.0, 0.0);
      glRotated(VAL(RIGHT_LEG_ROTATE_Y), 0.0, 1.0, 0.0);
      drawRoundCylinder(VAL(LEG_LENGTH) - 0.15, 0.3, 0.4);
      glTranslated(0, 0, VAL(LEG_LENGTH) * 0.85f);
      glRotated(90, 1, 0, 0);
      drawTorus(VAL(FEET_SIZE), VAL(FEET_SIZE)/4);

		glPopMatrix();

		//left leg
		glPushMatrix();
      glTranslated(0.5, VAL(LEG_LENGTH), 0);
      glRotated(VAL(LEFT_LEG_ROTATE_X), 1.0, 0.0, 0.0);
      glRotated(VAL(LEFT_LEG_ROTATE_Y), 0.0, 1.0, 0.0);
      drawRoundCylinder(VAL(LEG_LENGTH) - 0.15, 0.3, 0.4);
      glTranslated(0, 0, VAL(LEG_LENGTH) * 0.85f);
      glRotated(90, 1, 0, 0);
      drawTorus(VAL(FEET_SIZE), VAL(FEET_SIZE)/4);

		glPopMatrix();

	glPopMatrix();
	endDraw();
}

int main()
{
	// Initialize the controls
	// Constructor is 
	// ModelerControl(name, minimumvalue, maximumvalue, stepsize, defaultvalue)
    ModelerControl controls[NUMCONTROLS];
	// gosh, this should be the right way..
    controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
    controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
    controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);
    controls[XSCALE] = ModelerControl("X Scale", 0, 3, 0.1f, 1.0f);
    controls[YSCALE] = ModelerControl("Y Scale", 0, 3, 0.1f, 1.0f);
    controls[ZSCALE] = ModelerControl("Z Scale", 0, 3, 0.1f, 1.0f);
    controls[CARPET_SIZE] = ModelerControl("Carpet Size", 0, 10, 0.1f, 5.0f);
    controls[CARPET_DEPTH] = ModelerControl("Carpet Depth", 0, 10, 1, 4);
    controls[HEAD_SIZE] = ModelerControl("Head Size", 0, 2, 0.1f, 1);
    controls[HEAD_ROTATE] = ModelerControl("Head Rotate", -135, 135, 1, 0);
    controls[HEIGHT] = ModelerControl("Height", 1, 5, 0.1f, 2);

    controls[UPPER_ARM_LENGTH] = ModelerControl("Upper Arm Length", 1, 5, 0.1f, 0.8);
    controls[LEFT_UPPER_ARM_ROTATE_X] = ModelerControl("Left Upper Arm Rotate X", -90, 100, 1.0f, 80);
    controls[LEFT_UPPER_ARM_ROTATE_Y] = ModelerControl("Left Upper Arm Rotate Y", -30, 90, 1.0f, 0);
    controls[RIGHT_UPPER_ARM_ROTATE_X] = ModelerControl("Right Upper Arm Rotate X", -90, 100, 1.0f, -40);
    controls[RIGHT_UPPER_ARM_ROTATE_Y] = ModelerControl("Right Upper Arm Rotate Y", -90, 30, 1.0f, 0);

    controls[LOWER_ARM_LENGTH] = ModelerControl("Lower Arm Length", 1, 5, 0.1f, 0.8);
    controls[LEFT_LOWER_ARM_ROTATE] = ModelerControl("Left Lower Arm Rotate", 20, 180, 1.0f, 80);
    controls[RIGHT_LOWER_ARM_ROTATE] = ModelerControl("Right Lower Arm Rotate", 20, 180, 1.0f, 180);
    controls[RIGHT_HAND_ANGLE] = ModelerControl("Right Hand Angle", 0, 70, 1, 0);
    controls[LEFT_HAND_ANGLE] = ModelerControl("Left Hand Angle", 0, 70, 1, 0);

    controls[LEG_LENGTH] = ModelerControl("Leg Length", 1, 5, 0.1f, 2);
    controls[LEFT_LEG_ROTATE_X] = ModelerControl("Left Leg Rotate X", 0, 90, 1.0f, 80);
    controls[LEFT_LEG_ROTATE_Y] = ModelerControl("Left Leg Rotate Y", -80, 90, 1.0f, 0);
    controls[RIGHT_LEG_ROTATE_X] = ModelerControl("Right Leg Rotate X", 0, 90, 1.0f, 90);
    controls[RIGHT_LEG_ROTATE_Y] = ModelerControl("Right Leg Rotate Y", -90, 80, 1.0f, 0);

    //controls[TAIL_X] = ModelerControl("Tail Rotate X", -1, 1, 0.03f, 0.001);
    //controls[TAIL_Y] = ModelerControl("Tail Rotate Y", -1, 1, 0.03f, 1);
    //controls[TAIL_LENGTH] = ModelerControl("Tail Length", 0.5, 5, 0.1f, 2);
    //
    controls[FEET_SIZE] = ModelerControl("Feet Size", 0.1, 1, 0.02f, 0.3);
    controls[TORUS_R] = ModelerControl("Torus R", 0, 10, 0.1f, 0.6);
    controls[TORUS_r] = ModelerControl("Torus r", 0, 10, 0.1f, 0.15);

    controls[DETAILS] = ModelerControl("Details", 1, 3, 1, 3);

	ParticleSystem *ps = new ParticleSystem();	
	ModelerApplication::Instance()->SetParticleSystem(ps);
	
    ModelerApplication::Instance()->Init(&createSampleModel, controls, NUMCONTROLS);
    return ModelerApplication::Instance()->Run();
}



Mat4f getModelViewMatrix()
{
	/**************************
	**
	**	GET THE OPENGL MODELVIEW MATRIX
	**
	**	Since OpenGL stores it's matricies in
	**	column major order and our library
	**	use row major order, we will need to
	**	transpose what OpenGL gives us before returning.
	**
	**	Hint:  Use look up glGetFloatv or glGetDoublev
	**	for how to get these values from OpenGL.
	**
	*******************************/

        GLfloat m[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, m);
        Mat4f matMV(m[0], m[1], m[2], m[3],
            m[4], m[5], m[6], m[7],
            m[8], m[9], m[10], m[11],
            m[12], m[13], m[14], m[15] );

        return matMV.transpose(); // convert to row major
}


void SpawnParticles( Mat4f CameraTransforms )
{	
	Mat4f WorldMatrix = CameraTransforms.inverse() * getModelViewMatrix();
	Vec4f WorldPoint = WorldMatrix * Vec4f(0, 0, 0, 1);
	ModelerApplication::Instance()->GetParticleSystem()->addParticle(WorldPoint);
}