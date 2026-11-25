/*
 * app.h
 *
 *  Created on: 25 Nov 2026
 *      Author: nicks
 */

#ifndef INC_APP_H_
#define INC_APP_H_

/* --------------------------------------------- */
/* WHY: the extern "C" guard gives these functions C linkage, so main.c (C)
   can call functions implemented in app.cpp (C++). Without it the linker
   fails with "undefined reference to app_init". */

// https://claude.ai/share/4dc9b8e9-1304-4f1a-9f0f-5956f63ff31f

#ifdef __cplusplus
extern "C" {
#endif

void app_init(void);   /* runs once, from USER CODE 2 in main.c */
void app_loop(void);   /* runs every iteration, from USER CODE 3 in main.c */

#ifdef __cplusplus
}
#endif
/* --------------------------------------------- */

#endif /* INC_APP_H_ */
