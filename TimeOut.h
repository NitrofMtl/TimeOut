/*
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

#ifndef TimeOut_h
#define TimeOut_h

#if (ARDUINO >= 100)
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#define timeOut_Normal 0			//timer can be overwriten or cleared
#define timeOut_Lock 1				//timer cannot be overwriten
#define timeOut_Undeleable 2 		//timer cannot be cleared
#define timeOut_Lock_Undelable 3	//timer cannot be overwriten or cleared

#ifndef sc(x)
#define sc(x)(x*1000UL)
#endif 

#ifndef mn(x)
#define mn(x)(x*60000UL)
#endif

#ifndef hr(x)
#define hr(x)(x*3600000UL)
#endif

#define sizeOfTimeOut 20

class TimeOut {
public:
	TimeOut();
	TimeOut(unsigned long _delay, void (*_callback)());
	bool timeOut(unsigned long _delay, void (*_callback)());
	bool timeOut(unsigned long _delay, void (*_callback)(), uint8_t _timerType);
	void cancel();
	static bool handler();
	void printContainer();
protected:
	bool timeOut(unsigned long _delay, TimeOut* ptr, uint8_t _timerType);
private:
	void (*callback)();
	unsigned long delay = 0;
	unsigned long timeStamp = 0;
	void triage(TimeOut *input);
	bool lock = false;
	bool undeletable = false;
	bool isOnHeap = false;
	virtual void TO_callbackCaller();//enable inheritance support overwrite this function inderived class
	static TimeOut *timerList[sizeOfTimeOut];
};


#define sizeOfInterval 10

class Interval {
public:
	bool interval(unsigned long _delay, void (*_callback)());	
	void cancel();
	static bool handler();
	void printContainer();
protected:
	bool interval(unsigned long _delay, Interval* ptr);
private:
	void (*callback)();
	unsigned long delay = 0;
	unsigned long timeStamp = 0;
	static void triage(Interval *input);
	static void triage_handler();
	bool set = false;
	virtual void ITV_callbackCaller();//enable inheritance support overwrite this function inderived class
	static Interval *intervalList[sizeOfInterval];
};


#endif

