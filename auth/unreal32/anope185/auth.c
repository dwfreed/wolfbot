#include "global.h"

static void queue_cleanup(void *data, void *user_data __attribute__((__unused__))){
	free(data);
}

static int commands_cleanup(void *key __attribute__((__unused__)), void *value, void *user_data __attribute__((__unused__))){
	GQueue *queue = (GQueue *)value;
	g_queue_foreach(queue, queue_cleanup, NULL);
	return TRUE;
}

void auth_init(struct irc_ctx_t *context){
	context->commands = g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)g_queue_free);
	context->commands_mutex = g_mutex_new();
}

void auth_fini(struct irc_ctx_t *context){
	g_mutex_lock(context->commands_mutex);
	g_hash_table_foreach_remove(context->commands, commands_cleanup, NULL);
	g_hash_table_destroy(context->commands);
	g_mutex_unlock(context->commands_mutex);
	g_mutex_free(context->commands_mutex);
}

void check_auth(irc_session_t *session, char *nick, char *command){
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	g_mutex_lock(context->commands_mutex);
	GQueue *commands;
	if( (commands = (GQueue *)g_hash_table_lookup(context->commands, nick)) ){
		g_queue_push_tail(commands, strdup(command));
	} else {
		commands = g_queue_new();
		g_queue_push_tail(commands, strdup(command));
		g_hash_table_insert(context->commands, strdup(nick), commands);
		irc_cmd_whois(session, nick);
	}
	g_mutex_unlock(context->commands_mutex);
}

static void call_command(void *data, void *user_data){
	char *command = (char *)data;
	irc_session_t *session = (irc_session_t *)user_data;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	int numlocks;
	if( g_static_rec_mutex_trylock(context->thread_mutex) ){
		if( ( numlocks = g_static_rec_mutex_unlock_full(context->thread_mutex) ) == 1 ){
			if( g_atomic_int_get(&context->thread_count) > -1 ){
				g_atomic_int_inc(&context->thread_count);
				if( !strcmp(command, "die") ){
					pthread_t thread_id;
					if( pthread_create(&thread_id, NULL, die, session) ){
						fprintf(stderr, "Failed to thread die, calling direct!\n");
						die(session);
					} else {
						pthread_detach(thread_id);
					}
				} else if( !strcmp(command, "restart") ){
					pthread_t thread_id;
					if( pthread_create(&thread_id, NULL, restart, session) ){
						fprintf(stderr, "Failed to thread restart, calling direct!\n");
						restart(session);
					} else {
						pthread_detach(thread_id);
					}
				} else if( !strcmp(command, "upgrade") ){
					pthread_t thread_id;
					if( pthread_create(&thread_id, NULL, upgrade, session) ){
						fprintf(stderr, "Failed to thread upgrade, calling direct!\n");
						upgrade(session);
					} else {
						pthread_detach(thread_id);
					}
				} else if( !strcmp(command, "rehash") ){
					pthread_t thread_id;
					if( pthread_create(&thread_id, NULL, rehash, session) ){
						fprintf(stderr, "Failed to thread rehash, calling direct!\n");
						rehash(session);
					} else {
						pthread_detach(thread_id);
					}
					
				} else {
					irc_send_raw(session, command);
				}
			}
		} else {
			g_static_rec_mutex_lock_full(context->thread_mutex, numlocks - 1);
		}
	}
}

int notice(struct func_args *args __attribute__((__unused__))){
	return 0;
}

int numeric(struct func_args *args){
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(args->session);
	GQueue *commands;
	int i;
	switch( args->event ){
		case 307:
			if( !strcmp(config_get_string(context->config, "bot.owner"), args->params[1]) ){
				g_mutex_lock(context->commands_mutex);
				if( (commands = g_hash_table_lookup(context->commands, args->params[1])) ){
					g_queue_foreach(commands, call_command, args->session);
					g_queue_foreach(commands, queue_cleanup, NULL);
					g_hash_table_remove(context->commands, args->params[1]);
				}
				g_mutex_unlock(context->commands_mutex);
			} else {
				int ops_size;
				char **ops = (char **)config_get_array(context->config, "bot.ops", NULL, &ops_size);
				for( i = 0; i < ops_size; ++i ){
					if( !strcmp(ops[i], args->params[1]) ){
						break;
					}
				}
				g_mutex_lock(context->commands_mutex);
				if( (commands = g_hash_table_lookup(context->commands, args->params[1])) ){
					if( i != ops_size ){
						g_queue_foreach(commands, call_command, args->session);
					}
					g_queue_foreach(commands, queue_cleanup, NULL);
					g_hash_table_remove(context->commands, args->params[1]);
				}
				g_mutex_unlock(context->commands_mutex);
			}
			break;
		case 319:;
			char **channels = g_strsplit(args->params[2], " ", 0);
			for( i = 0; channels[i]; ++i){
				if( !strcmp(channels[i] + 1, config_get_string(context->config, "bot.channel.channel")) ){
					g_mutex_lock(context->commands_mutex);
					if( (commands = g_hash_table_lookup(context->commands, args->params[1])) ){
						switch( channels[i][0] ){
							case '~':
							case '&':
							case '@':
								g_queue_foreach(commands, call_command, args->session);
							default:
								g_queue_foreach(commands, queue_cleanup, NULL);
								g_hash_table_remove(context->commands, args->params[1]);
								break;
						}
					}
					g_mutex_unlock(context->commands_mutex);
					break;
				}
			}
			g_strfreev(channels);
			break;
		case 313:
			g_mutex_lock(context->commands_mutex);
			if( (commands = g_hash_table_lookup(context->commands, args->params[1])) ){
				g_queue_foreach(commands, call_command, args->session);
				g_queue_foreach(commands, queue_cleanup, NULL);
				g_hash_table_remove(context->commands, args->params[1]);
			}
			g_mutex_unlock(context->commands_mutex);
			break;
		case 318:
			g_mutex_lock(context->commands_mutex);
			if( (commands = g_hash_table_lookup(context->commands, args->params[1])) ){
				g_queue_foreach(commands, queue_cleanup, NULL);
				g_hash_table_remove(context->commands, args->params[1]);
			}
			g_mutex_unlock(context->commands_mutex);
			break;
	}
	return 0;
}

int privmsg(struct func_args *args __attribute__((__unused__))){
	return 0;
}
