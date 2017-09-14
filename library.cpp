#include <vector>
#include <string>
#include <stdio.h>
#include <cstring>
#include <bitset>
#include <iostream>
#include <algorithm> 
#include <sstream>
#include <iterator>
#include <fstream>
#include "library.h"



int atr_len = 10;

/*int main(){
	Record rec(3);
    rec[0] = "oneABCDEFG";
    rec[1] = "twoABCDEFG";
    rec[2] = "threeABCDE";

    Record res(2);

    int bufSize =  fixed_len_sizeof(&rec);
    printf("sizeof rec = %i\n", bufSize);


    char* buf = new char[bufSize];

    fixed_len_write(&rec, buf);
    printf("BUFFAR : %s\n", buf);
    fixed_len_read(buf, fixed_len_sizeof(&res), &res);
    //std::cout << fixed_len_sizeof(&res) << "\n";
   	//std::cout << res [0] << res[1] << "\n";
   
	Page *page = new Page;
	init_fixed_len_page(page, 4096, 1000);
	//std::cout << fixed_len_page_capacity(page) << "\n";
	//std::cout << fixed_len_page_freeslots(page) << "\n";
	//std::cout << add_fixed_len_page(page, &res) << "\n";
	//std::cout << add_fixed_len_page(page, &rec) << "\n";
	return 0;
}*/
/**
 * Compute the number of bytes required to serialize record
 */
int fixed_len_sizeof(Record *record){

    Record r = *record;
    // Plus 1 for terminal byte
    return r.size() * atr_len;

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

    //printf("%s\n", tmp.c_str());
   	std::memcpy(buf, tmp.c_str(), fixed_len_sizeof(record)); 
    /*buf = tmp.c_str();*/
    
}

/**
 * Deserializes `size` bytes from the buffer, `buf`, and
 * stores the record in `record`.
 */
void fixed_len_read(char *buf, int size, Record *record){

    int tuples = size/atr_len;
	std::string tmp;

    for(int i = 0; i < tuples; i++){
    	for(int j = i*atr_len; j < i*atr_len+atr_len; j++){

    		char elem = buf[j];
    		
    		tmp+= elem;
    	}
    	//printf("%s\n", tmp.c_str());
    	(*record).push_back(tmp);
    	tmp="";
    	
    }
	
}


/**
 * Initializes a page using the given slot size
 */
void init_fixed_len_page(Page *page, int page_size, int slot_size){

	page->page_size = page_size;
	page->slot_size = slot_size;
	
	
	int free_slots = fixed_len_page_capacity(page);
	//std::cout << free_slots << " slots \n";

	Bitmap map(free_slots);
	map.assign(free_slots, 0);

	//std::cout << "dunzo the nunzo\n";

	page->data = new char[page_size];
	page->map = map;

}
/**
 * Calculates the maximal number of records that fit in a page
 */
int fixed_len_page_capacity(Page *page){
	int free_slots = page->page_size / page->slot_size;
	int size = free_slots + free_slots*page->slot_size;
	if (size > page->page_size){
		free_slots--;
	}
	return (free_slots);
}
/**
 * Calculate the free space (number of free slots) in the page
 */
int fixed_len_page_freeslots(Page *page){
	int slots_left = std::count (page->map.begin(), page->map.end(), 0);;
	return slots_left;
}
/**
 * Add a record to the page
 * Returns:
 *   record slot offset if successful,
 *   -1 if unsuccessful (page full)
 */
int add_fixed_len_page(Page *page, Record *r){
	int len = fixed_len_sizeof(r);
	char* buf = new char[len];
	fixed_len_write(r, buf);

	Bitmap::iterator location;

	location = std::find(page->map.begin(), page->map.end(), 0);
	int index =  ( location - page->map.begin() );
	//printf("sup %i\n", index);
	if (fixed_len_page_freeslots(page) != 0){
		std::memcpy(page->data+page->slot_size*index, buf, len);
		page->map[index] = 1;
		//printf("sup %i\n", index);
		return index;
	}
	
	return -1;
}

/**
 * Write a record into a given slot.
 */
void write_fixed_len_page(Page *page, int slot, Record *r){
	int len = fixed_len_sizeof(r);
	char* buf = new char[len];
	fixed_len_write(r, buf);
	std::memcpy(page->data+page->slot_size*slot, buf, len);
	page->map[slot] = 1;
}
/**
 * Read a record from the page from a given slot.
 */
