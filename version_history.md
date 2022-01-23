### **New on version 5.0**
* Remove TimeOut with argument function and remove MicroTuple dependency
* Remove print container


### **New on version 4.0**
* change function pointer to template, enableling to use of functor and lambdas
* Remove virtual TO_callbackTrigguer function

#### minor internal change
* deprecate print container
* optimise node constructor inititalization
* make macros sec min and hour to millisecond constant expression
* change pointer to NULL to nullptr
* add head to nullptr gard on cancel
* change pointer to NULL to nullptr


### **New on version 3.0**
* TimeOut and Interal are on the same handler, only one `handler()` is needed on `void loop()`
* New callack function with any type and number of arguments:

```
TimeOut0.timeout<char*, int, String, float>(delay, callback, char*, int, String, float); //use with instance
Timeout<char*, int, String, float>(delay, callback, char*, int, String, float); //static scope operator
interval0.interval<char*, int, String, float>(delay, callback, char*, int, String, float);
```

Support: integer, floating point, object and pointers.
# Version Histrory
### Callback signature MUST match the arguments into the timeOut/Interval call!
Example:
```
void callback(char* c, int i, String s, float f){
    
}
```






###		**New on version 2.0**

* Dynamic instance allocation, not limit in size
* linked list operation, faster operation


**2.1 add**
new constructor an call for  timeOut and interval:
####
```
<TimeOut t1(hour, minute, seconde, callback);

TimeOut t2;
t2.timeOut(hour, minute, seconde, callback, mode);


Interval i1;
i1.interval(hour, minute, seconde, callback);
```

