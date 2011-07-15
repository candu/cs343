#include <iostream>
using namespace std;

int (*fixup)(int);

int myfxup1( int i ) { return i + 2; }
int myfxup2( int i ) { return i + 1; }
int myfxup3( int i ) { return i + 3; }

int rtn2( int p );

int rtn1( int p ) {
  if ( p <= 0 ) return 0;
  int (*myfxup)(int) = fixup;
    if ( p % 2 ) {
        fixup = myfxup2;
        p = rtn2( p - 1 );
    } else {
        fixup = myfxup3;
        p = rtn1( p - 2 );
    }
    if ( p % 3 ) p = myfxup( p );
    cout << p << " ";
    return p + 1;
}
int rtn2( int p ) {
  if ( p <= 0 ) return 0;
  int (*myfxup)(int) = fixup;
    if ( p % 3 ) { 
        fixup = myfxup1;
        p = rtn2( p - 2 );
    } else {
        fixup = myfxup2;
        p = rtn1( p - 1 );
    }
    if ( p % 2 ) p = myfxup( p );
    cout << p << " ";
    return p + 2;
}
int main() {
    fixup = myfxup1;
    cout << rtn2( 30 ) << endl;
}
