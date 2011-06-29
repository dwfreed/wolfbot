#include "global.h"

void event_channel(struct irc_session *session, const char *event __attribute__((__unused__)), const char *origin, const char **params, unsigned int count){
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	if( g_mutex_trylock(context->thread_mutex) ){
		g_mutex_unlock(context->thread_mutex);
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
	}
}

void event_connect(struct irc_session *session, const char *event __attribute__((__unused__)), const char *origin __attribute__((__unused__)), const char **params __attribute__((__unused__)), unsigned int count __attribute__((__unused__))){
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	if( g_mutex_trylock(context->thread_mutex) ){
		g_mutex_unlock(context->thread_mutex);
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
	}
}

void event_ctcp_req(struct irc_session *session __attribute__((__unused__)), const char *event __attribute__((__unused__)), const char *origin __attribute__((__unused__)), const char **params __attribute__((__unused__)), unsigned int count __attribute__((__unused__))){

}

void event_join(struct irc_session *session, const char *event __attribute__((__unused__)), const char *origin, const char **params, unsigned int count){
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	if( g_mutex_trylock(context->thread_mutex) ){
		g_mutex_unlock(context->thread_mutex);
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
	}
}

void event_kick(struct irc_session *session, const char *event __attribute__((__unused__)), const char *origin, const char **params, unsigned int count){
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	if( g_mutex_trylock(context->thread_mutex) ){
		g_mutex_unlock(context->thread_mutex);
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
	}
}

void event_nick(struct irc_session *session, const char *event __attribute__((__unused__)), const char *origin, const char **params, unsigned int count){
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	if( g_mutex_trylock(context->thread_mutex) ){
		g_mutex_unlock(context->thread_mutex);
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
	}
}

void event_notice(struct irc_session *session, const char *event __attribute__((__unused__)), const char *origin, const char **params, unsigned int count){
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	if( g_mutex_trylock(context->thread_mutex) ){
		g_mutex_unlock(context->thread_mutex);
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
	}
}

void event_numeric(struct irc_session *session, unsigned int event, const char *origin, const char **params, unsigned int count){
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	if( g_mutex_trylock(context->thread_mutex) ){
		g_mutex_unlock(context->thread_mutex);
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
	}
}

void event_part(struct irc_session *session, const char *event __attribute__((__unused__)), const char *origin, const char **params __attribute__((__unused__)), unsigned int count __attribute__((__unused__))){
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	if( g_mutex_trylock(context->thread_mutex) ){
		g_mutex_unlock(context->thread_mutex);
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
	}
}

void event_privmsg(struct irc_session *session, const char *event __attribute__((__unused__)), const char *origin, const char **params, unsigned int count){
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	if( g_mutex_trylock(context->thread_mutex) ){
		g_mutex_unlock(context->thread_mutex);
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
	}
}

void event_quit(struct irc_session *session, const char *event __attribute__((__unused__)), const char *origin, const char **params __attribute__((__unused__)), unsigned int count __attribute__((__unused__))){
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	if( g_mutex_trylock(context->thread_mutex) ){
		g_mutex_unlock(context->thread_mutex);
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
	}
}
