#include "global.h"

void *threaded_channel(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(struct_args->session);
	char **message_parts = g_strsplit(struct_args->params[1], " ", 0);
	if( !strcmp(message_parts[0], ".die") ){
		void (*check_auth_fcn)(struct irc_session *, char *, char *) = (void (*)(struct irc_session *, char *, char *))dlsym(context->auth_library, "check_auth");
		check_auth_fcn(struct_args->session, struct_args->origin, message_parts[0] + 1);
	} else if( !strcmp(message_parts[0], ".restart") ){
		void (*check_auth_fcn)(struct irc_session *, char *, char *) = (void (*)(struct irc_session *, char *, char *))dlsym(context->auth_library, "check_auth");
		check_auth_fcn(struct_args->session, struct_args->origin, message_parts[0] + 1);
	} else if( !strcmp(message_parts[0], ".upgrade") ){
		void (*check_auth_fcn)(struct irc_session *, char *, char *) = (void (*)(struct irc_session *, char *, char *))dlsym(context->auth_library, "check_auth");
		check_auth_fcn(struct_args->session, struct_args->origin, message_parts[0] + 1);
	} else if( !strcmp(message_parts[0], ".rehash") ){
		void (*check_auth_fcn)(struct irc_session *, char *, char *) = (void (*)(struct irc_session *, char *, char *))dlsym(context->auth_library, "check_auth");
		check_auth_fcn(struct_args->session, struct_args->origin, message_parts[0] + 1);
	} else if( !strcmp(message_parts[0], ".raw") ){
		if( message_parts[1] ){
			char *command = g_strdup(message_parts[1]);
			int i;
			for( i = 2; message_parts[i]; ++i ){
				char *old_command = command;
				command = g_strdup_printf("%s %s", command, message_parts[i]);
				g_free(old_command);
			}
			void (*check_auth_fcn)(struct irc_session *, char *, char *) = (void (*)(struct irc_session *, char *, char *))dlsym(context->auth_library, "check_auth");
			check_auth_fcn(struct_args->session, struct_args->origin, command);
			g_free(command);
		} else {
			irc_cmd_notice(struct_args->session, struct_args->origin, ".raw requires at least 1 parameter");
		}
	}
	g_strfreev(message_parts);
	free(struct_args->origin);
	unsigned int i = 0;
	for( i = 0; i < struct_args->count; ++i ){
		free(struct_args->params[i]);
	}
	free(struct_args->params);
	free(struct_args);
	g_atomic_int_add(&context->thread_count, -1);
	return NULL;
}

void *threaded_connect(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(struct_args->session);
	irc_cmd_user_mode(struct_args->session, config_get_string(context->config, "bot.user_mode"));
	char ** part_channels;
	int part_channels_size;
	if( (part_channels = (char **)config_get_array(context->config, "bot.channel.part_channels", NULL, &part_channels_size)) ){
		int i;
		for( i = 0; i < part_channels_size; ++i ){
			irc_cmd_part(struct_args->session, part_channels[i], "I'm not supposed to be in this channel.");
		}
	}
	irc_cmd_join(struct_args->session, config_get_string(context->config, "bot.channel.channel"), NULL);
	free(struct_args);
	g_atomic_int_add(&context->thread_count, -1);
	return NULL;
}

void *threaded_join(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(struct_args->session);
	if( !strcmp(config_get_string(context->config, "bot.channel.channel"), struct_args->params[0]) && !strcmp(config_get_string(context->config, "bot.nick"), struct_args->origin) ){
		irc_cmd_channel_mode(struct_args->session, config_get_string(context->config, "bot.channel.channel"), config_get_string(context->config, "bot.channel.mode_on_enter"));
		irc_cmd_msg(struct_args->session, config_get_string(context->config, "bot.channel.channel"), "I'm here now.  Let's get this started!");
		irc_cmd_msg(struct_args->session, config_get_string(context->config, "bot.channel.channel"), "To join the game, say \2.join\2.  To start the game at least 60 seconds after the first person joins, say \2.start\2.  To increase the minimum wait time after the first person joins, say \2.wait\2.  \2.wait\2 is limited to \0022\2 uses per game.");
		irc_send_raw(struct_args->session, "WHO %s", config_get_string(context->config, "bot.channel.channel"));
	} else if( !strcmp(config_get_string(context->config, "bot.channel.channel"), struct_args->params[0]) ){
		irc_send_raw(struct_args->session, "WHO %s", struct_args->origin);
	}
	free(struct_args->origin);
	unsigned int i = 0;
	for( i = 0; i < struct_args->count; ++i ){
		free(struct_args->params[i]);
	}
	free(struct_args->params);
	free(struct_args);
	g_atomic_int_add(&context->thread_count, -1);
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
	g_atomic_int_add(&context->thread_count, -1);
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
	g_atomic_int_add(&context->thread_count, -1);
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
	g_atomic_int_add(&context->thread_count, -1);
	return NULL;
}

void *threaded_numeric(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(struct_args->session);
	int (*numeric_fcn)(struct func_args *) = (int (*)(struct func_args *))dlsym(context->auth_library, "numeric");
	if( !numeric_fcn(struct_args) ){
		switch( struct_args->event ){
			case 352:;
				int go_on = TRUE;
				char *autovoice_nick;
				if( (autovoice_nick = config_get_string(context->config, "bot.autovoice.nick")) ){
					if( strcmp(autovoice_nick, struct_args->params[5]) ){
						go_on = FALSE;
					}
				}
				if( go_on ){
					char *autovoice_username;
					if( *struct_args->params[2] == '~' ){
						if( (autovoice_username = config_get_string(context->config, "bot.autovoice.username")) ){
							if( strcmp(autovoice_username, struct_args->params[2] + 1) ){
								go_on = FALSE;
							}
						}
					} else {
						if( (autovoice_username = config_get_string(context->config, "bot.autovoice.username")) ){
							if( strcmp(autovoice_username, struct_args->params[2]) ){
								go_on = FALSE;
							}
						}	
					}
					if( go_on ){
						char *autovoice_hostname = config_get_string(context->config, "bot.autovoice.hostname");
						char *autovoice_ipaddress = config_get_string(context->config, "bot.autovoice.ip_address");
						if( autovoice_hostname || autovoice_ipaddress ){
							if( strcmp(autovoice_hostname, struct_args->params[3]) && strcmp(autovoice_ipaddress, struct_args->params[3]) ){
								go_on = FALSE;
							}
						}
						if( go_on ){
							char *voice_command = g_strdup_printf("+v %s", struct_args->params[5]);
							irc_cmd_channel_mode(struct_args->session, config_get_string(context->config, "bot.channel.channel"), voice_command);
							g_free(voice_command);
						}
					}
				}
				break;
		}
	}
	free(struct_args->origin);
	unsigned int i = 0;
	for( i = 0; i < struct_args->count; ++i ){
		free(struct_args->params[i]);
	}
	free(struct_args->params);
	free(struct_args);
	g_atomic_int_add(&context->thread_count, -1);
	return NULL;
}

void *threaded_part(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(struct_args->session);
	
	free(struct_args->origin);
	free(struct_args);
	g_atomic_int_add(&context->thread_count, -1);
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
	g_atomic_int_add(&context->thread_count, -1);
	return NULL;
}

void *threaded_quit(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(struct_args->session);
	
	free(struct_args->origin);
	free(struct_args);
	g_atomic_int_add(&context->thread_count, -1);
	return NULL;
}
