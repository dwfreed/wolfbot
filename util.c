#include "global.h"

int strcmp_data(const void *a, const void *b, void *data __attribute__((__unused__))){
	return strcmp((char *)a, (char *)b);
}

char *tilde_expansion(char *path){
	char *new_path;
	if( *path == '~' ){
		char *home = getenv("HOME");
		new_path = g_strdup_printf("%s%s", home, path + 1);
	} else {
		new_path = g_strdup(path);
	}
	return new_path;
}

void *die(void *args __attribute__((__unused__))){
	return NULL;
}

void *restart(void *args __attribute__((__unused__))){
	return NULL;
}

void *upgrade(void *args __attribute__((__unused__))){
	return NULL;
}

void *rehash(void *args __attribute__((__unused__))){
	return NULL;
}
