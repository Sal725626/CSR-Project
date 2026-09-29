// CMSC 341 - Fall 2026 - Project 1
#include "csr.h"
///////////////CSR Class Implementation///////////////
CSR::CSR(){
    // want to set to points to nullptr and ints to 0
    m_values = nullptr;
    m_col_index = nullptr;
    m_row_index = nullptr;
    m_nonzeros = 0;
    m_m = 0;
    m_n = 0;
    m_next = nullptr;
}
CSR::~CSR(){
    clear(); 
// call on clear func, keeps things clean
}
CSR::CSR(const CSR & rhs){
    // make deep copy for copy construtor 
    // diff mem locations n independent betwen arrays

    m_values = nullptr;
    m_col_index = nullptr;
    m_row_index = nullptr;
    m_nonzeros = 0;
    m_m = 0;
    m_n = 0;
    m_next = nullptr;

    if(!rhs.empty()){
     // pretty much same thing we did in compress again
        m_n = rhs.m_n; // copy rows, cols, nonzeros
        m_m = rhs.m_m;
        m_nonzeros = rhs.m_nonzeros;
        m_row_index = new int[m_m + 1];

        if (m_nonzeros>0){
        m_values = new int[m_nonzeros];
        m_col_index = new int [m_nonzeros];
        }

        for (int i = 0; i < m_nonzeros; i++){ // copy vals and col index
        m_values[i] = rhs.m_values[i];
        m_col_index[i] = rhs.m_col_index[i];
        }

        for (int i = 0; i < m_m + 1; i++){ // copy row index
        m_row_index[i] = rhs.m_row_index[i];
        }

    }

}  
void CSR::clear(){
    // this func will basically delete all allocated arrays and thenn make it new again n rdy
    // use []to dlete arrays and set to nullptr and int =0
    delete[] m_values;
    delete[] m_col_index;
    delete[] m_row_index;

    m_values = nullptr; // set to nullptrs again
    m_col_index = nullptr;
    m_row_index = nullptr;
    m_nonzeros = 0;
    m_m = 0;
    m_n = 0;
}
bool CSR::empty() const{
    return m_row_index == nullptr; // if row index is nullptr, then empty
}
void CSR::compress(int m, int n, int array[], int arraySize){
    // to able to compress again we gotta empty the arrays
    clear();

    if (m == 0 && n == 0){ // when zero matrix then return
    return;
    }

    m_m = m;
    m_n = n; // save the marix size
    int matrix_size = m*n;

    // arraySize will give us the lenght but wont  always equal m*n so gotta be careful with that (Mark emphasized that in discord)
    // so we need the missing values to be made 0 auto
    int ideal_size = arraySize;

    if (ideal_size > matrix_size){
        ideal_size = matrix_size; // to prevent when the array is not the same size
    }

    // we want sumthing to count our zeroes n then non zero numbers
    // compressd m_values ignores 0
    m_nonzeros = 0;

    for (int i = 0; i <ideal_size; i++){

        if(array[i] != 0){
            
            m_nonzeros++;

        }

    }
    // make space for row index
    m_row_index = new int[m_m + 1];

    if (m_nonzeros > 0){ // only exists when we have actual non zeroes vals or else nullpt
   
        m_values = new int[m_nonzeros];
        m_col_index = new int[m_nonzeros];

    }
   
    // ns build the 3 arrays 
    // keep getting confused and mixing up rows n cols since array start at col 0 and row 0 unlike real matrix at which start at 1 :(
    int valueIndex = 0; // to help keep track of loc in compressed array

    m_row_index[0] = 0; // val being a CSR index 0

// going to comment everything for this loop for my own sanity cant lie
    for(int row = 0; row <m_m; row++){

        for(int col = 0; col < m_n; col++){ // this is just to read the orig array only so far so like which box of array 
            // first the pos in matrix , then pos in orgi array and the pos in compresed CSR array
            int basicIndex = row * m_n + col; // keep track of matrix pos in 1d array[] , basically 2d loc to 1d arry loc
            int value = 0;

            if(basicIndex <arraySize){
                value = array[basicIndex];
            }

            if (value != 0){
                m_values[valueIndex] = value;
                m_col_index[valueIndex] = col;
                valueIndex++;

            }

        }

        m_row_index[row+1] = valueIndex;

    }

}

int CSR::getAt(int row, int  col) const{
    if (row < 0 || row >= m_m || col < 0 || col>= m_n){
        throw runtime_error("Invalid matrix index");
    }

    int start = m_row_index[row]; // search the row for val in m_row_index in compres()
    int end = m_row_index[row + 1];

    for (int i = start; i < end; i++){

        if (m_col_index[i] == col){
            return m_values[i];
        }
        
    }
    
    return 0; // since CSR doesnt store 0s s assume thoat if not val found then its a zero.
} 

bool CSR::operator==(const CSR & rhs) const{
    // return true if we get the same matrix only
    // gotta compare them, look at size ,numbers,values...

    // but first gotta see if empty CSR
    if (empty() && rhs.empty()){
        return true;

    }

    if (empty() || rhs.empty()){
        return false; 
    }

    if (m_m != rhs.m_m || m_n != rhs.m_n|| m_nonzeros != rhs.m_nonzeros){
        return false; // if any dont dimension match send back false
    }

    for (int i = 0; i < m_nonzeros; i++){
        if (m_values[i] != rhs.m_values [i] || m_col_index[i] != rhs.m_col_index[i]){
            return false; //if values and col index dont match send back false
        }
    }

    for (int i = 0; i < m_m + 1; i++){
        if (m_row_index[i] != rhs.m_row_index[i]){
            return false; // if row index dont match then not same
        }
    }

    return true;

}

