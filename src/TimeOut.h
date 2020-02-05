/*

	V3.0
  Copyright (c) 2/04/2020

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

#ifndef TimeOut_h
#define TimeOut_h

#if (ARDUINO >= 100)
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#include "microTuple.h"

#ifndef sc
#define sc(x)(x*1000UL)
#endif /* sc */

#ifndef mn
#define mn(x)(x*60000UL)
#endif /* mn */

#ifndef hr
#define hr(x)(x*3600000UL)
#endif /* hr */


enum class TIMEOUT { NORMAL, UNDELETABLE, INTERVAL };  // timer can be overwriten or cleared , timer cannot be cleared


class TimeOut;


class TimeOutNode {
protected:
	TimeOutNode();
	virtual ~TimeOutNode() = default;
	friend class TimeOut;
	friend class Interval;
	void (*callback)();
	unsigned long delay;
	unsigned long timeStamp;
	TIMEOUT type;	
	TimeOutNode *next;
	TimeOut *linkedTO; //bound timeOut instance*/
	virtual void callbackTrigger();
};


template <typename ... Args>
class TimeOutNodeArgs : public TimeOutNode {
	TimeOutNodeArgs<Args...>(Args ... ts, void (*cb)(Args ..._args) ) : args(ts...), pack(ParamsPack<Args...> { args, cb } ){};
	friend class TimeOut;
	MicroTuple<Args...> args;
	ParamsPack<Args...> pack;
	void callbackTrigger() { if(pack) pack.getPack();};
};


class TimeOut {
public:
	TimeOut();
	TimeOut(unsigned long _delay, void (*_callback)());
	TimeOut(uint8_t hour, uint8_t minute, uint8_t seconde, void (*_callback)());
	template<typename ... Args>
	TimeOut(unsigned long _delay, void (*_callback)(Args ... args), Args ... args) { timeout(_delay, _callback, args...); };

protected:
	TimeOutNode *node = NULL;

private:	
	static TimeOutNode *head;
	static void sort(TimeOutNode *current){	//sort timer by time remainin to be triggered
		if (!TimeOut::head) { //if list is emty, set it to first
			TimeOut::head = current;
			return;
		}
		unsigned long now = millis();

		TimeOutNode *it = TimeOut::head;
		TimeOutNode *previous = NULL;

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
	};

public:	
	void timeOut(unsigned long _delay, void (*_callback)());
	void timeOut(unsigned long _delay, void (*_callback)(), TIMEOUT _timerType);
	void timeOut(uint8_t hour, uint8_t minute, uint8_t seconde, void (*_callback)(), TIMEOUT _timerType);

	template<typename ... Args>
	void timeOut(unsigned long _delay, void (*_callback)(Args ... args), Args ... args){
		TimeOutNodeArgs<Args...> *derivedNode =  new TimeOutNodeArgs<Args...>(args..., _callback) ;
		derivedNode->delay = _delay;
		derivedNode->timeStamp = millis();
		sort(derivedNode);
	};

	void cancel();
	static bool handler();
	static void printContainer(HardwareSerial& stream);
	//enable inheritance support overwrite this function in derived class
	virtual void TO_callbackCaller(){};
	
};


class Interval : public TimeOut {
public:
	bool interval(unsigned long _delay, void (*_callback)());
	bool interval(uint8_t hour, uint8_t minute, uint8_t seconde, void (*_callback)());	

	template<typename ... Args>
	void interval(unsigned long _delay, void (*_callback)(Args ... args), Args ... args){
		timeOut(_delay, _callback, args...);
		node->type = TIMEOUT::INTERVAL;
	};
};

#endif
