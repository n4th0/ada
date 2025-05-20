// Nathan Rodriguez Moyses
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>
#include <fstream>
#include <string.h>

#define CENTINELA -1

using namespace std;

const int START_I = 0;
const int START_J = 0;

int END_I = 0;
int END_J = 0;

bool resuelve_3_movs = false;
bool p2D_flag = false;
bool p_flag = false;

int visited_leaf_nodes = 0;
int visited_nodes = 0;
int explored_nodes = 0;
int no_promissing_discarded_nodes = 0;
int no_feasible_discarded_nodes = 0;

void print_usage() {
  cerr << "Usage:\nmaze: [--p2D] [-p] -f file" << endl;
}

int min(int a, int b, int c) {
  if (a < b && a < c) {
    return a;
  } else if (b <= a && b < c) {
    return b;
  }
  return c;
}
int sum_if_not_inf(int a){
  if (a != numeric_limits<int>::max())
    return a +1;
  return a ;
}

///
/// 1 norte 2 noreste 3 este 4 sureste 5 sur ...
///
void directions(int n, int & pos_i, int & pos_j){
  switch (n) {
    case 1:
      pos_i--;
      return;
    case 2:
      pos_i--;
      pos_j++;
      return;
    case 3:
      pos_j++;
      return;
    case 4:
      pos_i++;
      pos_j++;
      return;
    case 5:
      pos_i++;
      return;
    case 6:
      pos_i++;
      pos_j--;
      return;
    case 7:
      pos_j--;
      return;
    case 8:
      pos_i--;
      pos_j--;
      return;
    default: 
      return;
  }

}

int sumIfexists(bool b, int a){
  if (b) {
    return a + 1;
  }else {
    return a;
  }
}

////////////////////////////////////

int maze_greedy(const vector<vector<int>> &matrix, unsigned pos_i, unsigned pos_j, bool & exists) {
  if (matrix[pos_i][pos_j] == 0) {
    return 0;
  }

  if (pos_i == (matrix.size()-1) && pos_j == (matrix[0].size() -1)) {
    exists = true;
    return 1;
  }

  if (pos_i+1 < matrix.size()  && pos_j+1 < matrix[0].size() && matrix[pos_i+1][pos_j+1] == 1) {
    return sumIfexists(exists, maze_greedy(matrix, pos_i+1, 1 + pos_j, exists));
  }

  if (pos_i < matrix.size()  && pos_j+1 < matrix[0].size() && matrix[pos_i][pos_j+1] == 1) {
    return sumIfexists(exists, maze_greedy(matrix, pos_i, 1 + pos_j, exists));
  }

  if (pos_i + 1< matrix.size()  && pos_j < matrix[0].size() && matrix[pos_i+1][pos_j] == 1) {
    return sumIfexists(exists,  maze_greedy(matrix, pos_i + 1, pos_j, exists));
  }

  return 0;
}

