#include <random>
#include <fstream>
#include <string>
using namespace std;

int main(){
    
    string path = "circle.csv";
    ofstream ofile(path);
    
    int seed = 415;
    mt19937 mt(seed);
    uniform_real_distribution<double> xd(-1, 1);
    uniform_real_distribution<double> yd(-1, 1);
   
    for(int i=0;i<8096;i++){
        double x = xd(mt);
        double y = yd(mt);
        if (x*x + y*y <= 1.0){
            ofile << x << ", " << y << "\n";
        }
    }
       
    return 0;
}
