#ifndef _trainer_h
#define _trainer_h

struct _Probability {

	double p1;
	double p2;

};
typedef struct _Probability Probability;

int read_file_to_set(GHashTable * counter,char* file);

int add_line_to_set(GHashTable* set, char line[],int size);

void reduce_dictionary(GHashTable* set);

void stemming(GHashTable* set);

void smoothing(GHashTable* set);

void rescaling();

void make_model(GHashTable* set);

void m1_for (gpointer key, gpointer value, gpointer userdata);
void m2_for (gpointer key, gpointer value, gpointer userdata);
void model_for (gpointer key, gpointer value, gpointer userdata);

#endif
