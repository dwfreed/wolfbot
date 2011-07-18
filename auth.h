void auth_init(struct irc_ctx_t *context);
void auth_fini(struct irc_ctx_t *context);
void check_auth(struct irc_session *session, char *nick, char *command);
int auth_notice(struct func_args *args);
int auth_numeric(struct func_args *args);
int auth_privmsg(struct func_args *args);
