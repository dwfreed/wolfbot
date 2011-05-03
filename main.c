#include "global.h"

static irc_session_t *session;

void sigint_handler(int signal){
	irc_cmd_quit(session, "I just received a SIGINT");
}

int main(void){
	irc_callbacks_t *callbacks = (irc_callbacks_t *)calloc(1, sizeof(irc_callbacks_t));
	callbacks->event_channel = event_channel;
	callbacks->event_connect = event_connect;
	callbacks->event_ctcp_req = event_ctcp_req;
	callbacks->event_join = event_join;
	callbacks->event_kick = event_kick;
	callbacks->event_nick = event_nick;
	callbacks->event_part = event_part;
	callbacks->event_privmsg = event_privmsg;
	callbacks->event_quit = event_quit;
	session = irc_create_session(callbacks);
	free(callbacks);
	irc_option_set(session, LIBIRC_OPTION_STRIPNICKS);
	g_thread_init(NULL);
	struct irc_ctx_t *context = (struct irc_ctx_t *)calloc(1, sizeof(struct irc_ctx_t));
	context->game_data_mutex = g_mutex_new();
	//Setup hashtables
	irc_set_ctx(session, context);
	curl_global_init(CURL_GLOBAL_NOTHING);
	char *pw = getpass("Password: ");
#if USE_IPv6 == 1
	irc_connect6(session, WOLFBOT_SERVER, WOLFBOT_PORT, pw, WOLFBOT_NICK, WOLFBOT_USER, WOLFBOT_NAME);
#else
	irc_connect(session, WOLFBOT_SERVER, WOLFBOT_PORT, pw, WOLFBOT_NICK, WOLFBOT_USER, WOLFBOT_NAME);
#endif
	memset(pw, 0, strlen(pw) + 1);
	free(pw);
	struct sigaction *sigint_sigaction_struct = (struct sigaction *)calloc(1, sizeof(struct sigaction));
	sigint_sigaction_struct->sa_handler = sigint_handler;
	sigaction(SIGINT, sigint_sigaction_struct, NULL);
	irc_run(session);
	sleep(1);
	free(sigint_sigaction_struct);
	g_hash_table_destroy(context->playerdb);
	free(context);
	curl_global_cleanup();
	irc_destroy_session(session);
	return 0;
}

