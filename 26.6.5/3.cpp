#include <fstream>
#include <string>
using namespace std;

int main(){
    double start, end;
    start = -0.5;
    end = 1;
    
    string path = "res.csv";
    ofstream ofile(path);
    ofile << "x, f(x)\n";
    for(double i = start; i <= end; i += 0.01){
        double value = i * i;
        ofile << i << "," << value << "\n";
    }
    
    return 0;
}
