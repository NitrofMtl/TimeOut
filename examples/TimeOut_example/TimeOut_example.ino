#include <TimeOut.h>


//start instance, have to be inside global scope 
  //if instance are declare into a scope, leaving the scope will delete the instance.
  //You will not be able to cancel the timer anymore but it will be trigged.
TimeOut timeout0;
TimeOut timeout1;
TimeOut timeout2;
TimeOut timeout3;
TimeOut timeout4;
//TimeOut timeout6;

void callback0();
void callback1();
void callback2();
void callback3();
void callback4();

/*
    TIMEOUT::NORMAL       timer can be overwriten or cleared(same as omit it)
 
    TIMEOUT::UNDELETABLE      timer can not be cleared
/*********

    time macro : sc(number_of_secondes) , mn(number_of_secondes) , hr(number_of_secondes)
*/

void setup() {
  Serial.begin(9600);
      //   calling timeOut could be inside any function -->
  TimeOut(45000, callback5); // call timer anonymously, this cannot be canceled
  timeout0.timeOut(10000, callback0); //delay, callback function
  //timeout2.timeOut(4500, callback2, TIMEOUT_LOCK_UNDELETABLE); //NOT WORKING ANYMORE
  timeout1.timeOut(15000, callback1); 
  timeout2.timeOut(60000, callback2);
  timeout2.cancel();//will not append because canceled
  timeout3.timeOut(75000, callback3, TIMEOUT::UNDELETABLE);
  timeout3.cancel(); //timer will not be delete because it have been set to Undelable
  
  // set timeOut in 1 hour, 33 minutes and 0 seconde
  timeout4.timeOut(1, 33, 0, callback4, TIMEOUT::NORMAL);

  //  ----> removed
  //timeout6.timeOut<char*, int, String, float>(5000, callbackWithArguments, "this argument timer call after; ", 5000, " micro second ", 5.5 );


}

void loop() {
  //monitor handler, Have to be into void loop
  //handler is static. you could call:
  TimeOut::handler();
}

void callback0() {
  Serial.println("Timer0 have been trigged after 10 seconde.");
  Serial.println("");
}

void callback1() {
  Serial.println("Timer1 have been trigged after 15 sec.");
  Serial.println("");
}

void callback2() {
  Serial.println("Timer2 will not append because canceled.");
  Serial.println("");
}


void callback3() {
  Serial.println("Timer3 have been trigger after 1 minute and 15 sec");
  Serial.println("");
}

void callback4() {
  Serial.println("Timer4 will execute after 1 hour, 33 minutes and 0 seconde.");
  Serial.println("");
}

void callback5() {
  Serial.println("Anonymous timer was after after 45 second.");
  Serial.println("");
}


/*   ---> removed
void callbackWithArguments(char* msg1, int value1, String msg2, float value2) {
  Serial.print(msg1);
  Serial.print(value1);
  Serial.print(msg2);
  Serial.println(value2);
}
*/