void read_fixed_len_page(Page *page, int slot, Record *r){
	char* buf = new char[page->slot_size];
	std::memcpy(buf, page->data+page->slot_size*slot, page->slot_size);
	fixed_len_read(buf, page->slot_size, r);
}

void write_page_to_file(Page *page, FILE *fp){
	int map_size;
	std::ostringstream oss;
	if (!page->map.empty())
	  {
	    // Convert all but the last element to avoid a trailing ","
	    std::copy(page->map.begin(), page->map.end()-1,
	        std::ostream_iterator<char>(oss));
	    ////std::cout << oss.str() << "\n";
	    // Now add the last element with no delimiter
	    oss << page->map.back();
	  }	
	map_size =  oss.str().size();
	//std::cout << map_size << " len\n";
	//std::cout << fixed_len_page_capacity(page) << " slots\n";

	const char* map = oss.str().c_str();
	//printf("%s\n", page->data+page->page_size-sizeof(*map)-1);
	printf("%lu len\n", map_size);

	std::memcpy(page->data+page->page_size-map_size, map, map_size);
  	//std::cout << "this works\n";
	//printf("asd\n");

	fwrite(page->data, page->page_size, 1, fp);
	fflush(fp);
	delete[] page->data;
}

void write_record_to_csv(Record *record, const char* file){
	std::ofstream ofs;
  	ofs.open (file, std::ofstream::out | std::ofstream::app);
  	int vsize = (*record).size();
  	////std::cout << "asjkfhlasjhlaglkshaga" << std::endl;
	for (int n=0; n<vsize-1; n++){

	    ofs << (*record)[n] << ",";
	}
	ofs << (*record)[vsize-1] << std::endl;
}


/**
 * Initalize a heapfile to use the file and page size given.
 */
void init_heapfile(Heapfile *heapfile, int page_size, FILE *file) {
	heapfile->page_size = page_size;
	heapfile->file_ptr = file;

	// Initialize header page
	//Page *headerPage = new Page;

	//init_fixed_len_page(headerPage, heapfile->page_size, slot_size);

	DirHeader *dirHeader = new DirHeader;
	dirHeader->next = 0;
	dirHeader->num_entries = 0; //how many entries this directory currently holds
	dirHeader->capacity = (page_size-sizeof(DirHeader))/sizeof(DirEntry); // how many entries this directory can hold;

	//std::cout << "INIT HEAP: capacity = " << dirHeader->capacity << std::endl;

	//char* buf = new char[sizeof(dirHeader)];


	// Write dirHeader to beginning of headerPage
	//std::memcpy(headerPage->data, dirHeader, dirHeadSize);
	
	// turn the first bit off after adding first record to slot
	//headerPage.map[0] = 1;
	rewind(heapfile->file_ptr);
	fwrite(dirHeader, sizeof(DirHeader), 1, file); 
	char *garbage = new char[page_size-sizeof(DirHeader)];
	fwrite(garbage, page_size-sizeof(DirHeader), 1, file); 
	delete garbage;


	// Write headerPage to heapfile
	//write_page_to_file(headerPage, file); // this is redundant if we're writing the header 
	//directly into the file
	delete dirHeader;
}

/**
 * Allocate another page in the heapfile.  This grows the file by a page.
 * TODO: handle case where directory is full and must create new dir page
 */
