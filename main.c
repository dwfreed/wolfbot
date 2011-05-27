#include "global.h"

static irc_session_t *session;

void signal_handler(int signal){
	static timer_t signal_timer = 0;
	struct irc_ctx_t *context = irc_get_ctx(session);
	g_static_rec_mutex_lock(context->thread_mutex);
	if( g_atomic_int_get(&context->thread_count) == 0 && !g_atomic_int_dec_and_test(&context->thread_count) ){
		if( signal_timer != 0 ){
			timer_delete(signal_timer);
		}
		irc_cmd_channel_mode(session, context->config_data.WOLFBOT_CHANNEL, context->config_data.WOLFBOT_CMODE_ONEND);
		irc_cmd_channel_mode(session, context->config_data.WOLFBOT_CHANNEL, context->config_data.WOLFBOT_CMODE_ONLEAVE);
		if( signal == SIGINT ){
			irc_cmd_quit(session, "I just received a SIGINT");
		} else if( signal == SIGRTMIN ){
			if( context->restart ){
				irc_cmd_quit(session, "Be back soon!");
			} else {
				irc_cmd_quit(session, "Bye!");
			}
		}
	} else {
		if( !signal_timer ){
			if( signal == SIGINT ){
				irc_cmd_msg(session, context->config_data.WOLFBOT_CHANNEL, "I've just received a SIGINT.  No events will be processed other than what I've already received.");
			} else if( signal == SIGRTMIN ){
				if( context->restart ){
					irc_cmd_msg(session, context->config_data.WOLFBOT_CHANNEL, "I've been asked to restart.  No events will be processed other than what I've already received.");
				} else {
					irc_cmd_msg(session, context->config_data.WOLFBOT_CHANNEL, "I've been asked to quit.  No events will be processed other than what I've already received.");
				}
			}
		} else {
			timer_delete(signal_timer);
		}
		struct sigevent signal_timer_event;
		signal_timer_event.sigev_notify = SIGEV_SIGNAL;
		signal_timer_event.sigev_signo = signal;
		timer_create(CLOCK_REALTIME, &signal_timer_event, &signal_timer);
		struct itimerspec timer_time = { {0, 0}, {1, 0} };
		timer_settime(signal_timer, 0, &timer_time, NULL);
	}
}

