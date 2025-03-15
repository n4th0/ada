// Nathan Rodriguez Moyses
// 48727425Q
#include <iostream>

using namespace std;

unsigned long pasos = 0;

// \Theta n
unsigned long pow2_1(unsigned n) {
  pasos++;
  if (n == 0) {
    return 1;
  } else {
    return 2 * pow2_1(n - 1);
  }
}

// \Theta logn
unsigned long pow2_2(unsigned n) {
  pasos++;
  if (n == 0) {
    return 1;
  } else if (n == 1) {
    return 2;
  } else {
    unsigned long aux = 1;
    if (n % 2 == 1) {
      aux = pow2_2(n / 2);
      return 2 * aux * aux;
    }

    aux = pow2_2(n / 2);
    return aux * aux;
  }
}

// \Theta 2**n
unsigned long pow2_3(unsigned n) {
  pasos++;
  if (n == 0) {
    return 1;
  } else {
    return pow2_3(n - 1) + pow2_3(n - 1);
  }
}
void cabecera() {
  cout << "# THREE DIFERENT WAYS TO CALCULATE 2**n.                            "
          "         "
       << endl;
  cout << "# n      Θ(n)          Θ(log(n))                Θ(2**n)             "
          "         "
       << endl;
  cout << "#==================================================================="
          "========="
       << endl;
}

int main() {

  cabecera();
  for (unsigned i = 0; i < 20; i++) {
    pasos = 0;
    cout << i << " ";
    pow2_1(i);
    cout << "\t" << pasos << " ";
    pasos = 0;
    pow2_2(i);
    cout << "\t\t" << pasos << " ";
    pasos = 0;
    pow2_3(i);
    cout << "\t\t\t" << pasos << " ";
    pasos = 0;
    cout << endl;
  }

  return 0;
}
