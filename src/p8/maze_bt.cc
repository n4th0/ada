
#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>

using namespace std;



void print_usage() {
  cerr << "Usage:\nmaze: [--p2D] [-p] -f file" << endl;
}


int main(int argc, char ** argv){

  if (argc < 2) {
    print_usage();
    exit(-1);
  }

  string filename;
  bool p2D_flag = false;
  bool p_flag = false;

  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-f")) {
      i++;
      if (i >= argc) {
        cerr << "ERROR: missing filename" << endl;
        print_usage();
        exit(-1);
      }
      filename = argv[i];
    } else if (!strcmp(argv[i], "-p")) {
      p_flag = true;
    } else if (!strcmp(argv[i], "--p2D")) {
      p2D_flag = true;
    } else {
      cerr << "ERROR: unknown option " << argv[i] << endl;
      print_usage();
      exit(-1);
    }
  }


  ifstream is(filename);
  if (!is) {
    cerr << "ERROR: can't open file: " << filename << endl;
    print_usage();
    exit(-1);
  }

  int rows, cols;
  is >> rows >> cols;
  vector<vector<int>> matrix(rows, vector<int>(cols));
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      is >> matrix[i][j];

  for (int i = 0; i<matrix.size(); i++) {
    for (int j = 0; j<matrix[i].size(); j++) {
      cout << matrix[i][j];
    }
    cout << endl;
  }
  cout << p2D_flag <<endl;
  cout << p_flag <<endl;

}