int CSR::sparseRatio(){
    // find the total empty space and send it as a percetage number

    //check for empty CSR
    if (empty()){
        return 0;
    }

    int total = m_m * m_n; // find the size 
    int zeros = total - m_nonzeros; // find the zeroes

    return(zeros * 100) / total; // convert to %

}
void CSR::dump(){
    cout << endl;
    if (!empty()){
        for (int i=0;i<m_nonzeros;i++)
            cout << m_values[i] << " ";
        cout << endl;
        for (int i=0;i<m_nonzeros;i++)
            cout << m_col_index[i] << " ";
        cout << endl;
        for (int i=0;i<m_m+1;i++)
            cout << m_row_index[i] << " ";
    }
    else
        cout << "The object is empty!";
    cout << endl;
}

//////////////CSRList Class Implementation///////////////
CSRList::CSRList(){
    m_head = nullptr; //empty linked list n rdy intial state
    m_size = 0;
}
CSRList::CSRList(const CSRList & rhs){
    // make new list with deep copies of all the CSR in rhs 
    // check orginial list, make new copy of each node, add new node at end of list , repeat
    // clear n initalize new list again
    m_head = nullptr;
    m_size = 0;
    
    CSR* orig = rhs.m_head; // folow the orignal list
    CSR* tail = nullptr;    // last node of this new list
    while (orig != nullptr){

    
        //make  node in the copy of the CSR
        CSR* newNode = new CSR(*orig); // reuse old copy constructer and make newNode our start of deep copy
    
        if(m_head == nullptr){ 
            m_head = newNode;
            tail = newNode;  // now the head and tail point to new node of copy
            
        }
   
        else {
            tail->m_next = newNode; // so newNode points to now next node in the copy, and head to the preivoous node
            tail = newNode; // now tail is at end of next node
        }

        m_size++;
        orig = orig->m_next; // move down the orignal list

    }

}
CSRList::~CSRList(){
    clear(); // clear func does this alr
}
bool CSRList::empty() const{

    return m_head == nullptr; // check if there is even a node to start at
}
void CSRList::insertAtHead(const CSR & matrix){
    
    CSR* newNode = new CSR(matrix); // call on copy constructor n deep cop matrix in new node

    newNode->m_next = m_head;  //connect to old head
    m_head = newNode; // move the head to newNode again
    
    m_size++;
}
void CSRList::clear(){ // to clear out nodes , save n move the ptr before delete it 
    CSR* temp;
    while (m_head != nullptr){
        temp = m_head;
        m_head = m_head->m_next; //move list foward b4 deleting first node  
        delete temp; // delete temp instead
    } //deleting node auto clears out CSR arrays too 
    m_size = 0;
    
}

int CSRList::getAt(int CSRIndex, int row, int col) const{
    // first make sure the CSR index is Valid n in list
    if(CSRIndex < 0 || CSRIndex >= m_size){
        throw runtime_error("Object isnt in list");
    }

    CSR* temp = m_head; //temp CSR as the head
    for (int i = 0; i <CSRIndex; i++){
        temp = temp->m_next; // go through one by one n change

    }

    return temp->getAt(row,col); //give back the location and assk getAt to find snce i set it up eariler n does same thing

}
bool CSRList::operator== (const CSRList & rhs) const{
    // check to see if the order is same and size also
    if (m_size != rhs.m_size){
        return false; // diff size? then false
    }

    //check order now
    CSR* left = m_head;
    CSR* right = rhs.m_head;

    while (left != nullptr && right != nullptr){ // check right alr but might as well to be safe
         if (!(*left == *right)){
            return false;
         }
    
    // go down the lists
    left = left->m_next;
    right = right->m_next;

    }

    return true;

}
const CSRList& CSRList::operator=(const CSRList & rhs){
    // this one deals with if the matrix has not new or data in
    // first check if same object 
    if (this == &rhs){
        return *this;
    }

    // clear and initialize
    clear();
    CSR* orig = rhs.m_head;
    CSR* tail = nullptr;
    while (orig != nullptr){ //reusing code from CSRList since same logic now

    
        //make  node in the copy of the CSR
        CSR* newNode = new CSR(*orig); // reuse old copy constructer and make newNode our start of deep copy
    
        if(m_head == nullptr){ 
            m_head = newNode;
            tail = newNode;  // now the head and tail point to new node of copy
    
        }
   
        else {
            tail->m_next = newNode; // so newNode points to now next node in the copy, and head to the preivoous node
            tail = newNode; // now tail is at end of next node
        }

        m_size++;
        orig = orig->m_next; // move down the orignal list

    }

    return *this; // by ref

    
}
int CSRList::averageSparseRatio(){
    // call CSR object seperately n then calc
    //fisrst check for empty
    if(empty()){
        return 0;
    }

    int start = 0;
    CSR* temp = m_head;

    while (temp != nullptr){
        start += temp->sparseRatio(); // sump up each node ration from func as moving 
        temp = temp->m_next; // point temp to next ptr

    }

    return start / m_size;
}
void CSRList::dump(){
    if (!empty()){
        CSR* temp = m_head;
        while (temp != nullptr){
            temp->dump();
            temp = temp->m_next;
        }
    }
    else
        cout << "Error: List is empty!" << endl;
}
