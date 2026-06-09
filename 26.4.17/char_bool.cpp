#include <iostream>
using namespace std;
int main() {
    char grade = 'A';
    bool isPassed = true;
    
    cout << "成績: " << grade << endl;
    cout << "合格ですか？: " << (isPassed ? "はい" : "いいえ") << endl;
    return 0;
    
}
