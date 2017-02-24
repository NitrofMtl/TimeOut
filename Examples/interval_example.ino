#include <TimeOut.h>

Interval interval0; //start instance
Interval interval1;

void setup() {
  Serial.begin(9600);
  interval0.interval(1000,intervalPrint0);//delay,callback function
  interval1.interval(3000,intervalPrint1);
}

void loop() {
  interval0.handler(); //main monitor handler
}

void intervalPrint0() {
  Serial.println("Interval 0 have been trigged");
}

void intervalPrint1() {
  Serial.println("Interval 1 have been trigged");
}
