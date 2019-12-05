#ifndef _trainer_h
#define _trainer_h

int read_file_to_set(GHashTable * counter,char* file);

int add_line_to_set(GHashTable* set, char line[],int size);

char* stemming(const char* str);

void make_model();

void m1_for (gpointer key, gpointer value, gpointer userdata);
void m2_for (gpointer key, gpointer value, gpointer userdata);

bool valid_range(char* word,int freq);

#endif
