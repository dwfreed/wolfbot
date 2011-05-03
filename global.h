#include <curl/curl.h>
#include <glib.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "libircclient.h"
#include "callbacks.h"
#include "threaded_functions.h"
#include "game.h"
#define WOLFBOT_SERVER "lug.mtu.edu"
#define WOLFBOT_PORT 6667
#define WOLFBOT_NICK "wolfgame"
#define WOLFBOT_NAME "wolfgame"
#define WOLFBOT_USER "wolfgame"
#define WOLFBOT_CHANNEL "#wolfgame"
#define USE_IPv6 0
struct func_args {
	irc_session_t *session;
	char *origin;
	char **params;
	unsigned int count;
};
struct game_info {
	char is_running;
	char is_night;
	char bullets;
	GHashTable *players;
	GHashTable *villagers;
	GHashTable *wolves;
	GHashTable *harlots;
	GHashTable *seers;
	GHashTable *traitors;
	GHashTable *detectives;
	char *marked_for_death;

};
struct irc_ctx_t {
	struct game_info game_data;
	GMutex *game_data_mutex;
};
