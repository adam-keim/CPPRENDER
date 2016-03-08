#include <cstdlib> 
#include <cstdio>
#include <math.h>
#include <iostream> 
#include <iomanip> 
 
using namespace std;


template<typename T>
class Vec3 {
    public:
        Vec3(): x(T(0)), y(T(0)), z(T(0)) {}
        Vec3(const T &xx): x(xx), y(xx), z(xx) {}
        Vec3(T xx, T yy, T zz): x(xx), y(yy), z(zz) {}
        T x, y, z;
        T length() {
            return sqrt(x*x+y*y+z*z);
        }
};
int main () {
}