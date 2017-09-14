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


int dirHeadSize = 16;
int dirEntrySize = 12;
int slot_size = 1000;

void generate_page(Page *page, int page_size){
	std::ifstream	file("test.csv");
	std::string		line, tmp;
	bool new_page = true;
	int records=0, pages=0;
	int record_size, rid=0;
	//std::cout << page_size << "\n";
	

	while(file.good() && rid>=0){
		getline(file, line);
    	
    	std::stringstream	lineStream(line);
    	std::string 		cell;

	    while(std::getline(lineStream,cell, ','))
	    {
	        tmp+=cell;
	    }
	    std::cout << tmp << strlen(tmp.c_str()) << " RECOJDSK \n" ; 
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
			
	    }
	   	
	}
}

void page_to_csv(Page *page){
	const char* csv = "lel.csv";

	std::string		line, tmp;

	int records=0, pages=0;
	int record_size, page_size, rid;

	records = fixed_len_page_capacity(page);
	char* raw_map = new char[records];
	std::memcpy(raw_map, page->data+page->page_size-records, records);
	printf("map %lu\n", raw_map);
	printf("%i records, %lu length\n", records, strlen(raw_map));
	Bitmap map(raw_map, raw_map+strlen(raw_map));
	page->map = map;
	for(int slot = 0; slot <records ; slot++){
		printf("slot %c\n", map[slot]);
		if (map[slot]==1){
			Record record;
			read_fixed_len_page(page, slot, &record);
			write_record_to_csv(&record, csv);
		}
	}
}

int main() {
	const char* heapPath = "testheap";
	Heapfile *heapfile = new Heapfile;
	FILE *fp = fopen(heapPath, "w+");

	init_heapfile(heapfile, 4096, fp);
	std::cout << alloc_page(heapfile) << " PAGEDDIS\n";
	std::cout << alloc_page(heapfile) << " PAGEDDIS\n";
	std::cout << alloc_page(heapfile) << " PAGEDDIS\n";
	//FILE *rfp = fopen(heapPath, "r");
	DirHeader *dirHeader = new DirHeader;
	//read_header_from_disk(dirHeader, heapPath);
	rewind(heapfile->file_ptr);
	read_header_from_heapfile(dirHeader, heapfile);

	Page *wpage = new Page; //for writing
	Page *rpage = new Page; //for reading

	generate_page(wpage, heapfile->page_size);

	//write_fixed_len

	write_page(wpage, heapfile, 2);
	read_page(heapfile, 2, rpage);

	page_to_csv(rpage);

	fclose(fp);
	return 0;
}

