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

/*void page_to_csv(Page *page){
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
}*/

int main(int argc, const char * argv[]){
	const char* csv = "lel.csv";
	std::string		line, tmp;
	bool new_page = false;
	int records=0, pages=0;
	int record_size, page_size, rid, pid;
	page_size = atoi(argv[2]);
	//std::cout << page_size << "\n";
	Record *record;

	FILE *fp = fopen(argv[1], "r+");
	Heapfile *heapfile = new Heapfile;
	heapfile->file_ptr = fp;
	heapfile->page_size = page_size;

/*	Page *rpage; //for reading

	for (int i =0; i < 3; i++){
		rpage =  new Page;
		read_page(heapfile, i, rpage);

		page_to_csv(rpage);

		delete rpage;
	}*/

	RecordIterator iterator(heapfile);


	
	while (iterator.hasNext()) {
		record = new Record;
		record = iterator.next();
		write_record_to_csv(record, csv);
		delete record;
	}
	fclose(fp);
	return 0;
}