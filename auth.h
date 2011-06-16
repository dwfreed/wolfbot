void auth_init(struct irc_ctx_t *context);
void auth_fini(struct irc_ctx_t *context);
void check_auth(irc_session_t *session, char *nick, char *command);
int notice(struct func_args *args);
int numeric(struct func_args *args);
int privmsg(struct func_args *args);
