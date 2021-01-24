/*
 *  Keyboard Shortcuts Configurations
 */

#include <Grapha.h>

#include <iomanip>
#include <iostream>

Grapha::Grapha() : ColorBarArray(1e3) {
  //-- setting parameters --//
  Line.reserve(1e3);
  CanvasColor = {1., 1., 1.};
  DefaultLine = {
      {0., 0., 0.},
      2.,
  };
  CurrentLineID = -1;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < Xpixel; ++j) {
      for (int k = 0; k < Ypixel; ++k) {
        Pixel[i][j][k] = 1.0;
        TmpPixel[i][j][k] = 1.0;
      }
    }
  }
  ColorBarWidth = new double[ColorBarArray];
  for (int i = 0; i < ColorBarArray; ++i) ColorBarWidth[i] = 3.0;
  DrawMode = MFREEHAND;
  TmpDrawMode = MFREEHAND;
  TmpFlag = false;
  PXFLAG = false;
  ERFLAG = false;
  STFLAG = true;
  PixelSize = 1;
  PixelEraserCounter = 0;
}

Grapha::~Grapha() { delete[] ColorBarWidth; }

void Grapha::Init() {
  //-- configuration of viewing (anti-aliasing) --//
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Grapha::Resize(int wx, int wy) {
  //-- setting of canvas --//
  Cmargin = 0.0;
  WX = wx;
  WY = wy;
}

void Grapha::Reset() {
  // DrawMode = MFREEHAND;
  CanvasColor = {1., 1., 1.};
  DefaultLine = {
      {0., 0., 0.},
      2.0,
  };
  Line.clear();
  CurrentLineID = -1;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < Xpixel; ++j) {
      for (int k = 0; k < Ypixel; ++k) {
        Pixel[i][j][k] = 1.0;
        TmpPixel[i][j][k] = 1.0;
      }
    }
  }
}

void Grapha::NewLine() {
  if (DrawMode < MAXMODENUMBER) {
    while (CurrentLineID < (int)Line.size() - 1) Line.pop_back();
    CurrentLineID++;
    while ((int)Line.size() - 1 < CurrentLineID) Line.push_back(DefaultLine);
    if (TmpFlag) TmpFlag = false;
    // if(ERFLAG) ++PixelEraserCounter;
  }
}

void Grapha::Undo() {
  if (0 <= CurrentLineID) CurrentLineID--;
}

void Grapha::Redo() {
  if (CurrentLineID < (int)Line.size() - 1) CurrentLineID++;
}

void Grapha::EraseLine() {
  if (PXFLAG) {
    PixelEraser();
  }
}

void Grapha::DrawColorBar() {
  if (DrawMode == MCOLORBARLINE || DrawMode == MCOLORBARCANVAS) {
    double hmax = 1.0;
    double hbin = hmax / ColorBarArray;
    int i = 0;
    glBegin(GL_QUADS);
    for (double h = 0.0; h < hmax; h += hbin) {
      hue2rgb hue(h, hmax);
      glColor3d(hue.R, hue.G, hue.B);
      double yoff = Cmargin + 5;
      glVertex2d(h, (yoff) / 100);
      glVertex2d(h + hbin, (yoff) / 100);
      glVertex2d(h + hbin, (yoff + ColorBarWidth[i]) / 100);
      glVertex2d(h, (yoff + ColorBarWidth[i]) / 100);
      ++i;
    }
    glEnd();
  }
}

void Grapha::SetCanvasColor(double R, double G, double B) {
  for (int j = 0; j < Ypixel; ++j) {
    for (int i = 0; i < Xpixel; ++i) {
      if (Pixel[0][i][j] == CanvasColor.R) {
        if (Pixel[1][i][j] == CanvasColor.G) {
          if (Pixel[2][i][j] == CanvasColor.B) {
            Pixel[0][i][j] = R;
            Pixel[1][i][j] = G;
            Pixel[2][i][j] = B;
          }
        }
      }
    }
  }
  CanvasColor = {R, G, B};
  if (R == G && G == B)
    SetDefaultLineColor(0.5 - R, 0.5 - G, 1.0 - B);
  else
    SetDefaultLineColor(1.0 - R, 1.0 - G, 1.0 - B);
  FillTmpPixel();
}

