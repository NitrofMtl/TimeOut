/*

	V2.0
  Copyright (c) 4/04/2017

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

TimeOutNodePtr TimeOut::head = NULL;

void TimeOutNode::callbackTrigger() {
	if(callback)callback(); 
	else linkedTO->TO_callbackCaller();
}

TimeOut::TimeOut(){}

TimeOut::TimeOut(unsigned long _delay, void (*_callback)()) {
	timeOut(_delay,_callback);
}


bool TimeOut::timeOut(unsigned long _delay, void (*_callback)()){
	if (node) {
		if(node->lock) return false; //exit if node is not overwritable
		else cancel();//delete a set timeOut for overwrite
	}	
	node = new TimeOutNode;		
	node->callback = _callback;
	node->delay = _delay;
	node->linkedTO = this;
	node->timeStamp = millis();
	triage(node); //place the instance into container
	return true;
}


bool TimeOut::timeOut(unsigned long _delay, void (*_callback)(), uint8_t _timerType){
	if(!timeOut(_delay, _callback)) return false; //if timer can't be set
	switch (_timerType) {
		case 0: break;
		case TIMEOUT_LOCK: 
			if (node->lock) return false ; //do not set a timer to a locked instance
			node->lock = true; 
			break;
		case TIMEOUT_UNDELETABLE: 
			node->undeletable = true; 
			break;
		case TIMEOUT_LOCK_UNDELETABLE:
			if (node->lock) return false ; //do not set a timer to a locked instance
			node->lock = true;
			node->undeletable = true; 
			break;
	}
	return true;
}


void TimeOut::cancel(){
	if (node->undeletable) return; //do not cancel a timer if Undeleable
	TimeOutNodePtr tmpNode = TimeOut::head;
	TimeOutNodePtr previous = NULL;
	while(this->node!=tmpNode) {
		previous = tmpNode;
		tmpNode=tmpNode->next;
	}
	if(!previous) TimeOut::head = tmpNode->next;
	else previous->next =  tmpNode->next;
	delete tmpNode;
}


bool TimeOut::handler(){
	if(!TimeOut::head) return false; // if there is no timer set, do nothing
	unsigned long now = millis();
	if (now - TimeOut::head->timeStamp > TimeOut::head->delay){
		TimeOut::head->callbackTrigger();
		TimeOut::head->lock = false;
		TimeOut::head->undeletable = false;
		TimeOutNodePtr temp = TimeOut::head;		
		if(TimeOut::head->next)
			TimeOut::head = TimeOut::head->next; //switch to next timer
		else 
			TimeOut::head = NULL;
		delete temp; //delete triggered timer
	}
	else {
		return false;
	}
}

void TimeOut::triage(TimeOutNodePtr current){	//sort timer by time remainin to be triggered
	if (!TimeOut::head) { //if list is emty, set it to first
		TimeOut::head = current;
		return;
	}
	unsigned long now = millis();

	TimeOutNodePtr it = TimeOut::head;
	TimeOutNodePtr previous = NULL;

	while (it){
		long itRemain = it->timeStamp+it->delay-now;
		long currentRemain = current->timeStamp+current->delay-now;
		if (itRemain > currentRemain) { //spot with timer a smaler than this
			//insert instance
			if(previous) previous->next = current;
			else TimeOut::head = current;
			current->next = it;
			return;
		}
		else if (!it->next) {//if delay is the bigger, place it at the end
			it->next = current;
			return;
		}	
		previous = it;
		it = it->next;	//it++	
	}
}

void TimeOut::printContainer(){
	Serial.println("Timer container contain the following timer: ");
	unsigned long now = millis();
	TimeOutNodePtr it = TimeOut::head;
	while(it){	
		if(!TimeOut::head){
			Serial.println("No timer set");
			return;
		}
		Serial.print("Container delay ");
		Serial.print(it->delay);
		Serial.print(" remain: ");
		Serial.println(it->timeStamp+it->delay-now);
		it = it->next;
	}
	Serial.println("End.");
	Serial.println();
}



intervalNodePtr Interval::head = NULL;

void intervalNode::callbackTrigger() {
	if(callback)callback(); 
	else {
		Serial.println("callbackCaller trig");
		linkedInterv->ITV_callbackCaller();
	}
}


bool Interval::interval(unsigned long _delay, void (*_callback)()){
	if (node) return false; //can't set an already set interval
	node = new intervalNode;
	node->timeStamp = millis();
	node->callback = _callback;
	node->delay = _delay;
	node->linkedInterv = this;
	triage(node); //place the instance into container
	return true;
}

void Interval::cancel(){
	intervalNodePtr tmp = Interval::head;
	intervalNodePtr previous = NULL;
	while(this->node!=tmp){
		previous = tmp;
		tmp = tmp->next;
	}
	if(!previous) Interval::head = tmp->next;
	else previous->next = tmp->next;
	delete tmp;
}

bool Interval::handler(){
	if(!Interval::head) return false;
	unsigned long now = millis();
	if (now - Interval::head->timeStamp > Interval::head->delay){
		Interval::head->callbackTrigger();
		intervalNodePtr temp = Interval::head;	
		if(Interval::head->next)
			Interval::head = Interval::head->next; //switch to next timer
		else 
			Interval::head = NULL;
			//reset triggered instance
		temp->timeStamp = now;
		temp->next = NULL;
		triage(temp);
	}
	else {
		return false;
	}
}

void Interval::printContainer(){
	Serial.println("Interval container contain the following timer: ");
	if(!Interval::head) Serial.println("Container emty! ");
	unsigned long now = millis();
	intervalNodePtr it = Interval::head;
	while(it){	
		if(!Interval::head){
			Serial.println("No timer set");
			return;
		}
		Serial.print("Container delay ");
		Serial.print(it->delay);
		Serial.print(" remain: ");
		Serial.println(it->timeStamp+it->delay-now);
		it = it->next;
	}
	Serial.println("End.");
	Serial.println();
}

void Interval::triage(intervalNodePtr current){ //sort timer by time to be trigged
	if (!Interval::head) {
		Interval::head = current;
		return;
	}
	unsigned long now = millis();
	intervalNodePtr it = Interval::head;
	intervalNodePtr previous = NULL;
	while(it){
		long itRemain = it->timeStamp+it->delay-now;
		long currentRemain = current->timeStamp+current->delay-now;
		if (itRemain > currentRemain) { //spot with timer a smaler than this
			//insert instance
			if(previous) previous->next = current;
			else Interval::head = current;
			current->next = it;
			return;
		}
		else if (!it->next) {//if delay is the bigger, place it at the end
			it->next = current;
			return;
		}	
		previous = it;
		it = it->next;	//it++	
	}
}