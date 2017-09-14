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
	std::string s(argv[2]);
	std::string delimiter = ":";
	std::string token = s.substr(0, s.find(delimiter)); 
	s.erase(0, s.find(delimiter) + delimiter.length());
	std::cout << token << " "<< s << std::endl;


	std::string		line, tmp;

	PageID p = atoi(token.c_str());
	RecordID recordId;
	recordId.page_id = p;
	recordId.slot = atoi(s.c_str());
	std::cout << recordId.page_id << " "<< recordId.slot << std::endl;



	int record_size, page_size;
	page_size = atoi(argv[3]);
	//std::cout << page_size << "\n";
	Page *page = new Page;

	FILE *fp = fopen(argv[1], "r+");
	Heapfile *heapfile = new Heapfile;
	heapfile->file_ptr = fp;
	heapfile->page_size = page_size;


	read_page(heapfile, recordId.page_id, page);
	page->map[recordId.slot] = 2;
	write_page(page, heapfile, recordId.page_id);

	return 0;
}