#ifndef COMMAND_h_
#define COMMAND_h_

/*****************************/

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef linux
#include <GL/glut.h>
#endif

#include <Colors.h>
#include <Utility.h>

#include <iostream>
#include <thread>
#include <vector>

class Command {
 private:
  int windowW, windowH;                   /* Window Size */
  bool CMFLAG;                            /* Command Line Flag */
  std::vector<std::string> CommandString; /* Input Command String */
  int CmdID;                              /* Current Command ID */
  int CmdCursor;                          /* Command Cursor */
  int HstCounter; /* Go Back Counter for CommandHistory */
 public:
  Command();
  void Resize(int WX, int WY);
  void Help();
  void Notification();
  void DrawCommand();
  void CommandMode();
  bool GetCommandFlag();
  int CommandStore(unsigned char key);
  int CommandHistory(int key);
  void CommandCursor(int key);
  bool RunCommand(const char *s0);
  bool RunCommand(const char *s0, const char *s1);
  bool RunCommand(const char *s0, const char *s1, const char *s2);
  bool RunCommand(const char *s0, double *a1);
  bool RunCommand(const char *s0, double *a1, double *a2);
  bool RunCommand(const char *s0, double *a1, double *a2, double *a3);
  bool RunCommand(const char *s0, double *a1, double *a2, double *a3,
                  double *a4);
  bool RunCommand(const char *s0, const char *s1, double *a1);
  bool RunCommandString(const char *s0, char *arg);
  bool RunCommandString(const char *s0, const char *s1, char *arg);
};

inline Command::Command() {
  CMFLAG = false;
  CommandString.resize(100);
  CmdID = 0;
  CmdCursor = 0;
  HstCounter = 0;
}

inline void Command::Resize(int WX, int WY) {
  windowW = WX;
  windowH = WY;
}

inline void Command::Help() {
  printf("\n\033[30m");
  printf(
      "\033[47m+-------------[ USAGE ]--------------+\033[49m\n"
      "\033[47m| let's try the following commands.  |\033[49m\n"
      "\033[47m|   circle   - draw circle.          |\033[49m\n"
      "\033[47m|   square   - draw square.          |\033[49m\n"
      "\033[47m|   polygon  - draw polygon.         |\033[49m\n"
      "\033[47m|    line    - draw line.            |\033[49m\n"
      "\033[47m|    rand    - draw random line.     |\033[49m\n"
      // "\033[47m|   kaleido  - draw kaleido.         |\033[49m\n"
      "\033[47m|    undo    - undo draw line.       |\033[49m\n"
      "\033[47m|    redo    - redo draw line.       |\033[49m\n"
      "\033[47m|     mv     - move line.            |\033[49m\n"
      "\033[47m|     cp     - copy line.            |\033[49m\n"
      "\033[47m|     co     - change line color.    |\033[49m\n"
      "\033[47m|     lw     - change line width.    |\033[49m\n"
      "\033[47m|   status   - show status.          |\033[49m\n"
      "\033[47m|    pixel   - pixel mode.           |\033[49m\n"
      // "\033[47m|   eraser   - pixel eraser.         |\033[49m\n"
      "\033[47m|   save (w) - save current sketch.  |\033[49m\n"
      "\033[47m|   edit (e) - load saved data.      |\033[49m\n"
      "\033[47m|   quit (q) - quit Grapha.          |\033[49m\n"
      "\033[47m+------------------------------------+\033[49m\n");
  printf("\033[39m\n");
  fflush(stdout);
}

inline void Command::Notification() {
  std::string _notification = "No such command. : " + CommandString[CmdID];
  std::cout << _notification << std::endl;

  // CMFLAG = true;
  // glMatrixMode(GL_PROJECTION);
  // glPushMatrix();
  // glLoadIdentity();
  // gluOrtho2D(0, windowW, windowH, 0);
  // glMatrixMode(GL_MODELVIEW);
  // glPushMatrix();
  // glLoadIdentity();
  //
  // glColor3d(1.00,0.00,0.00);
  // glBegin(GL_QUADS);
  // glVertex2d(0,windowH-30);
  // glVertex2d(windowW,windowH-30);
  // glVertex2d(windowW,windowH);
  // glVertex2d(0,windowH);
  // glEnd();
  // glColor3d(1,1,1);
  // glDrawString2(_notification.c_str(),5,windowH-10);
  //
  // glPopMatrix();
  // glMatrixMode(GL_PROJECTION);
  // glPopMatrix();
  // glMatrixMode(GL_MODELVIEW);
  // std::this_thread::sleep_for(std::chrono::seconds(3));
}

