//TODO: when remove duplicate, size doesnt change


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glib.h>
#include "trainer.h"
#include "../include/libstemmer.h"

#define MAX_W 100
#define MIN_W 10
#define DUMMY_W 10

GHashTable * neg_set;
GHashTable * non_neg_set;

int neg_size;
int non_neg_size;

struct sb_stemmer * stemmer ;

const char out_csv[50] = "model.csv";
FILE* f;

GHashTable* checked;

FILE* f_del; //TODO: remove
FILE* f_stay; //TODO: remove


int main() {

	f = fopen(out_csv, "w");

	f_del = fopen("deleted.csv", "w"); //TODO: remove
	f_stay = fopen("stay.csv", "w"); //TODO: remove


	stemmer = sb_stemmer_new("english", 0x0) ;

	neg_set = g_hash_table_new(g_str_hash, g_str_equal);
	non_neg_set = g_hash_table_new(g_str_hash, g_str_equal);

	neg_size = read_file_to_set(neg_set,"../data/train.negative.csv");
	non_neg_size = read_file_to_set(non_neg_set,"../data/train.non-negative.csv");

	//printf("neg: %d, non-neg: %d\n",neg_size,non_neg_size);

	checked = g_hash_table_new(g_str_hash, g_str_equal);	
	make_model();

	sb_stemmer_delete(stemmer) ;
	fclose(f);

	fclose(f_del); //TODO: remove
	fclose(f_stay); //TODO: remove

	return 0;
}

int read_file_to_set(GHashTable * counter,char* file) {

	FILE * f = fopen(file, "r") ;

        char * line = 0x0 ;
        size_t r ;
        size_t n = 0 ;

	int size = 0;
	
        while (getline(&line, &n, f) >= 0) {
                char * t ;
                char * _line = line ;

                size = add_line_to_set(counter,line,size);

                line = 0x0 ;
                free(_line) ;
        }

        fclose(f) ;

	return size;
}

int add_line_to_set(GHashTable* set, char line[],int size) {
		char* t;

		char dili[50] = " \n\t,.#i?()*&^%$@!~`/\'\"";

		for (t = strtok(line, dili) ; t != 0x0 ; t = strtok(0x0, dili)) {

			for(int i = 0; i < strlen(t); i++) {
				if(t[i] >= 'A' && t[i] <= 'Z') {
					t[i] = t[i] - 'A' + 'a';
				}
			}

			int * d ;			
			d = g_hash_table_lookup(set, t) ;
			if (d == NULL) {
				d = malloc(sizeof(int)) ;
				*d = 1 ;
				
				const char* stem = sb_stemmer_stem(stemmer, strdup(t), strlen(t)) ;

				g_hash_table_insert(set, strdup(stem), d) ;
			}
			else {
				*d = *d + 1 ;
			}
			size ++;
		}

		return size;
}

void make_model() {

	g_hash_table_foreach(neg_set, m1_for, 0x0);
	g_hash_table_foreach(non_neg_set, m2_for, 0x0);
}

void m1_for (gpointer key, gpointer value, gpointer userdata) 
{
	char * word = key ;
	int * n = value ;

	int * d = g_hash_table_lookup(checked,word) ;
	if(d == NULL) {
		
		g_hash_table_insert(checked, strdup(word), 0x0) ;

		double p1,p2;
		
		int n2;
		int * d2 = g_hash_table_lookup(non_neg_set,word) ;
		if(d2 == NULL) n2 = 0;
		else n2 = *d2;

		int freq = *n + n2;


		if(!valid_range(word,freq)) return;

		p1 = (double) (*n + DUMMY_W) / (double) neg_size;
		p2 = (double) (n2 + DUMMY_W) / (double) non_neg_size;

		fprintf(f,"%s, %lf, %lf\n",word,p1,p2);
			
	} else {
		return;
	}
}
void m2_for (gpointer key, gpointer value, gpointer userdata)
{
        char * word = key ;
        int * n = value ;

        int * d = g_hash_table_lookup(checked,word) ;
        if(d == NULL) {

                double p1,p2;

                int n2;
                int * d2 = g_hash_table_lookup(neg_set,word) ;
                if(d2 == NULL) n2 = 0;
                else n2 = *d2;

		int freq = *n + n2;

		if(!valid_range(word,freq)) return;

		p1 = (double) (*n + DUMMY_W) / (double) non_neg_size;
		p2 = (double) (n2 + DUMMY_W) / (double) neg_size;

		fprintf(f,"%s, %lf, %lf\n",word,p2,p1);

                g_hash_table_insert(checked, strdup(word), 0x0) ;

        } else {
                return;
        }
}
bool valid_range(char* word, int freq) {
	//return freq >= MIN_W && freq <= MAX_W; //TODO: uncomment

	/* TODO: remove below */
	if(freq >= MIN_W && freq <= MAX_W) {
		fprintf(f_stay,"%s, %d\n",word,freq);
		return true;		
	} else {
		fprintf(f_del,"%s, %d\n",word,freq);
		return false;
	}

}
