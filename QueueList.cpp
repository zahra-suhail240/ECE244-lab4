
#include "QueueList.h"

#include "Customer.h"

QueueList::QueueList() { head = nullptr; }

QueueList::QueueList(Customer* customer) { head = customer; }

QueueList::~QueueList() {
 //destructor, so when queuelist is deleted we want to delete the customers
  
  while(head != nullptr){
    Customer* deleteCustomer;
    deleteCustomer=head;
    head = head->get_next();
    delete deleteCustomer;
    
  }
}

Customer* QueueList::get_head() { return head; }

void QueueList::enqueue(Customer* customer) {

 
  // a customer is placed at the end of the queue
  //customer->set_next(nullptr);

  // if the queue is empty, the customer becomes the head
  if(head == nullptr){
    customer->set_next(nullptr);
    head = customer;
  }
  //queue not empty, have to place customer at the end of list
  else if(head != nullptr){
   customer->set_next(nullptr);
    Customer *traverse;
    traverse=head;
    while(traverse -> get_next() != nullptr){
      traverse = traverse -> get_next();
    }
    //exits loop when traverse null then we reached end of queue
    traverse -> set_next(customer);
      

 }
  
}

Customer* QueueList::dequeue() {
  // remove a customer from the head of the queue 
  // and return a pointer to it
  
  if(head == nullptr){
    return nullptr; //empty queue
  }
 
     Customer* removeCustomer;
     removeCustomer = head;

      head = head -> get_next();
      return removeCustomer; 
 
}

int QueueList::get_items() {
  // count total number of items each customer in the queue has
  
   Customer* trackItems;
    trackItems = head;
    int items=0;
    
  //empty queue
  if(head== nullptr){
 
    return 0;
  }
  

    while( trackItems != nullptr){
      items = items + trackItems -> get_numOfItems();
      //next customer
      trackItems = trackItems -> get_next();
      
    }
    return items;
 
  
}

void QueueList::print() {
  // print customers in a queue
  Customer* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}



