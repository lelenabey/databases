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

int get_last_page(Heapfile *heapfile, Page *page){
	DirHeader *dirHeader = new DirHeader;
	rewind(heapfile->file_ptr);

	read_header_from_heapfile(dirHeader, heapfile);
	int pid = dirHeader->num_entries-1;
	std::cout << "num entries " << pid+1 << std::endl;
	read_page(heapfile, pid, page);

	Bitmap::iterator location;

	location = std::find(page->map.begin(), page->map.end(), 0);
	int index =  ( location - page->map.begin() );

	if (index >= page->map.size()){
		return -1;
	}
	return pid;
}


int main(int argc, const char * argv[]){
	std::ifstream	file(argv[2]);
	std::string		line, tmp;
	bool new_page = false;
	int records=0, pages=0;
	int record_size, page_size, rid, pid;
	page_size = atoi(argv[3]);
	//std::cout << page_size << "\n";
	Page *page = new Page;

	FILE *fp = fopen(argv[1], "r+");
	Heapfile *heapfile = new Heapfile;
	heapfile->file_ptr = fp;
	heapfile->page_size = page_size;

	pid = get_last_page(heapfile, page);
	std::cout << "PID of last page " << pid << std::endl;
	if (pid < 0){
		delete page;
		page = new Page;
		new_page = true;
	}
	

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
	    	pid = alloc_page(heapfile);
	    	std::cout << "PID of new page " << pid << std::endl;
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
	    		write_page(page, heapfile, pid);
	    		pages++;
	    		delete page;
	    		page = new Page;
	    		init_fixed_len_page(page, page_size, record_size);
	    		pid = alloc_page(heapfile);
	    		rid = add_fixed_len_page(page, &record);
	    	}

	    }
	   	
	}
	
	write_page(page, heapfile, pid);
	delete page;
	delete heapfile;

	fclose(fp);
	std::cout << "Number of records " << records << "\n";
	std::cout << "Number of pages " << pages << "\n";
	std::cout << "TBD TIME" <<  "\n";
	return 0;
}