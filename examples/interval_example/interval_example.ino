#include <TimeOut.h>

Interval interval0; //start instance, instance have to be into global scope
Interval interval1;
Interval interval2;
Interval interval3;
Interval interval4;

// for functor as callback, type and number of member to be initialize do not mather
  //You could add constructor to initialize member, or use brace initializer like later in example
struct FunctorExample {
  FunctorExample(int someArguments) : arg(someArguments) {}
  void operator()() { 
    Serial.print("functor have been call and the pass argument increment is now: ");
    Serial.println(arg);
    arg++;
  }
  private:
  int arg;
};

Intereval_Example_Inheritance class1;

void setup() {
  Serial.begin(9600);
  interval0.interval(1000, intervalPrint0); //delay,callback function (calling interval could be inside any function)
  
  interval1.interval(3000, intervalPrint1);
  interval1.cancel();
  
  //interval2 will be triggered after 0 hour, 0 minutes and 15 seconde
  interval2.interval(0, 0, 15, intervalPrint2);

  interval3.interval<char*, int, String, float>(5000, callbackWithArguments, "this argument timer call after; ", 5000, " micro second ", 5.5 );

  interval4.interval( 4000, FunctorExample { 3 });
  
  


}

void loop() {
  Interval::handler(); //main monitor handler, Have to be into void loop, if using also timeOut, only one handler is needed for both
}

void intervalPrint0() {
  Serial.println("Interval 0 have been trigged");
}

void intervalPrint1() {
  Serial.println("Interval 1 have been trigged");
}

void intervalPrint2() {
  Serial.println("Interval 2 have been trigged");
}


void callbackWithArguments(char* msg1, int value1, String msg2, float value2) {
  Serial.print(msg1);
  Serial.print(value1);
  Serial.print(msg2);
  Serial.println(value2);
}