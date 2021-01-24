#ifndef FILEIO_h_
#define FILEIO_h_
/*****************************/

#include <Grapha.h>

#include <fstream>
#include <iostream>
#include <vector>

class FileIO {
 private:
  std::vector<std::string> FormatID;
  std::string EditFileName;

 public:
  FileIO();
  ~FileIO(){};
  void Save(std::string savefile);
  void Load(std::string loadfile);
  void DrawFileName();
  std::vector<std::string> GetFormatID();

 protected:
  bool EmptyEditFileName();
  bool CheckFileFormat(std::ifstream &fp_load);
};

/*****************************/
#endif  // FILEIO_h_
