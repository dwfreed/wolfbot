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

void *die(void *args){
	irc_session_t *session = (irc_session_t *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	irc_cmd_msg(session, config_get_string(context->config, "bot.channel.channel"), "Die, die, die!");
	context->restart = FALSE;
	kill(getpid(), SIGRTMIN);
	(void)g_atomic_int_dec_and_test(&context->thread_count);
	return NULL;
}

void *restart(void *args){
	irc_session_t *session = (irc_session_t *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	irc_cmd_msg(session, config_get_string(context->config, "bot.channel.channel"), "Preparing to restart");
	context->restart = TRUE;
	kill(getpid(), SIGRTMIN);
	(void)g_atomic_int_dec_and_test(&context->thread_count);
	return NULL;
}

void *upgrade(void *args __attribute__((__unused__))){
	irc_session_t *session = (irc_session_t *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	(void)g_atomic_int_dec_and_test(&context->thread_count);
	return NULL;
}

void *rehash(void *args __attribute__((__unused__))){
	irc_session_t *session = (irc_session_t *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	(void)g_atomic_int_dec_and_test(&context->thread_count);
	return NULL;
}
