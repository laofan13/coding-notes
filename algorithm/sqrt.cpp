#include <iostream>
#include <algorithm>

using namespace std;

double sqrt(double x) {
    double l = 0, r = x;
    while(l < x) {
        double mid = (l + r) / 2;
        double res = mid * mid;
        if(abs(res - x) < 0.000001) {
            return mid;
        }else if( res < x) {
            l = mid;
        }else{
            r = mid;
        }
    }
    return 0;
}

int main() {
    double x = 2;
    cout << sqrt(x);

    return 0;
}