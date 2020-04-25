
// 3dglasses.cpp
//
// Credit should deservedly go to: 
// http://astronomy.swin.edu.au/pbourke/

#include "game.h"

XYZ origin = {0.0,0.0,0.0};
CAMERA camera;

/*
   Normalise a vector
*/
void Normalise(XYZ *p)
{
   double length;

   length = sqrt(p->x * p->x + p->y * p->y + p->z * p->z);
   if (length != 0) {
      p->x /= length;
      p->y /= length;
      p->z /= length;
   } else {
      p->x = 0;
      p->y = 0;
      p->z = 0;
   }
}

/*
   Calculate the unit normal at p given two other points
   p1,p2 on the surface. The normal points in the direction
   of p1 crossproduct p2
*/
XYZ CalcNormal(XYZ p,XYZ p1,XYZ p2)
{
   XYZ n,pa,pb;

   pa.x = p1.x - p.x;
   pa.y = p1.y - p.y;
   pa.z = p1.z - p.z;
   pb.x = p2.x - p.x;
   pb.y = p2.y - p.y;
   pb.z = p2.z - p.z;
   Normalise(&pa);
   Normalise(&pb);

   n.x = pa.y * pb.z - pa.z * pb.y;
   n.y = pa.z * pb.x - pa.x * pb.z;
   n.z = pa.x * pb.y - pa.y * pb.x;
   Normalise(&n);

   return(n);
}


/*
   Move the camera to the home position 
   Or to a predefined stereo configuration
   The model is assumed to be in a 10x10x10 cube
   Centered at the origin
*/
void CameraHome(int mode)
{
   camera.aperture = 60;
   camera.pr = origin;

   camera.vd.x = 1;
   camera.vd.y = 0;
   camera.vd.z = 0;

   camera.vu.x = 0;
   camera.vu.y = 1;
   camera.vu.z = 0;

   camera.vp.x = -6.2;
   camera.vp.y = 0;
   camera.vp.z = 0;

	camera.screenwidth = GetScreenWidth();
	camera.screenheight = GetScreenHeight();

   switch (mode) {
   case 0:
   case 2:
   case 4:
      camera.focallength = 10;
      break;
   case 1:
      camera.focallength = 5;
      break;
   case 3:
      camera.focallength = 15;
      break;
   }
   
   /* Non stressful stereo setting */
   camera.eyesep = camera.focallength / 30.0;

   if (mode == 4)
      camera.eyesep = 0;
}


XYZ r, focus;
double radians,wd2,ndfl;
double ratio, left,right,top,bottom;
double dblNear=0.1f;
double dblFar=10000;

void SetupLens(const int &lens)
{
	static BOOL bOneShot = FALSE;

	if (!bOneShot)
	{

		// Setup camera
		CameraHome(3);

		/* Clip to avoid extreme stereo */
		dblNear = camera.focallength * 0.2f;

		/* Derive the the "right" vector */
		CROSSPROD(camera.vd,camera.vu,r);
		Normalise(&r);
		r.x *= camera.eyesep * 0.5f;
		r.y *= camera.eyesep * 0.5f;
		r.z *= camera.eyesep * 0.5f;

		focus.x = camera.vp.x + camera.focallength * camera.vd.x;
		focus.y = camera.vp.y + camera.focallength * camera.vd.y;
		focus.z = camera.vp.z + camera.focallength * camera.vd.z;

		/* Misc stuff */
		ratio  = camera.screenwidth / camera.screenheight;
		radians = DTOR * camera.aperture * 0.5f;
		wd2     = dblNear * tan(radians);
		ndfl    = dblNear / camera.focallength;

		// Clear one shot flag
		bOneShot = TRUE;
	}

	switch (lens)
	{
		case 0:

			/* Left eye filter */
			glColorMask(GL_TRUE,GL_FALSE,GL_FALSE,GL_TRUE);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
			/* Create the projection */
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			left  = - ratio * wd2 + 0.5 * camera.eyesep * ndfl;
			right =   ratio * wd2 + 0.5 * camera.eyesep * ndfl;
			top    =   wd2;
			bottom = - wd2;
			glFrustum(left, right, bottom, top, dblNear, dblFar);

			/* Create the model */
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			// Reset The Current Viewport
			glViewport(0,0,camera.screenwidth, camera.screenheight);

			break;


		case 1:

			/* The projection */
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			left  = - ratio * wd2 - 0.5 * camera.eyesep * ndfl;
			right =   ratio * wd2 - 0.5 * camera.eyesep * ndfl;
			top    =   wd2;
			bottom = - wd2;
			glFrustum(left,right,bottom,top,dblNear, dblFar);
					
			/* Right eye filter */
			/*
			glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
			switch (glassestype) {
			case REDBLUE:
				glColorMask(GL_FALSE,GL_FALSE,GL_TRUE,GL_TRUE);
				break;
			case REDGREEN:
				glColorMask(GL_FALSE,GL_TRUE,GL_FALSE,GL_TRUE);
				break;
			case REDCYAN:
				glColorMask(GL_FALSE,GL_TRUE,GL_TRUE,GL_TRUE);
				break;
			case BLUERED:
			case GREENRED:
			case CYANRED:
				glColorMask(GL_TRUE,GL_FALSE,GL_FALSE,GL_TRUE);
				break;
			} 
			*/
			glColorMask(GL_FALSE, GL_FALSE, GL_TRUE, GL_TRUE);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			break;

		default:
			break;

	}

}

