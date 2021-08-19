/* Disabling and enabling input buffering */  
#ifndef INBUFFER_H
#define INBUFFER_H

#ifdef _WIN32

#include <windows.h>
#include <conio.h>

#else 

#include <sys/termios.h>
#include <unistd.h>

struct termios original_tio;

void disable_input_buffering();

void restore_input_buffering(); 

#endif
#endif