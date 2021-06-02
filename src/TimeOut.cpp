#include "TimeOut.h"

TimeOutNode::TimeOutNode()
    : delay(0), timeStamp(0), type(TIMEOUT::NORMAL), next(nullptr) {}

TimeOutNode::TimeOutNode(unsigned long delay, unsigned long timeStamp)
    : delay(delay), timeStamp(timeStamp), type(TIMEOUT::NORMAL), next(nullptr) {
}
/*
TimeOutNode::TimeOutNode(unsigned long delay, unsigned long timeStamp,
                         TIMEOUT type)
    : delay(delay), timeStamp(timeStamp), type(type), next(nullptr) {}
*/
TimeOutNode *TimeOut::head = nullptr;

void TimeOut::cancel() {
  if (TIMEOUT::UNDELETABLE == node->type)
    return; // do not cancel a timer if Undeleable
  if (!TimeOut::head)
    return; // guard if no timer arer set
  TimeOutNode *tmpNode = TimeOut::head;
  TimeOutNode *previous = nullptr;
  while (this->node != tmpNode) {
    if (!tmpNode->next)
      return; // break loop if no instace correspond
    previous = tmpNode;
    tmpNode = tmpNode->next;
  }
  if (!previous)
    TimeOut::head = tmpNode->next;
  else
    previous->next = tmpNode->next;
  node = nullptr;
  delete tmpNode;
}

void TimeOut::handler() {
  if (!TimeOut::head)
    return; // if there is no timer set, do nothing
  unsigned long now = millis();
  if (now - TimeOut::head->timeStamp > TimeOut::head->delay) {

    TimeOut::head->callback();
    TimeOutNode *temp = TimeOut::head;
    if (TimeOut::head->next)
      TimeOut::head = TimeOut::head->next; // switch to next timer
    else
      TimeOut::head = nullptr;
    if (TIMEOUT::INTERVAL == temp->type) {
      // reset triggered interval instance
      temp->timeStamp = now;
      temp->next = nullptr;
      sort(temp);
    } else
      delete temp; // delete triggered timeout instance
  }
}

[[deprecated("will be remove on next version")]] void
TimeOut::printContainer(HardwareSerial &stream) {
  stream.println("Timer container contain the following timer: ");
  unsigned long now = millis();
  TimeOutNode *it = TimeOut::head;
  while (it) {
    if (!TimeOut::head) {
      stream.println("No timer set");
      return;
    }
    stream.print("Container delay ");
    stream.print(it->delay);
    stream.print(" remain: ");
    stream.println(it->timeStamp + it->delay - now);
    it = it->next;
  }
  stream.println("End.");
  stream.println();
}