int main(int argc, char *argv[]){
	struct rlimit limits;
	getrlimit(RLIMIT_AS, &limits);
	limits.rlim_cur = limits.rlim_max;
	setrlimit(RLIMIT_AS, &limits);
	getrlimit(RLIMIT_CORE, &limits);
	limits.rlim_cur = limits.rlim_max;
	setrlimit(RLIMIT_CORE, &limits);
	getrlimit(RLIMIT_CPU, &limits);
	limits.rlim_cur = limits.rlim_max;
	setrlimit(RLIMIT_CPU, &limits);
	getrlimit(RLIMIT_DATA, &limits);
	limits.rlim_cur = limits.rlim_max;
	setrlimit(RLIMIT_DATA, &limits);
	getrlimit(RLIMIT_FSIZE, &limits);
	limits.rlim_cur = limits.rlim_max;
	setrlimit(RLIMIT_FSIZE, &limits);
	getrlimit(RLIMIT_NOFILE, &limits);
	limits.rlim_cur = limits.rlim_max;
	setrlimit(RLIMIT_NOFILE, &limits);
	getrlimit(RLIMIT_NPROC, &limits);
	limits.rlim_cur = limits.rlim_max;
	setrlimit(RLIMIT_NPROC, &limits);
	getrlimit(RLIMIT_SIGPENDING, &limits);
	limits.rlim_cur = limits.rlim_max;
	setrlimit(RLIMIT_SIGPENDING, &limits);
	getrlimit(RLIMIT_STACK, &limits);
	limits.rlim_cur = limits.rlim_max;
	setrlimit(RLIMIT_STACK, &limits);
	irc_callbacks_t *callbacks = (irc_callbacks_t *)calloc(1, sizeof(irc_callbacks_t));
	callbacks->event_channel = event_channel;
	callbacks->event_connect = event_connect;
	callbacks->event_ctcp_req = event_ctcp_req;
	callbacks->event_join = event_join;
	callbacks->event_kick = event_kick;
	callbacks->event_nick = event_nick;
	callbacks->event_numeric = event_numeric;
	callbacks->event_part = event_part;
	callbacks->event_privmsg = event_privmsg;
	callbacks->event_quit = event_quit;
	session = irc_create_session(callbacks);
	free(callbacks);
	irc_option_set(session, LIBIRC_OPTION_STRIPNICKS);
	g_thread_init(NULL);
	struct irc_ctx_t *context = (struct irc_ctx_t *)calloc(1, sizeof(struct irc_ctx_t));
	context->config_data.library_handle = dlopen("./config.so", RTLD_NOW);
	context->config_data.USE_IPv6 = *(char *)dlsym(context->config_data.library_handle, "USE_IPv6");
	context->config_data.WOLFBOT_SERVER = *(char **)dlsym(context->config_data.library_handle, "WOLFBOT_SERVER");
	context->config_data.WOLFBOT_PORT = *(int *)dlsym(context->config_data.library_handle, "WOLFBOT_PORT");
	context->config_data.WOLFBOT_NICK = *(char **)dlsym(context->config_data.library_handle, "WOLFBOT_NICK");
	context->config_data.WOLFBOT_NAME = *(char **)dlsym(context->config_data.library_handle, "WOLFBOT_NAME");
	context->config_data.WOLFBOT_USER = *(char **)dlsym(context->config_data.library_handle, "WOLFBOT_USER");
	context->config_data.WOLFBOT_UMODE = *(char **)dlsym(context->config_data.library_handle, "WOLFBOT_UMODE");
	context->config_data.WOLFBOT_CHANNEL = *(char **)dlsym(context->config_data.library_handle, "WOLFBOT_CHANNEL");
	context->config_data.WOLFBOT_CMODE_ONENTER = *(char **)dlsym(context->config_data.library_handle, "WOLFBOT_CMODE_ONENTER");
	context->config_data.WOLFBOT_CMODE_ONLEAVE = *(char **)dlsym(context->config_data.library_handle, "WOLFBOT_CMODE_ONLEAVE");
	context->config_data.WOLFBOT_CMODE_ONSTART = *(char **)dlsym(context->config_data.library_handle, "WOLFBOT_CMODE_ONSTART");
	context->config_data.WOLFBOT_CMODE_ONEND = *(char **)dlsym(context->config_data.library_handle, "WOLFBOT_CMODE_ONEND");
	context->config_data.WOLFBOT_CMODE_ONDIE = *(char **)dlsym(context->config_data.library_handle, "WOLFBOT_CMODE_ONDIE");
	context->config_data.WOLFBOT_OWNER = *(char **)dlsym(context->config_data.library_handle, "WOLFBOT_OWNER");
	context->config_data.WOLFBOT_OP_MODE = *(char **)dlsym(context->config_data.library_handle, "WOLFBOT_OP_MODE");
	context->config_data.WOLFBOT_DEOP_MODE = *(char **)dlsym(context->config_data.library_handle, "WOLFBOT_DEOP_MODE");
	context->config_data.WOLFBOT_AUTOVOICE_NICK = *(char **)dlsym(context->config_data.library_handle, "WOLFBOT_AUTOVOICE_NICK");
	context->config_data.WOLFBOT_AUTOVOICE_USERNAME = *(char **)dlsym(context->config_data.library_handle, "WOLFBOT_AUTOVOICE_USERNAME");
	context->config_data.WOLFBOT_AUTOVOICE_HOST_NAME = *(char **)dlsym(context->config_data.library_handle, "WOLFBOT_AUTOVOICE_HOST_NAME");
	context->config_data.WOLFBOT_AUTOVOICE_HOST_IP = *(char **)dlsym(context->config_data.library_handle, "WOLFBOT_AUTOVOICE_HOST_IP");
	context->config_data.WOLFBOT_GAME_LOG = *(char **)dlsym(context->config_data.library_handle, "WOLFBOT_GAME_LOG");
	context->config_data.WOLFBOT_QA_LOG = *(char **)dlsym(context->config_data.library_handle, "WOLFBOT_QA_LOG");
	context->config_data.WOLFBOT_ROLES_ANGELS = *(char *)dlsym(context->config_data.library_handle, "WOLFBOT_ROLES_ANGELS");
	context->config_data.WOLFBOT_ROLES_TRAITORS_AND_DETECTIVES = *(char *)dlsym(context->config_data.library_handle, "WOLFBOT_ROLES_TRAITORS_AND_DETECTIVES");
	g_static_rec_mutex_init(context->thread_mutex);
	context->game_data_mutex = g_mutex_new();
	context->game_data.random_source = fopen("/dev/urandom", "r");
	if( context->config_data.WOLFBOT_GAME_LOG ){
		context->game_data.game_log = fopen(context->config_data.WOLFBOT_GAME_LOG, "a");
		char gamelogopenclose[35];
		time_t gamelogopenclosetime = time(NULL);
		strftime(gamelogopenclose, 35, "--- Log opened %Y-%m-%d %H-%M-%S", localtime(&gamelogopenclosetime));
		fprintf(context->game_data.game_log, gamelogopenclose, NULL);
	};
	if( context->config_data.WOLFBOT_QA_LOG ){
		context->game_data.qa_log = fopen(context->config_data.WOLFBOT_QA_LOG, "a");
		char qalogopenclose[35];
		time_t qalogopenclosetime = time(NULL);
		strftime(qalogopenclose, 35, "--- Log opened %Y-%m-%d %H-%M-%S", localtime(&qalogopenclosetime));
		fprintf(context->game_data.qa_log, qalogopenclose, NULL);
	}
	context->game_data.players = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);
	context->game_data.hash_roles = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, free);
	context->game_data.tree_roles = g_tree_new((GCompareFunc)strcmp);
	context->game_data.lynch_votes = g_tree_new_full(strcmp_data, NULL, NULL, (GDestroyNotify)g_tree_destroy);
	context->game_data.seer_visions = g_hash_table_new(g_str_hash, g_str_equal);
	context->game_data.harlot_guests = g_hash_table_new(g_str_hash, g_str_equal);
	context->game_data.wolf_votes = g_hash_table_new(g_str_hash, g_str_equal);
	context->game_data.gunmen_bullets = g_hash_table_new(g_str_hash, g_str_equal);
	context->game_data.guardian_defends = g_hash_table_new(g_str_hash, g_str_equal);
	context->game_data.guardian_defendants = g_hash_table_new(g_str_hash, g_str_equal);
	context->game_data.detective_ids = g_hash_table_new(g_str_hash, g_str_equal);
	irc_set_ctx(session, context);
	char *pw = getpass("Password: ");
	if( context->config_data.USE_IPv6 ){
		irc_connect6(session, context->config_data.WOLFBOT_SERVER, context->config_data.WOLFBOT_PORT, pw, context->config_data.WOLFBOT_NICK, context->config_data.WOLFBOT_USER, context->config_data.WOLFBOT_NAME);
	} else {
		irc_connect(session, context->config_data.WOLFBOT_SERVER, context->config_data.WOLFBOT_PORT, pw, context->config_data.WOLFBOT_NICK, context->config_data.WOLFBOT_USER, context->config_data.WOLFBOT_NAME);
	}
	memset(pw, 0, strlen(pw) + 1);
	free(pw);
	struct sigaction *signal_sigaction_struct = (struct sigaction *)calloc(1, sizeof(struct sigaction));
	signal_sigaction_struct->sa_handler = signal_handler;
	signal_sigaction_struct->sa_flags = SA_RESTART;
	sigaction(SIGINT, signal_sigaction_struct, NULL);
	sigaction(SIGRTMIN, signal_sigaction_struct, NULL);
	irc_run(session);
	sleep(1);
	free(signal_sigaction_struct);
	g_hash_table_destroy(context->game_data.detective_ids);
	g_hash_table_destroy(context->game_data.guardian_defendants);
	g_hash_table_destroy(context->game_data.guardian_defends);
	g_hash_table_destroy(context->game_data.gunmen_bullets);
	g_hash_table_destroy(context->game_data.wolf_votes);
	g_hash_table_destroy(context->game_data.harlot_guests);
	g_hash_table_destroy(context->game_data.seer_visions);
	g_tree_destroy(context->game_data.lynch_votes);
	g_tree_destroy(context->game_data.tree_roles);
	g_hash_table_destroy(context->game_data.hash_roles);
	g_hash_table_destroy(context->game_data.players);
	fclose(context->game_data.random_source);
	if( context->config_data.WOLFBOT_QA_LOG ){
		char qalogopenclose[35];
		time_t qalogopenclosetime = time(NULL);
		strftime(qalogopenclose, 35, "--- Log closed %Y-%m-%d %H-%M-%S", localtime(&qalogopenclosetime));
		fprintf(context->game_data.qa_log, qalogopenclose, NULL);
		fclose(context->game_data.qa_log);
	}
	if( context->config_data.WOLFBOT_GAME_LOG ){
		char gamelogopenclose[35];
		time_t gamelogopenclosetime = time(NULL);
		strftime(gamelogopenclose, 35, "--- Log closed %Y-%m-%d %H-%M-%S", localtime(&gamelogopenclosetime));
		fprintf(context->game_data.game_log, gamelogopenclose, NULL);
		fclose(context->game_data.game_log);
	}
	g_mutex_free(context->game_data_mutex);
	g_static_rec_mutex_unlock_full(context->thread_mutex);
	g_static_rec_mutex_free(context->thread_mutex);
	dlclose(context->config_data.library_handle);
	irc_destroy_session(session);
	if( context->restart ){
		free(context);
		execvp(argv[0], argv);
	}
	free(context);
	return 0;
}

