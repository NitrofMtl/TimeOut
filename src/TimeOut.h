/*

        V5.0
  Copyright (c) 22/01/2022

    By Nitrof

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef TimeOut_h
#define TimeOut_h

#include "Arduino.h"

//#include "microTuple.h"

constexpr uint32_t sc(uint16_t x) { return x * 1000; }

constexpr uint32_t mn(uint16_t x) { return x * 60000; }

constexpr uint32_t hr(uint16_t x) { return x * 3600000; }

constexpr uint32_t timeElementToMillis(uint8_t hour, uint8_t minute,
                                       uint8_t seconde) {
  return hr(hour) + mn(minute) + sc(seconde);
}

enum class TIMEOUT {
  NORMAL,
  UNDELETABLE,
  INTERVAL
}; // timer can be overwriten or cleared , timer cannot be cleared

class TimeOut;

class TimeOutNode {
protected:
  TimeOutNode();
  TimeOutNode(unsigned long delay, unsigned long timeStamp);
  virtual ~TimeOutNode() = default;
  friend class TimeOut;
  friend class Interval;
  unsigned long delay;
  unsigned long timeStamp;
  TIMEOUT type;
  TimeOutNode *next;
  virtual void callback() = 0;
};

template <typename Callable> class TimeOutNodeDerived : public TimeOutNode {
  friend class TimeOut;
  friend class Interval;
  TimeOutNodeDerived(unsigned long delay, unsigned long &&timeStamp,
                     Callable cb)
      : TimeOutNode(delay, timeStamp), callable(cb) {}
  Callable callable;
  void callback() override { callable(); }
  friend class TimeOut;
};


class TimeOut {
public:
  friend class Interval;
  TimeOut(){};
  template <typename Callable>
  TimeOut(unsigned long delay, Callable callable)
      : node(new TimeOutNodeDerived<Callable>(delay, millis(), callable)) {
    sort(node); // place the instance into container
  }

  template <typename Callable>
  TimeOut(uint8_t hour, uint8_t minute, uint8_t seconde, Callable callable)
      : node(new TimeOutNodeDerived<Callable>(
            timeElementToMillis(hour, minute, seconde), millis(), callable)) {
    sort(node); // place the instance into container
  }



private:
  static TimeOutNode *head;

protected:
  TimeOutNode *node = nullptr;

  static void
  sort(TimeOutNode *current) { // sort timer by time remainin to be triggered
    if (!TimeOut::head) {      // if list is emty, set it to first
      TimeOut::head = current;
      return;
    }
    unsigned long now = millis();

    TimeOutNode *it = TimeOut::head;
    TimeOutNode *previous = nullptr;

    while (it) {
      long itRemain = it->timeStamp + it->delay - now;
      long currentRemain = current->timeStamp + current->delay - now;
      if (itRemain > currentRemain) { // spot if the timer is smaler than it timer
        // insert instance
        if (previous)
          previous->next = current;
        else
          TimeOut::head = current;
        current->next = it;
        return;
      } else if (!it->next) { // if delay is the bigger, place it at the end
        it->next = current;
        return;
      }
      previous = it; 
      it = it->next; // it++
    }
  };

public:
  template <typename Callable>
  void timeOut(unsigned long delay, Callable callable) {
    node = new TimeOutNodeDerived<Callable>(delay, millis(), callable);
    sort(node); // place the instance into container
  }

  template <typename Callable>
  void timeOut(unsigned long delay, Callable callable, TIMEOUT timerType) {
    node = new TimeOutNodeDerived<Callable>(delay, millis(), callable);
    if (TIMEOUT::UNDELETABLE == timerType)
      node->type = TIMEOUT::UNDELETABLE;
    else
      node->type = timerType;
    sort(node); // place the instance into container
  }

  template <typename Callable>
  void timeOut(uint8_t hour, uint8_t minute, uint8_t seconde, Callable callable,
               TIMEOUT timerType) {
    node = new TimeOutNodeDerived<Callable>(
        timeElementToMillis(hour, minute, seconde), millis(), callable);
        node->type = timerType;
    sort(node); // place the instance into container
  }

  void cancel();
  static void handler();

};



class Interval : public TimeOut {
public:
  template <typename Callable>
  void interval(unsigned long delay, Callable callable) {
    node = new TimeOutNodeDerived<Callable>(delay, millis(), callable);
    node->type = TIMEOUT::INTERVAL;
    sort(node); // place the instance into container
  }

  template <typename Callable>
  void interval(uint8_t hour, uint8_t minute, uint8_t seconde,
                Callable callable) {
    node = new TimeOutNodeDerived<Callable>(
        timeElementToMillis(hour, minute, seconde), millis(),
        callable);
    node->type = TIMEOUT::INTERVAL;
    sort(node); // place the instance into container
  }

  constexpr static void (*handler)(){TimeOut::handler};
};

#endif
