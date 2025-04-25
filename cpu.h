#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#include "memory_handler.h"
#include "hachage.h"
#include "parser.h"



typedef struct {
    MemoryHandler* memory_handler;
    HashMap* context;
    HashMap* constant_pool;
} CPU;



CPU* cpu_init(int memory_size);

void cpu_destroy(CPU* cpu);

void* store(MemoryHandler* handler, const char* segment_name, int pos, void* data);

void* load(MemoryHandler* handler, const char* segment_name, int pos);

void allocate_variables(CPU* cpu, Instruction** data_instructions, int data_count);

void print_data_segment(CPU* cpu);

int matches(const char *pattern, const char *string);








#endif