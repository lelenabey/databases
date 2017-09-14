

typedef std::string V;
typedef std::vector<V> Record;

typedef std::vector<char> Bitmap;

typedef struct {
    char *data;
    int page_size;
    int slot_size;
    Bitmap map;
} Page;

int fixed_len_sizeof(Record *record);
void fixed_len_write(Record *record, char *buf);
void fixed_len_read(char *buf, int size, Record *record);

void init_fixed_len_page(Page *page, int page_size, int slot_size);
 

int fixed_len_page_capacity(Page *page);
 

int fixed_len_page_freeslots(Page *page);
 

int add_fixed_len_page(Page *page, Record *r);
/**
 * Write a record into a given slot.
 */
void write_fixed_len_page(Page *page, int slot, Record *r);
/**
 * Read a record from the page from a given slot.
 */
void read_fixed_len_page(Page *page, int slot, Record *r);

void write_page_to_file(Page *page, FILE* fp);

// Heap headers
typedef struct {
    FILE *file_ptr;
    int page_size;
} Heapfile;

typedef int PageID;
 
typedef struct {
    int page_id;
    int slot;
} RecordID;

/**
 * DirHeader, 16 bytes
 * capacity     : how many directory entries fit here?
 * num_entries	: # of valid entries in directory
 * next         : offest to next dir page
**/
typedef struct {
	int page_size;
    int num_entries; 
    int capacity;
    unsigned long next;
} __attribute__((packed)) DirHeader;

/**
 * DirEntry, 12 bytes
 * page_offset	: offset to this page
**/
typedef struct { 
	unsigned long page_offset;
	int free_slots;
}  __attribute__((packed))  DirEntry;

// custom functions

void serialize_dir_header(DirHeader *dirHeader, char *buf);
void deseralize_dir_header(char *buf, DirHeader *dirHeader);
void update_header(Heapfile *heapfile, DirHeader *dirHeader);


/**
 * Initalize a heapfile to use the file and page size given.
 */
void init_heapfile(Heapfile *heapfile, int page_size, FILE *file);

/**
 * Allocate another page in the heapfile.  This grows the file by a page.
 */
PageID alloc_page(Heapfile *heapfile);

/**
 * Read a page into memory
 */
void read_page(Heapfile *heapfile, PageID pid, Page *page);

/**
 * Write a page from memory to disk
 */
void write_page(Page *page, Heapfile *heapfile, PageID pid);

void read_header_from_heapfile(DirHeader *dirHeader, Heapfile *heapfile);

void write_record_to_csv(Record *record, const char* file);

class RecordIterator {
	Heapfile *heapfile;
	DirHeader *dirHeader;
	Page *page;
	RecordID *recordId;
    public:
    RecordIterator(Heapfile *heapfile);
    Record* next();
    bool hasNext();
};