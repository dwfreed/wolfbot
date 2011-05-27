#include "global.h"

void *threaded_channel(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	
	free(struct_args->origin);
	int i = 0;
	for( i = 0; i < struct_args->count; ++i ){
		free(struct_args->params[i]);
	}
	free(struct_args->params);
	free(struct_args);
	return NULL;
}

void *threaded_connect(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	struct irc_ctx_t *context = irc_get_ctx(struct_args->session);
	irc_cmd_user_mode(struct_args->session, context->config_data.WOLFBOT_UMODE);
	irc_cmd_join(struct_args->session, context->config_data.WOLFBOT_CHANNEL, NULL);
	free(struct_args);
	return NULL;
}

void *threaded_join(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	struct irc_ctx_t *context = irc_get_ctx(struct_args->session);
	if( !strcmp(context->config_data.WOLFBOT_NICK, struct_args->origin) ){
		irc_cmd_msg(struct_args->session, struct_args->params[0], "I'm here now.  Let's get this started!");
		irc_cmd_msg(struct_args->session, struct_args->params[0], "To join the game, say \2.join\2.  To start the game at least 60 seconds after the first person joins, say \2.start\2.  To increase the minimum wait time, say \2.wait\2.  \2.wait\2 is limited to \x022\2 uses per game.");
	}
	free(struct_args->origin);
	int i = 0;
	for( i = 0; i < struct_args->count; ++i ){
		free(struct_args->params[i]);
	}
	free(struct_args->params);
	free(struct_args);
	return NULL;
}

void *threaded_kick(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	
	free(struct_args->origin);
	int i = 0;
	for( i = 0; i < struct_args->count; ++i ){
		free(struct_args->params[i]);
	}
	free(struct_args->params);
	free(struct_args);
	return NULL;
}

void *threaded_nick(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	
	free(struct_args->origin);
	int i = 0;
	for( i = 0; i < struct_args->count; ++i ){
		free(struct_args->params[i]);
	}
	free(struct_args->params);
	free(struct_args);
	return NULL;
}

void *threaded_numeric(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	
	free(struct_args->origin);
	int i = 0;
	for( i = 0; i < struct_args->count; ++i ){
		free(struct_args->params[i]);
	}
	free(struct_args->params);
	free(struct_args);
	return NULL;
}

void *threaded_part(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	
	free(struct_args->origin);
	free(struct_args);
	return NULL;
}

void *threaded_privmsg(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	
	free(struct_args->origin);
	int i = 0;
	for( i = 0; i < struct_args->count; ++i ){
		free(struct_args->params[i]);
	}
	free(struct_args->params);
	free(struct_args);
	return NULL;
}

void *threaded_quit(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	
	free(struct_args->origin);
	free(struct_args);
	return NULL;
}
