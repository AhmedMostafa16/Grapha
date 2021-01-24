/*
 *  Grapha Commands Settings
 */

#include <Command.h>
#include <Declaration.h>
#include <FileIO.h>
#include <Grapha.h>

#include <iostream>

extern bool MFLAG;  // defined in Mouse.cpp
extern Grapha Gra;
extern Command Cmd;
extern FileIO Fio;

void Commands() {  // call when 'return key' is pushed
  static double Arg1, Arg2, Arg3;
  static char ArgString[200];

  if (Cmd.RunCommand("q", "quit", "exit"))
    Exit();

  else if (Cmd.RunCommand("h", "help"))
    Cmd.Help();

  else if (Cmd.RunCommand("undo"))
    Gra.Undo();

  else if (Cmd.RunCommand("redo"))
    Gra.Redo();

  else if (Cmd.RunCommand("status"))
    Gra.ShowStatus();

  else if (Cmd.RunCommand("reset", "clear"))
    Gra.Reset();

  else if (Cmd.RunCommand("mv", "move"))
    Gra.SetDrawMode(MLINEMOVE);

  else if (Cmd.RunCommand("cp", "copy"))
    Gra.SetDrawMode(MLINECOPY);

  else if (Cmd.RunCommand("rot", "rotate"))
    Gra.SetDrawMode(MLINEROTATE);

  else if (Cmd.RunCommand("co"))
    Gra.SetDrawMode(MCOLORBARLINE);

  else if (Cmd.RunCommand("co", &Arg1, &Arg2, &Arg3))
    Gra.SetDefaultLineColor(Arg1, Arg2, Arg3);

  else if (Cmd.RunCommand("cco"))
    Gra.SetDrawMode(MCOLORBARCANVAS);

  else if (Cmd.RunCommand("cco", &Arg1, &Arg2, &Arg3))
    Gra.SetCanvasColor(Arg1, Arg2, Arg3);

  else if (Cmd.RunCommand("lw", "width", &Arg1))
    Gra.SetDefaultLineWidth(Arg1);

  else if (Cmd.RunCommand("p", "pixel"))
    Gra.PixelMode();

  else if (Cmd.RunCommand("ps", "pixelsize", &Arg1))
    Gra.SetPixelSize(Arg1);

  else if (Cmd.RunCommand("erase", "eraser"))
    Gra.PixelEraser();

  else if (Cmd.RunCommand("free", "freehand"))
    Gra.SetDrawMode(MFREEHAND);

  else if (Cmd.RunCommand("circle"))
    Gra.SetDrawMode(MCIRCLE);

  else if (Cmd.RunCommand("square"))
    Gra.SetDrawMode(MSQUARE);

  else if (Cmd.RunCommand("polygon", &Arg1))
    Gra.SetDrawMode(MPOLYGON, Arg1);

  else if (Cmd.RunCommand("line"))
    Gra.SetDrawMode(MSTRAIGHTLINE);

  else if (Cmd.RunCommand("rand", "random"))
    Gra.SetDrawMode(MRANDOM);

  else if (Cmd.RunCommand("kaleido"))
    Gra.SetDrawMode(MKALEIDO);

  else if (Cmd.RunCommandString("w", "save", ArgString))
    Fio.Save(ArgString);

  else if (Cmd.RunCommandString("e", "edit", ArgString))
    Fio.Load(ArgString);

  else if (Cmd.RunCommandString("wq", ArgString)) {
    Fio.Save(ArgString);
    Exit();
  }

  else
    Cmd.Notification();

  fflush(stdout);
}

void InputKey(unsigned char key, int x, int y) {
  static int size;
  switch (key) {
    case 9:  // tab key
      break;

    case 13:  // return key
      Commands();
      Cmd.CommandStore(key);
      KeyboardMode();
      break;

    case 27:  // escape key
      Cmd.CommandStore(key);
      KeyboardMode();
      break;

    case 127:  // delete key
      size = Cmd.CommandStore(key);
      if (size == 0) KeyboardMode();
      break;

    default:
      Cmd.CommandStore(key);
      break;
  }
}

/* arrow key */
void keyboard_sp(int key, int x, int y) {
  if (key == GLUT_KEY_UP || key == GLUT_KEY_DOWN) Cmd.CommandHistory(key);
  if (key == GLUT_KEY_RIGHT || key == GLUT_KEY_LEFT) Cmd.CommandCursor(key);
}