void Grapha::SetLineColor(double R, double G, double B) {
  if (!Line.empty()) Line[Line.size() - 1].Color = {R, G, B};
}

void Grapha::SetDefaultLineColor(double R, double G, double B) {
  DefaultLine.Color = {R, G, B};
}

void Grapha::SetLineWidth(double w) {
  if (5.0 < w) w = 5.0;
  if (!Line.empty()) Line[Line.size() - 1].Width = w;
}

void Grapha::SetDefaultLineWidth(double w) {
  if (5.0 < w) w = 5.0;
  DefaultLine.Width = w;
}

void Grapha::SetPixelSize(int size) {
  static int max = 10;
  if (1 < size && size < max)
    PixelSize = size;
  else if (max <= size)
    PixelSize = max;
  else
    PixelSize = 1;
}

void Grapha::GetDrawModeString(char* _name) {
  switch (DrawMode) {
    case MFREEHAND:
      sprintf(_name, "free hand");
      break;
    case MCOLORBARLINE:
      sprintf(_name, "color bar");
      break;
    case MCOLORBARCANVAS:
      sprintf(_name, "color bar");
      break;
    case MCIRCLE:
      sprintf(_name, "circle");
      break;
    case MSQUARE:
      sprintf(_name, "square");
      break;
    case MPOLYGON:
      sprintf(_name, "polygon");
      break;
    case MSTRAIGHTLINE:
      sprintf(_name, "lines");
      break;
    case MRANDOM:
      sprintf(_name, "random walk");
      break;
    case MKALEIDO:
      sprintf(_name, "kaleido");
      break;
    case MLINEMOVE:
      sprintf(_name, "line move");
      break;
    case MLINECOPY:
      sprintf(_name, "line copy");
      break;
    default:
      break;
  }
  if (ERFLAG) sprintf(_name, "eraser");
}

void Grapha::SetDrawMode() {
  if (DrawMode == MCOLORBARLINE || DrawMode == MCOLORBARCANVAS) {
    for (int i = 0; i < ColorBarArray; ++i) ColorBarWidth[i] = 3.0;
    std::cout << std::endl;
    DrawMode = TmpDrawMode;
  }
  if (TmpFlag) TmpFlag = false;
}

void Grapha::SetDrawMode(int mode) {
  if (TmpFlag) TmpFlag = false;
  if (mode != DrawMode) {
    TmpDrawMode = DrawMode;
    DrawMode = mode;
  }
}

void Grapha::SetDrawMode(int mode, int N_side) {
  if (mode == MPOLYGON) {
    Nside = N_side;
    if (N_side < 3) Nside = 3;
    if (CircleSample < N_side) Nside = CircleSample;
    TmpDrawMode = DrawMode;
    DrawMode = MPOLYGON;
  }
}

void Grapha::ClearCurrentLine() {
  if (0 <= CurrentLineID && CurrentLineID < (int)Line.size())
    Line[CurrentLineID].P.clear();
}

void Grapha::PushCurrentLine(position p) {
  if (0 <= CurrentLineID && CurrentLineID < (int)Line.size())
    Line[CurrentLineID].P.push_back(p);
}

