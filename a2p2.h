#ifndef A2P2_H
#define A2P2_H

int fixed_len_sizeof(Record *record);
void fixed_len_write(Record *record, char *buf);
void fixed_len_read(char *buf, int size, Record *record);

#endif