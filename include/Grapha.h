#ifndef Grapha_h_
#define Grapha_h_

/*****************************/

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef linux
#include <GL/glut.h>
#endif

#include <Colors.h>
#include <Utility.h>

#include <vector>

/* Pixel Cells */
#define Xpixel 100
#define Ypixel 80

/* Mode */
#define MAXMODENUMBER 100
enum {
  MFREEHAND = 0,
  MCIRCLE,
  MSQUARE,
  MPOLYGON,
  MSTRAIGHTLINE,
  MRANDOM,
  MKALEIDO,
  MCOLORBARLINE = MAXMODENUMBER + 1,
  MCOLORBARCANVAS,
  MLINEMOVE,
  MLINECOPY,
  MLINEROTATE,
};

/* Number of Circle Sampling */
#define CircleSample 2e2

/* Line Data Structure */
typedef struct postion {
  int x, y;
} position;
typedef struct line {
  color Color;              // line color
  double Width;             // line width
  std::vector<position> P;  // positions of point elements
} line;

class Grapha {
 public:
  //-- Menber Variables --//
  double Cmargin;                      // Margin of Canvas (%)
  int WX, WY;                          // Window Size
  std::vector<line> Line;              // Line Data
  color CanvasColor;                   // Canvas Color R,G,B
  line DefaultLine;                    // Default Line
  int CurrentLineID;                   // for Undo/Redo Management
  int DrawMode;                        // Draw Mode
  int TmpDrawMode;                     // Temporary Draw Mode
  int Nside;                           // for draw N-side Polygon
  bool TmpFlag;                        // Temporary Flag for FillTmpPixel
  bool PXFLAG;                         // Pixel Mode Flag
  bool ERFLAG;                         // Eraser Flag
  bool STFLAG;                         // Status Flag
  const int ColorBarArray;             // Number of Color Bar Array
  double *ColorBarWidth;               // Width of Color Bar
  double Pixel[3][Xpixel][Ypixel];     // [RGB][X][Y]
  double TmpPixel[3][Xpixel][Ypixel];  // [RGB][X][Y]
  int PixelSize;                       // Size of Pixel
  int PixelEraserCounter;              // Counter of Pixel Eraser

  //-- Menber Functions --//
  Grapha();
  ~Grapha();
  void Init();
  void Resize(int WX, int XY);
  void Reset();
  void NewLine();
  void Undo();
  void Redo();
  void EraseLine();
  void DrawColorBar();
  void SetCanvasColor(double R, double G, double B);
  void SetLineColor(double R, double G, double B);
  void SetDefaultLineColor(double R, double G, double B);
  void SetLineWidth(double w);
  void SetDefaultLineWidth(double w);
  void SetPixelSize(int size);
  void GetDrawModeString(char *name);
  void SetDrawMode();
  void SetDrawMode(int mode);
  void SetDrawMode(int mode, int N_side);
  void SetCoordinate(int x, int y);
  void SetLineCircle(int x, int y);
  void SetLineSquare(int x, int y);
  void SetLinePolygon(int x, int y);
  void SetStraightLine(int x, int y);
  void SetRandom(int x, int y);
  void SetKaleido(int x, int y);
  void LineMove(int x, int y);
  void LineCopy(int x, int y);
  void LineRotate(int x, int y);
  void PixelMode();
  void FillPixel();
  void FillTmpPixel();
  void UndoPixel();
  void PixelEraser();
  bool PixelEraserFlag();
  void DrawCanvas();
  void DrawLine();
  void DrawPixel();
  void DrawDisplay();
  void ShowStatus();

  line GetLine();
  position GetPosition();
  void ClearCurrentLine();
  void PushCurrentLine(position p);
  bool LineExist();
};

/*****************************/

#endif  // Grapha_h_