void Grapha::SetCoordinate(int x, int _y) {
  int y = WY - _y;
  switch (DrawMode) {
    case MFREEHAND: /* Store Coordinate */
    {
      if (!TmpFlag) {
        FillTmpPixel();
        TmpFlag = true;
      }
      PushCurrentLine({x, y});
      FillPixel();
    } break;
    case MCOLORBARLINE: /* Set Line Color by ColorBar */
    {
      int ch = (int)((double)x / WX * ColorBarArray);
      static double s = 15.0 / WX * ColorBarArray;
      while (ch < 0) ch += ColorBarArray;
      while (ColorBarArray <= ch) ch -= ColorBarArray;
      for (int i = 0; i < ColorBarArray; ++i)
        ColorBarWidth[i] = 3.0 + exp(-(i - ch) * (i - ch) / (2 * s * s));
      hue2rgb C(x, WX);
      SetDefaultLineColor(C.R, C.G, C.B);
      std::cout << std::fixed << std::setprecision(4) << "\r\tR:" << C.R
                << "\tG:" << C.G << "\tB:" << C.B;
      fflush(stdout);
    } break;
    case MCOLORBARCANVAS: /* Set Canvas Color by ColorBar */
    {
      int ch = (int)((double)x / WX * ColorBarArray);
      static double s = 15.0 / WX * ColorBarArray;
      while (ch < 0) ch += ColorBarArray;
      while (ColorBarArray <= ch) ch -= ColorBarArray;
      for (int i = 0; i < ColorBarArray; ++i)
        ColorBarWidth[i] = 3.0 + exp(-(i - ch) * (i - ch) / (2 * s * s));
      hue2rgb C(x, WX);
      SetCanvasColor(C.R, C.G, C.B);
      std::cout << std::fixed << std::setprecision(4) << "\r\tR:" << C.R
                << "\tG:" << C.G << "\tB:" << C.B;
      fflush(stdout);
    } break;
    case MCIRCLE: /* Draw Circle */
      SetLineCircle(x, y);
      break;
    case MSQUARE: /* Draw Square */
      SetLineSquare(x, y);
      break;
    case MPOLYGON: /* Draw N-side Polygon */
      SetLinePolygon(x, y);
      break;
    case MSTRAIGHTLINE: /* Draw Straight Line */
      SetStraightLine(x, y);
      break;
    case MRANDOM: /* Draw Random */
      SetRandom(x, y);
      break;
    case MKALEIDO: /* Draw Kaleido */
      SetKaleido(x, y);
      break;
    case MLINEMOVE: /* Line Move */
      LineMove(x, y);
      break;
    case MLINECOPY: /* Line Copy */
      LineCopy(x, y);
      break;
    case MLINEROTATE: /* Line Rotate */
      LineRotate(x, y);
      break;
    default:
      break;
  }
}

void Grapha::SetLineCircle(int x, int y) {
  static int x0, y0;
  static position pc;
  static double r, theta;
  if (!TmpFlag) {
    FillTmpPixel();
    x0 = x;
    y0 = y;
    TmpFlag = true;
  }
  UndoPixel();
  ClearCurrentLine();
  pc.x = (x + x0) / 2;
  pc.y = (y + y0) / 2;
  r = sqrt(pow(x - pc.x, 2) + pow(y - pc.y, 2));
  for (int i = 0; i <= CircleSample; ++i) {
    theta = (double)i / CircleSample * 2 * PI;
    PushCurrentLine(
        {(int)(pc.x + r * sin(theta)), (int)(pc.y - r * cos(theta))});
    FillPixel();
  }
}

void Grapha::SetLineSquare(int x, int y) {
  static int x0, y0;
  if (!TmpFlag) {
    FillTmpPixel();
    x0 = x;
    y0 = y;
    TmpFlag = true;
  }
  UndoPixel();
  ClearCurrentLine();
  for (int i = 0; i <= 4; ++i) {
    PushCurrentLine({(((i + 2) % 4) / 2) * x0 + ((i % 4) / 2) * x,
                     (((i + 3) % 4) / 2) * y0 + (((i + 1) % 4) / 2) * y});
    FillPixel();
  }
}

void Grapha::SetLinePolygon(int x, int y) {
  static int x0, y0;
  static position pc;
  static double r, theta;
  if (!TmpFlag) {
    FillTmpPixel();
    x0 = x;
    y0 = y;
    TmpFlag = true;
  }
  UndoPixel();
  ClearCurrentLine();
  pc.x = (x + x0) / 2;
  pc.y = (y + y0) / 2;
  r = sqrt(pow(x - pc.x, 2) + pow(y - pc.y, 2));
  for (int i = 0; i <= Nside; ++i) {
    theta = (double)i / Nside * 2 * PI;
    PushCurrentLine(
        {(int)(pc.x + r * sin(theta)), (int)(pc.y - r * cos(theta))});
    FillPixel();
  }
}

void Grapha::SetStraightLine(int x, int y) {
  static int x0, y0;
  if (!TmpFlag) {
    FillTmpPixel();
    x0 = x;
    y0 = y;
    TmpFlag = true;
  }
  UndoPixel();
  ClearCurrentLine();
  PushCurrentLine({x0, y0});
  PushCurrentLine({x, y});
  FillPixel();
}

