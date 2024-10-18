// COMSC-210 | Mauricio Espinosa | Midterm #2
// IDE Used: Visual Studio Code & Github
// Starting with code from Midterm #1

#include <iostream>
#include <cstdlib>  // for random
#include <ctime>    // for seeding time
#include <fstream>  // for files
#include <vector>   // for vectors
#include <string>   // for strings
#include <iomanip>  // for setw
using namespace std;


const int MIN_NR = 10, MAX_NR = 99, MIN_LS = 5, MAX_LS = 20;
const int INITIAL_LINE_SIZE = 5;

// class declaration
class DoublyLinkedList {
// private member variables
private:
    struct Node {
        int data;
        Node* prev;   // pointer for previous node in the list
        Node* next;   // pointer for next node in the list
        // constructor which initializes each variable inside the Node struct
        Node(int val, Node* p = nullptr, Node* n = nullptr) {
            data = val; // sets data equal to what val has stored
            prev = p;   // sets the previous pointer
            next = n;   // sets the next pointer
        }
    };

    Node* head;   // pointer for the first node of the list
    Node* tail;   // pointer for the last node of the list

// public member variables
public:
    // consturctor setting the head and tail nodes to nullptr. Avoids dangling pointers
    DoublyLinkedList() { head = nullptr; tail = nullptr; }

    // method to insert a value after a specified position
    void insert_after(int value, int position) {
        if (position < 0) {   // input validation statement
            cout << "Position must be >= 0." << endl;  // error output
            return;   // exit the function
        }

        Node* newNode = new Node(value);  // create a new node and stores the value in the node
        if (!head) {   // if the list is empty or head is nullptr
            head = tail = newNode;   // both head and tail point to the new node
            return;   // exit the function
        }

        Node* temp = head;   // creates a temporary node and sets it to point to head
        for (int i = 0; i < position && temp; ++i)   // for loop to traverse the list
            temp = temp->next;  // move temp pointer to next node

        if (!temp) {  // if temp is nullptr meaning it reaches one past the end of the list
            cout << "Position exceeds list size. Node not inserted.\n";  // output statement
            delete newNode;  // delete the ndoe to free dynamic memory and avoid memory leak
            return;   // exit the function
        }

        // temp has stopped and we are inserting a node after temp so we need to do some "rewiring" / "replumbing" 
        newNode->next = temp->next;   // set the newNodes next pointer to the temp next node position
                                      // i.e. to the node that is going to be ahead of newNode
        newNode->prev = temp;         // set the newNodes prev pointer to where temp is pointing
                                      // i.e. to the node that is going to be behind newNode
        
        if (temp->next)   // if there is a node after where temp is pointing, meaning there is more nodes ahead..
            temp->next->prev = newNode;  // then set the "after" temp node's previous pointer to the newNode
        else
            tail = newNode;   // if temp was the last node then move tail to the newNode as that is the new tail
        temp->next = newNode;   // temp next pointer gets updated to point to newNode
    }

    // delete a node by value
    void delete_val(int value) {
        if (!head) return;   // if the list is empty or head is nullptr

        Node* temp = head;   // creates a temporary node and sets it to point to head
        
        while (temp && temp->data != value)  // traverses the list one at a time until the node containing the value is found
            temp = temp->next;  // move temp up to the next node

        if (!temp) return;   // if temp is nullptr meaning it reaches one past the end of the list exit the function

        if (temp->prev)      // if there is something in temp->prev meaning temp is not at the head
            temp->prev->next = temp->next;   // update temp->next to the node in front of where temp is
        else
            head = temp->next;   // otherwise update the head pointer if temp is the first node

        if (temp->next)      // if there is something in temp->next meaning temp is not at the end
            temp->next->prev = temp->prev;  // update temp->prev to the node behind where temp is
        else
            tail = temp->prev;   // otherwise update the tail pointer if temp is at the end

        delete temp;   // delete the temporary ndoe to free dynamic memory and avoid memory leak
    }

    // delete a node by position
    void delete_pos(int pos) {   // position as an argument
        if (!head) {  // if the list is empty or head is nullptr
            cout << "List is empty." << endl;   // error output
            return;   // exit the function
        }
    
        if (pos == 1) {   // if position is equal to 1
            pop_front();  // call to pop_front() function which will delete the front node
            return;  // exit the function
        }
    
        Node* temp = head;   // creates a temporary node and sets it to point to head
    
        for (int i = 1; i < pos; i++){   // for loop to traverse the list
            if (!temp) {    // if head is nullptr then the position doesnt exist
                cout << "Position doesn't exist." << endl;  // error message
                return;    // exit the function
            }
            else
                temp = temp->next;  // otherwise move to the next node
        } // end of for loop
        if (!temp) {  // if head is nullptr then the position doesnt exist
            cout << "Position doesn't exist." << endl;  // error message
            return;  // exit the function
        }
    
        if (!temp->next) {  // if temp->next is pointing to nullptr meaning temp is at the last node 
            pop_back();  // call to pop_back() function which will delete the back node
            return;  // exit the function
        }
    
        // creates a tempPrev node and sets it to where the original temp's prev node is pointing to
        Node* tempPrev = temp->prev;  
        tempPrev->next = temp->next;   // links the tempPrev node to the next node ahead of temp
        temp->next->prev = tempPrev;   // links the next node back to tempPrev
        delete temp;                   // delete the temporary ndoe to free dynamic memory and avoid memory leak     
    }

