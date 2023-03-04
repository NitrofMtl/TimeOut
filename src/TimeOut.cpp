#include "TimeOut.h"

TimeOutNode::TimeOutNode()
    : delay(0), timeStamp(0), type(TIMEOUT::NORMAL), next(nullptr) {}

TimeOutNode::TimeOutNode(unsigned long delay, unsigned long timeStamp)
    : delay(delay), timeStamp(timeStamp), type(TIMEOUT::NORMAL), next(nullptr) {
}


TimeOutNode *TimeOut::head = nullptr;

void TimeOut::cancel() {
  if (node && TIMEOUT::UNDELETABLE == node->type) //check if node is null first
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
    if (!TimeOut::head)
      return; // if a cancel have deleted head
    TimeOutNode *temp = TimeOut::head;
    if (TimeOut::head->next)
      TimeOut::head = TimeOut::head->next; // switch to next timer
    else
      TimeOut::head = nullptr;
    if (TIMEOUT::INTERVAL == temp->type) {
      // reset triggered interval instance
      temp->timeStamp = now;
      temp->next = nullptr;
      sort(temp);//
      return;
    }
    delete temp;
  }
}


