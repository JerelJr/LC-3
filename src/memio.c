#include "../include/memio.h"

int check_key()
{
#ifdef _WIN32
  return kbhit();
#else

  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(STDIN_FILENO, &readfds);

  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;

  return select(1, &readfds, NULL, NULL, &timeout) != 0;

#endif
}

/* reads an LC-3 program into memory */
void read_image_file(FILE *file)
{
  /* origin is the memory location for the image to be placed */
  uint16_t origin;
  fread(&origin, sizeof(origin), 1, file);
  origin = swap16(origin);

  uint16_t max_read = UINT16_MAX - origin;
  uint16_t *p = memory + origin;
  size_t read = fread(p, sizeof(uint16_t), max_read, file);

  /*  switch to little endian */
  while (read-- > 0)
    {
      *p = swap16(*p);
      ++p;
    }
}

/* creates a FILE pointer from a string to pass to read_image_file */
int read_image(const char *image_path)
{
  FILE *file = fopen(image_path, "rb");
  if (!file)
    return 0;
  read_image_file(file);
  fclose(file);

  return 1;
}

void mem_write(uint16_t address, uint16_t val) { memory[address] = val; }

uint16_t mem_read(uint16_t address)
{
  if (address == MR_KBSR)
    {
      if (check_key())
	{
	  memory[MR_KBSR] = (1 << 15);
	  memory[MR_KBDR] = getchar();
	}
      else
	{
	  memory[MR_KBSR] = 0;
	}
    }

  return memory[address];
}