void Grapha::SetRandom(int x, int y) {
  static int x0, y0;
  if (!TmpFlag) {
    TmpFlag = true;
    Line[CurrentLineID].Color = (color){randf(), randf(), randf()};
    x0 = x;
    y0 = y;
    PushCurrentLine({x0, y0});
  }
  double _width = Line[CurrentLineID].Width * 5.;
  PushCurrentLine({GetPosition().x + (int)((2 * randf() - 1) * _width),
                   GetPosition().y + (int)((2 * randf() - 1) * _width)});
  FillPixel();
}

void Grapha::SetKaleido(int x, int y) {
  //     static int x0;
  //     static int y0;
  //     if(!TmpFlag){
  //         hue2rgb C(LineID,10);
  //         SetDefaultLineColor(C.R,C.G,C.B);
  //         ++LineID;
  //         FillTmpPixel();
  //         x0 = x;
  //         y0 = y;
  //         TmpFlag = true;
  //     }
  //     double xc = (x+x0)/2;
  //     double yc = (y+y0)/2;
  //     double r  = sqrt(pow(x-xc,2)+pow(y-yc,2));
  //     double theta,a;
  //     if(r==0) theta = 0.0;
  //     else{
  //         a = (x0-xc)/r;
  //         if(a<-1) a =-1.0;
  //         if(1<a)  a = 1.0;
  //         theta = acos(a);
  //     }
  //     for(int i=0;i<=CircleSample;++i){
  //         Px[Counter[LineID]][LineID] =
  //         xc+r*cos((double)i/CircleSample*2*PI+theta);
  //         Py[Counter[LineID]][LineID] =
  //         yc+r*sin((double)i/CircleSample*2*PI+theta);
  //         ++Counter[LineID];
  //         if(FreeMAX-1<Counter[LineID]){
  //             hue2rgb C(LineID,10);
  //             SetDefaultLineColor(C.R,C.G,C.B);
  //             ++LineID;
  //         }
  //         FillPixel();
  //     }
}

void Grapha::LineMove(int x, int y) {
  static int x0, y0;
  if (LineExist()) {
    if (!TmpFlag) {
      FillTmpPixel();
      x0 = x;
      y0 = y;
      TmpFlag = true;
    }
    UndoPixel();
    for (auto&& p : Line[CurrentLineID].P) {
      p.x += x - x0;
      p.y += y - y0;
      FillPixel();
    }
    x0 = x;
    y0 = y;
  }
}

void Grapha::LineCopy(int x, int y) {
  static int x0, y0;
  if (LineExist()) {
    if (!TmpFlag) {
      FillTmpPixel();
      x0 = x;
      y0 = y;
      Line.push_back(GetLine());
      ++CurrentLineID;
      TmpFlag = true;
    }
    UndoPixel();
    for (auto&& p : Line[CurrentLineID].P) {
      p.x += x - x0;
      p.y += y - y0;
      FillPixel();
    }
    x0 = x;
    y0 = y;
  }
}

void Grapha::LineRotate(int x, int y) {
  //     if(0<LineID){
  //         static int x0 = x;
  //         static int y0 = y;
  //         static double xc,yc;
  //         static double dot,vec0,vec1,theta;
  //         if(!TmpFlag){
  //             xc = (Px[0][LineID]+Px[Counter[LineID]-1][LineID])/2.0;
  //             yc = (Py[0][LineID]+Py[Counter[LineID]-1][LineID])/2.0;
  //             TmpFlag = true;
  //         }
  //         UndoPixel();
  //         dot   = (x0-xc)*(x-xc)+(y0-yc)*(y-yc);
  //         vec0  = sqrt(pow(x0-xc,2)+pow(y0-yc,2));
  //         vec1  = sqrt(pow(x-xc,2)+pow(y-yc,2));
  //         double a = dot/(vec0*vec1);
  //         if(a<-1) a = -1.0;
  //         if(1<a)  a =  1.0;
  //         theta = acos(a);
  //         if(vec0==0 || vec1==0) theta = 0.0;
  //         int max = Counter[LineID];
  //         Counter[LineID] = 0;
  //         for(int i=0;i<max;++i){
  //             double X = Px[i][LineID];
  //             double Y = Py[i][LineID];
  //             Px[i][LineID] = xc+(X-xc)*cos(theta)-(Y-yc)*sin(theta);
  //             Py[i][LineID] = yc+(X-xc)*sin(theta)+(Y-yc)*cos(theta);
  //             ++Counter[LineID];
  //             FillPixel();
  //         }
  //         x0 = x;
  //         y0 = y;
  //     }
}

