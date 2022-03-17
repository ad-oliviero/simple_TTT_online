#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include "include/gui.h"
#ifdef ANDROID
	#include "../lib/imgui/imgui.h"
	#include "../lib/imgui/backends/imgui_impl_android.h"
#else
	#include "../lib/imgui/backends/imgui_impl_glfw.h"
	#include "../lib/imgui/backends/imgui_impl_opengl3.h"
#endif
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
	if ((*lJavaVM).AttachCurrentThread(&lJNIEnv, &lJavaVMAttachArgs) == JNI_ERR) return;

	// get native activity class and the method that we need
	jobject lNativeActivity	   = app->activity->clazz;
	jclass ClassNativeActivity = (*lJNIEnv).GetObjectClass(lNativeActivity);
	// method name depending on the show parameter
	char *lMethodName = NULL;
	if (show)
		lMethodName = (char *)"showKeyboard";
	else
		lMethodName = (char *)"hideKeyboard";
	jmethodID _method = (*lJNIEnv).GetMethodID(ClassNativeActivity, lMethodName, "()V");

	// call that method
	(*lJNIEnv).CallVoidMethod(lNativeActivity, _method);
}
bool getKeyboardState() {
	// some "aliases"
	JavaVM *lJavaVM = app->activity->vm;
	JNIEnv *lJNIEnv = app->activity->env;

	// java vm args
	JavaVMAttachArgs lJavaVMAttachArgs;
	lJavaVMAttachArgs.version = JNI_VERSION_1_6;
	lJavaVMAttachArgs.name	  = "NativeThread";
	lJavaVMAttachArgs.group	  = NULL;

	// attaching to the java vm
	if ((*lJavaVM).AttachCurrentThread(&lJNIEnv, &lJavaVMAttachArgs) == JNI_ERR) return false;

	// get native activity class and the method that we need
	jobject lNativeActivity	   = app->activity->clazz;
	jclass ClassNativeActivity = (*lJNIEnv).GetObjectClass(lNativeActivity);
	// method name depending on the show parameter
	jmethodID _method = (*lJNIEnv).GetMethodID(ClassNativeActivity, "getKeyboardState", "()Z");

	// call that method
	return (*lJNIEnv).CallBooleanMethod(lNativeActivity, _method);
}

float SCR_HEIGHT = 0;
float SCR_WIDTH	 = 0;
#else
float SCR_WIDTH	 = 450;
float SCR_HEIGHT = 800;
#endif
Color BG_COLOR = (Color){45, 45, 45, 255};
Color FG_COLOR = (Color){175, 175, 175, 255};

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
	DrawLineEx((Vector2){0, 1}, (Vector2){SCR_WIDTH, 1}, THICKNESS + 1, FG_COLOR);
	DrawLineEx((Vector2){1, 1}, (Vector2){1, SCR_WIDTH}, THICKNESS + 1, FG_COLOR);
	DrawLineEx((Vector2){0, SCR_WIDTH}, (Vector2){SCR_WIDTH, SCR_WIDTH}, THICKNESS + 1, FG_COLOR);
	DrawLineEx((Vector2){SCR_WIDTH, 0}, (Vector2){SCR_WIDTH, SCR_WIDTH}, THICKNESS + 1, FG_COLOR);

	// vertical grid
	DrawLineEx((Vector2){BLOCK, 0}, (Vector2){BLOCK, SCR_WIDTH}, THICKNESS + 1, FG_COLOR);
	DrawLineEx((Vector2){BLOCK * 2, 0}, (Vector2){BLOCK * 2, SCR_WIDTH}, THICKNESS + 1, FG_COLOR);

	// horizontal grid
	DrawLineEx((Vector2){0, BLOCK}, (Vector2){SCR_WIDTH, BLOCK}, THICKNESS + 1, FG_COLOR);
	DrawLineEx((Vector2){0, BLOCK * 2}, (Vector2){SCR_WIDTH, BLOCK * 2}, THICKNESS + 1, FG_COLOR);
}

