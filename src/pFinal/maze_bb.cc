// Nathan Rodriguez Moyses
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <queue>
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

int completed_nodes = 0;
int visited_nodes = 0;
int expanded_nodes = 0;
int no_promissing_discarded_nodes = 0;
int no_feasible_discarded_nodes = 0;
int explored_nodes = 0;
int current_best_updates_from_pessimistic_bounds = 0;
int current_best_updates_from_completed_nodes = 0;

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

void sum_if_not_inf(int & a){
  if (a != numeric_limits<int>::max())
     a++;

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

int maze_memo(const vector<vector<int>> &matrix, vector<vector<int>> &memo) {
  int rows = matrix.size();
  int cols = matrix[0].size();
  int INF = numeric_limits<int>::max();

  for (int i = rows - 1; i >= 0; i--) {
    for (int j = cols - 1; j >= 0; j--) {
      if (i == END_I && j == END_J) {
        memo[i][j] = 0;
        continue;
      }
      if (matrix[i][j] == 0) {
        memo[i][j] = INF;
        continue;
      }
      int minNext = INF;
      for (int d = 1; d <= 8; d++) {
        int ni = i, nj = j;
        directions(d, ni, nj);
        if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && matrix[ni][nj] == 1 && memo[ni][nj]!= CENTINELA) {
          minNext = min(minNext, memo[ni][nj]);
        }
      }
      memo[i][j] = (minNext != INF) ? (1 + minNext) : INF;
    }
  }

  /*
  for (int i = rows - 1; i >= 0; i--) {
    for (int j = cols - 1; j >= 0; j--) {
      if (i == rows - 1 && j == cols - 1) {
        memo[i][j] = 1;
        continue;
      }

      if (matrix[i][j] == 0) {
        memo[i][j] = INF;
        continue;
      }


      int minNext = INF;

      if (i + 1 < rows) minNext = min(minNext, memo[i + 1][j]);        // Abajo
      if (j + 1 < cols ) minNext = min(minNext, memo[i][j + 1]);        // Derecha
      if (i + 1 < rows && j + 1 < cols) minNext = min(minNext, memo[i + 1][j + 1]); // Diagonal

      if (minNext != INF)
        memo[i][j] = 1 + minNext;
      else
        memo[i][j] = INF;
    }
  }*/

  return memo[0][0];
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

  return memo[matrix.size() - 1][matrix[0].size() - 1];
  // return memo[matrix.size() - 1][matrix[0].size() - 1] ==
  //                numeric_limits<int>::max()
  //            ? 0
  //            : memo[matrix.size() - 1][matrix[0].size() - 1];

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

int optimistic_limit(int pos_i, int pos_j){
  int a_i = END_I - pos_i;
  int a_j = END_J - pos_j;

  // int s = a_i*a_i + a_j*a_j;
  return max(a_i, a_j);
}

int maze_bb(const vector<vector<int>> & matrix, vector<vector<int>> & arrived, int iterative_sol){
// int maze_bb(const vector<vector<int>> & matrix, vector<vector<int>> & arrived, const vector<vector<int>> & memo, int iterative_sol){
 

  if (!matrix[0][0] || !matrix[END_I][END_J] ) {
    completed_nodes++;
    return 0;
  }

  // 1
  int sol = iterative_sol;
  // double sol = iterative_sol;

  // 1
  // using Node = tuple<int, int, int>;

  //                  i    j  current opt
  using Node = tuple<int, int, int, int>;


  // 2
  // struct is_worse{
  //   bool operator()(const Node&a, const Node&b){
  //     return get<2>(a) > get<2>(b);
  //   }
  // };

  struct is_worse{
    bool operator()(const Node&a, const Node&b){
      return get<3>(a) > get<3>(b);
    }
  };

  // 1
  // priority_queue<Node> pq;

  // 2
  priority_queue<Node, vector<Node>, is_worse> pq;

  // 3
  // queue<Node> pq;

  pq.emplace(0, 0, 1, optimistic_limit(0, 0)+1);


  while (!pq.empty()) {

    // 3
    // auto [i, j, current, opt] = pq.front();

    auto [i, j, current, opt] = pq.top();

    pq.pop();

    if (opt >= sol) {
      no_promissing_discarded_nodes++;
      continue;
    }

    if (i == END_I && j == END_J) {
      completed_nodes++;
      if (sol > current) {
        current_best_updates_from_completed_nodes++;
        sol = current;
      }
      continue;
    }

    int a_i, a_j;
    int n;

    expanded_nodes++;
    for (int a = 1; a < 9; a++) {
      visited_nodes++;

      n = prioritize(a);
      a_i = i;
      a_j = j;

      int sol_aux = current;
      directions(n, a_i, a_j); // get the direction of the n

      // no feasible
      if (a_i < 0 || a_j < 0 || 
        a_j > END_J || a_i > END_I || 
        matrix[a_i][a_j] == 0) {
        no_feasible_discarded_nodes++;
        continue;
      }

      sol_aux++;


      // 1
      // // no cycles
      // if (arrived[a_i][a_j] <= sol_aux) {
      //   continue;
      // }

      // no cycles
      if (arrived[a_i][a_j] <= sol_aux) {
        no_promissing_discarded_nodes++;
        continue;
      }

      // 2
      //
      // if (sol <= sol_aux) {
      //   continue;
      // }


      if (sol <= sol_aux) {
        no_promissing_discarded_nodes++;
        continue;
      }

      // int vor = maze_greedy(matrix, i, j);
      // if ( vor!= numeric_limits<int>::max() && vor+current < sol) {
      //   sol = vor+current;
      // }

      // if (memo[a_i][a_j] != numeric_limits<int>::max() &&
      //   sol > memo[a_i][a_j]+sol_aux) {
      //   current_best_updates_from_pessimistic_bounds++;
      //   sol = memo[a_i][a_j]+sol_aux;
      //   break;
      // }

      arrived[a_i][a_j] = sol_aux;

      int opt_lim = optimistic_limit(a_i, a_j) + sol_aux;
      // double opt_lim = 1.0205*optimistic_limit(a_i, a_j) + sol_aux;

      if (opt_lim >= sol) {
        no_promissing_discarded_nodes++;
        continue;
      }

      pq.emplace(a_i, a_j, sol_aux, opt_lim);
    }
  }

  return sol;
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
  vector<vector<int>> memo(rows, vector<int>(cols));
  vector<vector<int>> memo2(rows, vector<int>(cols, CENTINELA));

  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      is >> matrix[i][j];

  END_I = (int) matrix.size()-1;
  END_J = (int) matrix[0].size()-1;

  vector<int> solution;

  auto start = clock();

  // maze_memo(matrix, memo2);

  int sol_aux = maze_it_matrix(matrix, memo);

  // int sol = maze_bb(matrix, memo, memo2, sol_aux);
  int sol = maze_bb(matrix, memo, sol_aux);


  auto end = clock();

  cout << sol <<endl;

  // for (unsigned i = 0; i<matrix.size(); i++) {
  //   for (unsigned j = 0; j<matrix[i].size(); j++) {
  //     cout << matrix[i][j] << " ";
  //   }
  //   cout << endl;
  // }
  // cout << "---------------"<<endl;
  // for (unsigned i = 0; i<matrix.size(); i++) {
  //   for (unsigned j = 0; j<matrix[i].size(); j++) {
  //     if (memo2[i][j] == numeric_limits<int>::max()) {
  //       cout << "I ";
  //     }
  //     else{
  //       cout << memo2[i][j] << " ";
  //     }
  //   }
  //   cout << endl;
  // }

  cout << visited_nodes << " ";
  cout << expanded_nodes << " ";
  cout << completed_nodes << " ";
  cout << no_feasible_discarded_nodes << " ";
  cout << no_promissing_discarded_nodes << " ";
  cout << explored_nodes << " ";
  cout << current_best_updates_from_completed_nodes << " ";
  cout << current_best_updates_from_pessimistic_bounds << " ";
  cout << endl;

  cout << 1000.0 * (end - start) / CLOCKS_PER_SEC << endl;

  memo[0][0] = 1;
  if (p2D_flag || p_flag ) {
    maze_parser(matrix, memo, END_I, END_J, solution);
  }

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
