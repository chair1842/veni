#pragma once

#include <kernel/keyboard.h>
#include <stdint.h>
#include <kernel/io.h>

#define DATA_PORT   0x60
#define STATUS_PORT 0x64
#define BUFFER_SIZE 128

static char buffer[BUFFER_SIZE];
static int buffer_head = 0;
static int buffer_tail = 0;

static char scancode_to_ascii[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,'\\','z','x','c','v','b','n','m',',','.','/',
    0,'*', 0,' ', 0,
};

void buffer_push(char c);

bool buffer_empty();

char buffer_pop();

void keyboard_handler();