void Grapha::PixelMode() {
  if (PXFLAG)
    PXFLAG = false;
  else
    PXFLAG = true;
}

#define FillEachPixel(x, y)                                       \
  {                                                               \
    for (int k = 1; k <= PixelSize; ++k) {                        \
      int jmax = k * k;                                           \
      for (int j = 0; j < jmax; ++j) {                            \
        double xi = x + k * cos(2 * PI * j / jmax);               \
        double yi = y + k * sin(2 * PI * j / jmax);               \
        int chx = (int)((double)Xpixel * xi / WX);                \
        int chy = (int)((double)Ypixel * yi / WY);                \
        if (0 < chx && chx < Xpixel && 0 < chy && chy < Ypixel) { \
          Pixel[0][chx][chy] = GetLine().Color.R;                 \
          Pixel[1][chx][chy] = GetLine().Color.G;                 \
          Pixel[2][chx][chy] = GetLine().Color.B;                 \
        }                                                         \
      }                                                           \
    }                                                             \
  }
void Grapha::FillPixel() {
  static double vx, vy, v_length, x0, y0, x, y, distance;
  static unsigned int whilecount, _size;
  _size = GetLine().P.size();
  if (1 < _size) {
    x0 = GetLine().P[_size - 2].x;
    y0 = GetLine().P[_size - 2].y;
    vx = GetPosition().x - x0;
    vy = GetPosition().y - y0;
    v_length = sqrt(vx * vx + vy * vy);
    if (v_length > EPS) {
      vx /= v_length;
      vy /= v_length;
      x = x0;
      y = y0;
      distance = 0.0;
      whilecount = 0;
      while (distance < v_length) {
        x += vx;
        y += vy;
        distance = sqrt(pow(x0 - x, 2) + pow(y0 - y, 2));
        FillEachPixel(x, WY - y);
        ++whilecount;
        if (1e6 < whilecount) break;
      }
    } else  // v==0
      FillEachPixel(GetPosition().x, WY - GetPosition().y);
  } else {  //_size<=1
    if (LineExist()) FillEachPixel(GetLine().P[0].x, WY - GetLine().P[0].y);
  }
}
#undef FillEachPixel

void Grapha::FillTmpPixel() {
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < Xpixel; ++j)
      for (int k = 0; k < Ypixel; ++k) TmpPixel[i][j][k] = Pixel[i][j][k];
}

void Grapha::UndoPixel() {
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < Xpixel; ++j)
      for (int k = 0; k < Ypixel; ++k)
        if (Pixel[i][j][k] != TmpPixel[i][j][k])
          Pixel[i][j][k] = TmpPixel[i][j][k];
}

void Grapha::PixelEraser() {
  //     if(PXFLAG){
  //         static int TmpPixelSize = 2;
  //         static double TmpColor[3];
  //         if(!ERFLAG){ // Eraser ON
  //             TmpDrawMode = DrawMode;
  //             DrawMode = MFREEHAND;
  //             ERFLAG = true;
  //             TmpPixelSize = PixelSize;
  //             PixelSize = 20;
  //             for(int i=0;i<3;++i)
  //                 TmpColor[i] = LineColor[i][LineID];
  //             SetDefaultLineColor(CanvasColor.R,CanvasColor.G,CanvasColor.B);
  //         }
  //         else{ // Eraser OFF
  //             DrawMode = TmpDrawMode;
  //             ERFLAG = false;
  //             if(PixelEraserCounter){
  //                 for(int i=0;i<PixelEraserCounter;++i){
  //                     TmpCount[LineID] = Counter[LineID];
  //                     Counter[LineID] = 0;
  //                     if(0<LineID) --LineID;
  //                 }
  //                 PixelEraserCounter = 0;
  //             }
  //             PixelSize = TmpPixelSize;
  //             SetDefaultLineColor(TmpColor[0],TmpColor[1],TmpColor[2]);
  //         }
  //     }
}

bool Grapha::PixelEraserFlag() { return ERFLAG; }

