#include <dlfcn.h>
#include <glib.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <time.h>
#include <unistd.h>
#include "libircclient.h"
#include "callbacks.h"
#include "game.h"
#include "threaded_functions.h"
#include "util.h"

struct config {
	void *library_handle;
	char USE_IPv6;
	char *WOLFBOT_SERVER;
	int WOLFBOT_PORT;
	char *WOLFBOT_NICK;
	char *WOLFBOT_NAME;
	char *WOLFBOT_USER;
	char *WOLFBOT_UMODE;
	char *WOLFBOT_CHANNEL;
	char *WOLFBOT_CMODE_ONENTER;
	char *WOLFBOT_CMODE_ONLEAVE;
	char *WOLFBOT_CMODE_ONSTART;
	char *WOLFBOT_CMODE_ONEND;
	char *WOLFBOT_CMODE_ONDIE;
	char *WOLFBOT_OWNER;
	char *WOLFBOT_OP_MODE;
	char *WOLFBOT_DEOP_MODE;
	char *WOLFBOT_AUTOVOICE_NICK;
	char *WOLFBOT_AUTOVOICE_USERNAME;
	char *WOLFBOT_AUTOVOICE_HOST_NAME;
	char *WOLFBOT_AUTOVOICE_HOST_IP;
	char *WOLFBOT_GAME_LOG;
	char *WOLFBOT_QA_LOG;
	char WOLFBOT_ROLES_ANGELS;
	char WOLFBOT_ROLES_TRAITORS_AND_DETECTIVES;
};
struct func_args {
	irc_session_t *session;
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
	char can_start;
	char is_pregame;
	char is_running;
	char is_night;
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
	struct config config_data;
	GStaticRecMutex *thread_mutex;
	struct game_info game_data;
	GMutex *game_data_mutex;
};
