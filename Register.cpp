
// Created by Salma Emara on 2023-06-02.
#include "Register.h"
#include <iostream>

using namespace std;

Register::Register(int id, double timePerItem, double overhead,
                   double entryTime) {
  ID = id;
  secPerItem = timePerItem;
  overheadPerCustomer = overhead;
  availableTime = entryTime;
  next = nullptr;           // no other registers yet
  queue = new QueueList();  // no customers in line yet, but has to initialize a
                            // queue
}

Register::~Register() { 
  delete queue; 
  }

QueueList* Register::get_queue_list() { 
  return queue; 
  }

Register* Register::get_next() { 
  return next; 
  }

int Register::get_ID() { 
  return ID; 
  }

double Register::get_secPerItem() { 
  return secPerItem; 
  }

double Register::get_overheadPerCustomer() { 
  return overheadPerCustomer; 
  }

double Register::get_availableTime() { 
  return availableTime; 
  }

void Register::set_next(Register* nextRegister) { 
  next = nextRegister; 
  }


void Register::set_availableTime(double availableSince) {
  availableTime = availableSince;
}



double Register::calculateDepartTime() {
  //  the queue is empty or there is no customer at the head
  if (queue == nullptr) {
    return -1;
  }
  if(queue->get_head() == nullptr){
    return -1;
  }

 
  Customer* headcustomer = queue->get_head();
  double timeOfArrival= headcustomer->get_arrivalTime();
  double numItems = headcustomer->get_numOfItems();

  //  when the customer arrives
  if (timeOfArrival <= availableTime) {
    
    return (secPerItem * numItems) + overheadPerCustomer + availableTime;
  }

  // Customer arrives after register becomes available
  return (secPerItem * numItems) + overheadPerCustomer + timeOfArrival;
}




void Register::departCustomer(QueueList* doneList) {
   // Check if the queue is not empty
  if (queue == nullptr) {
    // Enqueue nullptr to doneList if the queue is empty
    doneList->enqueue(nullptr);
  }
 
  
  double timeofDeparture = calculateDepartTime();
  //availableTime =  timeofDeparture;

  Customer* departingCustomer = queue->get_head();
  departingCustomer->set_departureTime(calculateDepartTime());

  // Move the customer from the register queue to the done list
  doneList->enqueue(queue->dequeue());

  // Update the register's available time to the customer's departure time
  availableTime =  timeofDeparture;
  

  
}

void Register::print() {
  cout << "Register ID: " << ID << endl;
  cout << "Time per item: " << secPerItem << endl;
  cout << "Overhead per customer: " << overheadPerCustomer << endl;
  if (queue->get_head() != nullptr) {
    cout << "Queue has customers: \n";
    queue->print();
  }
}


