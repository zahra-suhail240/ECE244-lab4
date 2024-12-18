
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "Customer.h"
#include "QueueList.h"
#include "Register.h"
#include "RegisterList.h"

using namespace std;



// Set mode of the simulation
string getMode();

// Register
void parseRegisterAction(stringstream &lineStream, string mode);
void openRegister(
    stringstream &lineStream,
    string mode);  // register opens (it is upto customers to join)
void closeRegister(stringstream &lineStream,
                   string mode);  // register closes 

// Customer
void addCustomer(stringstream &lineStream,
                 string mode);  // customer wants to join


// Helper functions
bool getInt(stringstream &lineStream, int &iValue);
bool getDouble(stringstream &lineStream, double &dValue);
bool foundMoreArgs(stringstream &lineStream);

// Global variables
RegisterList *registerList; // holding the list of registers
QueueList *doneList; // holding the list of customers served
QueueList *singleQueue; // holding customers in a single virtual queue
double expTimeElapsed; // time elapsed since the beginning of the simulation



//Helper function decleration


void SingleCustomerMode() {

if(singleQueue->get_head()==nullptr){
  return;
}

Register* freeReg = registerList-> get_free_register();

if(freeReg != nullptr){
  cout << "Queued a customer with free register " << freeReg->get_ID() << endl;

  freeReg->get_queue_list()->enqueue(singleQueue->dequeue());
}else{
  cout << "No free registers" << endl;
}


} 



// List of commands:
// To open a register
// register open <ID> <secPerItem> <setupTime> <timeElapsed>
// To close register
// register close <ID> <timeElapsed>
// To add a customer
// customer <items> <timeElapsed>

int main() {
  registerList = new RegisterList();
  doneList = new QueueList();
  singleQueue = new QueueList();
  expTimeElapsed = 0;

  // Set mode by the user
  string mode = getMode();

  string line;
  string command;

  cout << "> ";  // Prompt for input
  getline(cin, line);

  while (!cin.eof()) {
    stringstream lineStream(line);
    lineStream >> command;
    if (command == "register") {
      parseRegisterAction(lineStream, mode);
    } else if (command == "customer") {
      addCustomer(lineStream, mode);
    } else {
      cout << "Invalid operation" << endl;
    }
    cout << "> ";  // Prompt for input
    getline(cin, line);
  }
  
  //print the stats 
  

    Customer* currentCustomer = doneList->get_head();

    double waitTime, max, resultofWaitTime, averageWait, avgTimeDiff, timeDiff;
    int countCustomer;

    while(true ){
      if(currentCustomer != nullptr){
        countCustomer++;

      waitTime = currentCustomer-> get_departureTime() - currentCustomer->get_arrivalTime();
      resultofWaitTime = resultofWaitTime + waitTime;

      if(waitTime > max){
        max = waitTime;
      }

     currentCustomer = currentCustomer->get_next();
      }
      if(currentCustomer == nullptr){
          break;
      }
    
    }
      averageWait = resultofWaitTime / countCustomer;
      currentCustomer = doneList->get_head();

   while(currentCustomer != nullptr){
      timeDiff = currentCustomer->get_departureTime() - currentCustomer->get_arrivalTime()-averageWait;

      avgTimeDiff = avgTimeDiff + pow(timeDiff,2);
      currentCustomer = currentCustomer->get_next();
    }


    cout << "Finished at time " << expTimeElapsed << endl;
    cout<< "Statistics:" << endl;
    cout << "Maximum wait time: " << max << endl;
    cout << "Average wait time: " << averageWait << endl;
    cout << "Standard Deviation of wait time: " <<  sqrt(avgTimeDiff / countCustomer ) << endl;

  // You have to make sure all dynamically allocated memory is freed 
  // before return 0

  delete  registerList;
  delete doneList;
  delete singleQueue;

  return 0;
}

string getMode() {
  string mode;
  cout << "Welcome to ECE 244 Grocery Store Queue Simulation!" << endl;
  cout << "Enter \"single\" if you want to simulate a single queue or "
          "\"multiple\" to "
          "simulate multiple queues: \n> ";
  getline(cin, mode);

  if (mode == "single") {
    cout << "Simulating a single queue ..." << endl;
  } else if (mode == "multiple") {
    cout << "Simulating multiple queues ..." << endl;
  }

  return mode;
}

