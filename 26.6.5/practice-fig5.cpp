#include <random>
#include <fstream>
#include <string>
using namespace std;

int main(){
    
    string path = "neocircle.csv";
    ofstream ofile(path);
    
    int seed = 415;
    mt19937 mt(seed);
    uniform_real_distribution<double> xd(1, 2);
    uniform_real_distribution<double> yd(1, 2);

    double cx = 1.3, cy = 1.425; // 絞り込み範囲の中心
    double r = 0.1;               // 半径

    int count = 0;
    while(count < 8096){
        double x = xd(mt);
        double y = yd(mt);
        if(1.2 < x && x < 1.4){
            if(1.35 < y && y < 1.5){
                double dx = x - cx;
                double dy = y - cy;
                if(dx*dx + dy*dy <= r*r){
                    ofile << x << ", " << y << "\n";
                    count++;
                }
            }
        }
    }
       
    return 0;
}
