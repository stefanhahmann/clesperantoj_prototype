#include "clesperantoj.h"
#include <iostream>
#include <vector>

using namespace std;

ClesperantoJ::ClesperantoJ()
{}

void ClesperantoJ::sayHello() 
{
    std::cout<<"Hello Java, from c++\n";
}

void ClesperantoJ::gaussianBlur2d(ObjectJ in, ObjectJ out, float sx, float sy) {
    cle.GaussianBlur(in.obj, out.obj, sx, sy,1);
} 
