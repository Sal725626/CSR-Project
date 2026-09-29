#include "csr.h"

class Tester{
    
        
public:

    bool testCompressNorm(){
    //Making sure compress works and makes the righ CSR arrays (NORMAL CASE)
        int array[] ={10,0,20,0,30,40};
        CSR csr;
        csr.compress(2,3,array,6);


        if(csr.m_m != 2|| csr.m_n != 3 || csr.m_nonzeros != 4){
            return false; // check basic for matrix parameters
        }

    
    
    // here is what is expected the matrix to be
    int knownVal[] = {10, 20, 30, 40};
    int knownCols[] = {0, 2, 1, 2};
    int knownRows[] = {0, 2, 4};

    //compare to see if we got that ^

    for (int i = 0; i < csr.m_nonzeros; i++){

        if (csr.m_col_index[i] != knownCols[i]){
            return false; // match column with known with test

        }

        if (csr.m_values[i] != knownVal[i]){
            return false; // match known values withe test
        }

    }
    
    for (int i = 0; i< csr.m_m + 1; i++){ // +1 in the index since array start at 0
        
        if(csr.m_row_index[i] != knownRows[i]){
            return false;
        
        }
    }

        return true; //passed 
    }
bool testCompressSmallArray(){
    // test to see when the array is too small for the matrix
    // the missing spots will be 0 naturally
    int array[] = {10, 20, 30, 40};

    CSR csr;
    csr.compress(2, 3, array, 4);

    if(csr.m_m != 2 || csr.m_n != 3 || csr.m_nonzeros != 4){
        return false;
    }

    // our known values that we will compare the test values 
    int knownVal[] = {10, 20, 30, 40};
    int knownCols[] = {0, 1, 2, 0};
    int knownRows[] = {0, 3, 4};

    //compare known with test
    for (int i = 0; i < csr.m_nonzeros; i++){

        if (csr.m_values[i] != knownVal[i] || csr.m_col_index[i] != knownCols[i]){
            return false; // dont match then fail tetst
        }
    }

    for (int i = 0; i <csr.m_m + 1; i++){

        if (csr.m_row_index[i] != knownRows[i]){
        return false;
    }
    }

    return true; //passed

}

bool testCompressZeroMat(){
    // Test will enter a Zero matrix in and it must stay empty even if we add in data

    int array[] = {10, 20, 30};

    CSR csr;
    csr.compress(0, 0, array, 3);
    // check for val, col index, row index, rows and cols being = 0 then if true then PASS
    if(csr.m_values == nullptr && csr.m_col_index == nullptr && csr.m_row_index == nullptr && csr.m_nonzeros == 0 && csr.m_m == 0 && csr.m_n == 0){
        return true;

    }
    return false;
}

bool testEqualityNorm(){
    // same matrix should be seens as equal

    int array[] = { 10, 0, 20, 0, 30, 40};

    CSR first;
    CSR next;

    first.compress(2, 3, array, 6);
    next.compress(2, 3, array, 6);

    if(first == next ){
        return true;
    }
    return false;

}

bool testEqualityEmpty(){
    // both empyt CSR bojects should be seen as equal
    
    CSR first;
    CSR next;

    if(first == next){
        return true;
    }
    return false;
}

bool testCSRGetAtError(){

    int array[] = {10, 20, 30, 40};

    CSR csr;
    csr.compress(2, 2, array, 4);

    try{
        csr.getAt(2,0);

        // autoomatcially fail if we even get this far 
        return false;
    }

    catch(runtime_error & e){
        return true;
    }

}

bool testAssignmentNormal(){
    // normal case: assignment should make a deep copy of the whole list

    int array1[] = {10, 0,
                     0, 20};

    int array2[] = {0, 30,
                   40,  0};

    CSR first;
    CSR second;

    first.compress(2, 2, array1, 4);
    second.compress(2, 2, array2, 4);

    CSRList source;
    source.insertAtHead(first);
    source.insertAtHead(second);

    CSRList copy;
    copy = source;

    // Lists should contain the same number of nodes
    if (copy.m_size != source.m_size){
        return false;
    }

    // Heads should NOT be the same node in memory
    if (copy.m_head == source.m_head){
        return false;
    }

    CSR* sourceNode = source.m_head;
    CSR* copyNode = copy.m_head;

    // Move through both lists together
    while (sourceNode != nullptr && copyNode != nullptr){

        // Nodes should not be the same objects in memory
        if (sourceNode == copyNode){
            return false;
        }

        // But their matrix contents should be equal
        if (!(*sourceNode == *copyNode)){
            return false;
        }

        // Internal dynamic arrays should also be different memory
        if (sourceNode->m_nonzeros > 0){

            if (sourceNode->m_values == copyNode->m_values ||
                sourceNode->m_col_index == copyNode->m_col_index){
                return false;
            }
        }

        if (sourceNode->m_row_index == copyNode->m_row_index){
            return false;
        }

        // Move both pointers to their next nodes
        sourceNode = sourceNode->m_next;
        copyNode = copyNode->m_next;
    }

    // Both should reach the end at the same time
    if (sourceNode == nullptr && copyNode == nullptr){
        return true;
    }

    return false;
}

bool testAssignmentEmpty(){
    // edge case: assigning an empty list to a populated list
    // should clear the populated list

    int array[] = {10, 20,
                   30, 40};

    CSR csr;
    csr.compress(2, 2, array, 4);

    CSRList full;
    full.insertAtHead(csr);

    CSRList empty;

    full = empty;

    if (full.m_head == nullptr &&
        full.m_size == 0){
        return true;
    }

    return false;
}


bool testCSRListGetAtError(){
    // error case where cannot get a CSR from an empty linked list

    CSRList list;

    try{
        list.getAt(0,0,0);

        // if we get this far then nothing was thrown
        return false;
    }

    catch(runtime_error & e){
        return true;
    }
}

bool testCSRListGetAtNormal(){
    // normal case where find a val insde the CSR store in a list
    
    int array[] = {10, 0, 30, 40};

    CSR csr;
    csr.compress(2, 2, array, 4);

    CSRList list;
    list.insertAtHead(csr);

    int result = list.getAt(0,1,0);

    if(result == 30){
        return true;
    }
    return false;
}


};

