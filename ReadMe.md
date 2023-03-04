# TimeOut 

Current Version: V5.1

## Description

TimeOut is an Arduino library to easily handle non-blocking delays. It is able to store all Timeout instances into a container and sort them to save MCU time at each cycle.

## Recommended usage

This library is designed to be used when the Arduino «delay()» could have worked, but you need to be able to do other tasks between the timer's start and it's triggered time.

Non recommended usage: 
	-For very  precise and/or time-critical uses.
		It run into « void loop », so it is dependent of the speed of the loop. It could be a couple of millisecond offset. For time critical timing, use interrupt instead.
	-For short very short or very long delay :
		For very short delay, it take some time to sort the container when adding and when triggering a delay, so It could be better to use interrupt for really small delay. For long delay, « millis() » is also not so accurate. There could be a better way. (No test have been done. If people want to do some testing for short and log delay, you could post your result on TimeOut github.)

## Version History
Please see [this page](/version_history.md).

## Example use

Please see the [/examples/](/examples/) folder for working examples.

### Timeout

TimeOut usage : Callback is triggered only once after being set, after the delay.
* Start an instance :
```
TimeOut timeout0; (instance declaration have to be in global scope)
```
* Set the timeout :
```
timeout0.timeOut(10000, callback0); //delay, function callback, could be call inside any function.
Call it more then once add a new callback in the queue, it do not overwrite. To do so, programmer must delete it first.
```
* add the handler into void loop :
```
timeout0.handler(); (you need only one handler for all instance of TimeOut. It have to be put in void loop();)
```
* cancel a timeOut :
```
timeout0.cancel(); 
```
If more  then one call is the queue list, the timer will be cancelled FIFO.

To set the timeout, there is 2 modes possible :
~~TIMEOUT_NORMAL~~
~~TIMEOUT_UNDELETABLE~~
## New syntax since v3.0
```
TIMEOUT::NORMAL 			//timer can be overwriten or cleared, you can just omit it, it have overload
TIMEOUT::UNDELETABLE  		//timer cannot be cleared
```
===> REMOVED!

~~TIMEOUT_LOCK 				//timer cannot be overwriten~~

~~TIMEOUT_LOCK_UNDELETABLE 	//timer cannot be cleared or overwriten~~
* Normal mode :
You can clear timeout with `<cancel()>` and overwrite it with a new value .
Example :

```
timeout0.timeOut(10000, callback0); //set time out
delay(1000); //wait a seconde for demonstration
timeout0.timeOut(10000, callback0); //set new timeout time 10 seconde from now
timeout0.cancel();//timeOut have been deleted and callback will not append
```

  TIMEOUT_UNDELETABLE :
You can overwrite a timeout like in normal mode but clear() will not work.
Example :

``
timeout0.timeOut(, callback0, TIMEOUT_UNDELETABLE);
timeout0.cancel(); //will be ignore and callback will be triggered after delay
``


### Interval

After being set, a callback will be triggered at EACH delay interval.

```
Interval interval0; (Start an instance)

interval0.interval(1000,intervalPrint0);(set the interval, To change callback or delay, you must cancel() the interval first, could be call inside any function)

interval0.cancel();(cancel an interval, could be call inside any function)

interval0.handler();(you need only one handler for all instance of interval. It have to be put on void loop();)
```

#### HINT: 
Time macro supporting second, minutes and hour:
* `sc(int)`, return int*1000 for a second
* `mn(int)`, return int*60000 for a minute
* `hr(int)`, return int*3600000 for an hour

For example, for a delay you could do:

```
unsigned long someDelay = hr(1)+mn(30)+sc(20) //will do 1:30 and 20 second delay
timeout0.timeOut(someDelay, callback0);	
```

**Please Note:**
The time macro do not support floating point! You can't do: `hr(1.5)`



