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
	const char* csv = "lel.csv";
	FILE *fp = fopen(argv[1], "r");
	std::string		line, tmp;
	bool new_page = true;
	int records=0, pages=0;
	int record_size, page_size, rid;
	page_size = atoi(argv[2]);
	//std::cout << page_size << "\n";
	Record record(100);
	record_size = fixed_len_sizeof(&record);
	printf("%i\n", record_size);
	char* buffer = new char[page_size];

	while (fread(buffer, 1, page_size, fp) == page_size){
		Page page;
		init_fixed_len_page(&page, page_size, record_size);
		std::memcpy(page.data, buffer, page_size);

		records = fixed_len_page_capacity(&page);
		char* raw_map = new char[records];
		std::memcpy(raw_map, page.data+page.page_size-records, records);
		printf("%s\n", raw_map);
		printf("%i records, %lu length\n", records, strlen(raw_map));
		Bitmap map(raw_map, raw_map+strlen(raw_map));
		page.map = map;
		for(int slot = 0; slot <records ; slot++){
			printf("slot %c\n", map[slot]);
			if (map[slot]==1){
				Record record;
				read_fixed_len_page(&page, slot, &record);
				write_record_to_csv(&record, csv);
			}
		}
		
	}
	fclose(fp);
	return 0;
}