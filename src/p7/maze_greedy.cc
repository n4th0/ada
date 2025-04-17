// Nathan Rodriguez Moyses
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

// const static int CENTINELA = -1;

bool p2D_flag = false;

// int min(int a, int b, int c) {
//   if (a < b && a < c) {
//     return a;
//   } else if (b <= a && b < c) {
//     return b;
//   }
//   return c;
// }

int sumIfexists(bool b, int a){
  if (b) {
    return a + 1;
  }else {
    return a;
  }
}

int maze_greedy(vector<vector<int>> &matrix, unsigned pos_i, unsigned pos_j, bool & exists) {
  if (matrix[pos_i][pos_j] == 0) {
    return 0;
  }

  if (pos_i == (matrix.size()-1) && pos_j == (matrix[0].size() -1)) {
    exists = true;
    matrix[pos_i][pos_j] = 3;
    return 1;
  }

  if (pos_i+1 < matrix.size()  && pos_j+1 < matrix[0].size() && matrix[pos_i+1][pos_j+1] == 1) {
    matrix[pos_i][pos_j] = 3;
    return sumIfexists(exists, maze_greedy(matrix, pos_i+1, 1 + pos_j, exists));
  }

  if (pos_i < matrix.size()  && pos_j+1 < matrix[0].size() && matrix[pos_i][pos_j+1] == 1) {
      matrix[pos_i][pos_j] = 3;
    return sumIfexists(exists, maze_greedy(matrix, pos_i, 1 + pos_j, exists));
  }

  if (pos_i + 1< matrix.size()  && pos_j < matrix[0].size() && matrix[pos_i+1][pos_j] == 1) {
    matrix[pos_i][pos_j] = 3;
    return sumIfexists(exists,  maze_greedy(matrix, pos_i + 1, pos_j, exists));
  }

  matrix[pos_i][pos_j] = 3;
  return 0;
}


void print_usage() {
  cerr << "Usage:\nmaze_greedy  [--p2D] -f file" << endl;
}

int main(int argc, char **argv) {

  if (argc < 2) {
    print_usage();
    exit(-1);
  }

  string filename;

  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-f")) {
      i++;
      if (i >= argc) {
        cerr << "ERROR: missing filename" << endl;
        print_usage();
        exit(-1);
      }
      filename = argv[i];
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

  bool exists = false;
  cout << maze_greedy(matrix, 0, 0, exists) << endl;
  // cout << (exists ? "existe" : "no existe") << endl;

  if (p2D_flag) {
    for (unsigned i = 0; i<matrix.size(); i++) {
      for (unsigned j = 0; j<matrix[i].size(); j++) {
        if (matrix[i][j] == 3) {
          cout << "*";
        }else {
          cout << matrix[i][j];
        }
      }
      cout << endl;
    }
  }
}
