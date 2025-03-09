
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

int main() {

  if (pow2_3(10) == 1024) {
    cout << "ok" << endl;
  }
  if (pow2_3(20) == 1048576) {
    cout << "ok" << endl;
  }
  if (pow2_3(26) == 67108864) {
    cout << "ok" << endl;
  }
  if (pow2_3(5) == 32) {
    cout << "ok" << endl;
  }
  if (pow2_3(2) == 4) {
    cout << "ok" << endl;
  }

  return 0;
}
