#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <math.h>
#include "trainer.h"

GHashTable* model;

GHashTable * neg_set;
GHashTable * non_neg_set;

int neg_size;
int non_neg_size;

int main() {

	neg_set = g_hash_table_new(g_str_hash, g_str_equal);
	non_neg_set = g_hash_table_new(g_str_hash, g_str_equal);

	neg_size = read_file_to_set(neg_set,"../data/train.negative.csv");
	non_neg_size = read_file_to_set(non_neg_set,"../data/train.non-negative.csv");

	printf("neg: %d, non-neg: %d\n",neg_size,non_neg_size);

	model = g_hash_table_new(g_str_hash, g_str_equal);	

	rescaling(neg_set,non_neg_set,neg_size,non_neg_size);

	return 0;
}

int read_file_to_set(GHashTable * counter,char* file) {

	//FILE * f = fopen("../data/train.negative.csv", "r") ;
	FILE * f = fopen(file, "r") ;

        //GHashTable * counter = g_hash_table_new(g_str_hash, g_str_equal) ;

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

			//printf("t: %s\n",t);

			int * d ;			
			d = g_hash_table_lookup(set, t) ;
			if (d == NULL) {
				d = malloc(sizeof(int)) ;
				*d = 1 ;
				g_hash_table_insert(set, strdup(t), d) ;
			}
			else {
				*d = *d + 1 ;
			}
			size ++;
		}

		return size;
}

void reduce_dictionary(GHashTable* set) {

}

void stemming(GHashTable* set) {

}

void smoothing(GHashTable* set) {

}

void rescaling() {

	g_hash_table_foreach(neg_set, m1_for, 0x0);
	g_hash_table_foreach(non_neg_set, m2_for, 0x0);

//	g_hash_table_foreach(model, model_for, 0x0);
}

// new_set: model
/*
GHashTable * neg_set;
GHashTable * non_neg_set;

int neg_size;
int non_neg_size;
*/
void m1_for (gpointer key, gpointer value, gpointer userdata) 
{
	char * word = key ;
	int * n = value ;

	int * d = g_hash_table_lookup(model,word) ;
	if(d == NULL) {
		
		double p1,p2;
		
		int n2;
		int * d2 = g_hash_table_lookup(non_neg_set,word) ;
		if(d2 == NULL) n2 = 0;
		else n2 = *d2;

		p1 = (double) *n / (double) neg_size;
		p2 = (double) n2 / (double) non_neg_size;

		printf("%s, %lf, %lf\n",word,p1,p2);

		g_hash_table_insert(model, strdup(word), 0x0) ;
			
	} else {
		return;
	}
}
void m2_for (gpointer key, gpointer value, gpointer userdata)
{
        char * word = key ;
        int * n = value ;

        int * d = g_hash_table_lookup(model,word) ;
        if(d == NULL) {

                double p1,p2;

                int n2;
                int * d2 = g_hash_table_lookup(neg_set,word) ;
                if(d2 == NULL) n2 = 0;
                else n2 = *d2;

		p1 = (double) *n / (double) non_neg_size;
		p2 = (double) n2 / (double) neg_size;

		printf("%s, %lf, %lf\n",word,p2,p1);

                g_hash_table_insert(model, strdup(word), 0x0) ;

        } else {
                return;
        }
}

void model_for (gpointer key, gpointer value, gpointer userdata) {

}

void make_model(GHashTable* set) {

}
