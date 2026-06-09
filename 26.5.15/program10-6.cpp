#include <iostream>
using namespace std;

int main(){
    int x, y;
    int *p;
    p = &x;
    
    cout << p << endl;
    
    *p = 8;
    y = *p + x;
    
    cout << y << endl;
}
