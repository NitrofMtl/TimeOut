#include "TimeOut.h"


TimeOutNode::TimeOutNode() :
	delay(0),
	timeStamp(0),
	type(TIMEOUT::NORMAL),	
	next(NULL),
	linkedTO(NULL) //bound timeOut instance
{}


TimeOutNode *TimeOut::head = NULL;


void TimeOutNode::callbackTrigger() {
	if(callback)callback(); 
	else {
		linkedTO->TO_callbackCaller();
	}
}


TimeOut::TimeOut(){}


TimeOut::TimeOut(unsigned long _delay, void (*_callback)()) {
	timeOut(_delay,_callback);
}


TimeOut::TimeOut(uint8_t hour, uint8_t minute, uint8_t seconde, void (*_callback)()){
	unsigned long delay = hr(hour) + mn(minute) + sc(seconde);
	timeOut(delay, _callback);
}


void TimeOut::timeOut(unsigned long _delay, void (*_callback)()){
	node = new TimeOutNode;		
	node->callback = _callback;
	node->delay = _delay;
	node->linkedTO = this;
	node->timeStamp = millis();
	sort(node); //place the instance into container
}


void TimeOut::timeOut(unsigned long _delay, void (*_callback)(), TIMEOUT _timerType){
	timeOut(_delay, _callback);
	if ( TIMEOUT::UNDELETABLE == _timerType ) node->type = TIMEOUT::UNDELETABLE;
	else node->type = TIMEOUT::NORMAL;
}


void TimeOut::timeOut(uint8_t hour, uint8_t minute, uint8_t seconde, void (*_callback)(), TIMEOUT _timerType){
	unsigned long delay = hr(hour) + mn(minute) + sc(seconde);
	timeOut(delay, _callback, _timerType);
}


void TimeOut::cancel(){
	if (TIMEOUT::UNDELETABLE == node->type) return; //do not cancel a timer if Undeleable
	TimeOutNode *tmpNode = TimeOut::head;
	TimeOutNode *previous = NULL;
		while(this->node!=tmpNode) {
			if (!tmpNode->next) return; //break loop if no instace correspond
			previous = tmpNode;
			tmpNode=tmpNode->next;
		}
		if(!previous) TimeOut::head = tmpNode->next;
		else previous->next =  tmpNode->next;
		node = NULL;
		delete tmpNode;
}


bool TimeOut::handler(){
	if(!TimeOut::head) return false; // if there is no timer set, do nothing
	unsigned long now = millis();
	if (now - TimeOut::head->timeStamp > TimeOut::head->delay){
		
		TimeOut::head->callbackTrigger();
		TimeOutNode *temp = TimeOut::head;		
		if(TimeOut::head->next)
			TimeOut::head = TimeOut::head->next; //switch to next timer
		else 
			TimeOut::head = NULL;
		if (TIMEOUT::INTERVAL == temp->type) {
				//reset triggered interval instance
			temp->timeStamp = now;
			temp->next = NULL;
			sort(temp);
		}
		else
			delete temp; //delete triggered timeout instance
	}
	else {
		return false;
	}
}


void TimeOut::printContainer(HardwareSerial& stream){
	stream.println("Timer container contain the following timer: ");
	unsigned long now = millis();
	TimeOutNode *it = TimeOut::head;
	while(it){	
		if(!TimeOut::head){
			stream.println("No timer set");
			return;
		}
		stream.print("Container delay ");
		stream.print(it->delay);
		stream.print(" remain: ");
		stream.println(it->timeStamp+it->delay-now);
		it = it->next;
	}
	stream.println("End.");
	stream.println();
}


bool Interval::interval(unsigned long _delay, void (*_callback)()){
	timeOut(_delay, _callback);
	node->type = TIMEOUT::INTERVAL;
}


bool Interval::interval(uint8_t hour, uint8_t minute, uint8_t seconde, void (*_callback)()){
	unsigned long delay = hr(hour) + mn(minute) + sc(seconde);
	interval(delay, _callback);
}