void addCustomer(stringstream &lineStream, string mode) {
  int items;
  double timeElapsed;
  if (!getInt(lineStream, items) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments." << endl;
    return;
  }
  // Depending on the mode of the simulation (single or multiple),
  // add the customer to the single queue or to the register with
  // fewest items

  expTimeElapsed = expTimeElapsed + timeElapsed;

  Customer* addACustomer = new Customer(expTimeElapsed, items);
  

  Register* currentRegister = registerList->calculateMinDepartTimeRegister(expTimeElapsed);

  while (currentRegister != nullptr) {
    double TimeofDeparture = currentRegister->calculateDepartTime();
    
    if (TimeofDeparture<= expTimeElapsed) {
      currentRegister->departCustomer(doneList);
      cout << "Departed a customer at register ID " << currentRegister->get_ID() << " at " << TimeofDeparture << endl;
      
      if (mode == "single") {
       SingleCustomerMode();

      }

    }
   
    currentRegister = registerList->calculateMinDepartTimeRegister(expTimeElapsed);
  }
  cout<< "A customer entered" << endl;
  

  if(mode == "multiple"){
    //enque to the register with the least number of items
    Register* regWithFewestItems = registerList->get_min_items_register();
     
     registerList->get_min_items_register()->get_queue_list()->enqueue(addACustomer);
       cout<< "Queued a customer with quickest register " << regWithFewestItems-> get_ID() <<endl; 
    
  }

  if(mode == "single"){
    singleQueue -> enqueue(addACustomer);
   
    if(singleQueue->get_head()==nullptr){
  return;
}

Register* freeReg = registerList-> get_free_register();

if(freeReg != nullptr){
  cout << "Queued a customer with free register " << freeReg->get_ID() << endl;

  freeReg->get_queue_list()->enqueue(singleQueue->dequeue());
}else{
  cout << "No free registers" << endl;
}

  }

}

void parseRegisterAction(stringstream &lineStream, string mode) {
  string operation;
  lineStream >> operation;
  if (operation == "open") {
    openRegister(lineStream, mode);
  } else if (operation == "close") {
    closeRegister(lineStream, mode);
  } else {
    cout << "Invalid operation" << endl;
  }
}

void openRegister(stringstream &lineStream, string mode) {
  int ID;
  double secPerItem, setupTime, timeElapsed;
  // convert strings to int and double
  if (!getInt(lineStream, ID) || !getDouble(lineStream, secPerItem) ||
      !getDouble(lineStream, setupTime) ||
      !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }

  // Check if the register is already open
  // If it's open, print an error message
  // Otherwise, open the register
  // If we were simulating a single queue, 
  // and there were customers in line, then 
  // assign a customer to the new register

  if(registerList->foundRegister(ID)){
    cout<< "Error: register " << ID << " is already open" << endl;
  }
 else{
    expTimeElapsed = expTimeElapsed + timeElapsed;

    Register* newReg =  new Register(ID, secPerItem, setupTime, expTimeElapsed);
    //udate the system bcz we added a reg

    Register* currentRegister = registerList->calculateMinDepartTimeRegister(expTimeElapsed);
 
  while (currentRegister != nullptr) {
    double TimeofDeparture = currentRegister->calculateDepartTime();
    
    if (TimeofDeparture <= expTimeElapsed) {
      currentRegister->departCustomer(doneList);
      cout << "Departed a customer at register ID " << currentRegister->get_ID() << " at " << TimeofDeparture << endl;
      
      if (mode == "single") {
       SingleCustomerMode();

      }

    }

    currentRegister = registerList->calculateMinDepartTimeRegister(expTimeElapsed);
  }

    registerList->enqueue(newReg);
    

    cout << "Opened register " << ID << endl;

    if(mode == "single"){
      

    if(singleQueue->get_head()==nullptr){
    return;
  }

  Register* freeReg = registerList-> get_free_register();

  if(freeReg != nullptr){
  cout << "Queued a customer with free register " << freeReg->get_ID() << endl;

  freeReg->get_queue_list()->enqueue(singleQueue->dequeue());
}else{
  cout << "No free registers" << endl;
}

      
    }
 }

  
}

void closeRegister(stringstream &lineStream, string mode) {
  int ID;
  double timeElapsed;
  // convert string to int
  if (!getInt(lineStream, ID) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }
  // Check if the register is open
  // If it is open dequeue it and free it's memory
  // Otherwise, print an error message 

  if(!(registerList->foundRegister(ID))){
    cout<< "Error: register " << ID << " is not open"<< endl;
  }
  else if(registerList->foundRegister(ID)){
    //dequqe the reg

    expTimeElapsed =  expTimeElapsed +  timeElapsed;
   
   Register* currentRegister = registerList->calculateMinDepartTimeRegister(expTimeElapsed);
 
  while (currentRegister != nullptr) {
    double TimeofDeparture = currentRegister->calculateDepartTime();
    
    if (currentRegister->calculateDepartTime() <= expTimeElapsed) {
      currentRegister->departCustomer(doneList);
      cout << "Departed a customer at register ID " << currentRegister->get_ID() << " at " << TimeofDeparture << endl;
      
      if (mode == "single") {
       SingleCustomerMode();
     
      }

    }
    currentRegister = registerList->calculateMinDepartTimeRegister(expTimeElapsed);
  }

    Register* closeReg = registerList->dequeue(ID);
   delete closeReg;
    cout<< "Closed register "<< ID <<endl;
  }
  
}

bool getInt(stringstream &lineStream, int &iValue) {
  // Reads an int from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  iValue = stoi(command);
  return true;
}

bool getDouble(stringstream &lineStream, double &dvalue) {
  // Reads a double from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  dvalue = stod(command);
  return true;
}

bool foundMoreArgs(stringstream &lineStream) {
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  } else {
    return true;
  }
}





