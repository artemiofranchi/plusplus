#include <iostream>
using namespace std;

class Order{
    char *Firmname;
    int Firmlength; // добавляем поле, содержащее информацию о длине имени фирмы
    char *Phonenumber;
    int Phonelength; // добавляем поле, содержащее информацию о количестве цифр в номере телефона
    int Ordernumber;
    
public:
    Order *next; // в паблике объявляем указатели на текущий и предыдущий элементы списка
    Order *prev;
    Order();// конструктор по умолчанию
    Order(const Order &example); // конструктор копирования
    Order(const char *firm, const char *phone, int order);
    ~Order();
    void PrintOrder();
    Order& operator = (const Order& example){ // перегрузка оператора =
        if (this == &example){
            return *this;
        }
        if (Firmname != NULL) delete []Firmname; // убираем мусор
        Firmname = new char[example.Firmlength+1];
        Firmlength = example.Firmlength;
        int i = 0;
        while (*(example.Firmname+i) != '\0'){
            *(Firmname+i) = *(example.Firmname+i);
            i++;
        }
        *(Firmname+i) = '\0';
        if (Phonenumber!=NULL) delete []Phonenumber;
        Phonenumber = new char[example.Phonelength+1];
        Phonelength = example.Phonelength;
        int j = 0;
        while (*(example.Phonenumber+j) != '\0'){
            *(Phonenumber+j) = *(example.Phonenumber+j);
            j++;
        }
        *(Phonenumber+j) = '\0';
        Ordernumber = example.Ordernumber;
        return *this;
    }
}; 


Order :: Order(){
    Firmlength = 5;
    Firmname = new char[Firmlength+1];
    *(Firmname) = 'f'; 
    *(Firmname+1) = 'i'; 
    *(Firmname+2) = 'r'; 
    *(Firmname+3) = 'm'; 
    *(Firmname+4) = '0'; 
    *(Firmname+5) = '\0';
    Phonelength = 11;
    Phonenumber = new char[Phonelength+1];
    *(Phonenumber+0) = '8';
    *(Phonenumber+1) = '8';
    *(Phonenumber+2) = '8';
    *(Phonenumber+3) = '8';
    *(Phonenumber+4) = '8';
    *(Phonenumber+5) = '8';
    *(Phonenumber+6) = '8';
    *(Phonenumber+7) = '8';
    *(Phonenumber+8) = '8';
    *(Phonenumber+9) = '8';
    *(Phonenumber+10) = '8';
    *(Phonenumber+11) = '\0';
    Ordernumber = 0;
    cout << "Default was created\n";
}


Order :: Order(const Order &example){
    Firmname = new char[example.Firmlength+1];
    Firmlength = example.Firmlength;
    int i = 0;
    while (*(example.Firmname+i) != '\0'){
        *(Firmname+i) = *(example.Firmname+i);
        i++;
    }
    *(Firmname+i) = '\0';
    Phonenumber = new char[example.Phonelength+1];
    Phonelength = example.Phonelength;
    int j = 0;
    while (*(example.Phonenumber+j) != '\0'){
        *(Phonenumber+j) = *(example.Phonenumber+j);
        j++;
    }
    *(Phonenumber+j) = '\0';
    Ordernumber = example.Ordernumber;
    cout << "Copy was created\n";
}


Order :: Order(const char *firm, const char *phone, int order){
    int i = 0;
    while(*(firm+i) != '\0'){
        i++; // под Firmname нужно выделить память(нужно знать Firmlength), поэтому цикла будет 2
    }
    Firmlength = i;
    Firmname = new char[Firmlength+1];
    i = 0;
    while (*(firm+i) != '\0'){
        *(Firmname+i) = *(firm+i);
        i++;
    }
    *(Firmname+i) = '\0';
    i = 0;
    while(*(phone+i) != '\0'){
        i++; 
    }
    Phonelength = i;
    Phonenumber = new char[Phonelength+1];
    i = 0;
    while (*(phone+i) != '\0'){
        *(Phonenumber+i) = *(phone+i);
        i++;
    }
    *(Phonenumber+i) = '\0';
    Ordernumber = order;
    cout << "New Order was created\n";
}

Order :: ~Order(){
    delete []Firmname;
    delete []Phonenumber;
    cout << "Order was destructed\n";
}

void Order :: PrintOrder(){
    cout << "Name of the firm: " << Firmname << "\n";
    cout << "Phone Number: " << Phonenumber << "\n";
    cout << "Number of the order: " << Ordernumber << "\n";

}

class ListofOrders{
    Order *Front; //указатель на начало списка
    Order *Back; // указатель на конец списка
    int leng; // длина списка
public:
    ListofOrders(){
        Front = nullptr;
        Back = nullptr;
        leng = 0;
    }
    ~ListofOrders(){
        while (leng!=0){
            pop_front();
        }
    }
    void push_front(Order example);
    void push_back(Order example);
    void front();
    void back();
    void pop_front();
    void pop_back();
    void insert(int p, Order x);
    void erase(int p);
    void empty();
    void size();

};

void ListofOrders :: push_front(Order example){
    Order *temp;
    temp  = &example;
    temp->prev = nullptr;
    if (leng == 0){
        Front = Back = &example;
        leng++;
        temp->next = nullptr;
    }
    else{
        temp->next = Front;
        Front = &example;
        leng++;
    }
}

void ListofOrders :: push_back(Order example){
    Order *temp;
    temp = &example;
    temp->next = nullptr;
    if (leng == 0){
        Back = Front = &example;
        leng++;
        temp->prev = nullptr;
    }
    else{
        temp->prev = Back;
        Back = &example;
        leng++;
    }
}

void ListofOrders :: front(){
    if (leng!=0){
        Front->PrintOrder();
    }
}
void ListofOrders :: back(){
    if (leng!=0){
        Back->PrintOrder();
    }
}

void ListofOrders :: pop_front(){
    if (leng != 0){
        Order *temp;
        temp = Front->next;
        Front->next = nullptr;
        Front = temp;
        Front->prev = nullptr;
        leng--;
    }
}

void ListofOrders :: pop_back(){
    if (leng != 0){
        Order *temp;
        temp = Back->prev;
        Back->prev = nullptr;
        Back = temp;
        Back->next = nullptr;
        leng--;
    }
}

void ListofOrders :: insert(int p, Order x){

}




int main(){
    Order ex1;
    ex1.PrintOrder();
    Order ex2("Apple Inc", "022391029301", 1);
    ex2.PrintOrder();

    Order ex4;
    ex4 = ex2;
    ex4.PrintOrder();

    Order ex3(ex2);
    ex3.PrintOrder();

    ListofOrders l1;
    l1.push_front(ex2);
    l1.push_back(ex3);
    l1.front();
    l1.back();

    return 0;
    
}
