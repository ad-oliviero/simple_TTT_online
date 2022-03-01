#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include "include/gui.h"
#include "../lib/raylib-nuklear/include/raylib-nuklear.h"
#include "../lib/raylib/src/raylib.h"
#include "include/client.h"
#include "include/main.h"
#include "include/server.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

extern int game_running;
extern Rectangle game[3][3];

#ifdef ANDROID
	#include <android_native_app_glue.h>
	#include <jni.h>
void toggleKeyboard(bool show) {
	// some "aliases"
	JavaVM *lJavaVM = app->activity->vm;
	JNIEnv *lJNIEnv = app->activity->env;

	// java vm args
	JavaVMAttachArgs lJavaVMAttachArgs;
	lJavaVMAttachArgs.version = JNI_VERSION_1_6;
	lJavaVMAttachArgs.name	  = "NativeThread";
	lJavaVMAttachArgs.group	  = NULL;

	// attaching to the java vm
	if ((*lJavaVM)->AttachCurrentThread(lJavaVM, &lJNIEnv, &lJavaVMAttachArgs) == JNI_ERR) return;

	// get native activity class and the method that we need
	jobject lNativeActivity	   = app->activity->clazz;
	jclass ClassNativeActivity = (*lJNIEnv)->GetObjectClass(lJNIEnv, lNativeActivity);
	// method name depending on the show parameter
	char *lMethodName = NULL;
	if (show)
		lMethodName = "showKeyboard";
	else
		lMethodName = "hideKeyboard";
	jmethodID _method = (*lJNIEnv)->GetMethodID(lJNIEnv, ClassNativeActivity, lMethodName, "()V");

	// call that method
	(*lJNIEnv)->CallVoidMethod(lJNIEnv, lNativeActivity, _method);
}

int SCR_HEIGHT = 0;
int SCR_WIDTH  = 0;
#else
int SCR_WIDTH  = 450;
int SCR_HEIGHT = 800;
#endif

void initHitBox() { // creating boxes to detect touch or mouse clicks
	for (int i = 0; i < 9; i++) {
		game[i / 3][i % 3].x	  = (BLOCK * (i % 3)) + THICKNESS;
		game[i / 3][i % 3].y	  = (BLOCK * (i / 3)) + THICKNESS;
		game[i / 3][i % 3].height = BLOCK - 1;
		game[i / 3][i % 3].width  = BLOCK - 1;
	}
}

void grid() {
	// Borders
	DrawLineEx((Vector2){0, 1}, (Vector2){SCR_WIDTH, 1}, THICKNESS + 1, BLACK);
	DrawLineEx((Vector2){1, 1}, (Vector2){1, SCR_WIDTH}, THICKNESS + 1, BLACK);
	DrawLineEx((Vector2){0, SCR_WIDTH}, (Vector2){SCR_WIDTH, SCR_WIDTH}, THICKNESS + 1, BLACK);
	DrawLineEx((Vector2){SCR_WIDTH, 0}, (Vector2){SCR_WIDTH, SCR_WIDTH}, THICKNESS + 1, BLACK);

	// vertical grid
	DrawLineEx((Vector2){BLOCK, 0}, (Vector2){BLOCK, SCR_WIDTH}, THICKNESS + 1, BLACK);
	DrawLineEx((Vector2){BLOCK * 2, 0}, (Vector2){BLOCK * 2, SCR_WIDTH}, THICKNESS + 1, BLACK);

	// horizontal grid
	DrawLineEx((Vector2){0, BLOCK}, (Vector2){SCR_WIDTH, BLOCK}, THICKNESS + 1, BLACK);
	DrawLineEx((Vector2){0, BLOCK * 2}, (Vector2){SCR_WIDTH, BLOCK * 2}, THICKNESS + 1, BLACK);
}

