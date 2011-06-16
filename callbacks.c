#include "global.h"

void event_channel(irc_session_t *session, const char *event __attribute__((__unused__)), const char *origin, const char **params, unsigned int count){
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	int numlocks;
	if( g_static_rec_mutex_trylock(context->thread_mutex) ){
		if( ( numlocks = g_static_rec_mutex_unlock_full(context->thread_mutex) ) == 1 ){
			if( g_atomic_int_get(&context->thread_count) > -1 ){
				g_atomic_int_inc(&context->thread_count);
				struct func_args *args = (struct func_args *)malloc(sizeof(struct func_args));
				args->session = session;
				args->origin = strdup(origin);
				args->params = (char **)calloc(count, sizeof(char *));
				unsigned int i;
				for( i = 0; i < count; ++i ){
					args->params[i] = strdup(params[i]);
				}
				args->count = count;
				pthread_t thread_id;
				if( pthread_create(&thread_id, NULL, threaded_channel, args) ){
					fprintf(stderr, "Failed to thread channel, calling direct!\n");
					threaded_channel(args);
				} else {
					pthread_detach(thread_id);
				}
			}
		} else {
			g_static_rec_mutex_lock_full(context->thread_mutex, numlocks - 1);
		}
	}
}

void event_connect(irc_session_t *session, const char *event __attribute__((__unused__)), const char *origin __attribute__((__unused__)), const char **params __attribute__((__unused__)), unsigned int count __attribute__((__unused__))){
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	int numlocks;
	if( g_static_rec_mutex_trylock(context->thread_mutex) ){
		if( ( numlocks = g_static_rec_mutex_unlock_full(context->thread_mutex) ) == 1 ){
			if( g_atomic_int_get(&context->thread_count) > -1 ){
				g_atomic_int_inc(&context->thread_count);
				struct func_args *args = (struct func_args *)malloc(sizeof(struct func_args));
				args->session = session;
				pthread_t thread_id;
				if( pthread_create(&thread_id, NULL, threaded_connect, args) ){
					fprintf(stderr, "Failed to thread connect, calling direct!\n");
					threaded_connect(args);
				} else {
					pthread_detach(thread_id);
				}
			}
		} else {
			g_static_rec_mutex_lock_full(context->thread_mutex, numlocks - 1);
		}
	}
}

void event_ctcp_req(irc_session_t *session __attribute__((__unused__)), const char *event __attribute__((__unused__)), const char *origin __attribute__((__unused__)), const char **params __attribute__((__unused__)), unsigned int count __attribute__((__unused__))){

}

void event_join(irc_session_t *session, const char *event __attribute__((__unused__)), const char *origin, const char **params, unsigned int count){
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	int numlocks;
	if( g_static_rec_mutex_trylock(context->thread_mutex) ){
		if( ( numlocks = g_static_rec_mutex_unlock_full(context->thread_mutex) ) == 1 ){
			if( g_atomic_int_get(&context->thread_count) > -1 ){
				g_atomic_int_inc(&context->thread_count);
				struct func_args *args = (struct func_args *)malloc(sizeof(struct func_args));
				args->session = session;
				args->origin = strdup(origin);
				args->params = (char **)calloc(count, sizeof(char *));
				unsigned int i;
				for( i = 0; i < count; ++i ){
					args->params[i] = strdup(params[i]);
				}
				args->count = count;
				pthread_t thread_id;
				if( pthread_create(&thread_id, NULL, threaded_join, args) ){
					fprintf(stderr, "Failed to thread join, calling direct!\n");
					threaded_join(args);
				} else {
					pthread_detach(thread_id);
				}
			}
		} else {
			g_static_rec_mutex_lock_full(context->thread_mutex, numlocks - 1);
		}
	}
}