inline void Command::DrawCommand() {
  if (CMFLAG) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowW, windowH, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3d(0.00, 0.10, 0.15);
    glBegin(GL_QUADS);
    glVertex2d(0, windowH - 30);
    glVertex2d(windowW, windowH - 30);
    glVertex2d(windowW, windowH);
    glVertex2d(0, windowH);
    glEnd();
    glColor3d(1, 1, 1);
    static char s[200];
    static unsigned char count = 0;
    static bool flag = false;
    if (flag) {
      sprintf(s, ":%s", CommandString[CmdID].c_str());
      if (count % 10 == 0) flag = false;
    } else {
      memset(s, '\0', sizeof(s));
      sprintf(s, ":%s", CommandString[CmdID].c_str());
      int size = strlen(s);
      memset(s + size - CmdCursor, '_', 1);
      if (count % 10 == 0) flag = true;
    }
    ++count;
    glDrawString2(s, 5, windowH - 10);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
  }
}

inline void Command::CommandMode() {
  if (CMFLAG)
    CMFLAG = false;
  else
    CMFLAG = true;
}

inline bool Command::GetCommandFlag() { return CMFLAG; }

inline int Command::CommandStore(unsigned char key) {
  int size = CommandString[CmdID].size();
  if (CMFLAG) {
    switch (key) {
      case 13:  // return key
        if (0 < size) {
          if (HstCounter) HstCounter = 0;
          if (CmdCursor) CmdCursor = 0;
          ++CmdID;
          CMFLAG = false;
        }
        break;

      case 27:  // escape key
        HstCounter = 0;
        CmdCursor = 0;
        CMFLAG = false;
        if (0 < size) ++CmdID;
        break;

      case 127:  // delete key
        if (0 < size) {
          if (CmdCursor) {
            if (CmdCursor < size)
              CommandString[CmdID].erase(CommandString[CmdID].begin() + size -
                                         CmdCursor - 1);
          } else
            CommandString[CmdID].pop_back();
        } else {
          HstCounter = 0;
          CMFLAG = false;
        }
        break;

      default:  // input charactor
        if (CmdCursor && CmdCursor <= size && size < 50)
          CommandString[CmdID].insert(
              CommandString[CmdID].begin() + size - CmdCursor, key);
        else if (size < 50)
          CommandString[CmdID] += key;
        break;
    }
  }
  return size;
}

inline int Command::CommandHistory(int key) {
  if (CMFLAG) {
    switch (key) {
      case GLUT_KEY_UP:  // up-arrow key
        if (0 < CmdID - HstCounter) {
          if (CommandString[CmdID - HstCounter - 1].size()) {
            ++HstCounter;
            CommandString[CmdID] = CommandString[CmdID - HstCounter];
          }
        }
        break;
      case GLUT_KEY_DOWN:  // down-arrow key
        if (CommandString[CmdID - HstCounter + 1].size()) {
          if (0 < HstCounter) --HstCounter;
          CommandString[CmdID] = CommandString[CmdID - HstCounter];
        }
        if (HstCounter == 0) CommandString[CmdID].clear();
        break;
      default:
        break;
    }
  }
  return CommandString[CmdID].size();
}

inline void Command::CommandCursor(int key) {
  if (CMFLAG) {
    switch (key) {
      case GLUT_KEY_RIGHT:  // right-arrow key
        --CmdCursor;
        if (CmdCursor < 0) CmdCursor = 0;
        break;
      case GLUT_KEY_LEFT:  // left-arrow key
        ++CmdCursor;
        if ((int)CommandString[CmdID].size() < CmdCursor)
          CmdCursor = CommandString[CmdID].size();
        break;
      default:
        break;
    }
  }
}

inline bool Command::RunCommand(const char *s0) {
  bool match = false;
  if (CommandString[CmdID] == s0) match = true;
  if (match) std::cout << "Command: " << CommandString[CmdID] << std::endl;
  return match;
}

inline bool Command::RunCommand(const char *s0, const char *s1) {
  bool match = false;
  if (CommandString[CmdID] == s0) match = true;
  if (CommandString[CmdID] == s1) match = true;
  if (match) std::cout << "Command: " << CommandString[CmdID] << std::endl;
  return match;
}

