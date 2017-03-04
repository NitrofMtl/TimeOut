#include <TimeOut.h>

Interval interval0; //start instance, instance have to be into global scope
Interval interval1;

void setup() {
  Serial.begin(9600);
  interval0.interval(1000,intervalPrint0);//delay,callback function (calling interval could be inside any function)
  interval1.interval(3000,intervalPrint1);
}

void loop() {
  interval0.handler(); //main monitor handler, Have to be into void loop
}

void intervalPrint0() {
  Serial.println("Interval 0 have been trigged");
}

void intervalPrint1() {
  Serial.println("Interval 1 have been trigged");
}