int main(){
    Tester tester;


    if (tester.testCompressNorm())
        cout << "Compress normal test has PASSED" << endl;
    else
        cout << "Compress normal test has FAILED" << endl;

    if (tester.testCompressSmallArray())
        cout << "Compress small array test has PASSED" << endl;
    else
        cout << "Compress small array test has FAILED" << endl;

    if (tester.testCompressZeroMat())
        cout << "Compress zero matrix test has PASSED" << endl;
    else
        cout << "Compress zero matrix test has FAILED" << endl;

    if (tester.testEqualityNorm())
    cout << "CSR equality normal test has PASSED" << endl;
    else
    cout << "CSR equality normal test has FAILED" << endl;

    if (tester.testEqualityEmpty())
    cout << "CSR equality empty test has PASSED" << endl;
    else
    cout << "CSR equality empty test has FAILED" << endl;

    if (tester.testCSRGetAtError())
    cout << "CSR getAt error test has PASSED" << endl;
    else
    cout << "CSR getAt error test has FAILED" << endl;
    if (tester.testAssignmentNormal())
    cout << "CSRList assignment normal test has PASSED" << endl;
    else
    cout << "CSRList assignment normal test has FAILED" << endl;


    if (tester.testAssignmentEmpty())
    cout << "CSRList assignment empty test has PASSED" << endl;
    else
    cout << "CSRList assignment empty test has FAILED" << endl;


    if (tester.testCSRListGetAtError())
    cout << "CSRList getAt error test has PASSED" << endl;
    else
    cout << "CSRList getAt error test has FAILED" << endl;


    if (tester.testCSRListGetAtNormal())
    cout << "CSRList getAt normal test has PASSED" << endl;
    else
    cout << "CSRList getAt normal test has FAILED" << endl;
    
}

