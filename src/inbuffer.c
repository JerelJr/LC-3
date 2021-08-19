#include "../include/inbuffer.h"

#ifdef _WIN32

HANDLE hStdin;
DWORD fdwSaveOldMode;

void disable_input_buffering()
{
  // Handle for standard input
  hStdin = GetStdHandle( STD_INPUT_HANDLE );
  // Save current input mode to restore it when program exits
  GetConsoleMode( hStdin, &fdwSaveOldMode );

  SetConsoleMode( hStdin, fdwSaveOldMode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT) );
}

void restore_input_buffering()
{
  SetConsoleMode( hStdin, fdwSaveOldMode );
}
#else

#include <sys/termios.h>

struct termios original_tio;

void disable_input_buffering()
{
  tcgetattr(STDIN_FILENO, &original_tio);
  struct termios new_tio = original_tio;
  new_tio.c_lflag &= ~ICANON & ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

void restore_input_buffering() { tcsetattr(STDIN_FILENO, TCSANOW, &original_tio); }

#endif