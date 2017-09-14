#include <vector>
#include <string>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include "definitions.h"
#include "a2p2.h"





int main() {

    Record rec(3);
    rec[0] = "one~~";
    rec[1] = "two~~";
    rec[2] = "three";

    Record res(2);

    int bufSize =  fixed_len_sizeof(&rec);
    printf("sizeof rec = %i\n", bufSize);


    char* buf = new char[bufSize];

    fixed_len_write(&rec, buf);
    printf("BUFFAR : %s\n", buf);
    fixed_len_read(buf, fixed_len_sizeof(&res), &res);
    std::cout << fixed_len_sizeof(&res) << "\n";
   	std::cout << res [0] << res[1] << "\n";
    return 0;

}


/**
 * Compute the number of bytes required to serialize record
 */
int fixed_len_sizeof(Record *record){

    Record r = *record;
    // Plus 1 for terminal byte
    return r.size() * sizeof(r[0]) + 1;

}

/**
 * Serialize the record to a byte array to be stored in buf.
 */
void fixed_len_write(Record *record, char *buf) {

    std::string tmp;

    for(Record::size_type i = 0; i != record->size(); i++) {
        /*const char* elem = (*record)[i];*/
        /*printf("record[%lu] = %s\n", i, elem);*/
        tmp += (*record)[i];
    }

    printf("%s\n", tmp.c_str());
   	std::strcpy(buf, tmp.c_str()); 
    /*buf = tmp.c_str();*/
    
}

/**
 * Deserializes `size` bytes from the buffer, `buf`, and
 * stores the record in `record`.
 */
void fixed_len_read(char  *buf, int size, Record *record){

    int tuples = size/recordLen;
	std::string tmp;

    for(int i = 0; i < tuples; i++){
    	for(int j = i*recordLen; j < i*recordLen+recordLen; j++){

    		char elem = buf[j];
    		
    		tmp+= elem;
    	}
    	(*record)[i] = tmp;
    	tmp="";
    	
    }

}