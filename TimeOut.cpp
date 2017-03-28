/*

	V1.0
  Copyright (c) 22/02/2017

    By Nitrof

  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/  

#if (ARDUINO >= 100)
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#include "TimeOut.h"

TimeOut *TimeOut::timerList[sizeOfTimeOut];

TimeOut::TimeOut(){

}

TimeOut::TimeOut(unsigned long _delay, void (*_callback)()){
	TimeOut *onHeap = new TimeOut();
	for ( int i = 0; i < sizeOfTimeOut; i++){
		if (!timerList[i]){ //check for first emty spot								
			onHeap->timeStamp = millis();
			onHeap->callback = _callback;
			onHeap->delay = _delay;
			onHeap->isOnHeap = true;
			triage(onHeap); //place the instance into container
			return;
		}
		if(sizeOfTimeOut-1 == i) {
			Serial.println("TimeOut container is full !!! Can't add a new timer.");
			delete onHeap; //delete timer if container already full
		}
	}
}

bool TimeOut::timeOut(unsigned long _delay, void (*_callback)()){
	if (lock) return false; //do not set a timer to a locked instance
	for ( int i = 0; i < sizeOfTimeOut; i++){
		if (!timerList[i]){ //check for first emty spot								
			timeStamp = millis();
			callback = _callback;
			delay = _delay;
			triage(this); //place the instance into container
			return true;
		}
		if(sizeOfTimeOut-1 == i) {
			Serial.println("TimeOut container is full !!! Can't add a new timer.");
			return false;
		}
	}
}


bool TimeOut::timeOut(unsigned long _delay, void (*_callback)(), uint8_t _timerType){
	if(!timeOut(_delay, _callback)) return false; //if timer is full bool
	switch (_timerType) {
		case 0: break;
		case timeOut_Lock: 
			if (lock) return false ; //do not set a timer to a locked instance
			lock = true; 
			break;
		case timeOut_Undeleable: 
			undeletable = true; 
			break;
		case timeOut_Lock_Undelable:
			if (lock) return false ; //do not set a timer to a locked instance
			lock = true;
			undeletable = true; 
			break;
	}
	return true;
}

bool TimeOut::timeOut(unsigned long _delay, TimeOut* ptr, uint8_t _timerType){
	if (lock) return false; //do not set a timer to a locked instance
	for ( int i = 0; i < sizeOfTimeOut; i++){
		if (!timerList[i]){ //check for first emty spot								
			timeStamp = millis();
			delay = _delay;
			triage(this); //place the instance into container
			return true;
		}
		if(sizeOfTimeOut-1 == i) {
			Serial.println("TimeOut container is full !!! Can't add a new timer.");
			return false;
		}
	}
	switch (_timerType) {
		case 0: break;
		case timeOut_Lock: 
			if (lock) return false ; //do not set a timer to a locked instance
			lock = true; 
			break;
		case timeOut_Undeleable: 
			undeletable = true; 
			break;
		case timeOut_Lock_Undelable:
			if (lock) return false ; //do not set a timer to a locked instance
			lock = true;
			undeletable = true; 
			break;
	}
	return true;
}

void TimeOut::cancel(){//function to split ???
	if (undeletable) return; //do not cancel a timer if Undeleable
	int rank = 0;
	while (timerList[rank] != this){
		rank++;
		if(rank >= sizeOfTimeOut) {
			Serial.println("TimeOut instance not found ");
			return;
		}
	}
	for (int i = rank; i < sizeOfTimeOut-2; i++){
		timerList[i] = timerList[i+1];
	}
	timerList[sizeOfTimeOut-1] = NULL;
}

bool TimeOut::handler(){
	if(!timerList[0]) return false; // if there is no timer set, do nothing
	unsigned long now = millis();
	if(now - timerList[0]->timeStamp > timerList[0]->delay){
		timerList[0]->TO_callbackCaller();
		timerList[0]->lock = false;
		timerList[0]->undeletable = false;
		if (timerList[0]->isOnHeap) delete timerList[0]; //delete dynamic timer
		for (int i = 0; i < sizeOfTimeOut-1; i++){//rollback all pointer to beginning
			timerList[i] = timerList[i+1];
		}
		timerList[sizeOfTimeOut-1] =  NULL; //set last spot emty
		return true;
	}
	else  {
		return false;
	}	
}

void TimeOut::TO_callbackCaller(){
	if(timerList[0]->callback) callback();
}

void TimeOut::triage(TimeOut *input){	//sort timer by time remainin to be triggered
	uint8_t rank = 0;
	unsigned long now = millis();
	for (int i = 0; i < sizeOfTimeOut; i++){
		if(!timerList[i]) {
			rank = i;
			break;
		}
		long containerRemain = timerList[i]->timeStamp+timerList[i]->delay-now;
		long inputRemain = input->timeStamp+input->delay-now;
		if (containerRemain > inputRemain) { //spot with timer a smaler than this
			rank = i;
			break;
		}
	}
	for (int i = sizeOfTimeOut-1; i > rank; i--){ //push back from the end each bigger delay
		timerList[i] = timerList[i-1];	
	}
	timerList[rank] = input;
}

void TimeOut::printContainer(){
	Serial.println("Timer container contain the following timer: ");
	for (int i = 0; i < sizeOfTimeOut; i++){
		if(NULL == timerList[i]) {
			Serial.print("Container ");
			Serial.print(i);
			Serial.println(" empty");
		}
		else {
			Serial.print("timer ");
			Serial.print(i);
			Serial.print(" delay is ");
			Serial.println(timerList[i]->delay);
		}		
	}
}



Interval *Interval::intervalList[sizeOfInterval];

bool Interval::interval(unsigned long _delay, void (*_callback)()){
	if (set) return false; //can't set an already set interval
	if (intervalList[sizeOfInterval-1]){
		Serial.println("Interval container is full !!! Can't add a new interval timer.");
		return false; //return false if container is full
	}
	timeStamp = millis();
	callback = _callback;
	delay = _delay;
	set = true;
	triage(this); //place the instance into container
	return true;
}

bool Interval::interval(unsigned long _delay, Interval* ptr){
	if (set) return false; //can't set an already set interval
	if (intervalList[sizeOfInterval-1]){
		Serial.println("Interval container is full !!! Can't add a new interval timer.");
		return false; //return false if container is full
	}
	timeStamp = millis();
	delay = _delay;
	set = true;
	triage(this); //place the instance into container
	return true;
}

void Interval::cancel(){
	int rank = 0;
	while (intervalList[rank] != this){
		rank++;
		if(rank >= sizeOfInterval) {
			Serial.println("Interval instance not found ");
			return;
		}
	}
	set = false;
	for (int i = rank; i < sizeOfInterval-1; i++){
		intervalList[i] = intervalList[i+1];
	}
	intervalList[sizeOfInterval-1] = NULL;
}

bool Interval::handler(){
	if(!intervalList[0]) return false; // if there is no timer set, do nothing
	unsigned long now = millis();
	int elapsed = now - intervalList[0]->timeStamp;
	if(elapsed > intervalList[0]->delay){
		intervalList[0]->timeStamp = now;
		intervalList[0]->ITV_callbackCaller();		
		triage_handler();
		return true;
	}
	else  {
		return false;
	}	
}

void Interval::ITV_callbackCaller(){
	if(intervalList[0]->callback) intervalList[0]->callback();
}

void Interval::printContainer(){
	Serial.println("Interval container contain the following timer: ");
	unsigned long now = millis();	
	for (int i = 0; i < sizeOfInterval; i++){

		if(!intervalList[i]) {
			Serial.print("Container ");
			Serial.print(i);
			Serial.println(" empty");
		}
		else {
			Serial.print("timer ");
			Serial.print(i);
			Serial.print(" interval is ");
			Serial.print(intervalList[i]->delay);
			Serial.print(", time stamp:   ");
			Serial.print(intervalList[i]->timeStamp);
			Serial.print(", remain ");
			int remain = intervalList[i]->timeStamp+intervalList[i]->delay-now;
			if(remain<0) remain = 0;
			Serial.println(remain );
		}	
	}
	Serial.println();
}



void Interval::triage_handler(){
	Interval *trigged = intervalList[0]; //save trigged instance
	for (int i = 0; i < sizeOfInterval-1; i++){ //shift other to the beginnig
		intervalList[i] = intervalList[i+1];
	}
	intervalList[sizeOfInterval-1] = NULL;
	triage(trigged); //add saved trigged instance at the right position
}


void Interval::triage(Interval *input){ //sort timer by time to be trigged
	uint8_t rank = 0;
	unsigned long now = millis();
	for (int i = 0; i < sizeOfInterval; i++){
		if(!intervalList[i]) {//if container was emty, take the first spot
			rank = i;			
			break;
		}
		long container = intervalList[i]->timeStamp+intervalList[i]->delay-now;
		long input_ = input->timeStamp+input->delay-now;
		if(container>input_){
		rank = i;
		break;
		}
	}
	for (int i = sizeOfInterval-1; i > rank; i--){ //push back from the end each bigger delay
		intervalList[i] = intervalList[i-1];		
	}
	intervalList[rank] = input;
}