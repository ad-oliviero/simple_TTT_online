#ifndef __MAIN_H__
#define __MAIN_H__

extern float SCR_WIDTH, SCR_HEIGHT;
#define BLOCK SCR_WIDTH / 3
#ifdef _WIN32
	#define SOCK unsigned int
#else
	#define SOCK int
#endif

#ifdef ANDROID
	#include <jni.h>
	#include <android/log.h>
	#define THICKNESS 7.0f
	#define LOGV(...)                                                                          \
		{                                                                                      \
			char *log_msg = (char *)calloc(1, 1024);                                           \
			sprintf(log_msg, __VA_ARGS__);                                                     \
			__android_log_print(ANDROID_LOG_VERBOSE, "Simple_TTT", "Simple TTT: %s", log_msg); \
			free(log_msg);                                                                     \
		}
	#define LOGI(...)                                                                       \
		{                                                                                   \
			char *log_msg = (char *)calloc(1, 1024);                                        \
			sprintf(log_msg, __VA_ARGS__);                                                  \
			__android_log_print(ANDROID_LOG_INFO, "Simple_TTT", "Simple TTT: %s", log_msg); \
			free(log_msg);                                                                  \
		}
	#define LOGW(...)                                                                       \
		{                                                                                   \
			char *log_msg = (char *)calloc(1, 1024);                                        \
			sprintf(log_msg, __VA_ARGS__);                                                  \
			__android_log_print(ANDROID_LOG_WARN, "Simple_TTT", "Simple TTT: %s", log_msg); \
			free(log_msg);                                                                  \
		}
	#define LOGE(...)                                                                        \
		{                                                                                    \
			char *log_msg = (char *)calloc(1, 1024);                                         \
			sprintf(log_msg, __VA_ARGS__);                                                   \
			__android_log_print(ANDROID_LOG_ERROR, "Simple_TTT", "Simple TTT: %s", log_msg); \
			free(log_msg);                                                                   \
		}
extern struct android_app *app;
extern "C" struct android_app *GetAndroidApp();
#else
	#define THICKNESS 3.0f
	#define LOGV(...)                                \
		{                                            \
			char *log_msg = (char *)calloc(1, 1024); \
			sprintf(log_msg, __VA_ARGS__);           \
			fprintf(stdout, "%s\n", log_msg);        \
			free(log_msg);                           \
		}
	#define LOGI(...)                                \
		{                                            \
			char *log_msg = (char *)calloc(1, 1024); \
			sprintf(log_msg, __VA_ARGS__);           \
			fprintf(stdout, "%s\n", log_msg);        \
			free(log_msg);                           \
		}
	#define LOGW(...)                                        \
		{                                                    \
			char *log_msg = (char *)calloc(1, 1024);         \
			sprintf(log_msg, __VA_ARGS__);                   \
			fprintf(stderr, "\x1b[33m%s\x1b[0m\n", log_msg); \
			free(log_msg);                                   \
		}
	#define LOGE(...)                                        \
		{                                                    \
			char *log_msg = (char *)calloc(1, 1024);         \
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
	int uid;
	char users[4][32];
	char username[32];
	int game_mode;
	char *local_ip;
	SOCK sockfd;
};

struct server_data {
	char IP_ADDRESS[16];
	int PORT;
	int client_count;
	int client_running;
	int ready_check[4];
	SOCK clifd[4];
	struct client_data data;
};

void *window_main();

#endif // __MAIN_H__