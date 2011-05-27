#include "global.h"

void event_channel(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count){
	struct irc_ctx_t *context = irc_get_ctx(session);
	int numlocks;
	if( g_static_rec_mutex_trylock(context->thread_mutex) ){
		if( ( numlocks = g_static_rec_mutex_unlock_full(context->thread_mutex) ) == 1 ){
			struct func_args *args = (struct func_args *)malloc(sizeof(struct func_args));
			args->session = session;
			args->origin = strdup(origin);
			args->params = (char **)calloc(count, sizeof(char *));
			int i;
			for( i = 0; i < count; ++i ){
				args->params[i] = strdup(params[i]);
			}
			args->count = count;
			pthread_t thread_id;
			if( pthread_create(&thread_id, NULL, threaded_channel, (void *)args) ){
				fprintf(stderr, "Failed to thread channel, calling direct!\n");
				threaded_channel((void *)args);
			} else {
				pthread_detach(thread_id);
			}
		} else {
			g_static_rec_mutex_lock_full(context->thread_mutex, numlocks);
		}
	}
}

void event_connect(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count){
	struct irc_ctx_t *context = irc_get_ctx(session);
	int numlocks;
	if( g_static_rec_mutex_trylock(context->thread_mutex) ){
		if( ( numlocks = g_static_rec_mutex_unlock_full(context->thread_mutex) ) == 1 ){
			struct func_args *args = (struct func_args *)malloc(sizeof(struct func_args));
			args->session = session;
			pthread_t thread_id;
			if( pthread_create(&thread_id, NULL, threaded_connect, (void *)args) ){
				fprintf(stderr, "Failed to thread connect, calling direct!\n");
				threaded_connect((void *)args);
			} else {
				pthread_detach(thread_id);
			}
		} else {
			g_static_rec_mutex_lock_full(context->thread_mutex, numlocks);
		}
	}
}

void event_ctcp_req(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count){

}

void event_join(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count){
	struct irc_ctx_t *context = irc_get_ctx(session);
	int numlocks;
	if( g_static_rec_mutex_trylock(context->thread_mutex) ){
		if( ( numlocks = g_static_rec_mutex_unlock_full(context->thread_mutex) ) == 1 ){
			struct func_args *args = (struct func_args *)malloc(sizeof(struct func_args));
			args->session = session;
			args->origin = strdup(origin);
			args->params = (char **)calloc(count, sizeof(char *));
			int i;
			for( i = 0; i < count; ++i ){
				args->params[i] = strdup(params[i]);
			}
			args->count = count;
			pthread_t thread_id;
			if( pthread_create(&thread_id, NULL, threaded_join, (void *)args) ){
				fprintf(stderr, "Failed to thread join, calling direct!\n");
				threaded_join((void *)args);
			} else {
				pthread_detach(thread_id);
			}
		} else {
			g_static_rec_mutex_lock_full(context->thread_mutex, numlocks);
		}
	}
}

void event_kick(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count){
	struct irc_ctx_t *context = irc_get_ctx(session);
	int numlocks;
	if( g_static_rec_mutex_trylock(context->thread_mutex) ){
		if( ( numlocks = g_static_rec_mutex_unlock_full(context->thread_mutex) ) == 1 ){
			struct func_args *args = (struct func_args *)malloc(sizeof(struct func_args));
			args->session = session;
			args->origin = strdup(origin);
			args->params = (char **)calloc(count, sizeof(char *));
			int i;
			for( i = 0; i < count; ++i ){
				args->params[i] = strdup(params[i]);
			}
			args->count = count;
			pthread_t thread_id;
			if( pthread_create(&thread_id, NULL, threaded_kick, (void *)args) ){
				fprintf(stderr, "Failed to thread kick, calling direct!\n");
				threaded_kick((void *)args);
			} else {
				pthread_detach(thread_id);
			}
		} else {
			g_static_rec_mutex_lock_full(context->thread_mutex, numlocks);
		}
	}
}