PageID alloc_page(Heapfile *heapfile) {
	int slot_size = 1000;
	Page *page = new Page;
	init_fixed_len_page(page, heapfile->page_size, slot_size);
	DirHeader *dirHeader = new DirHeader;

	rewind(heapfile->file_ptr);
	read_header_from_heapfile(dirHeader, heapfile);
	dirHeader->num_entries++;
	// calculate new pageID
	int write_loc;
	//std::cout << dirHeader->num_entries << " entries\n";
	unsigned long offset = (heapfile->page_size*(dirHeader->num_entries));

	// Create dirEntry for new page0
	DirEntry *dirEntry = new DirEntry;
	dirEntry->page_offset = offset;
	dirEntry->free_slots = fixed_len_page_freeslots(page);


	write_loc = fseek(heapfile->file_ptr, offset,SEEK_SET);

	//std::cout << offset << " offset " << write_loc << " error\n";
	//std::cout << heapfile->file_ptr << " a4 pointer\n";
	
	write_page_to_file(page, heapfile->file_ptr);	//make logic more logical

	// Write dirEntry to directory
	rewind(heapfile->file_ptr);

	// get offset to write new directory entry
	offset = sizeof(DirHeader)+sizeof(DirEntry)*(dirHeader->num_entries-1);
	write_loc = fseek(heapfile->file_ptr, offset,SEEK_SET);

	fwrite(dirEntry, sizeof(DirEntry), 1, heapfile->file_ptr);
	rewind(heapfile->file_ptr);
	fwrite(dirHeader, sizeof(DirHeader), 1, heapfile->file_ptr);
	fflush(heapfile->file_ptr);
	PageID p = dirHeader->num_entries-1;
	delete page;
	delete dirHeader;
	delete dirEntry;
	return p;
}

/**
 * Read a page into memory
 */
void read_page(Heapfile *heapfile, PageID pid, Page *page){
	int slot_size = 1000;
	init_fixed_len_page(page, heapfile->page_size, slot_size);
	DirHeader *dirHeader = new DirHeader;
	rewind(heapfile->file_ptr);
	read_header_from_heapfile(dirHeader, heapfile);
	// get offset to dirEntry
	unsigned long offset = sizeof(DirHeader)+sizeof(DirEntry)*(pid);
	DirEntry *dirEntry = new DirEntry;
	
	fseek(heapfile->file_ptr, offset,SEEK_SET);
	fread(reinterpret_cast<char*>(dirEntry), sizeof(DirEntry), 1, heapfile->file_ptr);

	fseek(heapfile->file_ptr, dirEntry->page_offset,SEEK_SET);
	//std::cout << "this werqs\n";
	fread(page->data, heapfile->page_size, 1, heapfile->file_ptr);
	//std::cout << "this werqs\n";
	//printf("%s\n", page->data);
	
	rewind(heapfile->file_ptr);
	int records = fixed_len_page_capacity(page);
	char* raw_map = new char[records];
	std::memcpy(raw_map, page->data+page->page_size-records, records);
	Bitmap map(raw_map, raw_map+strlen(raw_map));
	page->map = map;
}

/**
 * Write a page from memory to disk
 */
void write_page(Page *page, Heapfile *heapfile, PageID pid){
	DirHeader *dirHeader = new DirHeader;
	////std::cout << page->slot_size << " page slot size\n";
	rewind(heapfile->file_ptr);
	read_header_from_heapfile(dirHeader, heapfile);
	unsigned long offset = sizeof(DirHeader)+sizeof(DirEntry)*(pid);
	DirEntry *dirEntry = new DirEntry;

	fseek(heapfile->file_ptr, offset,SEEK_SET);
	fread(reinterpret_cast<char*>(dirEntry), sizeof(DirEntry), 1, heapfile->file_ptr);

	//std::cout << dirEntry->page_offset << " page offset\n";
	fseek(heapfile->file_ptr, dirEntry->page_offset,SEEK_SET);
	write_page_to_file(page, heapfile->file_ptr);	//make logic more logical
}


/**
 * Reads the header of the first directory page
 * dirHeader    :    dirHeader struct to read into
**/
void read_header_from_disk(DirHeader *dirHeader, const char* heapPath) {
    std::ifstream fstr;

    //std::cout << "Loading (from disk) heapfile...\n";
    fstr.open(heapPath);
    fstr.read(reinterpret_cast<char*>(dirHeader), sizeof(DirHeader));
    fstr.close();

    //std::cout << dirHeader->num_entries << " entries in dirHeader\n";
    //std::cout << dirHeader->capacity << " capacity\n";
    //std::cout << dirHeader->next << " next dir page\n";
}

/**
 * Given HeapFile reads the header of first dirPage
 * dirHeader    :    dirHeader struct to read into
**/
void read_header_from_heapfile(DirHeader *dirHeader, Heapfile *heapfile) {
    
    //std::cout << "Loading (from narnia) heapfile...\n";
    // read first 20 bytes of file_ptr from heapfile
    fflush(heapfile->file_ptr);
	fread(reinterpret_cast<char*>(dirHeader), sizeof(DirHeader), 1, heapfile->file_ptr);
	//std::memcpy(&dirHeader, heapfile->file_ptr, sizeof(dirHeader));

    //std::cout << dirHeader->num_entries << " entries in dirHeader\n";
    //std::cout << dirHeader->capacity << " capacity\n";
    //std::cout << dirHeader->next << " next dir page\n";
}

