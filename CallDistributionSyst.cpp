#include <iostream>
using namespace std;

class Node{
public:    
    int data;
    Node *next;
    
    Node(int d){
        data = d;
        next = NULL;
    }
};

class Stack{
public:
    Node *top=NULL,*newnode;
    int count = 0;
    void push(int d){
        newnode = new Node(d);
        count++;
        if(top==NULL){
            top = newnode;
        }
        else{
            newnode->next = top;
            top=newnode;
        }
    }
    void pop(){
        Node *temp=top;
        top = temp->next;
        delete temp;
    }
    int Top(){
        return top->data;
    }
    bool isEmpty(){
        if(top==NULL){
            return true;
        }
        else{
            return false;
        }
    }
};

class Queue{
public:
    Node *front = NULL, *rear,*newnode;
    int count=0;

    void enqueue(int d){
        count++;
        newnode = new Node(d);
        if(front == NULL){
            front = newnode;
            rear = newnode;
        }
        else{
            rear->next = newnode;
            rear = newnode;
        }
    } 
    
    void dequeue(){
        Node *temp = front;
        front = front->next;
        delete temp;
        count--;
    }

    int Front(){
        return front->data;
    }

    int size(){
        return count;
    }

    bool isEmpty(){
        if(front == NULL){
            return true;
        }
        else{
            return false;
        }
    } 
};

class Server {
public:
    int id;
    bool isFree;
    int totalCalls;
    int currentCall;
    Stack *history = new Stack();
    Server *next;

    Server(int i){
        id = i;
        isFree = true;
        totalCalls = 0;
        currentCall = 0;
        next = NULL;
    }
};

class SLL {
public:
    Server *head = NULL, *newnode;

    void create(int id){
        newnode = new Server(id);
        if(head == NULL){
            head = newnode;
        } else {
            Server *temp = head;
            while(temp->next != NULL){
                temp = temp->next;
            }
            temp->next = newnode;
        }
    }

    Server *getFreeServer(){
        Server *temp = head;
        while(temp!=NULL){
            if(temp->isFree)
                return temp;
            temp = temp->next;
        }
        return NULL;
    }

    Server *getServerById(int sid){
        Server* temp = head;
        while(temp!=NULL){
            if(temp->id == sid)
                return temp;
            temp = temp->next;
        }
        return NULL;
    }

    int size(){
        int count = 0;
        Server *temp = head;
        while(temp!=NULL){
            count++;
            temp=temp->next;
        }
        return count;
    }

    void display(){
        Server *temp = head;
        while(temp!=NULL){
            cout << "Server " << temp->id << " : ";
            if(temp->isFree){
                cout << "FREE";
            } 
            else{
                cout << "BUSY";
            } 
            temp = temp->next;
        }
    }
}; 
SLL* serverList = new SLL();
Queue* callQueue = new Queue();
int waitingQueue = 0;

void initializeServers(int n){
    for(int i = 1; i <= n; i++){
        serverList->create(i);
    }
    cout<<n<<" servers initialized successfully."
        <<endl<< "All servers are FREE.\n";
}

void addIncomingCall(){
    int callId;
    cout << "Enter Call ID: ";
    cin >> callId;
    if(callId<=0){
        cout<<"\n!!Invalid Call ID!!\n";
    }
    else{
        callQueue->enqueue(callId);
        cout<< "Call "<<callId<<" added to waiting queue.\n";
        Server* srv = serverList->getFreeServer();
        if(srv==NULL){
            waitingQueue++;
        }    
    }
  
}

void distributeCall(){
    if(callQueue->isEmpty()){
        cout<< "No calls in queue.\n";
        return;
    }

    Server* srv = serverList->getFreeServer();
    if(srv == NULL){
        cout << "All servers are busy. Call is waiting in queue.\n";
        return;
    }
    else{
        int callId = callQueue->Front();
        callQueue->dequeue();

        srv->isFree = false;
        srv->currentCall = callId;

        cout<< "Call " << callId<< " assigned to Server "<<srv->id << ".\n";
        cout << "Server "<< srv->id<< " is now BUSY for 10 minutes.\n";
    }
}

void completeCall(){
    int sid;
    cout << "Enter Server ID: ";
    cin >> sid;
    Server* srv = serverList->getServerById(sid);
    if(srv == NULL){
        cout << "Invalid Server ID.\n";
        return;
    }

    if(srv->isFree){
        cout << "Server is already FREE.\n";
        return;
    }
    else{
        cout << "Call completed on Server " << sid << ".\n";
        srv->isFree = true;
        srv->history->push(srv->currentCall);
        srv->totalCalls++;
        srv->currentCall = 0;
        
        cout<< "Server "<<sid<<" is now FREE.\n";
        cout<< "Call pushed to Server "<<sid<<" call history (Stack).\n\n";
        if(!callQueue->isEmpty()){
            distributeCall();
        }
    }  
}

void showStatistics(){
    cout << "\n===== Statistics =====\n";
    Server* temp = serverList->head;
    while(temp!=NULL){
        cout<< "Server "<<temp->id<< " handled "<<temp->totalCalls<< " call(s)\n";
        temp = temp->next;
    }
    
    cout << "Total waiting time: "<< waitingQueue*10 << " minutes\n";
}

bool isServerBusy(){
    Server *temp = serverList->head;
    while(temp!=NULL){
        if(!temp->isFree){
            return true;
        }
        temp = temp->next;
    }
    return false;
}

void showMenu(){
    cout << "\n1. Add Incoming Call";
    cout << "\n2. Distribute Call";
    cout << "\n3. Complete Call";
    cout << "\n4. Show Statistics";
    cout << "\n5. Exit";
    cout << "\nEnter your choice: ";
}

int main(){
    int n, choice;

    cout << "===== Call Distributor System =====\n";
    cout << "Enter number of servers: ";
    cin >> n;

    initializeServers(n);

    do{
        showMenu();
        cin >> choice;

        switch(choice){
            case 1: 
                addIncomingCall(); 
                break;
            case 2: 
                distributeCall(); 
                break;
            case 3: 
                completeCall(); 
                break;
            case 4: 
                showStatistics(); 
                break;
            case 5: 
                if(isServerBusy()){
                    cout<<"\n!!ERROR!! Cannot exit! Some servers are still busy.!!"
                        <<endl<<"Please complete all calls!\n";
                    choice = 0;
                }
                else{
                    cout<< "Exiting program....."; 
                }
                break;
            default: 
                cout<<"!!Invalid choice!!\n";
        }

    } while(choice != 5);

    return 0;
}
