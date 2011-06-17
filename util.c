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

static void die_timer_callback(union sigval args){
	struct timer_callback_params *die_timer_callback_params = (struct timer_callback_params *)args.sival_ptr;
	timer_delete(die_timer_callback_params->timer);
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(die_timer_callback_params->session);
	g_mutex_lock(context->thread_mutex);
	if( g_atomic_int_get(&context->thread_count) < 1 ){
		g_atomic_int_set(&context->thread_count, -1);
		irc_cmd_channel_mode(die_timer_callback_params->session, config_get_string(context->config, "bot.channel.channel"), config_get_string(context->config, "bot.channel.mode_on_end"));
		irc_cmd_channel_mode(die_timer_callback_params->session, config_get_string(context->config, "bot.channel.channel"), config_get_string(context->config, "bot.channel.mode_on_leave"));
		irc_cmd_quit(die_timer_callback_params->session, "Bye!");
		free(die_timer_callback_params);
	} else {
		struct sigevent die_timer_event;
		die_timer_event.sigev_notify = SIGEV_THREAD;
		union sigval die_timer_callback_data;
		die_timer_callback_data.sival_ptr = die_timer_callback_params;
		die_timer_event.sigev_value = die_timer_callback_data;
		die_timer_event.sigev_notify_function = die_timer_callback;
		die_timer_event.sigev_notify_attributes = NULL;
		timer_create(CLOCK_REALTIME, &die_timer_event, &die_timer_callback_params->timer);
		struct itimerspec die_timer_time = { {0, 0}, {1, 0} };
		timer_settime(die_timer_callback_params->timer, 0, &die_timer_time, NULL);
	}
	g_mutex_unlock(context->thread_mutex);
}

