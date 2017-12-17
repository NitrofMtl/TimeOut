/*
V2.2
*/

#include <TimeOut.h>



void setup() {
  Serial.begin(9600);
  example_function();
  

}

void loop() {
  TimeOut::handler();

}

void example_function(){ //ramdom function that set a timeout
  Serial.println("boot");
  TimeOut test(10000,callback);
  // note that by adding dynamikly a timeout you can't cancel or overwriten it because its reference pointer will be deleted after exiting the function 
    //ATTENTION : each time dynamic TimeOut is call create a new instance and will not be deleted until trigered, be carefull that the function call do not append to often !!!
}

void callback(){
  Serial.println("timeout triggered");
  }
