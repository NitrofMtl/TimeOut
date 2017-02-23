TimeOut Is a library to easily handle non blocking delay. It is store all instance into a container and sort them to save MCU time at each checkup.

Usage: To be use when «delay()» could have work but you need arduino to do other task between the timer start and is triggered.

Non recommended usage: 
	-For very time critical timing.
		It run into « void loop », so it is dependant of the speed of the loop. It could be a couple of millisecond offset. For time critical timing, use interrupt instead.
	-For short very short or very long delay :
		For very short delay, it take some time to sort the container when addin and when triggering a delay, so It could be better to use interrupt for realy small delay. For long delay, « millis() » is also not so accurate. There could be a better way. (No test have been 		done. If people want to do some testing for short and log delay, you could post your 			result on TimeOut github.)


TimeOut usage : Callback is triggered only once after being set, after the delay.
	Start an instance :
		TimeOut timeout0; (instance declaration have to be in global scope)

	Set the timeout :
		timeout0.timeOut(10000, callback0); //delay, function callback

	add the handler into void loop :
		timeout0.handler(); (you need only one handler for all instance of TimeOut. It have to be put in void loop();)

	cancel a timeOut :
		timeout0.cancel(); 

To set the timeout, there is 4 mode possible :
Normal
timeOut_Lock
timeOut_Undeleable
timeOut_Lock_Undelable


	Normal mode :
You can clear timeout with clear() and overwrite it with a new value .
Example :

timeout0.timeOut(10000, callback0); //set time out
delay(1000); //wait a seconde for demonstration
timeout0.timeOut(10000, callback0); //set new timeout time 10 seconde from now
timeout0.cancel();//time out have been deleted and callback will not append

timeOut_Lock :
You can't overwrite a timeout previously set. It will keep the delay to the first value until it is triggered.
But it can be deleted.
Example :

timeout0.timeOut(10000, callback0, timeOut_Lock);//set time out
delay(1000); //wait a seconde for demonstration
timeout0.timeOut(10000, callback0); //will not set because it is lock
timeout0.timeOut(10000, callback0, timeOut_Lock);//will not set because it is lock
timeout0.cancel();//time out have been deleted and callback will not append

timeOut_Undeleable :
You can overwrite a timeout like in normal mode but clear() will not work.
Example :
timeout0.timeOut(, callback0, timeOut_Undeleable);
timeout0.cancel(); //will be ignore and callback will be triggered after delay


timeOut_Lock_Undelable :
Use functionnality of both, timeOut_Lock and  Undelable. So until triggered, when set the first time, the callback will fire after the delay.


Interval : 
After being set, callback will be triggered at each delay interval.

Interval interval0; (Start an instance)

interval0.interval(1000,intervalPrint0);(set the interval, To change callback or delay, you must cancel() the interval first)

interval0.cancel();(cancel an interval)

interval0.handler();(you need only one handler for all instance of interval. It have to be put on void loop();)
	



