#include <vector>
#include <string>
#include <stdio.h>
#include <cstring>
#include <bitset>
#include <iostream>
#include <algorithm> 
#include <iterator>
#include <fstream>
#include <sstream>
#include "library.h"


int main(int argc, const char * argv[]){
	std::ifstream	file(argv[1]);
	std::string		line, tmp;
	bool new_page = true;
	int records=0, pages=0;
	int record_size, page_size, rid;
	page_size = atoi(argv[3]);
	//std::cout << page_size << "\n";
	Page *page = new Page;
	

	while(file.good()){
		getline(file, line);
    	
    	std::stringstream	lineStream(line);
    	std::string 		cell;

	    while(std::getline(lineStream,cell, ','))
	    {
	        tmp+=cell;
	    }
	    //std::cout << tmp << strlen(tmp.c_str()) << "\n" ; 
	    record_size = strlen(tmp.c_str());
	    if(new_page){
	    	pages++;
	    	init_fixed_len_page(page, page_size, record_size);
	    	new_page = false;
	    }
	    if (record_size>0){
			
	    	records++;
	    	/* create record */
	    	char* buf = new char[record_size+1];
			
		   	std::strcpy(buf, tmp.c_str()); 
			
		   	Record record;
		   	fixed_len_read(buf, record_size, &record);
			
		   	//std::cout << fixed_len_sizeof(&record) << "\n";
		   	tmp = "";
			
	    	delete[] buf;
			
	    	/*write record to page*/
	    	rid = add_fixed_len_page(page, &record);
			
	    	//printf("sup \n");
	    	if (rid < 0){
				FILE *fp = fopen(argv[2], "a");

	    		write_page_to_file(page, fp);
	    		pages++;
	    		delete page;
	    		page = new Page;
	    		init_fixed_len_page(page, page_size, record_size);
	    		rid = add_fixed_len_page(page, &record);
	    	}

	    }
	   	
	}
	
	FILE *fp = fopen(argv[2], "a");

	write_page_to_file(page, fp);
	delete page;
	std::cout << "Number of records " << records << "\n";
	std::cout << "Number of pages " << pages << "\n";
	std::cout << "TBD TIME" <<  "\n";
	return 0;
}
