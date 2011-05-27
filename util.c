#include "global.h"

int strcmp_data(const void *a, const void *b, void *data){
	return strcmp((char *)a, (char *)b);
}
