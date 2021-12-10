#ifndef __MAIN_H__
#define __MAIN_H__

#include "../lib/raylib/src/raylib.h"

#define THICKNESS 3.0f
#define MAX_CLI 4
extern int SCR_WIDTH;
extern int SCR_HEIGHT;
#define BLOCK SCR_WIDTH / 3
#ifndef _WIN32
	#define SOCK int
#else
	#define SOCK unsigned int
#endif
#ifdef ANDROID
	#include <android/log.h> // for android debugging
	#define LOGI(...)                                                                          \
		{                                                                                      \
			char *log_msg = calloc(1, 1024);                                                   \
			sprintf(log_msg, __VA_ARGS__);                                                     \
			__android_log_print(ANDROID_LOG_VERBOSE, "Simple_TTT", "Simple TTT: %s", log_msg); \
			free(log_msg);                                                                     \
		}
	#define LOGW(...)                                                                       \
		{                                                                                   \
			char *log_msg = calloc(1, 1024);                                                \
			sprintf(log_msg, __VA_ARGS__);                                                  \
			__android_log_print(ANDROID_LOG_WARN, "Simple_TTT", "Simple TTT: %s", log_msg); \
			free(log_msg);                                                                  \
		}
	#define LOGE(...)                                                                        \
		{                                                                                    \
			char *log_msg = calloc(1, 1024);                                                 \
			sprintf(log_msg, __VA_ARGS__);                                                   \
			__android_log_print(ANDROID_LOG_ERROR, "Simple_TTT", "Simple TTT: %s", log_msg); \
			free(log_msg);                                                                   \
		}
#else
	// #define LOGI(...) fprintf(stderr, "%s\n", TextFormat(__VA_ARGS__))
	// #define LOGW(...) fprintf(stderr, "\x1b[33m\x1b[0m\n", __VA_ARGS__)
	// #define LOGE(...) fprintf(stderr, "\x1b[31m%s\x1b[0m\n", __VA_ARGS__)
	#define LOGI(...)                         \
		{                                     \
			char *log_msg = calloc(1, 1024);  \
			sprintf(log_msg, __VA_ARGS__);    \
			fprintf(stdout, "%s\n", log_msg); \
			free(log_msg);                    \
		}
	#define LOGW(...)                                        \
		{                                                    \
			char *log_msg = calloc(1, 1024);                 \
			sprintf(log_msg, __VA_ARGS__);                   \
			fprintf(stderr, "\x1b[33m%s\x1b[0m\n", log_msg); \
			free(log_msg);                                   \
		}
	#define LOGE(...)                                        \
		{                                                    \
			char *log_msg = calloc(1, 1024);                 \
			sprintf(log_msg, __VA_ARGS__);                   \
			fprintf(stderr, "\x1b[31m%s\x1b[0m\n", log_msg); \
			free(log_msg);                                   \
		}
#endif

struct client_data {
	int game_grid[3][3];
	int is_btn_pressed;
	int is_game_over;
	int click_position[2];
	int ready;
	int turn;
	int winsP[2];
	int winner;
	int user_id;
	char users[4][32];
	int bot_hardness;
	int game_mode;
	SOCK sockfd;
};

struct server_data {
	char IP_ADDRESS[16];
	int PORT;
	long int server_tid;
	int thread_id;
	SOCK clifd[4];
	long int client_tid[4];
	int client_running;
	struct client_data data;
};

void *window_main();

#endif // __MAIN_H__