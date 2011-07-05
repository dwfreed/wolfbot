#include "global.h"

int main(int argc __attribute__((__unused__)), char *argv[]){
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
	struct irc_callbacks *callbacks = (struct irc_callbacks *)calloc(1, sizeof(struct irc_callbacks));
	callbacks->event_channel = event_channel;
	callbacks->event_connect = event_connect;
	callbacks->event_ctcp_req = event_ctcp_req;
	callbacks->event_join = event_join;
	callbacks->event_kick = event_kick;
	callbacks->event_nick = event_nick;
	callbacks->event_notice = event_notice;
	callbacks->event_numeric = event_numeric;
	callbacks->event_part = event_part;
	callbacks->event_privmsg = event_privmsg;
	callbacks->event_quit = event_quit;
	struct irc_session *session;
	session = irc_create_session(callbacks);
	free(callbacks);
	irc_option_set(session, LIBIRCCLIENT_OPTION_STRIPNICKS);
	irc_option_set(session, LIBIRCCLIENT_OPTION_DEBUG);
	g_thread_init(NULL);
	struct irc_ctx_t *context = (struct irc_ctx_t *)calloc(1, sizeof(struct irc_ctx_t));
	context->config = g_hash_table_new_full(g_str_hash, g_str_equal, free, config_free);
	load_config(context->config);
	char *auth_library_path = g_strdup_printf("auth/%s/auth.so", config_get_string(context->config, "bot.auth_plugin"));
	context->auth_library = dlopen(auth_library_path, RTLD_NOW | RTLD_GLOBAL);
	g_free(auth_library_path);
	void (*auth_init_fcn)(struct irc_ctx_t *) = (void (*)(struct irc_ctx_t *))dlsym(context->auth_library, "auth_init");
	auth_init_fcn(context);
	context->thread_mutex = g_mutex_new();
	context->game_data_mutex = g_mutex_new();
	context->game_data.random_source = fopen("/dev/urandom", "r");
	char *game_log_path;
	if( (game_log_path = config_get_string(context->config, "bot.log.game")) ){
		game_log_path = path_expansion(game_log_path);
		context->game_data.game_log = fopen(game_log_path, "a");
		g_free(game_log_path);
		char gamelogopenclose[36];
		time_t gamelogopenclosetime = time(NULL);
		strftime(gamelogopenclose, 36, "--- Log opened %Y-%m-%d %H:%M:%S\n", localtime(&gamelogopenclosetime));
		fprintf(context->game_data.game_log, gamelogopenclose, NULL);
	};
	char *qa_log_path;
	if( (qa_log_path = config_get_string(context->config, "bot.log.qa")) ){
		qa_log_path = path_expansion(qa_log_path);
		context->game_data.qa_log = fopen(qa_log_path, "a");
		g_free(qa_log_path);
		char qalogopenclose[36];
		time_t qalogopenclosetime = time(NULL);
		strftime(qalogopenclose, 36, "--- Log opened %Y-%m-%d %H:%M:%S\n", localtime(&qalogopenclosetime));
		fprintf(context->game_data.qa_log, qalogopenclose, NULL);
	};
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
	if( config_get_int(context->config, "bot.ipv6") ){
		irc_connect6(session, config_get_string(context->config, "bot.server"), config_get_int(context->config, "bot.port"), config_get_string(context->config, "bot.password"), config_get_string(context->config, "bot.nick"), config_get_string(context->config, "bot.user"), config_get_string(context->config, "bot.name"));
	} else {
		irc_connect(session, config_get_string(context->config, "bot.server"), config_get_int(context->config, "bot.port"), config_get_string(context->config, "bot.password"), config_get_string(context->config, "bot.nick"), config_get_string(context->config, "bot.user"), config_get_string(context->config, "bot.name"));
	}
	irc_run(session);
	sleep(1);
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
	if( config_get_string(context->config, "bot.log.qa") ){
		char qalogopenclose[36];
		time_t qalogopenclosetime = time(NULL);
		strftime(qalogopenclose, 36, "--- Log closed %Y-%m-%d %H:%M:%S\n", localtime(&qalogopenclosetime));
		fprintf(context->game_data.qa_log, qalogopenclose, NULL);
		fclose(context->game_data.qa_log);
	}
	if( config_get_string(context->config, "bot.log.game") ){
		char gamelogopenclose[36];
		time_t gamelogopenclosetime = time(NULL);
		strftime(gamelogopenclose, 36, "--- Log closed %Y-%m-%d %H:%M:%S\n", localtime(&gamelogopenclosetime));
		fprintf(context->game_data.game_log, gamelogopenclose, NULL);
		fclose(context->game_data.game_log);
	}
	g_mutex_free(context->game_data_mutex);
	g_mutex_free(context->thread_mutex);
	void (*auth_fini_fcn)(struct irc_ctx_t *) = (void (*)(struct irc_ctx_t *))dlsym(context->auth_library, "auth_fini");
	auth_fini_fcn(context);
	dlclose(context->auth_library);
	g_hash_table_destroy(context->config);
	irc_destroy_session(session);
	if( context->restart ){
		free(context);
		execvp(argv[0], argv);
	}
	free(context);
	return 0;
}