void event_kick(irc_session_t *session, const char *event __attribute__((__unused__)), const char *origin, const char **params, unsigned int count){
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	int numlocks;
	if( g_static_rec_mutex_trylock(context->thread_mutex) ){
		if( ( numlocks = g_static_rec_mutex_unlock_full(context->thread_mutex) ) == 1 ){
			if( g_atomic_int_get(&context->thread_count) > -1 ){
				g_atomic_int_inc(&context->thread_count);
				struct func_args *args = (struct func_args *)malloc(sizeof(struct func_args));
				args->session = session;
				args->origin = strdup(origin);
				args->params = (char **)calloc(count, sizeof(char *));
				unsigned int i;
				for( i = 0; i < count; ++i ){
					args->params[i] = strdup(params[i]);
				}
				args->count = count;
				pthread_t thread_id;
				if( pthread_create(&thread_id, NULL, threaded_kick, args) ){
					fprintf(stderr, "Failed to thread kick, calling direct!\n");
					threaded_kick(args);
				} else {
					pthread_detach(thread_id);
				}
			}
		} else {
			g_static_rec_mutex_lock_full(context->thread_mutex, numlocks - 1);
		}
	}
}

void event_nick(irc_session_t *session, const char *event __attribute__((__unused__)), const char *origin, const char **params, unsigned int count){
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	int numlocks;
	if( g_static_rec_mutex_trylock(context->thread_mutex) ){
		if( ( numlocks = g_static_rec_mutex_unlock_full(context->thread_mutex) ) == 1 ){
			if( g_atomic_int_get(&context->thread_count) > -1 ){
				g_atomic_int_inc(&context->thread_count);
				struct func_args *args = (struct func_args *)malloc(sizeof(struct func_args));
				args->session = session;
				args->origin = strdup(origin);
				args->params = (char **)calloc(count, sizeof(char *));
				unsigned int i;
				for( i = 0; i < count; ++i ){
					args->params[i] = strdup(params[i]);
				}
				args->count = count;
				pthread_t thread_id;
				if( pthread_create(&thread_id, NULL, threaded_nick, args) ){
					fprintf(stderr, "Failed to thread nick, calling direct!\n");
					threaded_nick(args);
				} else {
					pthread_detach(thread_id);
				}
			}
		} else {
			g_static_rec_mutex_lock_full(context->thread_mutex, numlocks - 1);
		}
	}
}

void event_notice(irc_session_t *session, const char *event __attribute__((__unused__)), const char *origin, const char **params, unsigned int count){
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	int numlocks;
	if( g_static_rec_mutex_trylock(context->thread_mutex) ){
		if( ( numlocks = g_static_rec_mutex_unlock_full(context->thread_mutex) ) == 1 ){
			if( g_atomic_int_get(&context->thread_count) > -1 ){
				g_atomic_int_inc(&context->thread_count);
				struct func_args *args = (struct func_args *)malloc(sizeof(struct func_args));
				args->session = session;
				args->origin = strdup(origin);
				args->params = (char **)calloc(count, sizeof(char *));
				unsigned int i;
				for( i = 0; i < count; ++i ){
					args->params[i] = strdup(params[i]);
				}
				args->count = count;
				pthread_t thread_id;
				if( pthread_create(&thread_id, NULL, threaded_notice, args) ){
					fprintf(stderr, "Failed to thread notice, calling direct!\n");
					threaded_nick(args);
				} else {
					pthread_detach(thread_id);
				}
			}
		} else {
			g_static_rec_mutex_lock_full(context->thread_mutex, numlocks - 1);
		}
	}
}

