#include <TimeOut.h>

Interval interval0; //start instance, instance have to be into global scope
Interval interval1;
Interval interval2;
Interval interval3;

class Intereval_Example_Inheritance : public Interval {
  private:
    //this is the function that will be call by Interval class, it HAVE to be this exact name !!!
    void TO_callbackCaller() {
      Serial.println("Class ineheritance callback have been triggered ");
    }
  public:
    //let the callback parameter NULL to call inheritance method
    void setInterval(unsigned long delay) {
      interval(delay, NULL);
    }//let the callback parameter NULL to call inheritance method
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
  
  class1.setInterval(4000);

  Interval::printContainer(Serial);
}

void loop() {
  Interval::handler(); //main monitor handler, Have to be into void loop
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