void *die(void *args){
	irc_session_t *session = (irc_session_t *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	irc_cmd_msg(session, config_get_string(context->config, "bot.channel.channel"), "Die, die, die!");
	context->restart = FALSE;
	g_mutex_lock(context->thread_mutex);
	if( g_atomic_int_get(&context->thread_count) == 1 ){
		irc_cmd_channel_mode(session, config_get_string(context->config, "bot.channel.channel"), config_get_string(context->config, "bot.channel.mode_on_end"));
		irc_cmd_channel_mode(session, config_get_string(context->config, "bot.channel.channel"), config_get_string(context->config, "bot.channel.mode_on_leave"));
		irc_cmd_quit(session, "Bye!");
	} else {
		struct timer_callback_params *die_timer_callback_params = (struct timer_callback_params *)malloc(sizeof(struct timer_callback_params));
		die_timer_callback_params->session = session;
		struct sigevent die_timer_event;
		die_timer_event.sigev_notify = SIGEV_THREAD;
		union sigval die_timer_callback_data;
		die_timer_callback_data.sival_ptr = die_timer_callback_params;
		die_timer_event.sigev_value = die_timer_callback_data;
		die_timer_event.sigev_notify_function = die_timer_callback;
		die_timer_event.sigev_notify_attributes = NULL;
		timer_create(CLOCK_REALTIME, &die_timer_event, &die_timer_callback_params->timer);
		struct itimerspec die_timer_time = { {0, 0}, {1, 0} };
		timer_settime(die_timer_callback_params->timer, 0, &die_timer_time, NULL);
	}
	g_mutex_unlock(context->thread_mutex);
	g_atomic_int_add(&context->thread_count, -1);
	return NULL;
}

static void restart_timer_callback(union sigval args){
	struct timer_callback_params *restart_timer_callback_params = (struct timer_callback_params *)args.sival_ptr;
	timer_delete(restart_timer_callback_params->timer);
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(restart_timer_callback_params->session);
	g_mutex_lock(context->thread_mutex);
	if( g_atomic_int_get(&context->thread_count) < 1 ){
		g_atomic_int_set(&context->thread_count, -1);
		irc_cmd_channel_mode(restart_timer_callback_params->session, config_get_string(context->config, "bot.channel.channel"), config_get_string(context->config, "bot.channel.mode_on_end"));
		irc_cmd_channel_mode(restart_timer_callback_params->session, config_get_string(context->config, "bot.channel.channel"), config_get_string(context->config, "bot.channel.mode_on_leave"));
		irc_cmd_quit(restart_timer_callback_params->session, "Be back soon!");
		free(restart_timer_callback_params);
	} else {
		struct sigevent restart_timer_event;
		restart_timer_event.sigev_notify = SIGEV_THREAD;
		union sigval restart_timer_callback_data;
		restart_timer_callback_data.sival_ptr = restart_timer_callback_params;
		restart_timer_event.sigev_value = restart_timer_callback_data;
		restart_timer_event.sigev_notify_function = restart_timer_callback;
		restart_timer_event.sigev_notify_attributes = NULL;
		timer_create(CLOCK_REALTIME, &restart_timer_event, &restart_timer_callback_params->timer);
		struct itimerspec restart_timer_time = { {0, 0}, {1, 0} };
		timer_settime(restart_timer_callback_params->timer, 0, &restart_timer_time, NULL);
	}
	g_mutex_unlock(context->thread_mutex);
}

void *restart(void *args){
	irc_session_t *session = (irc_session_t *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	irc_cmd_msg(session, config_get_string(context->config, "bot.channel.channel"), "Preparing to restart");
	context->restart = TRUE;
	g_mutex_lock(context->thread_mutex);
	if( g_atomic_int_get(&context->thread_count) == 1 ){
		irc_cmd_channel_mode(session, config_get_string(context->config, "bot.channel.channel"), config_get_string(context->config, "bot.channel.mode_on_end"));
		irc_cmd_channel_mode(session, config_get_string(context->config, "bot.channel.channel"), config_get_string(context->config, "bot.channel.mode_on_leave"));
		irc_cmd_quit(session, "Be back soon!");
	} else {
		struct timer_callback_params *restart_timer_callback_params = (struct timer_callback_params *)malloc(sizeof(struct timer_callback_params));
		restart_timer_callback_params->session = session;
		struct sigevent restart_timer_event;
		restart_timer_event.sigev_notify = SIGEV_THREAD;
		union sigval restart_timer_callback_data;
		restart_timer_callback_data.sival_ptr = restart_timer_callback_params;
		restart_timer_event.sigev_value = restart_timer_callback_data;
		restart_timer_event.sigev_notify_function = restart_timer_callback;
		restart_timer_event.sigev_notify_attributes = NULL;
		timer_create(CLOCK_REALTIME, &restart_timer_event, &restart_timer_callback_params->timer);
		struct itimerspec restart_timer_time = { {0, 0}, {1, 0} };
		timer_settime(restart_timer_callback_params->timer, 0, &restart_timer_time, NULL);
	}
	g_mutex_unlock(context->thread_mutex);
	g_atomic_int_add(&context->thread_count, -1);
	return NULL;
}

void *upgrade(void *args){
	irc_session_t *session = (irc_session_t *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	int hg_ret_val = system("hg in --bundle incoming.bundle");
	if( WEXITSTATUS(hg_ret_val) == 0 ){
		system("hg pull -u incoming.bundle");
		remove("incoming.bundle");
		system("make clean");
	}
	int make_ret_val = system("make -q");
	if( WEXITSTATUS(make_ret_val) ){
		system("make");
		FILE *wolfbot_exec;
		if( !(wolfbot_exec = fopen("wolfbot", "r")) ){
			irc_cmd_msg(session, config_get_string(context->config, "bot.channel.channel"), "Upgrade failed!");
		} else {
			fclose(wolfbot_exec);
			irc_cmd_msg(session, config_get_string(context->config, "bot.channel.channel"), "Upgrade complete!");
			if( g_mutex_trylock(context->thread_mutex) ){
				g_mutex_unlock(context->thread_mutex);
				if( g_atomic_int_get(&context->thread_count) > -1 ){
					g_atomic_int_inc(&context->thread_count);
					pthread_t thread_id;
					if( pthread_create(&thread_id, NULL, restart, session) ){
						fprintf(stderr, "Failed to thread restart, calling direct!\n");
						restart(session);
					} else {
						pthread_detach(thread_id);
					}
				}
			}
		}
	}
	g_atomic_int_add(&context->thread_count, -1);
	return NULL;
}

void *rehash(void *args){
	irc_session_t *session = (irc_session_t *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(session);
	g_atomic_int_add(&context->thread_count, -1);
	return NULL;
}
