//A Self Contained example of Lazy Streams Natively in C++
//Based on work by D. Renz and M. Borowczak @ the University of Cincinnati
//http://www2.moreheadstate.edu/files/colleges/science/mcs/mejam/Renz.pdf

#include <iostream>
using namespace std;

class Stream {
public:
  int head;
  bool isEmpty;
  virtual Stream *tail() = 0;
  Stream() {isEmpty = false;}
};

class Successor : public Stream{
public:
  Successor( int i ){
    head = i;
  }
  Stream *tail(){ return new Successor( head + 1 ); }
};

class Times : public Stream {
  Stream *inStream;
  int multiplier;
public:
  Times( int n, Stream *s ) {
    multiplier = n;
    inStream = s;
    isEmpty = inStream->isEmpty;
    if ( !isEmpty ) head = multiplier * inStream->head;
  }
  Stream *tail() { return new Times( multiplier, inStream->tail() ); }
};

class Merge : public Stream {
  Stream *branch1, *branch2;
public:
  Merge ( Stream *a, Stream *b ) {
    if ( a->isEmpty && b->isEmpty ) {
      branch1 = a;
      branch2 = b;
      isEmpty = true;
    } else if ( b->isEmpty ) {
      branch1 = a;
      branch2 = b;
    } else if ( a->isEmpty ) {
      branch1 = b;
      branch2 = a;
    } else if ( a->head < b->head) {
      branch1 = a;
      branch2 = b;
    } else {
      branch1 = b;
      branch2 = a;
    }
    head = branch1-> head;
  }
  Stream *tail() { return new Merge( branch1->tail(), branch2 ); }
};

class List: public Stream {
  int *tokens;
  int size;
public:
  List(int *t, int s ){
    tokens = t;
    size = s;
    if ( size > 0 )
      head = tokens[0];
    else
      isEmpty = true;
  }
  Stream *tail(){ return new List( &tokens[1],(size - 1) ); }
};
//END LAZY STREAM CLASS DEFINITIONS

//CONSUMER HELPER FUNCTION
//Inputs: STREAM, # of Tokens to Process
void consumer( Stream *s , int toks){
  //Hacked for now just so no one complains
  if (toks < 0){
      toks = 0;
  }
  for ( int i = 0; ( i < toks && !s->isEmpty ); i++ ){
    cout << s->head << endl;
    s = s->tail();
  }
}

//A 'real' example
//Let's find the first 400 Hamming Numbers {3,5,11}
class Hamming : public Stream {
  Stream *p;
public:
  Hamming( Stream *givenPrimes ) {
    p = givenPrimes;
    isEmpty = p->isEmpty;
    if(!isEmpty) head = p->head;
  }
  Stream *tail() {
    return new Merge(new Times(p->head, this),new Hamming(p->tail()));
  }
};




int main(){


  int primes[3]= { 3, 5, 11 };
  Stream *h = new Hamming( new List(primes, 3) );
  consumer( h, 400 ); 

  return 0;

}
