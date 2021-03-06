#include <dlfcn.h>
#include <errno.h>
#include <glib.h>
#include <libconfig.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <time.h>
#include <unistd.h>
#include <wordexp.h>
#include "libircclient.h"
#include "callbacks.h"
#include "conf.h"
#include "game.h"
#include "threaded_functions.h"
#include "util.h"

struct func_args {
	struct irc_session *session;
	unsigned int event;
	char *origin;
	char **params;
	unsigned int count;
};
struct game_info {
	FILE *random_source;
	FILE *game_log;
	FILE *qa_log;
	time_t min_start_time;
	char can_start : 1;
	char is_pregame : 1;
	char is_running : 1;
	char is_night : 1;
	GHashTable *players;
	GHashTable *hash_roles;
	GTree *tree_roles;
	GTree *lynch_votes;
	GHashTable *wolf_votes;
	GHashTable *seer_visions;
	GHashTable *harlot_guests;
	GHashTable *gunmen_bullets;
	GHashTable *guardian_defends;
	GHashTable *guardian_defendants;
	GHashTable *detective_ids;
};
struct irc_ctx_t {
	char restart;
	int thread_count;
	GMutex *thread_mutex;
	void *auth_library;
	GHashTable *commands;
	GMutex *commands_mutex;
	GHashTable *config;
	struct game_info game_data;
	GMutex *game_data_mutex;
};
#include "auth.h"
struct config_entry {
	int type;
	int element_type;
	int size;
	void *data;
};

struct timer_callback_params {
	struct irc_session *session;
	timer_t timer;
};
