/*
 *  Mouse Configurations
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef linux
#include <GL/glut.h>
#endif

#include <Declaration.h>
#include <Grapha.h>

bool MFLAG;
extern Grapha Gra;

/* mouse click */
void MouseClick(int button, int state, int x, int y) {
  switch (button) {
    case GLUT_LEFT_BUTTON:
      if (state == GLUT_DOWN) {
        Gra.NewLine();
        Gra.SetCoordinate(x, y);
      }
      if (state == GLUT_UP) {
        MFLAG = false;
        Gra.SetDrawMode();
        if (Gra.PixelEraserFlag()) Gra.PixelEraser();
      }
      break;
    default:
      break;
  }
}

/* mouse motion */
void MouseMotion(int x, int y) {
  if (MFLAG) Gra.SetCoordinate(x, y);
  MFLAG = true;
}

void MouseDrawMode(void) {
  glutPassiveMotionFunc(NULL);
  glutMotionFunc(MouseMotion);
}
