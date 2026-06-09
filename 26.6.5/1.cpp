#include <fstream>
#include <string>
using namespace std;

int main(){
    string path = "text.txt";
    ofstream ofile(path);
    ofile << "This is output\n";
    ofile << "Can be output in the same manner you do on cout\n";
    return 0;
    
}
