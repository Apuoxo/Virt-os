#include <stdint.h>
#include <stddef.h>

#define LIMINE_REQUESTS_START_MARKER \
    __attribute__((used, section(".limine_requests_start"))) = { \
        0xf6b8f4b39de7d1ae, 0xfab91a6940fcb9cf, \
        0x785c6ed015d3e316, 0x181e920a7852b9d9 \
    }
#define LIMINE_REQUESTS_END_MARKER \
    __attribute__((used, section(".limine_requests_end"))) = { \
        0xadc0e0531bb10d03, 0x9572709f31764c62 \
    }
#define LIMINE_BASE_REVISION(N) \
    __attribute__((used, section(".limine_requests"))) static volatile uint64_t limine_base_revision[] = { \
        0xf9562b2d5c95a6c8, 0x6a7b384944536bdc, (N) \
    }

static volatile uint32_t *const COM1 = (volatile uint32_t *)0x3f8;

static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static void serial_init(void) {
    outb(0x3f9, 0x00);
    outb(0x3fb, 0x80);
    outb(0x3f8, 0x01);
    outb(0x3f9, 0x00);
    outb(0x3fb, 0x03);
    outb(0x3fa, 0xc7);
    outb(0x3fc, 0x0b);
}

static void serial_putc(char c) {
    for (uint32_t i = 0; i < 1000000; ++i) {
        if (inb(0x3fd) & 0x20) {
            outb(0x3f8, (uint8_t)c);
            return;
        }
    }
}

static void serial_write(const char *s) {
    while (*s) {
        if (*s == '\n') serial_putc('\r');
        serial_putc(*s++);
    }
}

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start[] = {
    0xf6b8f4b39de7d1ae, 0xfab91a6940fcb9cf,
    0x785c6ed015d3e316, 0x181e920a7852b9d9
};
LIMINE_BASE_REVISION(3);
__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end[] = {
    0xadc0e0531bb10d03, 0x9572709f31764c62
};

__attribute__((noreturn))
void _start(void) {
    serial_init();
    serial_write("\nAETHER C KERNEL: BOOTED\n");
    serial_write("ARCH=x86_64\n");
    serial_write("LANGUAGE=C11-FREESTANDING\n");
    serial_write("BOOT_PROTOCOL=LIMINE\n");
    serial_write("STAGE=EARLY_BOOT_DIAGNOSTICS\n");
    serial_write("NOTE=MEMORY_AND_INTERRUPTS_NOT_YET_IMPLEMENTED\n");
    for (;;) {
        __asm__ volatile ("cli; hlt");
    }
}
