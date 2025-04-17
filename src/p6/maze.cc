// Nathan Rodriguez Moyses
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

const static int CENTINELA = -1;

int min(int a, int b, int c) {
  if (a < b && a < c) {
    return a;
  } else if (b <= a && b < c) {
    return b;
  }
  return c;
}

void maze_parser(vector<vector<int>> &matrix, const vector<vector<int>> &memo,
                 int pos_i, int pos_j) {
  if (pos_j == (int)matrix[0].size() - 1 && pos_i == (int)matrix.size() - 1) {
    matrix[pos_i][pos_j] = 3;
  }

  if (pos_j == 0 && pos_i == 0) {
    matrix[pos_i][pos_j] = 3;
    return;
  }

  if (pos_i > 0 && pos_j > 0 &&
      memo[pos_i][pos_j] == (memo[pos_i - 1][pos_j - 1] + 1)) {
    matrix[pos_i - 1][pos_j - 1] = 3;
    return maze_parser(matrix, memo, pos_i - 1, pos_j - 1);
  }

  if (pos_i > 0 && memo[pos_i][pos_j] == (memo[pos_i - 1][pos_j] + 1)) {
    matrix[pos_i - 1][pos_j] = 3;
    return maze_parser(matrix, memo, pos_i - 1, pos_j);
  }
  if (pos_j > 0 && memo[pos_i][pos_j] == (memo[pos_i][pos_j - 1] + 1)) {
    matrix[pos_i][pos_j - 1] = 3;
    return maze_parser(matrix, memo, pos_i, pos_j - 1);
  }

  if (pos_i == 0) {
    matrix[pos_i][pos_j - 1] = 3;
    return maze_parser(matrix, memo, pos_i, pos_j - 1);
  }
  if (pos_j == 0) {
    matrix[pos_i - 1][pos_j] = 3;
    return maze_parser(matrix, memo, pos_i - 1, pos_j);
  }
}

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

