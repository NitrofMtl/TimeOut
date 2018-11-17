/*

	V2.2
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

#ifndef TimeOut_h
#define TimeOut_h

#if (ARDUINO >= 100)
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

//#include <SoftwareSerial.h>


#define TIMEOUT_NORMAL 0			//timer can be overwriten or cleared
//#define TIMEOUT_LOCK 1	//removed			//timer cannot be overwriten
#define TIMEOUT_UNDELETABLE 2 		//timer cannot be cleared
//#define TIMEOUT_LOCK_UNDELETABLE 3	//removed  //timer cannot be overwriten or cleared

#ifndef sc
#define sc(x)(x*1000UL)
#endif /* sc */

#ifndef mn
#define mn(x)(x*60000UL)
#endif /* mn */

#ifndef hr
#define hr(x)(x*3600000UL)
#endif /* hr */


class TimeOut;

typedef class TimeOutNode {
	TimeOutNode();
	friend class TimeOut;
	void (*callback)();
	unsigned long delay;
	unsigned long timeStamp;
	bool lock;
	bool undeletable;	
	TimeOutNode *next;
	TimeOut *linkedTO; //bound timeOut instance*/
	void callbackTrigger();
}*TimeOutNodePtr;

class TimeOut {
public:
	TimeOut();
	TimeOut(unsigned long _delay, void (*_callback)());
	TimeOut(uint8_t hour, uint8_t minute, uint8_t seconde, void (*_callback)());
	bool timeOut(unsigned long _delay, void (*_callback)());
	bool timeOut(unsigned long _delay, void (*_callback)(), uint8_t _timerType);
	bool timeOut(uint8_t hour, uint8_t minute, uint8_t seconde, void (*_callback)(), uint8_t _timerType);
	void cancel();
	static bool handler();
	static void printContainer(HardwareSerial& stream);
	//enable inheritance support overwrite this function in derived class
	virtual void TO_callbackCaller(){};

private:
	static TimeOutNodePtr head;
	TimeOutNodePtr node = NULL;
	inline void triage(TimeOutNodePtr current);
};



class Interval;

typedef class intervalNode {
	friend class Interval;
	void (*callback)();
	unsigned long delay = 0;
	unsigned long timeStamp = 0;
	bool set = false;

	intervalNode *next = NULL;
	Interval *linkedInterv = NULL;
	void callbackTrigger();
} *intervalNodePtr;


class Interval {
public:
	bool interval(unsigned long _delay, void (*_callback)());
	bool interval(uint8_t hour, uint8_t minute, uint8_t seconde, void (*_callback)());	
	void cancel();
	static bool handler();
	static void printContainer(HardwareSerial& stream);

	virtual void ITV_callbackCaller(){};//enable inheritance support overwrite this function inderived class
private:
	static intervalNodePtr head;
	intervalNodePtr node = NULL;
	static inline void triage(intervalNodePtr current);
	Interval *linkedInterv = NULL;
	uint8_t _num;
};

#endif
