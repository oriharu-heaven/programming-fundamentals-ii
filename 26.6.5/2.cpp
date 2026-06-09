#include <fstream>
#include <string>
using namespace std;

int main(){
    string path = "text.txt";
    ofstream ofile(path);
    for(int i = 0; i < 256; i++){
        ofile << "This is test" << i << "\n";
    }
    return 0;    
}
