
#include <cstddef>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

const static int CENTINELA = -1;

struct Coord { // for the parsing stuff
  int i;
  int j;
  Coord(int i, int j) {
    this->i = i;
    this->j = j;
  }
};

int min(int a, int b, int c) {
  if (a < b && a < c) {
    return a;
  } else if (b <= a && b < c) {
    return b;
  }
  return c;
}

vector<Coord> parsing;

/*
int maze_memo(int final_i, int final_j, int pos_i, int pos_j, int ac,
              const vector<vector<int>> &matrix, vector<vector<int>> &memo) {

  if (memo[pos_i][pos_j] != CENTINELA) {
    return memo[pos_i][pos_j];
  }

  if (matrix[pos_i][pos_j] == 0) {
    return 0;
  }

  ac++;
  if (final_i == pos_i && final_j == pos_j) {
    return ac;
  }

  int a = numeric_limits<int>::max(), b = numeric_limits<int>::max(),
      c = numeric_limits<int>::max();
  if (pos_i + 1 <= final_i && matrix[pos_i + 1][pos_j] == 1) {
    a = maze_memo(final_i, final_j, pos_i + 1, pos_j, ac, matrix, memo);
  }

  if (pos_j + 1 <= final_j && matrix[pos_i][pos_j + 1] == 1) {
    b = maze_memo(final_i, final_j, pos_i, pos_j + 1, ac, matrix, memo);
  }

  if (pos_i + 1 <= final_i && pos_j + 1 <= final_j &&
      matrix[pos_i + 1][pos_j + 1] == 1) {
    c = maze_memo(final_i, final_j, pos_i + 1, pos_j + 1, ac, matrix, memo);
  }

  memo[pos_i][pos_j] = ac;
  return min(a, b, c);
}
*/

///
/// maze_naive O(3**n) -> n = m*l
/// final_i -> 0
/// final_j -> 0
/// pos_i -> matrix.size()-1
/// pos_j -> matrix[0].size()-1
/// matrix -> matrix
int maze_naive(int final_i, int final_j, int pos_i, int pos_j,
               const vector<vector<int>> &matrix) {
  if (matrix[pos_i][pos_j] == 0) {
    return 0;
  }

  if (final_i == pos_i && final_j == pos_j) {
    return 1;
  }

  int a = numeric_limits<int>::max(), b = numeric_limits<int>::max(),
      c = numeric_limits<int>::max();

  if (pos_i - 1 >= final_i && matrix[pos_i - 1][pos_j] == 1) {
    a = maze_naive(final_i, final_j, pos_i - 1, pos_j, matrix);
  }

  if (pos_j - 1 >= final_j && matrix[pos_i][pos_j - 1] == 1) {
    b = maze_naive(final_i, final_j, pos_i, pos_j - 1, matrix);
  }

  if (pos_i - 1 >= final_i && pos_j - 1 >= final_j &&
      matrix[pos_i - 1][pos_j - 1] == 1) {
    c = maze_naive(final_i, final_j, pos_i - 1, pos_j - 1, matrix);
  }
  int aux = min(a, b, c);

  if (aux != numeric_limits<int>::max()) {
    aux++;
  } else if ((int)(matrix[0].size() - 1) == pos_j &&
             (int)(matrix.size() - 1) == pos_i) {
    aux = 0;
  }

  return aux;
}

int maze_naive2(int final_i, int final_j, int pos_i, int pos_j,
                const vector<vector<int>> &matrix, vector<vector<int>> &memo) {

  if (matrix[pos_i][pos_j] == 0) {
    memo[pos_i][pos_j] = numeric_limits<int>::max();
    return numeric_limits<int>::max();
  }

  if (memo[pos_i][pos_j] != CENTINELA) {
    return memo[pos_i][pos_j];
  }
  if (final_i == pos_i && final_j == pos_j) {
    return 1;
  }
  int a, b, c;
  if (pos_i - 1 >= final_i) {
    a = maze_naive2(final_i, final_j, pos_i - 1, pos_j, matrix, memo);
  } else {
    a = numeric_limits<int>::max();
  }

  if (pos_j - 1 >= final_j) {
    b = maze_naive2(final_i, final_j, pos_i, pos_j - 1, matrix, memo);
  } else {
    b = numeric_limits<int>::max();
  }

  if (pos_i - 1 >= final_i && pos_j - 1 >= final_j) {
    c = maze_naive2(final_i, final_j, pos_i - 1, pos_j - 1, matrix, memo);
  } else {
    c = numeric_limits<int>::max();
  }

  int aux = min(a, b, c);

  if (aux != numeric_limits<int>::max()) {
    aux++;
  }

  memo[pos_i][pos_j] = aux;

  // no existe
  if (aux == numeric_limits<int>::max() &&
      (int)(matrix[0].size() - 1) == pos_j &&
      (int)(matrix.size() - 1) == pos_i) {
    aux = 0;
  }

  return aux;
}

void print_usage() {
  cerr << "Usage:\nmaze: [--p2D] [-t] [--ignore-naive] -f file" << endl;
}

int main(int argc, char **argv) {

  if (argc < 2) {
    print_usage();
    exit(-1);
  }
  string filename;
  bool p2D_flag = false;
  bool t_flag = false;
  bool ignoreNaive_flag = false;

  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-f")) {
      i++;
      if (i >= argc) {
        cerr << "ERROR: missing filename" << endl;
        print_usage();
        exit(-1);
      }
      filename = argv[i];
    } else if (!strcmp(argv[i], "-t")) {
      t_flag = true;
    } else if (!strcmp(argv[i], "--p2D")) {
      p2D_flag = true;
    } else if (!strcmp(argv[i], "--ignore-naive")) {
      ignoreNaive_flag = true;
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

  if (t_flag) {
  }

  if (p2D_flag) {
  }
  if (ignoreNaive_flag) {
  }

  for (size_t i = 0; i < matrix.size(); i++) {
    for (size_t j = 0; j < matrix[i].size(); j++) {
      cout << matrix[i][j];
    }
    cout << endl;
  }
  vector<vector<int>> memo(rows, vector<int>(cols));

  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      memo[i][j] = CENTINELA;

  cout << "sol: "
       << maze_naive2(0, 0, matrix.size() - 1, matrix[0].size() - 1, matrix,
                      memo)
       << endl;

  // for (int i = 0; i < rows; i++) { // la clave para el parsing
  //   cout << " ";
  //   for (int j = 0; j < cols; j++) {
  //     if (memo[i][j] == -1) {
  //       cout << " - ";
  //     } else if (memo[i][j] == numeric_limits<int>::max()) {
  //       cout << " X ";
  //     } else {
  //       cout << " " << memo[i][j] << " ";
  //     }
  //   }
  //   cout << endl;
  // }

  // cout << parsing.size() << endl;
  // for (size_t i = 0; i < parsing.size(); i++) {
  //   // matrix[parsing[i].i][parsing[i].j] = 3;
  //   cout << parsing[i].i << " " << parsing[i].j << endl;
  // }

  //   for (unsigned i = 0; i < matrix.size(); i++) {
  //     for (unsigned j = 0; j < matrix[i].size(); j++) {
  //       if (matrix[i][j] == 3) {
  //         cout << '*';
  //       } else {
  //         cout << matrix[i][j];
  //       }
  //     }
  //     cout << endl;
  //   }
}