    // function to add a value to the end of the list
    void push_back(string v) {
        Node* newNode = new Node(v);    // create a new node with the value passed in
        if (!tail)                  // if the list is empty
            head = tail = newNode;  // head and tail point to newNode
        else {
            tail->next = newNode;   // otherwise set the end nodes next pointer to newNode, making newNode the end of the list
            newNode->prev = tail;   // more rewiring but now we link the newNodes previous pointer back to the node before it where tail is
            tail = newNode;         // move tail up to the newNode making newNode the new tail
        }
    }

    // function to add a value to the front of the list    
    void push_front(string v) {
        Node* newNode = new Node(v);   // create a new node with the value passed in
        if (!head)                     // if the list is empty
            head = tail = newNode;    // head and tail point to newNode
        else {
            newNode->next = head;    // otherwise set the newNodes next pointer to where head is pointing
            head->prev = newNode;    // link the heads prev node to newNode, basically almost pushing head one above newNode
            head = newNode;          // move head to the newNode making newNode the new head
        }
    }
    
    // function to delete a value from the front
    void pop_front() {
 
        if (!head) {                            // if the list is empty
            cout << "List is empty." << endl;   // error message
            return;                             // exit the function as there is nothing to delete
        }

        Node * temp = head;     // create a temporary pointer and point to where head is pointing

        if (head->next) {     // if there is another node after the head node
            head = head->next;   // move head to the next node, temp pointer stays as is
            head->prev = nullptr;  // rewire head prev to nullptr
        }
        else
            head = tail = nullptr;  // if there was only one node, then set head and tail to nullptr
        delete temp;  // delete the node where temp is
    }

    // function to delete a value fron the end
    void pop_back() {
        if (!tail) {                            // if the list is empty
            cout << "List is empty." << endl;   // error message
            return;                             // exit the function as there is nothing to delete
        }
        Node * temp = tail;   // create a temporary pointer and point to where tail is pointing

        if (tail->prev) {       // if there is another node before the tail node
            tail = tail->prev;   // move tail to the previous node
            tail->next = nullptr;  // set tail next to nullptr to avoid dangling pointers
        }
        else
            head = tail = nullptr;   // if there was only one node, then set head and tail to nullptr
        delete temp;   // delete the node where temp is
    }

    // Destructor to delete the double linked list
    ~DoublyLinkedList() {
        while (head) {     // while head is pointing to something, meaning the list is NOT empty
            Node* temp = head;   // creates a temporary pointer pointing to where head is pointing
            head = head->next;   // move head up to next node, temp stays as is
            delete temp;         // delete node that temp is pointing to
        }  // repeat until ist is empty
    }

    // prints the list
    void print() {
        Node* current = head;   // creates a current node and points to where head is pointing, the start of the list
        if (!current) {    // if the list is empty
            cout << "List is empty." << endl; // error message
            return;   // exit the function as there is nothing to print
        }
        while (current) {   // while current is pointing to something
            cout << current->data << " ";   // print the data inside the node current is pointing to with a space
            current = current->next;   // move current to the next node
        } // printing complete
        cout << endl;   // new line
    }

    // print the list in reverse
    void print_reverse() {
        Node* current = tail;  // creates a current node and points to where tail is pointing, the end of the list
        if (!current) { // if the list is empty
            cout << "List is empty." << endl;// error message
            return; // exit the function as there is nothing to print
        }
        while (current) {   // while current is pointing to something
            cout << current->data << " "; // print the data inside the node current is pointing to with a space
            current = current->prev;  // move current to the previous node
        } // printing complete
        cout << endl; // new line
    }

    // method to print every other node starting with first node
    void every_other_element() {
        Node* temp = head;     // create a temp node and points to where head is pointing to, the front of the list
        
        while (temp) {       // while temp is pointing to something, meaning not nullptr, meaning not pst the end of the list
            cout << temp->data << " ";  // output the data in the pointer
            if (temp->next) {   // if there is another node ahead of where temp is
                temp = temp->next->next;  // skip the next node and continue to the next one after that
            }
            else
                break;   // otherwise stop
        }

        cout << endl;  // new line
    }

    // function to read names from a file
    vector<string> read_names(const string& fileName) {
        vector<string> names;          // string vector called names
        ifstream file(fileName);       // opens the file
        string name;                   // name variable
        while (getline(file, name)) {
            names.push_back(name);      // add a name to the end of the vector
        }

        return names;  // return names vector
    }

    void simulation(DoublyLinkedList& shopLine, const vector<string>& lineNames)
    {
        cout << "Store Opens:\n";
        for (int i = 0; i < INITIAL_LINE_SIZE; i++)  // immediately populate 5 people in line
        {
            shopLine.push_back(lineNames[rand() % names.size()]);  // add names to the back of the linked list picking randomly from the names.txt file
            cout << " " << names[i] << " joined the line" << endl;  // output that name and that they joined the line
        }


    }

};

// main function
int main() {
    srand(time(0));  // seed time for random

    DoublyLinkedList line;    // creates a doubly linked list called line

    vector<string> nameVec = read_names("names.txt");   // calls read_names function to populate the nameVec vector from the names.txt file

    simulation(line, nameVec); // call to simulation function

    cout << MIN_NR + MIN_LS + MAX_NR + MAX_LS;  // dummy statement to avoid compiler warning



    
    return 0;
}