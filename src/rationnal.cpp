#include "rationnal.hpp"

//-----
// gcd
//-----
Int gcd(Int a,Int b){
  Int r=a;
  Int rp=b;
  while(r!=0){
    Int q=rp/r;
    Int t=r;
    r=rp-(q*r);
    rp=t;
  }
  return rp;
}

//------------
// operator<<
//------------
ostream& operator<<(ostream& os,const Rationnal& r){
  if(r.denominator()==1) return os<<r.numerator();
  return os<<r.numerator()<<'/'<<r.denominator();
}
