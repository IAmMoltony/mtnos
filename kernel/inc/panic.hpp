#ifndef __PANIC_HPP__
#define __PANIC_HPP__

void panic(const char *panic_msg);

// if true, displays panic as a red screen with white text
// otherwise, prints the panic message and hangs
extern bool g_panic_fullscreen;

#endif