inline bool Command::RunCommand(const char *s0, const char *s1,
                                const char *s2) {
  bool match = false;
  if (CommandString[CmdID] == s0) match = true;
  if (CommandString[CmdID] == s1) match = true;
  if (CommandString[CmdID] == s2) match = true;
  if (match) std::cout << "Command: " << CommandString[CmdID] << std::endl;
  return match;
}

inline bool Command::RunCommand(const char *s0, double *a1) {
  bool match = false;
  char Command[100];
  double arg1 = 0.0;
  sscanf(CommandString[CmdID].c_str(), "%s %lf", Command, &arg1);
  if (!strcmp(Command, s0)) match = true;
  if (match) {
    (*a1) = arg1;
    printf("Command: %s\t%f\n", Command, (*a1));
  }
  return match;
}

inline bool Command::RunCommand(const char *s0, double *a1, double *a2) {
  bool match = false;
  char Command[100];
  double arg1 = 0.0;
  double arg2 = 0.0;
  sscanf(CommandString[CmdID].c_str(), "%s %lf %lf", Command, &arg1, &arg2);
  if (!strcmp(Command, s0)) match = true;
  if (match) {
    (*a1) = arg1;
    (*a2) = arg2;
    printf("Command: %s\t%f\t%f\n", Command, (*a1), (*a2));
  }
  return match;
}

inline bool Command::RunCommand(const char *s0, double *a1, double *a2,
                                double *a3) {
  bool match = false;
  char Command[100];
  double arg1 = 0.0;
  double arg2 = 0.0;
  double arg3 = 0.0;
  sscanf(CommandString[CmdID].c_str(), "%s %lf %lf %lf", Command, &arg1, &arg2,
         &arg3);
  if (!strcmp(Command, s0)) match = true;
  if (match) {
    (*a1) = arg1;
    (*a2) = arg2;
    (*a3) = arg3;
    printf("Command: %s\t%f\t%f\t%f\n", Command, (*a1), (*a2), (*a3));
  }
  return match;
}

inline bool Command::RunCommand(const char *s0, double *a1, double *a2,
                                double *a3, double *a4) {
  bool match = false;
  char Command[100];
  double arg1 = 0.0;
  double arg2 = 0.0;
  double arg3 = 0.0;
  double arg4 = 0.0;
  sscanf(CommandString[CmdID].c_str(), "%s %lf %lf %lf %lf", Command, &arg1,
         &arg2, &arg3, &arg4);
  if (!strcmp(Command, s0)) match = true;
  if (match) {
    (*a1) = arg1;
    (*a2) = arg2;
    (*a3) = arg3;
    (*a4) = arg4;
    printf("Command: %s\t%f\t%f\t%f\t%f\n", Command, (*a1), (*a2), (*a3),
           (*a4));
  }
  return match;
}

inline bool Command::RunCommand(const char *s0, const char *s1, double *a1) {
  bool match = false;
  char Command[100];
  double arg1 = 0.0;
  sscanf(CommandString[CmdID].c_str(), "%s %lf", Command, &arg1);
  if (!strcmp(Command, s0)) match = true;
  if (!strcmp(Command, s1)) match = true;
  if (match) {
    (*a1) = arg1;
    printf("Command: %s\t%f\n", Command, (*a1));
  }
  return match;
}

inline bool Command::RunCommandString(const char *s0, char *arg) {
  static char Command[100];
  memset(arg, '\0', sizeof(*arg));
  sscanf(CommandString[CmdID].c_str(), "%s %s", Command, arg);
  bool match = false;
  if (!strcmp(Command, s0)) match = true;
  if (match) printf("Command: %s\t'%s'\n", Command, arg);
  return match;
}

inline bool Command::RunCommandString(const char *s0, const char *s1,
                                      char *arg) {
  static char Command[100];
  memset(arg, '\0', sizeof(*arg));
  sscanf(CommandString[CmdID].c_str(), "%s %s", Command, arg);
  bool match = false;
  if (!strcmp(Command, s0)) match = true;
  if (!strcmp(Command, s1)) match = true;
  if (match) printf("Command: %s\t'%s'\n", Command, arg);
  return match;
}

/*****************************/
#endif  // COMMAND_h_
