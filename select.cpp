

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
	int attr_id = atoi(argv[2]);
	std::string start(argv[3]);
	std::string end(argv[4]);
	std::string		line, tmp;
	bool new_page = false;
	int records=0, pages=0;
	int record_size, page_size, rid, pid;
	page_size = atoi(argv[5]);
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

	Record results;
	
	while (iterator.hasNext()) {
		record = new Record;
		record = iterator.next();
		if ((*record)[attr_id] >= start && (*record)[attr_id] <= end){
			results.push_back((*record)[attr_id]);
		}
		delete record;
	}
	const char* csv = "results.csv";
	std::ofstream ofs;
  	ofs.open (csv, std::ofstream::out | std::ofstream::app);
  	int vsize = results.size();
  	////std::cout << "asjkfhlasjhlaglkshaga" << std::endl;
	for (int n=0; n<vsize; n++){

	    ofs << results[n] << "," << std::endl;
	}

	fclose(fp);
	return 0;
}