void event_nick(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count){
	struct irc_ctx_t *context = irc_get_ctx(session);
	int numlocks;
	if( g_static_rec_mutex_trylock(context->thread_mutex) ){
		if( ( numlocks = g_static_rec_mutex_unlock_full(context->thread_mutex) ) == 1 ){
			struct func_args *args = (struct func_args *)malloc(sizeof(struct func_args));
			args->session = session;
			args->origin = strdup(origin);
			args->params = (char **)calloc(count, sizeof(char *));
			int i;
			for( i = 0; i < count; ++i ){
				args->params[i] = strdup(params[i]);
			}
			args->count = count;
			pthread_t thread_id;
			if( pthread_create(&thread_id, NULL, threaded_nick, (void *)args) ){
				fprintf(stderr, "Failed to thread nick, calling direct!\n");
				threaded_nick((void *)args);
			} else {
				pthread_detach(thread_id);
			}
		} else {
			g_static_rec_mutex_lock_full(context->thread_mutex, numlocks);
		}
	}
}

void event_numeric(irc_session_t *session, unsigned int event, const char *origin, const char **params, unsigned int count){
	struct irc_ctx_t *context = irc_get_ctx(session);
	int numlocks;
	if( g_static_rec_mutex_trylock(context->thread_mutex) ){
		if( ( numlocks = g_static_rec_mutex_unlock_full(context->thread_mutex) ) == 1 ){
			struct func_args *args = (struct func_args *)malloc(sizeof(struct func_args));
			args->session = session;
			args->event = event;
			args->origin = strdup(origin);
			args->params = (char **)calloc(count, sizeof(char *));
			int i;
			for( i = 0; i < count; ++i ){
				args->params[i] = strdup(params[i]);
			}
			args->count = count;
			pthread_t thread_id;
			if( pthread_create(&thread_id, NULL, threaded_numeric, (void *)args) ){
				fprintf(stderr, "Failed to thread numeric, calling direct!\n");
				threaded_numeric((void *)args);
			} else {
				pthread_detach(thread_id);
			}
		} else {
			g_static_rec_mutex_lock_full(context->thread_mutex, numlocks);
		}
	}
}

void event_part(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count){
	struct irc_ctx_t *context = irc_get_ctx(session);
	int numlocks;
	if( g_static_rec_mutex_trylock(context->thread_mutex) ){
		if( ( numlocks = g_static_rec_mutex_unlock_full(context->thread_mutex) ) == 1 ){
			struct func_args *args = (struct func_args *)malloc(sizeof(struct func_args));
			args->session = session;
			args->origin = strdup(origin);
			args->count = count;
			pthread_t thread_id;
			if( pthread_create(&thread_id, NULL, threaded_part, (void *)args) ){
				fprintf(stderr, "Failed to thread part, calling direct!\n");
				threaded_part((void *)args);
			} else {
				pthread_detach(thread_id);
			}
		} else {
			g_static_rec_mutex_lock_full(context->thread_mutex, numlocks);
		}
	}
}

void event_privmsg(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count){
	struct irc_ctx_t *context = irc_get_ctx(session);
	int numlocks;
	if( g_static_rec_mutex_trylock(context->thread_mutex) ){
		if( ( numlocks = g_static_rec_mutex_unlock_full(context->thread_mutex) ) == 1 ){
			struct func_args *args = (struct func_args *)malloc(sizeof(struct func_args));
			args->session = session;
			args->origin = strdup(origin);
			args->params = (char **)calloc(count, sizeof(char *));
			int i;
			for( i = 0; i < count; ++i ){
				args->params[i] = strdup(params[i]);
			}
			args->count = count;
			pthread_t thread_id;
			if( pthread_create(&thread_id, NULL, threaded_privmsg, (void *)args) ){
				fprintf(stderr, "Failed to thread privmsg, calling direct!i\n");
				threaded_privmsg((void *)args);
			} else {
				pthread_detach(thread_id);
			}
		} else {
			g_static_rec_mutex_lock_full(context->thread_mutex, numlocks);
		}
	}
}

void event_quit(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count){
	struct irc_ctx_t *context = irc_get_ctx(session);
	int numlocks;
	if( g_static_rec_mutex_trylock(context->thread_mutex) ){
		if( ( numlocks = g_static_rec_mutex_unlock_full(context->thread_mutex) ) == 1 ){
			struct func_args *args = (struct func_args *)malloc(sizeof(struct func_args));
			args->session = session;
			args->origin = strdup(origin);
			pthread_t thread_id;
			if( pthread_create(&thread_id, NULL, threaded_quit, (void *)args) ){
				fprintf(stderr, "Failed to thread quit, calling direct!i\n");
				threaded_quit((void *)args);
			} else {
				pthread_detach(thread_id);
			}
		} else {
			g_static_rec_mutex_lock_full(context->thread_mutex, numlocks);
		}
	}
}
