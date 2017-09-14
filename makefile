CC = g++ 

#library: a2p3.cpp definitions.h
	#$(CC) -o $@ $<
all: library.o write_fixed_len_pages read_fixed_len_page csv2heapfile insert scan update delete select

library.o: library.cpp library.h
	$(CC) -o $@ -c $<
 
write_fixed_len_pages: write_fixed_len_pages.cpp library.o
	$(CC) -o $@ $< library.o	

read_fixed_len_page: read_fixed_len_page.cpp library.o
	$(CC) -o $@ $< library.o	

csv2heapfile: csv2heapfile.cpp library.o
	$(CC) -o $@ $< library.o	

insert: insert.cpp library.o
	$(CC) -o $@ $< library.o	

scan: scan.cpp library.o
	$(CC) -o $@ $< library.o	

update: update.cpp library.o
	$(CC) -o $@ $< library.o	

delete: delete.cpp library.o
	$(CC) -o $@ $< library.o	

select: select.cpp library.o
	$(CC) -o $@ $< library.o	