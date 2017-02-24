#include <TimeOut.h>

TimeOut timeout0;
TimeOut timeout2;
TimeOut timeout3;
TimeOut timeout5;
TimeOut timeout6;

/*
    timeOut_Lock            timer can ont be overwriten
    timeOut_Undeleable      timer can not be cleared
    timeOut_Lock_Undelable  timer can not be overwriten or cleared

    time macro : sc(number_of_secondes) , mn(number_of_secondes) , hr(number_of_secondes)
*/

void setup() {
  Serial.begin(9600);
  
  timeout0.timeOut(10000, callback0); //delay, callback function
  timeout2.timeOut(4500, callback2, timeOut_Lock); //delay, callback function, parameter
  timeout2.timeOut(15000), callback2); //will not change timeout2 because it was lock.
  timeout3.timeOut(60000, callback3);
  timeout5.timeOut(75000, callback5, timeOut_Undeleable);
  timeout5.cancel(); //timer will not be delete because it have been set to Undelable
  timeout6.timeOut(9000, callback6, timeOut_Lock_Undelable);
}

void loop() {
  timeout0.handler(); //monitor handler
}

void callback0() {
  Serial.println("Timer0 have been triggedafter 10 seconde.");
  Serial.println("");
}

void callback2() {
  Serial.println("Timer2 have been trigged after 4,5 sec.");
  Serial.println("");
}

void callback3() {
  Serial.println("Timer3 have been trigged after 1 minute.");
  Serial.println("");
}


void callback5() {
  Serial.println("Timer5 have been trigger after 1 minute and 15 sec");
  Serial.println("");
}

void callback6() {
  Serial.println("Timer6 will execute after 9 sec.");
  Serial.println("");
}

