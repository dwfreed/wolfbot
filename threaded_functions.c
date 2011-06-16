#include "global.h"

void *threaded_channel(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(struct_args->session);
	char **message_parts = g_strsplit(struct_args->params[1], " ", 0);
	if( !strcmp(message_parts[0], ".die") ){
		void (*check_auth_fcn)(irc_session_t *, char *, char *) = (void (*)(irc_session_t *, char *, char *))dlsym(context->auth_library, "check_auth");
		check_auth_fcn(struct_args->session, struct_args->origin, message_parts[0] + 1);
	} else if( !strcmp(message_parts[0], ".restart") ){
		void (*check_auth_fcn)(irc_session_t *, char *, char *) = (void (*)(irc_session_t *, char *, char *))dlsym(context->auth_library, "check_auth");
		check_auth_fcn(struct_args->session, struct_args->origin, message_parts[0] + 1);
	} else if( !strcmp(message_parts[0], ".upgrade") ){
		void (*check_auth_fcn)(irc_session_t *, char *, char *) = (void (*)(irc_session_t *, char *, char *))dlsym(context->auth_library, "check_auth");
		check_auth_fcn(struct_args->session, struct_args->origin, message_parts[0] + 1);
	} else if( !strcmp(message_parts[0], ".rehash") ){
		void (*check_auth_fcn)(irc_session_t *, char *, char *) = (void (*)(irc_session_t *, char *, char *))dlsym(context->auth_library, "check_auth");
		check_auth_fcn(struct_args->session, struct_args->origin, message_parts[0] + 1);
	}
	g_strfreev(message_parts);
	free(struct_args->origin);
	unsigned int i = 0;
	for( i = 0; i < struct_args->count; ++i ){
		free(struct_args->params[i]);
	}
	free(struct_args->params);
	free(struct_args);
	(void)g_atomic_int_dec_and_test(&context->thread_count);
	return NULL;
}

void *threaded_connect(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(struct_args->session);
	irc_cmd_user_mode(struct_args->session, config_get_string(context->config, "bot.user_mode"));
	irc_cmd_join(struct_args->session, config_get_string(context->config, "bot.channel.channel"), NULL);
	free(struct_args);
	(void)g_atomic_int_dec_and_test(&context->thread_count);
	return NULL;
}

void *threaded_join(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(struct_args->session);
	if( !strcmp(config_get_string(context->config, "bot.nick"), struct_args->origin) ){
		irc_cmd_channel_mode(struct_args->session, struct_args->params[0], config_get_string(context->config, "bot.channel.mode_on_enter"));
		irc_cmd_msg(struct_args->session, struct_args->params[0], "I'm here now.  Let's get this started!");
		irc_cmd_msg(struct_args->session, struct_args->params[0], "To join the game, say \2.join\2.  To start the game at least 60 seconds after the first person joins, say \2.start\2.  To increase the minimum wait time after the first person joins, say \2.wait\2.  \2.wait\2 is limited to \0022\2 uses per game.");
	}
	free(struct_args->origin);
	unsigned int i = 0;
	for( i = 0; i < struct_args->count; ++i ){
		free(struct_args->params[i]);
	}
	free(struct_args->params);
	free(struct_args);
	(void)g_atomic_int_dec_and_test(&context->thread_count);
	return NULL;
}

void *threaded_kick(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(struct_args->session);
	
	free(struct_args->origin);
	unsigned int i = 0;
	for( i = 0; i < struct_args->count; ++i ){
		free(struct_args->params[i]);
	}
	free(struct_args->params);
	free(struct_args);
	(void)g_atomic_int_dec_and_test(&context->thread_count);
	return NULL;
}

void *threaded_nick(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(struct_args->session);
	
	free(struct_args->origin);
	unsigned int i = 0;
	for( i = 0; i < struct_args->count; ++i ){
		free(struct_args->params[i]);
	}
	free(struct_args->params);
	free(struct_args);
	(void)g_atomic_int_dec_and_test(&context->thread_count);
	return NULL;
}

void *threaded_notice(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(struct_args->session);
	int (*notice_fcn)(struct func_args *) = (int (*)(struct func_args *))dlsym(context->auth_library, "notice");
	notice_fcn(struct_args);
	free(struct_args->origin);
	unsigned int i = 0;
	for( i = 0; i < struct_args->count; ++i ){
		free(struct_args->params[i]);
	}
	free(struct_args->params);
	free(struct_args);
	(void)g_atomic_int_dec_and_test(&context->thread_count);
	return NULL;
}

void *threaded_numeric(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(struct_args->session);
	int (*numeric_fcn)(struct func_args *) = (int (*)(struct func_args *))dlsym(context->auth_library, "numeric");
	numeric_fcn(struct_args);
	free(struct_args->origin);
	unsigned int i = 0;
	for( i = 0; i < struct_args->count; ++i ){
		free(struct_args->params[i]);
	}
	free(struct_args->params);
	free(struct_args);
	(void)g_atomic_int_dec_and_test(&context->thread_count);
	return NULL;
}

void *threaded_part(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(struct_args->session);
	
	free(struct_args->origin);
	free(struct_args);
	(void)g_atomic_int_dec_and_test(&context->thread_count);
	return NULL;
}

void *threaded_privmsg(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(struct_args->session);
	int (*privmsg_fcn)(struct func_args *) = (int (*)(struct func_args *))dlsym(context->auth_library, "privmsg");
	if( !privmsg_fcn(struct_args) ){

	}
	free(struct_args->origin);
	unsigned int i = 0;
	for( i = 0; i < struct_args->count; ++i ){
		free(struct_args->params[i]);
	}
	free(struct_args->params);
	free(struct_args);
	(void)g_atomic_int_dec_and_test(&context->thread_count);
	return NULL;
}

void *threaded_quit(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(struct_args->session);
	
	free(struct_args->origin);
	free(struct_args);
	(void)g_atomic_int_dec_and_test(&context->thread_count);
	return NULL;
}
