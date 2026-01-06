# Veni OS Code Standards

This document outlines the coding standards for the Veni hobby operating system project. These are guidelines to keep the codebase consistent and maintainable. Since this is a learning project, standards are flexible but aim for clarity and simplicity.

## Coding Style

- **Braces**: Use K&R style (opening brace on the same line).
- **Indentation**: Use tabs (not spaces) for indentation. 1 tab per level.
- **Line Length**: No strict limit, but keep lines readable (aim for <100 chars where possible).
- **Spacing**: Spaces around operators (`=`, `+`, `==`, etc.). No spaces in function calls like `func(arg)`. Consistent spacing in conditionals and loops.
- **Blank Lines**: Use sparingly to group logical blocks (e.g., between functions or major sections).
- **Semicolons**: Always on the same line as the statement.

## Naming Conventions

- **Variables and Functions**: `snake_case` (e.g., `vfs_create`, `alloc_fd`).
- **Constants and Macros**: `ALL_CAPS` with underscores (e.g., `VFS_MAX_OPEN`, `ALIGN`).
- **Structs**: `typedef struct name name_t;` (e.g., `vfs_file_t`). Members in `snake_case`, struct name in `PascalCase`.
- **Enums**: 
- **Files**: Match the main struct or module name (e.g., `vfs.h`/`vfs.c`).
- **Globals**: Prefix with module name implicitly (e.g., `vfs_files`). Minimize globals.
- **Headers**: Guards like `#ifndef _KERNEL_VFS_H` (underscore-separated, ALL_CAPS). Do not use `#pragma once` (bad experiences in this project).

## Comments

- **File Headers**: Add a `/* */` comment at the start of every C source file explaining what the file does (e.g., `/* VFS definitions and operations */`).
- **Functions**: Use a `//` comment on the line before a function to document what it does (briefly).
- **Structs/Variables/Macros**: Optionally add comments for complex ones.
- **General**: Comments are encouraged for clarity, but don't overdo them. Use `//` for single-line, `/* */` for multi-line. Avoid redundant comments.

## File Organization

- **Includes**: Order: freestanding/system headers first (`<stdio.h>`), then public headers (`<kernel/vfs.h>`), then local headers (`"ramfs.h"`).
- **Prototypes**: Declare in headers, implement in sources.
- **Function Args**: For no arguments, leave empty (e.g., `void func()` not `void func(void)`).
- **Static**: Use for file-private functions and variables.

## Language and Patterns

- **C Version**: C99+ features allowed (e.g., `//` comments, `bool`, `restrict`).
- **Error Handling**: Return codes (e.g., -1 for error, 0 for success). No exceptions.
- **Types**: Use `size_t` for sizes/offsets. Pointers for buffers. Manual memory management.
- **Loops/Conditionals**: Standard C (e.g., `for (int i = 0; i < max; i++)`, `if (!ptr) return -1;`).
- **Arrays**: Fixed-size where possible for simplicity.
- **Assembly**: GAS syntax. Comments with `#`. Labels with underscores (e.g., `_start`).

## Build and Tools

- **Compiler Flags**: Use `-Wall -Wextra` for warnings. `-ffreestanding` for kernel code.
- **Makefiles**: One per major directory. Keep simple.
- **Testing**: Use `printf` and serial output for debugging. Run in QEMU.
- **Dependencies**: Cross-compiler toolchain only. No external libs.

## Miscellaneous

- **Consistency**: Follow existing code where standards aren't clear.
- **Commits**: Keep changes small and tested.
- **Expansion**: This is a living document—update as needed.

> Expanded from initial draft. :p
> thank you github copilot c: