#include "global.h"

void *threaded_channel(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	
	free(struct_args->origin);
	int i = 0;
	for( i = 0; i < struct_args->count; ++i ){
		free(struct_args->params[i]);
	}
	free(struct_args->params);
	free(struct_args);
	return NULL;
}

void *threaded_connect(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	irc_cmd_user_mode(struct_args->session, "+BpTx");
	irc_cmd_join(struct_args->session, WOLFBOT_CHANNEL, NULL);
	free(struct_args);
	return NULL;
}

void *threaded_join(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	if( !strcmp(WOLFBOT_NICK, struct_args->origin) ){
		irc_cmd_msg(struct_args->session, struct_args->params[0], "I'm here now.  Let's get this started!");
		irc_cmd_msg(struct_args->session, struct_args->params[0], "To join the game, message !join to me.  To start the game at least 90 seconds after the first person joins, say !start in the channel");
	} else {
		char *msg_buffer = (char *)calloc(247, sizeof(char));
		strcat(msg_buffer, "Welcome to ");
		strcat(msg_buffer, WOLFBOT_CHANNEL);
		strcat(msg_buffer, ", ");
		strcat(msg_buffer, struct_args->origin);
		strcat(msg_buffer, "!");
		irc_cmd_msg(struct_args->session, struct_args->origin, msg_buffer);
		free(msg_buffer);
		struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(struct_args->session);
		if( context->game_data.is_running ){
			irc_cmd_msg(struct_args->session, struct_args->origin, "The game is currently running.  Please wait until the game is finished and then message !join to me.");
		} else {
			irc_cmd_msg(struct_args->session, struct_args->origin, "The game is not running.  Message !join to me to play.");
		}
	}
	free(struct_args->origin);
	int i = 0;
	for( i = 0; i < struct_args->count; ++i ){
		free(struct_args->params[i]);
	}
	free(struct_args->params);
	free(struct_args);
	return NULL;
}

void *threaded_kick(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	
	free(struct_args->origin);
	int i = 0;
	for( i = 0; i < struct_args->count; ++i ){
		free(struct_args->params[i]);
	}
	free(struct_args->params);
	free(struct_args);
	return NULL;
}

void *threaded_nick(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	
	free(struct_args->origin);
	int i = 0;
	for( i = 0; i < struct_args->count; ++i ){
		free(struct_args->params[i]);
	}
	free(struct_args->params);
	free(struct_args);
	return NULL;
}

void *threaded_part(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	
	free(struct_args->origin);
	free(struct_args);
	return NULL;
}

void *threaded_privmsg(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	struct irc_ctx_t *context = (struct irc_ctx_t *)irc_get_ctx(struct_args->session);
	char *original_speech = strdup(struct_args->params[1]);
	char *tokenizer_pointer;
	char *command = strtok_r(struct_args->params[1], " ", &tokenizer_pointer);
	char *nick = strtok_r(NULL, " ", &tokenizer_pointer);
	if( !strcmp(command, "!join") ){
		if( g_hash_table_lookup(context->game_data.players, struct_args->origin) ){
			irc_cmd_msg(struct_args->session, struct_args->origin, "You're already in the game!");
		} else {
			g_hash_table_insert(context->playerdb, strdup(struct_args->origin), new_entry);
			char *mode_buffer = (char *)calloc(36, sizeof(char));
			strcat(mode_buffer, "+v ");
			strcat(mode_buffer, struct_args->origin);
			if( irc_cmd_channel_mode(struct_args->session, WOLFBOT_CHANNEL, mode_buffer) ){
				fprintf(stderr, irc_strerror(irc_errno(struct_args->session)));
			}
			free(mode_buffer);
		}
	} else if( !strcmp(command, "!kill") ){
		struct playerdb_entry *player_entry = g_hash_table_lookup(context->playerdb, struct_args->origin);
		if( player_entry && player_entry->role == WOLF ){
			if( context->game_data.is_night ){
				context->game_data.marked_for_death = strdup(nick);
				int i;
				char ok = 1;
			} else {
				irc_cmd_msg(struct_args->session, struct_args->origin, "It's not night!");
			}
		} else if( !player_entry ){
			irc_cmd_msg(struct_args->session, struct_args->origin, "You're not in the game!");
		} else if( player_entry->role != WOLF ){
			irc_cmd_msg(struct_args->session, struct_args->origin, "You're not a wolf!");
		}

	} else if( !strcmp(command, "!visit") ){

	} else if( !strcmp(command, "!see") ){

	}
	free(original_speech);
	free(struct_args->origin);
	int i = 0;
	for( i = 0; i < struct_args->count; ++i ){
		free(struct_args->params[i]);
	}
	free(struct_args->params);
	free(struct_args);
	return NULL;
}

void *threaded_quit(void *args){
	struct func_args *struct_args = (struct func_args *)args;
	
	free(struct_args->origin);
	free(struct_args);
	return NULL;
}
