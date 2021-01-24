#ifndef MyGLUT_h_
#define MyGLUT_h_

/***************************/

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef linux
#include <GL/glut.h>
#endif

#define PI M_PI
#define EPS 2.220e-16

// Function to create uniform random numbers from 0 to 1
inline double randf(void) { return (double)rand() / RAND_MAX; }

/* Drawing String Function (string,position x,position y) */
inline void glDrawString(const char *str, double x0, double y0) {
  int size = strlen(str);
  glRasterPos2d(x0, y0);
  for (int i = 0; i < size; ++i) {
    char ic = str[i];
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ic);
  }
}

/* Drawing String Function (string,position x,position y) */
inline void glDrawString2(const char *str, double x0, double y0) {
  int size = strlen(str);
  glRasterPos2d(x0, y0);
  for (int i = 0; i < size; ++i) {
    char ic = str[i];
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ic);
  }
}

typedef struct color {
  double R, G, B;
} color;

/* glColor3d */
inline void glColorStruct(color c) { glColor3d(c.R, c.G, c.B); }

#endif  //"MyGLUT_h_"