int maze_memo(int final_i, int final_j, int pos_i, int pos_j,
              const vector<vector<int>> &matrix, vector<vector<int>> &memo) {

  if (matrix[pos_i][pos_j] == 0) {
    memo[pos_i][pos_j] = numeric_limits<int>::max();
    return numeric_limits<int>::max();
  }

  if (memo[pos_i][pos_j] != CENTINELA) {
    return memo[pos_i][pos_j];
  }
  if (final_i == pos_i && final_j == pos_j) {
    memo[pos_i][pos_j] = 1;
    return 1;
  }
  int a, b, c;
  if (pos_i - 1 >= final_i) {
    a = maze_memo(final_i, final_j, pos_i - 1, pos_j, matrix, memo);
  } else {
    a = numeric_limits<int>::max();
  }

  if (pos_j - 1 >= final_j) {
    b = maze_memo(final_i, final_j, pos_i, pos_j - 1, matrix, memo);
  } else {
    b = numeric_limits<int>::max();
  }

  if (pos_i - 1 >= final_i && pos_j - 1 >= final_j) {
    c = maze_memo(final_i, final_j, pos_i - 1, pos_j - 1, matrix, memo);
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

int maze_it_vector(const vector<vector<int>> &matrix) {

  vector<int> memo(matrix[0].size(), CENTINELA);
  int var = numeric_limits<int>::max();

  for (int i = 0; i < (int)matrix.size(); i++) {
    var = numeric_limits<int>::max();
    for (int j = 0; j < (int)matrix[0].size(); j++) {

      if (matrix[i][j] == 0) {
        var = memo[j];
        memo[j] = numeric_limits<int>::max();
        continue;
      }

      if (i == 0 && j == 0) {
        memo[j] = 1;
        continue;
      }

      if (j > 0 && i == 0) {
        var = memo[j];
        if (memo[j - 1] != numeric_limits<int>::max()) {
          memo[j] = 1 + memo[j - 1];
        } else {
          memo[j] = memo[j - 1];
        }
        continue;
      }

      if (i > 0 && j == 0) {
        var = memo[j];
        if (memo[j] != numeric_limits<int>::max()) {
          memo[j] = 1 + memo[j];
        } else {
          memo[j] = memo[j];
        }
        continue;
      }

      int aux = min(memo[j], memo[j - 1], var);
      var = memo[j];
      memo[j] = aux;

      if (memo[j] != numeric_limits<int>::max()) {
        memo[j]++;
      }
    }
  }

  return memo[matrix[0].size() - 1] == numeric_limits<int>::max()
             ? 0
             : memo[matrix[0].size() - 1];
}

int maze_it_matrix(const vector<vector<int>> &matrix,
                   vector<vector<int>> &memo) {

  for (int i = 0; i < (int)memo.size(); i++) {
    for (int j = 0; j < (int)memo[0].size(); j++) {
      if (i == 0 && j == 0) {
        if (matrix[i][j] == 0) {
          memo[i][j] = numeric_limits<int>::max();
        } else {
          memo[i][j] = 1;
        }
        continue;
      }

      if (matrix[i][j] == 0) {
        memo[i][j] = numeric_limits<int>::max();
        continue;
      }

      if (j > 0 && i == 0) {
        if (memo[i][j - 1] != numeric_limits<int>::max()) {
          memo[i][j] = 1 + memo[i][j - 1];
        } else {
          memo[i][j] = memo[i][j - 1];
        }
        continue;
      }

      if (i > 0 && j == 0) {
        if (memo[i - 1][j] != numeric_limits<int>::max()) {
          memo[i][j] = 1 + memo[i - 1][j];
        } else {
          memo[i][j] = memo[i - 1][j];
        }
        continue;
      }

      memo[i][j] = min(memo[i - 1][j], memo[i - 1][j - 1], memo[i][j - 1]);

      if (memo[i][j] != numeric_limits<int>::max()) {
        memo[i][j]++;
      }
    }
  }

  return memo[matrix.size() - 1][matrix[0].size() - 1] ==
                 numeric_limits<int>::max()
             ? 0
             : memo[matrix.size() - 1][matrix[0].size() - 1];
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

  vector<vector<int>> memo(rows, vector<int>(cols, CENTINELA));

  vector<vector<int>> memo_it(matrix.size(),
                              vector<int>(matrix[0].size(), CENTINELA));

  if (!ignoreNaive_flag) {
    cout << maze_naive(0, 0, matrix.size() - 1, matrix[0].size() - 1, matrix);
  } else {
    cout << "-";
  }

  cout << " ";

  int memo_int =
      maze_memo(0, 0, matrix.size() - 1, matrix[0].size() - 1, matrix, memo);

  cout << memo_int << " " << maze_it_matrix(matrix, memo_it) << " "
       << maze_it_vector(matrix) << endl;

  if (p2D_flag) { // showing pathing
    // cout << "?" << endl;

    if (memo_int == 0) {
      cout << "0" << endl;
    } else {
      maze_parser(matrix, memo, matrix.size() - 1, matrix[0].size() - 1);

      for (unsigned i = 0; i < matrix.size(); i++) {
        for (unsigned j = 0; j < matrix[i].size(); j++) {
          if (matrix[i][j] == 3)
            cout << "*";
          else
            cout << matrix[i][j];
        }
        cout << endl;
      }
    }
  }

  if (t_flag) {
    // cout << "?" << endl;

    cout << "Memoization table:" << endl;
    for (int i = 0; i < rows; i++) {
      cout << " ";
      for (int j = 0; j < cols; j++) {
        if (memo[i][j] == -1) {
          cout << " - ";
        } else if (memo[i][j] == numeric_limits<int>::max()) {
          cout << " X ";
        } else {
          cout << " " << memo[i][j] << " ";
        }
      }
      cout << endl;
    }
    cout << "Iterative table:" << endl;
    for (int i = 0; i < rows; i++) {
      cout << " ";
      for (int j = 0; j < cols; j++) {
        if (memo_it[i][j] == -1) {
          cout << " - ";
        } else if (memo_it[i][j] == numeric_limits<int>::max()) {
          cout << " X ";
        } else {
          cout << " " << memo_it[i][j] << " ";
        }
      }
      cout << endl;
    }
  }
}