void Grapha::DrawCanvas() {
  if (!PXFLAG) {
    glColorStruct(CanvasColor);
    glBegin(GL_QUADS);
    glVertex2d(Cmargin / 100, Cmargin / 100);
    glVertex2d((100 - Cmargin) / 100, Cmargin / 100);
    glVertex2d((100 - Cmargin) / 100, (100 - Cmargin) / 100);
    glVertex2d(Cmargin / 100, (100 - Cmargin) / 100);
    glEnd();
  }
}

void Grapha::DrawLine() {
  if (!PXFLAG) {
    for (int id = 0; id <= CurrentLineID; ++id) {
      glColorStruct(Line[id].Color);
      if (1 < Line[id].P.size()) {
        glLineWidth(Line[id].Width);
        glBegin(GL_LINE_STRIP);
        for (auto&& p : Line[id].P)
          glVertex2d((double)p.x / WX, (double)p.y / WY);
        glEnd();
      } else if (1 == Line[id].P.size()) {
        glPointSize(Line[id].Width);
        glBegin(GL_POINTS);
        glVertex2d((double)Line[id].P[0].x / WX, (double)Line[id].P[0].y / WY);
        glEnd();
      }
    }
  }
}

void Grapha::DrawPixel() {
  if (PXFLAG) {
    glBegin(GL_QUADS);
    for (int j = 0; j < Ypixel; ++j) {
      for (int i = 0; i < Xpixel; ++i) {
        glColor3d(Pixel[0][i][j], Pixel[1][i][j], Pixel[2][i][j]);
        glVertex2d((i) / ((double)Xpixel), (Ypixel - j - 1) / ((double)Ypixel));
        glVertex2d((i + 1) / ((double)Xpixel),
                   (Ypixel - j - 1) / ((double)Ypixel));
        glVertex2d((i + 1) / ((double)Xpixel), (Ypixel - j) / ((double)Ypixel));
        glVertex2d((i) / ((double)Xpixel), (Ypixel - j) / ((double)Ypixel));
      }
    }
    glEnd();
  }
}

void Grapha::DrawDisplay() {
  if (STFLAG) {
    //-- Show Status Strings --//
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, WX, WY, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    if (CanvasColor.R == CanvasColor.G && CanvasColor.G == CanvasColor.B)
      glColor3d(0.5 - CanvasColor.R, 0.5 - CanvasColor.G, 1.0 - CanvasColor.B);
    else
      glColor3d(1.0 - CanvasColor.R, 1.0 - CanvasColor.G, 1.0 - CanvasColor.B);
    char s1[100];
    char s2[50];
    sprintf(s1, "[%d,%d]", GetPosition().x, GetPosition().y);
    glDrawString(s1, 10, 15);
    if (0 <= CurrentLineID) {
      if (LineExist())
        sprintf(s1, "Line No. %i Length %i", CurrentLineID,
                (int)GetLine().P.size() - 1);
      else
        sprintf(s1, "Line No. %i Length -", CurrentLineID);
    } else
      sprintf(s1, "Line No. - Length -");
    glDrawString(s1, 10, 30);

    if (PXFLAG) {
      GetDrawModeString(s2);
      sprintf(s1, "Mode : Pixel(%s)", s2);
      glDrawString(s1, 10, 45);
    } else {
      GetDrawModeString(s2);
      sprintf(s1, "Mode : GLUT Line(%s)", s2);
      glDrawString(s1, 10, 45);
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    //-- Show Current Line Style --//
    glColorStruct(DefaultLine.Color);
    glLineWidth(DefaultLine.Width);
    glBegin(GL_LINE_STRIP);
    glVertex2d(100. / WX, 1. - 10. / WY);
    glVertex2d(130. / WX, 1. - 10. / WY);
    glEnd();
  }
}

void Grapha::ShowStatus() {
  if (STFLAG)
    STFLAG = false;
  else
    STFLAG = true;
}

line Grapha::GetLine() {
  if (0 <= CurrentLineID && CurrentLineID < (int)Line.size())
    return Line[CurrentLineID];
  else
    return DefaultLine;
}

position Grapha::GetPosition() {
  if (LineExist())
    return GetLine().P.back();
  else
    return {0, 0};
}

bool Grapha::LineExist() {
  if (!GetLine().P.empty())
    return true;
  else
    return false;
}
