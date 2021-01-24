/*
 *  Save & Load Images
 */

#include <Command.h>
#include <Declaration.h>
#include <FileIO.h>

#include <iomanip>

extern Grapha Gra;
extern Command Cmd;

FileIO::FileIO() {
  FormatID = {"################################################",
              "#   ____ ____      _   _____ __  __     _      #",
              "#  / ___|  _ \\    / \\  | | \\ | | | |   / \\     #",
              "# | |  _| |_) |  / _ \\ | |_)|| |_| |  / _ \\    #",
              "# | |_| |  _ <  / ___ \\|  __/|  _  | / ___ \\   #",
              "#  \\____|_| \\_\\/_/   \\_\\_| | |_| |_|/_/   \\_\\  #",
              "################################################"};
}

void FileIO::Save(std::string savefile) {
  std::string _filename;
  if (savefile.empty()) {
    if (EmptyEditFileName()) {
      std::cout << "\tno file name." << std::endl;
      return;
    } else {
      _filename = EditFileName;
    }
  } else  // 'savefile' is not empty
    _filename = savefile;
  std::ofstream fp_save(_filename, std::ios::out | std::ios::trunc);
  for (auto id : FormatID) fp_save << id << std::endl;
  fp_save << std::endl;
  time_t timer = time(NULL);
  struct tm* local = localtime(&timer);
  char date[100];
  sprintf(date, "%04d/%02d/%02d %02d:%02d:%02d", local->tm_year + 1900,
          local->tm_mon + 1, local->tm_mday, local->tm_hour, local->tm_min,
          local->tm_sec);
  std::cout << "\tsave (" << date << ") -> \'" << _filename << "\'"
            << std::endl;
  fp_save << "#-- " << date << " --#\n";
  fp_save << "# WindowSize:\t\t" << Gra.WX << "\t" << Gra.WY << "\n";
  fp_save << "# CurrentLineID:\t" << Gra.CurrentLineID << "\n\n";
  unsigned int id = 0;
  for (auto&& l : Gra.Line) {
    fp_save << "# ID:\t\t" << id << "\n";
    fp_save << "# width:\t" << l.Width << "\n";
    fp_save << std::fixed << std::setprecision(6) << "# rgb:\t\t" << l.Color.R
            << "\t" << l.Color.G << "\t" << l.Color.B << "\n";
    fp_save << "# x,y:\n";
    for (auto&& p : l.P) {
      fp_save << p.x << "\t" << p.y << "\n";
    }
    fp_save << std::endl;
    ++id;
  }
}

void FileIO::Load(std::string loadfile) {
  if (loadfile == "") {
    std::cout << "\tno file name." << std::endl;
    return;
  }
  std::ifstream fp_load(loadfile);
  if (!fp_load) {
    std::cout << "\tfile not found. -> \'" << loadfile << "\'" << std::endl;
    Gra.Reset();
    EditFileName = loadfile;
    return;
  }
  if (!CheckFileFormat(fp_load)) {
    std::cout << "\tinvarid file format. -> \'" << loadfile << "\'"
              << std::endl;
    return;
  }
  EditFileName = loadfile;
  Gra.Reset();
  std::string buf;
  static char param[100], date[100], time[100];
  double arg[3];
  std::vector<position> InitPosition;
  bool PosFlag = false;
  while (getline(fp_load, buf)) {  // load file loop
    if (buf == "") {
      PosFlag = false;
      continue;
    } else if (buf[0] == '#' && buf.size() > 1) {
      if (buf[1] == '-') {
        sscanf(buf.c_str(), "#-- %s %s --#", date, time);
        std::cout << "\tload (" << date << " " << time << ") -> \'" << loadfile
                  << "\'" << std::endl;
      } else if (buf[1] == ' ') {
        sscanf(buf.c_str(), "# %s %lf %lf %lf", param, &arg[0], &arg[1],
               &arg[2]);
        if (!strcmp(param, "WindowSize:"))
          glutReshapeWindow((int)arg[0], (int)arg[1]);
        if (!strcmp(param, "CurrentLineID:")) Gra.CurrentLineID = (int)arg[0];
        if (!strcmp(param, "ID:"))
          Gra.Line.push_back({{0, 0, 0}, 0, InitPosition});
        if (!strcmp(param, "width:")) Gra.Line.back().Width = arg[0];
        if (!strcmp(param, "rgb:"))
          Gra.Line.back().Color = {arg[0], arg[1], arg[2]};
        if (!strcmp(param, "x,y:")) PosFlag = true;
        continue;
      }
    } else if (PosFlag) {
      sscanf(buf.c_str(), "%lf %lf", &arg[0], &arg[1]);
      Gra.Line.back().P.push_back({(int)arg[0], (int)arg[1]});
      Gra.FillPixel();
    }
  }
  if ((int)(Gra.Line.size() - 1) < Gra.CurrentLineID)
    Gra.CurrentLineID = Gra.Line.size() - 1;
}

bool FileIO::EmptyEditFileName() {
  if (EditFileName.size())
    return false;
  else
    return true;
}

bool FileIO::CheckFileFormat(std::ifstream& fp_load) {
  std::string _buf;
  bool flag = true;
  fp_load.seekg(0, std::ios_base::beg);
  for (auto id : FormatID) {
    getline(fp_load, _buf);
    if (_buf != id) flag = false;
  }
  return flag;
}

void FileIO::DrawFileName() {
  if (Gra.STFLAG) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, Gra.WX, Gra.WY, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3d(1.0 - Gra.CanvasColor.R, 1.0 - Gra.CanvasColor.G,
              1.0 - Gra.CanvasColor.B);
    if (Cmd.GetCommandFlag())
      glDrawString(EditFileName.c_str(), 10, Gra.WY - 35);
    else
      glDrawString(EditFileName.c_str(), 10, Gra.WY - 10);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
  }
}

std::vector<std::string> FileIO::GetFormatID() { return FormatID; }
