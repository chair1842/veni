__attribute__((noreturn))
void exception_handler(void);
void exception_handler() {
    __asm__ volatile ("cli"); // Completely hangs the computer
    __asm__ volatile ("hlt"); // Completely hangs the computer
}