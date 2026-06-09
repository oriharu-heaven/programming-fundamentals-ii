#include <iostream>
using namespace std;

int main() {
    int x[] = {1, 2, 3};
    int *p = x;

    cout << "x[]" << endl;
    for (int i = 0; i < 3; i++) {
        printf("x[%d] = %d\n", i, x[i]);
    }
    cout << endl;

    cout << "p" << endl;
    for (int i = 0; i < 3; i++) {
        printf("*(p+%d) = %d\n", i, *(p + i));
    }
}
