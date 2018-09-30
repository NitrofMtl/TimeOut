/*
V2.2
*/

#include <TimeOut.h>

TimeOut timeout0; //start instance, have to be inside global scope 
TimeOut timeout2;
TimeOut timeout3;
TimeOut timeout5;
TimeOut timeout6;
TimeOut timeOut7;

/*
    TIMEOUT_NORMAL       timer can be overwriten or cleared(same as omit it)
 
    TIMEOUT_UNDELETABLE      timer can not be cleared
/*********
===>TIMEOUT_LOCK_UNDELETABLE & TIMEOUT_LOCK_UNDELETABLE options have been remove because of bugs and limitations.
/*********
    time macro : sc(number_of_secondes) , mn(number_of_secondes) , hr(number_of_secondes)
*/

void setup() {
  Serial.begin(9600);
      //   calling timeOut could be inside any function -->
  timeout0.timeOut(10000, callback0); //delay, callback function
  //timeout2.timeOut(4500, callback2, TIMEOUT_LOCK_UNDELETABLE); //NOT WORKING ANYMORE
  //timeout2.timeOut(15000, callback2); 
  timeout3.timeOut(60000, callback3);
  timeout3.cancel();//will not append because canceled
  timeout5.timeOut(75000, callback5, TIMEOUT_UNDELETABLE);
  timeout5.cancel(); //timer will not be delete because it have been set to Undelable
  //timeout6.timeOut(9000, callback6, TIMEOUT_LOCK_UNDELETABLE);  //NOT WORKING ANYMORE
  // set timeOut in 1 hour, 33 minutes and 0 seconde
  timeOut7.timeOut(1, 33, 0, callback7, TIMEOUT_NORMAL);
}

void loop() {
  //monitor handler, Have to be into void loop
  //handler is static. you could call:
  TimeOut::handler();
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
  Serial.println("Timer3 will not append because canceled.");
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

void callback7() {
  Serial.println("Timer7 will execute after 1 hour, 33 minutes and 0 seconde.");
  Serial.println("");
}
