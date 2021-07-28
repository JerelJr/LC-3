/* Reading and writing memory */
#ifndef MEMIO_H
#define MEMIO_H

#include <stdint.h>
#include <stdio.h>

#include "binops.h"
#include "registers.h"

/* Memory Storage */
uint16_t memory[UINT16_MAX];

#include <stdint.h>

void read_image_file(FILE *file);

int read_image(const char *image_path);

int check_key();

void mem_write(uint16_t address, uint16_t val);

uint16_t mem_read(uint16_t address);

#ifdef _WIN32
#include <conio.h>

#else
#include <sys/time.h>
#include <unistd.h>

#endif

#endif