#include <random>
#include <iostream>
int main(){
    using namespace std;
    int seed = 777; // seed of random number. If seed is the same as before, then all result here after will be the same as before.
    mt19937 mt(seed); // Very famous pseudo random number generator, called Mersenne Twister, made in Japan.
    uniform_int_distribution<int> di(0, 2); // Uniform distribution of 0, 1, 2. Note that it includes 2.
    uniform_real_distribution<double> dd(0, 1); // Uniform distribution on the interval [0, 2], may be [0, 2). I forgot.
   
    for(int i=0;i<64;++i){
        double r = dd(mt);  // generate random number, like this.
        int a = di(mt);
    cout << r << ", " << a << "\n"; // check the result is really from our expected distribution.
    }
          
    return 0;
}