RecordIterator::RecordIterator(Heapfile *heap_file)
{
	heapfile = heap_file;
	int slot_size = 1000;
	recordId = new RecordID;
	recordId->page_id = -1;
	recordId->slot = 0;
	dirHeader = new DirHeader;
	read_header_from_heapfile(dirHeader, heapfile);


    
    // if there is a  data page
	if (dirHeader->num_entries != 0) {
		recordId->page_id = 0;
		page = new Page;
		init_fixed_len_page(page, heapfile->page_size, slot_size);
		
	    read_page(heapfile, 0, page);
	}
}

Record* RecordIterator::next()
{
	
	if (recordId->page_id == -1) {
		printf("No record!\n");
		exit(1);
	}
	int next_page_id = recordId->page_id;

	

	Record *record = new Record;

	int page_capacity = fixed_len_page_capacity(page);

	// search the current data page for record
	//std::cout << page_capacity<< " capacity" << std::endl;
	if (recordId->slot < page_capacity) {
		for (int slot = recordId->slot  ; slot < page_capacity; slot++) {
			//std::cout << slot << " current slot" << std::endl;
			if (page->map[slot]==1) {
				recordId->slot = slot+1;
				
				read_fixed_len_page(page, slot, record);
				//std::cout << slot << " current slot " << (*record)[0]<< std::endl;
				return record;
			}
		}
	}

	// search the next data pages for a record
	next_page_id++;
	//std::cout << next_page_id << " next page" << std::endl;
	if(next_page_id < dirHeader->num_entries) {
			delete page;
			page = new Page;
			read_page(heapfile, next_page_id, page);
			for (int slot = 0; slot < page_capacity; slot++) {
				if (page->map[slot]==1) {
					recordId->page_id = next_page_id;
					recordId->slot = slot+1;
					read_fixed_len_page(page, slot, record);

					return record;
				}
			}
		}


	/*if(dirHeader->next != 0){
		if(next_page_id < dirHeader->num_entries) {
			delete page;
			page = new Page;
			read_page(heapfile, next_page_id, page);
			for (int slot = 0; slot < page_capacity; slot++) {
				if (page->map[slot]==1) {
					recordId->page_id = next_page_id;
					recordId->slot = slot;
					read_fixed_len_page(page, slot, record);

					return record;
				}
			}
		}
		delete dirHeader;
		dirHeader = new DirHeader;
		read_header_from_heapfile(dirHeader, heapfile);
	}
*/
	
		printf("No next record.\n");
		exit(1);
	
}

bool RecordIterator::hasNext()
{
	
	if (recordId->page_id == -1) {
		return false;
	}
	int next_page_id = recordId->page_id;
	

	int page_capacity = fixed_len_page_capacity(page);

	// search the current data page for record
	if (recordId->slot < page_capacity) {
		for (int slot = recordId->slot; slot < page_capacity; slot++) {
			if (page->map[slot]==1) {
				//std::cout << recordId->slot << " has slot" << std::endl;
				return true;
			}
		}
	}

	// search the next data pages for a record
	next_page_id++;
	//std::cout << next_page_id << " has page" << std::endl;
	//std::cout << dirHeader->num_entries << " num entries" << std::endl;
	if(next_page_id < dirHeader->num_entries) {
			delete page;
			page = new Page;
			read_page(heapfile, next_page_id, page);
			//std::cout << dirHeader->num_entries << " num entries" << std::endl;
			for (int slot = 0; slot < page_capacity; slot++) {
				if (page->map[slot]==1) {
					

					return true;
				}
			}
		}


	/*if(dirHeader->next != 0){
		if (next_page_id < dirHeader->num_entries) {
			delete page;
			page = new Page;
			read_page(heapfile, next_page_id, page);
			for (int slot = 0; slot < page_capacity; slot++) {
				if (page->map[slot]==1) {

					return true;
				}
			}
		}
		delete dirHeader;
		dirHeader = new DirHeader;
		read_header_from_heapfile(dirHeader, heapfile);
	}*/

	
	return false;
	


}