void event_numeric(irc_session_t *session, unsigned int event, const char *origin, const char **params, unsigned int count){
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	int numlocks;
	if( g_static_rec_mutex_trylock(context->thread_mutex) ){
		if( ( numlocks = g_static_rec_mutex_unlock_full(context->thread_mutex) ) == 1 ){
			if( g_atomic_int_get(&context->thread_count) > -1 ){
				g_atomic_int_inc(&context->thread_count);
				struct func_args *args = (struct func_args *)malloc(sizeof(struct func_args));
				args->session = session;
				args->event = event;
				args->origin = strdup(origin);
				args->params = (char **)calloc(count, sizeof(char *));
				unsigned int i;
				for( i = 0; i < count; ++i ){
					args->params[i] = strdup(params[i]);
				}
				args->count = count;
				pthread_t thread_id;
				if( pthread_create(&thread_id, NULL, threaded_numeric, args) ){
					fprintf(stderr, "Failed to thread numeric, calling direct!\n");
					threaded_numeric(args);
				} else {
					pthread_detach(thread_id);
				}
			}
		} else {
			g_static_rec_mutex_lock_full(context->thread_mutex, numlocks - 1);
		}
	}
}

void event_part(irc_session_t *session, const char *event __attribute__((__unused__)), const char *origin, const char **params __attribute__((__unused__)), unsigned int count __attribute__((__unused__))){
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	int numlocks;
	if( g_static_rec_mutex_trylock(context->thread_mutex) ){
		if( ( numlocks = g_static_rec_mutex_unlock_full(context->thread_mutex) ) == 1 ){
			if( g_atomic_int_get(&context->thread_count) > -1 ){
				g_atomic_int_inc(&context->thread_count);
				struct func_args *args = (struct func_args *)malloc(sizeof(struct func_args));
				args->session = session;
				args->origin = strdup(origin);
				pthread_t thread_id;
				if( pthread_create(&thread_id, NULL, threaded_part, args) ){
					fprintf(stderr, "Failed to thread part, calling direct!\n");
					threaded_part(args);
				} else {
					pthread_detach(thread_id);
				}
			}
		} else {
			g_static_rec_mutex_lock_full(context->thread_mutex, numlocks - 1);
		}
	}
}

void event_privmsg(irc_session_t *session, const char *event __attribute__((__unused__)), const char *origin, const char **params, unsigned int count){
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	int numlocks;
	if( g_static_rec_mutex_trylock(context->thread_mutex) ){
		if( ( numlocks = g_static_rec_mutex_unlock_full(context->thread_mutex) ) == 1 ){
			if( g_atomic_int_get(&context->thread_count) > -1 ){
				g_atomic_int_inc(&context->thread_count);
				struct func_args *args = (struct func_args *)malloc(sizeof(struct func_args));
				args->session = session;
				args->origin = strdup(origin);
				args->params = (char **)calloc(count, sizeof(char *));
				unsigned int i;
				for( i = 0; i < count; ++i ){
					args->params[i] = strdup(params[i]);
				}
				args->count = count;
				pthread_t thread_id;
				if( pthread_create(&thread_id, NULL, threaded_privmsg, args) ){
					fprintf(stderr, "Failed to thread privmsg, calling direct!i\n");
					threaded_privmsg(args);
				} else {
					pthread_detach(thread_id);
				}
			}
		} else {
			g_static_rec_mutex_lock_full(context->thread_mutex, numlocks - 1);
		}
	}
}

void event_quit(irc_session_t *session, const char *event __attribute__((__unused__)), const char *origin, const char **params __attribute__((__unused__)), unsigned int count __attribute__((__unused__))){
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	int numlocks;
	if( g_static_rec_mutex_trylock(context->thread_mutex) ){
		if( ( numlocks = g_static_rec_mutex_unlock_full(context->thread_mutex) ) == 1 ){
			if( g_atomic_int_get(&context->thread_count) > -1 ){
				g_atomic_int_inc(&context->thread_count);
				struct func_args *args = (struct func_args *)malloc(sizeof(struct func_args));
				args->session = session;
				args->origin = strdup(origin);
				pthread_t thread_id;
				if( pthread_create(&thread_id, NULL, threaded_quit, args) ){
					fprintf(stderr, "Failed to thread quit, calling direct!i\n");
					threaded_quit(args);
				} else {
					pthread_detach(thread_id);
				}
			}
		} else {
			g_static_rec_mutex_lock_full(context->thread_mutex, numlocks - 1);
		}
	}
}
