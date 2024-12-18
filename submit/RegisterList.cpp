
#include "RegisterList.h"

#include <iostream>

#include "Register.h"
using namespace std;

RegisterList::RegisterList() {
  head = nullptr;
  size = 0;
}

RegisterList::~RegisterList() {
  // Delete all registers in the list

  //same approach with queuelist

  while(head != nullptr){
    Register* deleteRegister;
    deleteRegister=head;

    head = head->get_next();
    delete deleteRegister;
  }
}

Register* RegisterList::get_head() { 
   return head;
 }

int RegisterList::get_size() { 
  // return number of registers 
   
  //check if no register in the list
  if(head==nullptr){
    
    return 0;
  }
    //there are registers in the list
    int num;
    Register* counter;
    counter=head;

    while(counter != nullptr){
      num++;
      counter = counter-> get_next();
    }

    return num;
  
}


Register* RegisterList::get_min_items_register() {
  // loop all registers to find the register with least number of items
  //check is reg list is empty or not

  
  if (head == nullptr) {
    return nullptr;
  }

    Register* regLeastItem = head;
    int leastItem = head -> get_queue_list()->get_items();
    Register*temp;

    for(temp= head->get_next(); temp != nullptr; temp=temp->get_next()){
      int currentItems = temp->get_queue_list()->get_items();

      if(currentItems < leastItem){
        leastItem = currentItems;
        regLeastItem = temp;
      }
    }
    
    return regLeastItem;
  
  
 
}


Register* RegisterList::get_free_register() {
  // return the register with no customers
  // if all registers are occupied, return nullptr

  Register* free;
  for(free = head; free != nullptr; free = free->get_next()){
    if(free->get_queue_list()->get_head()==nullptr){
      return free;
    }
  }

  return nullptr;
}

void RegisterList::enqueue(Register* newRegister) {
  // a register is placed at the end of the queue
  // if the register's list is empty, the register becomes the head
  // Assume the next of the newRegister is set to null
  // You will have to increment size 

  Register* enqueueReg = head;

  if (head == nullptr) {
    head = newRegister;
  }
   else if (head != nullptr){
      while (enqueueReg->get_next() != nullptr) {
      enqueueReg = enqueueReg->get_next();
  }
  
    enqueueReg->set_next(newRegister);
   } 
  
  size++;
  
}

bool RegisterList::foundRegister(int ID) {
  // look for a register with the given ID
  // return true if found, false otherwise

    Register* temp;

    if(head == nullptr){
      return false;
    }

    for(temp=head; temp !=nullptr; temp=temp->get_next()){
      if(temp->get_ID()==ID){
        return true;
      }
    }

    return false; //becz no reg found that matches the given ID

}

Register* RegisterList::dequeue(int ID) {
  // dequeue the register with given ID

  // return the dequeued register
  // return nullptr if register was not found

    if(head == nullptr){
      return nullptr;
    }

    Register* prev=nullptr;
    Register* current;

    for(current=head; current != nullptr; current=current->get_next()){
      if((current->get_ID()==ID) && (prev == nullptr)){
        head = current->get_next();
      }

      else if((current->get_ID()==ID) && (prev != nullptr)){
        prev->set_next(current->get_next());
      }

      if(current->get_ID()==ID){
        current->set_next(nullptr);
        return current;
      }
      
       prev = current;

    }

   return nullptr;
}

Register* RegisterList::calculateMinDepartTimeRegister(double expTimeElapsed) {
  // return the register with minimum time of departure of its customer
  // if all registers are free, return nullptr
  // return the register with minimum time of departure of its customer
  // if all registers are free, return nullptr

  Register* temp=head;
  Register* current;
  double minDepartureTime;

  if(head==nullptr){
    return nullptr;
  }
  

  while(temp != nullptr && temp-> calculateDepartTime() < 0){
    temp = temp->get_next();
  }

  if(temp == nullptr){
    return nullptr;
  }

  minDepartureTime = temp-> calculateDepartTime();

  for(current = temp->get_next(); current != nullptr; current = current->get_next()){
    double currentDeparture = current->calculateDepartTime();

    if(currentDeparture < 0){
     continue;
   }

    if(currentDeparture <= expTimeElapsed && currentDeparture < minDepartureTime ){

      minDepartureTime  = currentDeparture;
      temp = current;
    }
  }

  if(temp->calculateDepartTime() > expTimeElapsed){
    return nullptr;
  }

  return temp;

}

void RegisterList::print() {
  Register* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}