int join_window(char *IP_ADDRESS, int *PORT, struct client_data *data, struct nk_context *ctx) {
	int selection_step = 0, game_mode = -1;
	bool game_hosting = false;
#ifdef ANDROID
	bool is_keyboard_open = false;
#endif
	char *title_label[4]	  = {"Select game mode", "Select Game Hosting", "Join match", "Host match"};
	char *btn_label[4]		  = {"Single Player", "Multi Player", "Host", "Join"};
	struct nk_rect wg_push[2] = {{15, (SCR_HEIGHT / 2) - 60, SCR_WIDTH - 35, (SCR_HEIGHT / 18)},
								 {15, (SCR_HEIGHT / 2), SCR_WIDTH - 35, (SCR_HEIGHT / 18)}};
	sprintf(data->username, "Nickname");
	sprintf(IP_ADDRESS, "127.0.0.1:5555");
	*PORT = 5555;
	while (!game_running && !WindowShouldClose()) {
		UpdateNuklear(ctx);
		if (nk_begin(ctx, "Join Window", (struct nk_rect){-1, -1, SCR_WIDTH + 1, SCR_HEIGHT + 1}, NK_WINDOW_NO_SCROLLBAR)) {
			nk_layout_space_begin(ctx, NK_STATIC, SCR_HEIGHT / STTT_TEXT_SIZE, 3);
			// draw title
			nk_layout_space_push(ctx, (struct nk_rect){(SCR_WIDTH - MeasureText(title_label[selection_step + game_hosting], STTT_TEXT_SIZE)) / 2, (SCR_HEIGHT / 2) - (SCR_HEIGHT / 9) - 20, SCR_WIDTH - MeasureText(title_label[selection_step], STTT_TEXT_SIZE), 40});
			nk_label(ctx, title_label[selection_step + game_hosting], NK_TEXT_ALIGN_LEFT);
			// widget 1
			nk_layout_space_push(ctx, wg_push[0]);
			if (selection_step == 0) { // single player
				if (nk_button_label(ctx, btn_label[selection_step * 2])) {
					game_running   = 1;
					game_mode	   = 0;
					selection_step = 3;
				}
			} else if (selection_step == 1) { // host a match
				if (nk_button_label(ctx, btn_label[selection_step * 2])) {
					selection_step++;
					game_hosting = true;
				}
			} else if (selection_step == 2) // username
				nk_edit_string_zero_terminated(ctx, NK_EDIT_BOX | NK_EDIT_AUTO_SELECT, data->username, sizeof(data->username), nk_filter_ascii);
			// widget 2
			nk_layout_space_push(ctx, wg_push[1]);
			if (selection_step == 0) { // multi player
				if (nk_button_label(ctx, btn_label[selection_step * 2 + 1])) {
					selection_step++;
					game_mode = 1;
				}
			} else if (selection_step == 1) { // join match
				if (nk_button_label(ctx, btn_label[selection_step * 2 + 1])) {
					selection_step++;
					game_hosting = false;
				}
			} else if (selection_step == 2)
				nk_edit_string_zero_terminated(ctx, NK_EDIT_BOX | NK_EDIT_AUTO_SELECT, IP_ADDRESS, 32, nk_filter_ascii);
			nk_layout_space_end(ctx);
			// end selection if multiplayer
			if (GetKeyPressed() == KEY_ENTER)
				selection_step = 3;
			if (selection_step == 3) {
				nk_end(ctx);
				char *port = strchr(IP_ADDRESS, ':');
				if (port) {
					*PORT	= atoi(port + 1);
					port[0] = '\0';
				}
				game_running = 1;
				game_mode += game_hosting;
			}
		}
		nk_end(ctx);
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawNuklear(ctx);
		EndDrawing();
	}
	return game_mode;
}

void matchInfo(struct client_data *data) { // draw match info
	const char *info_text = TextFormat("It's %s %s turn!", data->turn ? data->users[0] : data->users[1], data->turn ? "(x)" : "(O)");
	DrawText(info_text, (SCR_WIDTH - MeasureText(info_text, STTT_TEXT_SIZE)) / 2, BLOCK * 3 + 10, STTT_TEXT_SIZE, BLACK);
	DrawText(TextFormat("%s: %i\n%s: %i\n", data->users[0], data->winsP[0], data->users[1], data->winsP[1]), 10, BLOCK * 3 + 40, STTT_TEXT_SIZE, BLACK);
	DrawText(TextFormat("%s\n", data->local_ip), 10, SCR_HEIGHT - STTT_TEXT_SIZE * 2, STTT_TEXT_SIZE, BLACK);
}