int join_window(char *IP_ADDRESS, int *PORT, struct client_data *data) {
	int selection_step = 0, game_mode = -1;
	bool game_hosting = false;
	// nk_flags nkedit_event = 0;
	const char *title_label[4] = {"Select game mode", "Select Game Hosting", "Join match", "Host match"};
	const char *btn_label[4]   = {"Single Player", "Multi Player", "Host", "Join"};
#ifdef ANDROID
	// struct nk_rect wg_push[2] = {{15, (SCR_HEIGHT / 2) - 180, SCR_WIDTH - 35, (SCR_HEIGHT / 18)},
	// 							 {15, (SCR_HEIGHT / 2), SCR_WIDTH - 35, (SCR_HEIGHT / 18)}};
#else
	// struct nk_rect wg_push[2] = {{15, (SCR_HEIGHT / 2) - 60, SCR_WIDTH - 35, (SCR_HEIGHT / 18)},
	// 							 {15, (SCR_HEIGHT / 2), SCR_WIDTH - 35, (SCR_HEIGHT / 18)}};
#endif
	sprintf(data->username, "Nickname");
	sprintf(IP_ADDRESS, "127.0.0.1:5555");
	*PORT = 5555;
	while (!game_running && !WindowShouldClose()) {
		BeginDrawing();
#ifdef ANDROID
		ImGui_ImplAndroid_NewFrame();
#else
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
#endif
		ImGui::NewFrame();
		ClearBackground(RAYWHITE);

		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::SetNextWindowSize(ImVec2(SCR_WIDTH, SCR_HEIGHT), 1);
		ImGui::Begin("Test", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		// draw title
		// 			nk_layout_space_push(ctx, (struct nk_rect){(SCR_WIDTH - MeasureText(title_label[selection_step + game_hosting], STTT_TEXT_SIZE)) / 2, (SCR_HEIGHT / 2) - (SCR_HEIGHT / 9) - 20, SCR_WIDTH - MeasureText(title_label[selection_step], STTT_TEXT_SIZE), 40});
		ImGui::Text("%s", title_label[selection_step + game_hosting]);
		// widget 1
		// 			nk_layout_space_push(ctx, wg_push[0]);
		if (selection_step == 0) { // single player
			if (ImGui::Button(btn_label[selection_step * 2])) {
				game_running   = 1;
				game_mode	   = 0;
				selection_step = 3;
			}
		} else if (selection_step == 1) { // host a match
			if (ImGui::Button(btn_label[selection_step * 2])) {
				selection_step++;
				game_hosting = true;
			}
		} else if (selection_step == 2) { // username
			ImGui::InputText("Username", data->username, 32);

			// 				nkedit_event = nk_edit_string_zero_terminated(ctx, NK_EDIT_BOX | NK_EDIT_AUTO_SELECT, data->username, sizeof(data->username), nk_filter_ascii);
			// #ifdef ANDROID
			// if (nkedit_event & NK_EDIT_ACTIVATED)
			// 	toggleKeyboard(true);
			// else if (nkedit_event & NK_EDIT_DEACTIVATED)
			// 	toggleKeyboard(false);
			// #endif
		}						   // widget 2
								   // 			nk_layout_space_push(ctx, wg_push[1]);
		if (selection_step == 0) { // multi player
			if (ImGui::Button(btn_label[selection_step * 2 + 1])) {
				selection_step++;
				game_mode = 1;
			}
		} else if (selection_step == 1) { // join match
			if (ImGui::Button(btn_label[selection_step * 2 + 1])) {
				selection_step++;
				game_hosting = false;
			}
		} else if (selection_step == 2) {
			ImGui::InputText("IP Address", IP_ADDRESS, 32);
			if (ImGui::Button("Connect"))
				selection_step = 3;
			// nkedit_event = nk_edit_string_zero_terminated(ctx, NK_EDIT_BOX | NK_EDIT_AUTO_SELECT, IP_ADDRESS, 32, nk_filter_ascii);
			// #ifdef ANDROID
			// 			if (nkedit_event & NK_EDIT_ACTIVATED)
			// 				toggleKeyboard(true);
			// 			else if (nkedit_event & NK_EDIT_DEACTIVATED)
			// 				toggleKeyboard(false);
			// #endif
		}
		// 			nk_layout_space_end(ctx);
		// 			// end selection if multiplayer
		// 			if (GetKeyPressed() == KEY_ENTER)
		// 				selection_step = 3;
		if (selection_step == 3) {
			char *port = strchr(IP_ADDRESS, ':');
			if (port) {
				*PORT	= atoi(port + 1);
				port[0] = '\0';
			}
			game_running = 1;
			game_mode += game_hosting;
		}

		ImGui::End();

		ImGui::Render();
#ifndef ANDROID
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
		EndDrawing();
	}
	return game_mode;
}

void matchInfo(struct client_data *data) { // draw match info
	const char *info_text = TextFormat("It's %s %s turn!", data->turn ? data->users[0] : data->users[1], data->turn ? "(x)" : "(O)");
	DrawText(info_text, (SCR_WIDTH - MeasureText(info_text, STTT_TEXT_SIZE)) / 2, BLOCK * 3 + 10, STTT_TEXT_SIZE, FG_COLOR);
	DrawText(TextFormat("%s: %i\n%s: %i\n", data->users[0], data->winsP[0], data->users[1], data->winsP[1]), 10, BLOCK * 3 + 40, STTT_TEXT_SIZE, FG_COLOR);
	DrawText(TextFormat("%s\n", data->local_ip), 10, SCR_HEIGHT - STTT_TEXT_SIZE * 2, STTT_TEXT_SIZE, FG_COLOR);
}