int maze_greedy(const vector<vector<int>> & m, int pos_i, int pos_j){
  bool b = false;
  int a = maze_greedy(m, pos_i, pos_j, b);
  if (b) {
    return a;
  }  
  return numeric_limits<int>::max();
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

int maze_memo(const vector<vector<int>> &m,int pos_i, int pos_j,vector<vector<int>> &memo){

  int aux = maze_memo(END_I, END_J, pos_i, pos_j, m, memo);
  if (aux == 0) {
    return numeric_limits<int>::max();
  }
  return aux;
}

int maze_it_matrix(const vector<vector<int>> &matrix,
                   vector<vector<int>> &memo) {

  for (int i = 0; i < (int)memo.size(); i++) {
    for (int j = 0; j < (int)memo[0].size(); j++) {
      if (i == 0 && j == 0) {
        if (matrix[i][j] == 0) {
          memo[i][j] = numeric_limits<int>::max();
        } else {
          memo[i][j] = 2;
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

int prioritize(int i){
  switch (i) {
    case 1:
      return 4;
    case 2:
      return 5;
    case 4:
      return 1;
    case 5:
      return 2;
    default:
      return i;
  }
}

void maze_parser(vector<vector<int>> &matrix, const vector<vector<int>> &memo,
                 int pos_i, int pos_j, vector<int> &sol) {

  if (pos_j == (int)matrix[0].size() - 1 && pos_i == (int)matrix.size() - 1) {
    matrix[pos_i][pos_j] = 3;
  }

  if (pos_j == 0 && pos_i == 0) {
    matrix[pos_i][pos_j] = 3;
    return;
  }

  // 8 ~ 4
  if (pos_i > 0 && pos_j > 0 &&
      memo[pos_i][pos_j] == (memo[pos_i - 1][pos_j - 1] + 1)) {
    sol.push_back(4);
    matrix[pos_i - 1][pos_j - 1] = 3;
    return maze_parser(matrix, memo, pos_i - 1, pos_j - 1, sol);
  }
  // 1 ~ 5
  if (pos_i > 0 && memo[pos_i][pos_j] == (memo[pos_i - 1][pos_j] + 1)) {
    matrix[pos_i - 1][pos_j] = 3;
    sol.push_back(5);
    return maze_parser(matrix, memo, pos_i - 1, pos_j, sol);
  }
  // 7 ~ 3
  if (pos_j > 0 && memo[pos_i][pos_j] == (memo[pos_i][pos_j - 1] + 1)) {
    matrix[pos_i][pos_j - 1] = 3;
    sol.push_back(3);
    return maze_parser(matrix, memo, pos_i, pos_j - 1, sol);
  }
  // new moves
  // +- 6 ~ 2
  if (pos_i <= END_I && pos_j > 0 && 
      memo[pos_i][pos_j] == (memo[pos_i + 1][pos_j - 1] + 1)) {
    matrix[pos_i + 1][pos_j - 1] = 3;
    sol.push_back(2);
    return maze_parser(matrix, memo, pos_i + 1, pos_j - 1, sol);
  }
  // -+ 2 ~ 6
  if (pos_i > 0 && pos_j <= END_J &&
      memo[pos_i][pos_j] == (memo[pos_i - 1][pos_j + 1] + 1)) {
    matrix[pos_i - 1][pos_j + 1] = 3;
    sol.push_back(6);
    return maze_parser(matrix, memo, pos_i - 1, pos_j + 1, sol);
  }
  // ++ 4 ~ 8
  if (pos_i <= END_I && pos_j <= END_J &&
      memo[pos_i][pos_j] == (memo[pos_i + 1][pos_j + 1] + 1)) {
    matrix[pos_i + 1][pos_j + 1] = 3;
    sol.push_back(8);
    return maze_parser(matrix, memo, pos_i + 1, pos_j + 1, sol);
  }
  // 5 ~ 1
  if (pos_i <= END_I  &&
      memo[pos_i][pos_j] == (memo[pos_i + 1][pos_j] + 1)) {
    matrix[pos_i + 1][pos_j] = 3;
    sol.push_back(1);
    return maze_parser(matrix, memo, pos_i + 1, pos_j, sol);
  }
  // 3 ~ 7
  if (pos_j <= END_J &&
      memo[pos_i][pos_j] == (memo[pos_i][pos_j + 1] + 1)) {
    matrix[pos_i][pos_j + 1] = 3;
    sol.push_back(7);
    return maze_parser(matrix, memo, pos_i, pos_j + 1, sol);
  }

}

////////////////////////////////////

int optimistic_limit(int pos_i, int pos_j){
  int a_i = END_I - pos_i;
  int a_j = END_J - pos_j;

  // int s = a_i*a_i + a_j*a_j;
  return max(a_i, a_j);
}


void maze_bt(int pos_i, int pos_j,
             const vector<vector<int>> &matrix, vector<vector<int>> &arrived,
             int curr_sol, int &best_sol){

  if (pos_i == END_I && pos_j == END_J) {
    visited_leaf_nodes++;
    curr_sol++;
    best_sol = min(curr_sol, best_sol);
    return;
  }
  // bool debug = (pos_i == 16 && pos_j == 11);

  // int sol_aux = curr_sol;
  int a_i, a_j;
  // vector<int> v;
  int n;
  for (int i = 1; i <= 8; i++) {

    n = prioritize(i);

    int sol_aux = curr_sol;
    visited_nodes++;

    a_i = pos_i;
    a_j = pos_j;

    directions(n, a_i, a_j);

    // fuera de rango
    if (a_i < 0 || a_j < 0 || a_j > END_J || a_i > END_I) {
      no_feasible_discarded_nodes++;
      continue;
    }

    // es pared
    if (matrix[a_i][a_j] == 0) {
      no_feasible_discarded_nodes++;
      continue;
    }

    // exploro nodo
    sol_aux++;
    explored_nodes++;

    // he llegado con menos movimientos descarto
    if (arrived[a_i][a_j] <= sol_aux) {
      no_promissing_discarded_nodes++;
      continue;
    }

    // si llego mi mejor path es mejor que el actual, paso del actual
    if (best_sol <= sol_aux) {
      no_promissing_discarded_nodes++;
      continue;
    }

    // actualizo ya que hay un path nuevo con el que llego con menos movs
    arrived[a_i][a_j] = sol_aux;

    int opt_lim = optimistic_limit(a_i, a_j) + sol_aux;

    if (opt_lim > best_sol) { // compruebo cota optimista y pesimista
      no_promissing_discarded_nodes++;
      continue;
    }


    maze_bt(a_i, a_j, matrix, arrived, sol_aux, best_sol);

    no_promissing_discarded_nodes++;
  }

  return;
}



int maze_bt(vector<vector<int>> & matrix, vector<int> &sol){

  vector<vector<int>> arrived(matrix.size(), vector<int>(matrix[0].size()));

  if (matrix[0][0] == 0) {
    visited_nodes++;
    return 0;
  }

  // auto start = clock();

  int s = maze_it_matrix(matrix, arrived);
  if (s == 0) {
    s = numeric_limits<int>::max();
  }else {
    resuelve_3_movs = true;
  }
  int s_aux = s;

  // auto end = clock();

  // cout << "Preparation time: " <<1000.0 * (end - start) / CLOCKS_PER_SEC << endl;

  // start = clock();
  maze_bt(0, 0, matrix, arrived, 0, s);
  // end = clock();

  resuelve_3_movs = resuelve_3_movs && s == s_aux;

  // cout << "Execution time: " <<1000.0 * (end - start) / CLOCKS_PER_SEC << endl;


  if (s == numeric_limits<int>::max()) {
    return 0;
  }

  if (p2D_flag || p_flag) {

    arrived[0][0] = 0;

    if (resuelve_3_movs) {
      maze_it_matrix(matrix, arrived);
    }

    maze_parser(matrix, arrived, END_I, END_J, sol);
  }

  return s;
}


int main(int argc, char ** argv){

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

  END_I = (int) matrix.size()-1;
  END_J = (int) matrix[0].size()-1;

  vector<int> solution;

  auto start = clock();
  int sol = maze_bt(matrix, solution);
  auto end = clock();

  cout << sol <<endl;
  cout << visited_nodes << " ";
  cout << explored_nodes << " ";
  cout << visited_leaf_nodes << " ";
  cout << no_feasible_discarded_nodes << " ";
  cout << no_promissing_discarded_nodes << " ";
  cout << endl;
  cout << 1000.0 * (end - start) / CLOCKS_PER_SEC << endl;

  if (p2D_flag && sol != 0) {
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

  if (p_flag && sol!=0) {
    cout << "<";
    for (int i = solution.size()-1; i >= 0 ; i--) {
      cout << solution[i];
    }
    cout << ">"<<endl;
  }

  if (p2D_flag && sol==0) 
    cout << "0"<<endl;
  if (p_flag && sol==0) 
    cout << "<0>"<<endl;

}
