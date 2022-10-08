#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <core/main.hpp>
#include <std/decl.h>
const char HEX_VALUES[]  = "0123456789ABCDEF";
static inline void* tmalloc(size_t size, ALLOCTYPE type)
{
    if (size < System::Kernel::heap_large.alignment()) { return System::Kernel::heap_small.allocate(size, type, true); }
    return System::Kernel::heap_large.allocate(size, type, true);
}
static inline void* malloc(size_t size) { return tmalloc(size, ALLOCTYPE_UNSPECIFIED); }
static inline bool free(void* ptr)
{
    if (ptr == NULL) { return false; }
    System::Memory::heapblock_t* blk = System::Kernel::heap_small.fetch(ptr);
    if (blk != NULL) { return System::Kernel::heap_small.free(ptr); }
    blk = System::Kernel::heap_large.fetch(ptr);
    if (blk == NULL) { return false; }
    return System::Kernel::heap_large.free(ptr);
}

static inline bool freearray(void** ptr, uint32_t count)
{
    if (ptr == NULL) { return false; }
    for (size_t i = 0; i < count; i++) { if (ptr[i] != NULL) { if (!free(ptr[i])) { return false; } } }
    return free(ptr);
}


// we need a ASM memset
static inline void memset_asm(void* ptr, uint8_t val, size_t size)
{
    asm volatile("cld; rep stosb" : "+D"(ptr), "+c"(size) : "a"(val) : "memory");
}

static inline uint32_t memalign(uint32_t num, uint32_t align)
{
    uint32_t out = num;
    out &= (0xFFFFFFFF - (align - 1));
    if (out < num) { out += align; }
    return out;
}
static inline void memcpyd(uint32_t *dst, uint32_t *src, size_t sz)
{
    for (size_t i = 0; i < sz; i++) dst[i] = src[i];
}
static inline void memsetd(uint32_t *dst, uint16_t data, size_t sz)
{
    for (size_t i = 0; i < sz; i++) dst[i] = data;
}

static inline void memcpyw(uint16_t *dst, uint16_t *src, size_t sz)
{
    for (size_t i = 0; i < sz; i++) dst[i] = src[i];
}
static inline void memsetw(uint16_t *dst, uint16_t data, size_t sz)
{
    for (size_t i = 0; i < sz; i++) dst[i] = data;
}

static inline void memcpyb(uint8_t *dst, uint8_t *src, size_t sz)
{
    for (size_t i = 0; i < sz; i++) dst[i] = src[i];
}
static inline void memsetb(uint8_t *dst, uint8_t data, size_t sz)
{
    for (size_t i = 0; i < sz; i++) dst[i] = data;
}

static inline void memcpy(void *dst, void *src, size_t sz)
{
    int i;
    int s,k;
    
    s = sz/4;
    sz -= (k  = s*4);
    memcpyd((uint32_t*)dst, (uint32_t*)src, s);
    dst = (void *)((uintptr_t)dst + k);
    src = (void *)((uintptr_t)src + k);

    s = sz/2;
    sz -= (k = s*2);
    memcpyw((uint16_t*)dst, (uint16_t*)src, s);
    dst = (void *)((uintptr_t)dst + k);
    src = (void *)((uintptr_t)src + k);

    memcpyb((uint8_t*)dst, (uint8_t*)src, sz);
}
static inline void memset(void *dst, int data, size_t sz)
{
    if (data < INT8_MAX) memsetb((uint8_t*)dst, (uint8_t)data, sz);
    else if (data < INT16_MAX) memsetw((uint16_t*)dst, (uint16_t)data, sz);
    else memsetd((uint32_t*)dst, (uint32_t)data, sz);
}
static inline void* memcmp(void *dst, void *src, size_t sz)
{
    int i;
    int s,k;
    
    s = sz/4;
    sz -= (k  = s*4);
    for (i = 0; i < s; i++) if (((uint32_t*)dst)[i] != ((uint32_t*)src)[i]) return (void*)(((uint32_t*)dst)[i] - ((uint32_t*)src)[i]);
    dst = (void *)((uintptr_t)dst + k);
    src = (void *)((uintptr_t)src + k);

    s = sz/2;
    sz -= (k = s*2);
    for (i = 0; i < s; i++) if (((uint16_t*)dst)[i] != ((uint16_t*)src)[i]) return (void*)(((uint16_t*)dst)[i] - ((uint16_t*)src)[i]);
    dst = (void *)((uintptr_t)dst + k);
    src = (void *)((uintptr_t)src + k);

    for (i = 0; i < sz; i++) if (((uint8_t*)dst)[i] != ((uint8_t*)src)[i]) return (void*)(((uint8_t*)dst)[i] - ((uint8_t*)src)[i]);
    return 0;
}
static inline size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len])
    {
        len++;
    }
    return len;
}
static inline char* strcpy(char* dest, const char* src)
{
    size_t len = strlen(src);
    memcpy(dest, (void*)src, len + 1);
    return dest;
}
static inline char* strncpy(char* dest, const char* src, size_t num)
{
    size_t len = strlen(src);
    if (len > num)
    {
        len = num;
    }
    memcpy(dest, (void*)src, len);
    dest[len] = '\0';
    return dest;
}
static inline int strcmp(const char* str1, const char* str2)
{
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t len = len1;
    if (len2 < len)
    {
        len = len2;
    }
    int res = (int)memcmp((void*)str1, (void*)str2, len);
    if (res == 0)
    {
        if (len1 < len2)
        {
            return -1;
        }
        else if (len1 > len2)
        {
            return 1;
        }
    }
    return res;
}
static inline int strncmp(const char* str1, const char* str2, size_t num)
{
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t len = len1;
    if (len2 < len)
    {
        len = len2;
    }
    if (len > num)
    {
        len = num;
    }
    int res = (int)memcmp((void*)str1, (void*)str2, len);
    if (res == 0)
    {
        if (len1 < len2)
        {
            return -1;
        }
        else if (len1 > len2)
        {
            return 1;
        }
    }
    return res;
}
static inline char* strcat(char* dest, const char* src)
{
    size_t len = strlen(dest);
    strcpy(dest + len, src);
    return dest;
}
static inline char* strncat(char* dest, const char* src, size_t num)
{
    size_t len = strlen(dest);
    strncpy(dest + len, src, num);
    return dest;
}
static inline char* strchr(char* str, int character)
{
    while (*str != '\0')
    {
        if (*str == character)
        {
            return str;
        }
        str++;
    }
    return NULL;
}
static inline const char* strchr(const char* str, int character)
{
    while (*str != '\0')
    {
        if (*str == character)
        {
            return str;
        }
        str++;
    }
    return NULL;
}
static inline char* strrchr(char* str, int character)
{
    char* last = NULL;
    while (*str != '\0')
    {
        if (*str == character)
        {
            last = str;
        }
        str++;
    }
    return last;
}
static inline void _itoa_swap(char *x, char *y) { char t = *x; *x = *y; *y = t; }

static inline  char*_itoa_rev(char *buffer, int i, int j)
{
    while (i < j) { _itoa_swap(&buffer[i++], &buffer[j--]); }
    return buffer;
}
static inline int abs(int i) { return i < 0 ? -i : i; }
static inline char* itoa(int num, char* str, int base)
{
    if (base < 2 || base > 32) { return str; }

    int i = 0, n = abs(num); 
    while (n)
    {
        int r = n % base;

        if (r >= 10) { str[i++] = 65 + (r - 10); }
        else { str[i++] = 48 + r; }
        n = n / base;
    }

    if (i == 0) { str[i++] = '0'; }
    if (num < 0 && base == 10) { str[i++] = '-'; }
    str[i] = '\0';
    return _itoa_rev(str, 0, i - 1);
}
static inline void _ultoa(unsigned long value, char* result, int base)
{
    unsigned char index;
    char buffer[32];
    index = 32;
    do 
    {
        buffer[--index] = '0' + (value % base);
        if ( buffer[index] > '9') { buffer[index] += 'A' - ':'; }
        value /= base;
    } while (value != 0);

    do { *result++ = buffer[index++]; } while (index < 32);
    *result = 0;
}

static inline char* ltoa(size_t num, char* str, int base)
{
    _ultoa(num, str, base);
    return str;
}
static inline char* strhex(uint32_t value, char* result, bool prefix, uint8_t bytes)
{
    char* orig = result;
    orig[0] = 0;
    if (prefix) { result[0] = '0'; result[1] = 'x'; result += 2; }
    if (bytes == 1)
    {
        *result = HEX_VALUES[(uint8_t)((value & 0xF0) >> 4)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x0F))];
    }
    else if (bytes == 2)
    {
        *result = HEX_VALUES[(uint8_t)((value & 0xF000) >> 12)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x0F00) >> 8)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x00F0) >> 4)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x000F))]; 
    }
    else if (bytes == 4)
    {
        *result = HEX_VALUES[(uint8_t)((value & 0xF0000000) >> 28)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x0F000000) >> 24)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x00F00000) >> 20)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x000F0000) >> 16)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x0000F000) >> 12)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x00000F00) >> 8)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x000000F0) >> 4)]; result++;
        *result = HEX_VALUES[(uint8_t)((value & 0x0000000F))];
    }
    return orig;
}
//Color
static inline int term_vsprintf_color(Color FG,Color BG,const char* fmt, va_list args)
{
    char buffer[1024];
            while (*fmt != 0)
            {
                if (*fmt == '%')
                {
                    fmt++;
                    if (*fmt == '%') { System::Kernel::term.WriteChar(FG, BG,'%'); fmt++; continue; }
                    if (*fmt == 'c') { System::Kernel::term.WriteChar(FG, BG,(char)va_arg(args, int)); }
                    else if (*fmt == 'd')
                    {
                        int dec = va_arg(args, int);
                        char str[16];
                        memset(str, 0, 16);
                        System::Kernel::term.Write(FG, BG,itoa(dec, str, 10));
                    }
                    else if (*fmt == 'u')
                    {
                        uint32_t dec = va_arg(args, uint32_t);
                        char str[16];
                        memset(str, 0, 16);
                        System::Kernel::term.Write(FG, BG,ltoa(dec, str, 10));
                    }
                    else if (*fmt == 'x')
                    {
                        uint32_t dec = va_arg(args, uint32_t);
                        char str[16];
                        memset(str, 0, 16);
                        System::Kernel::term.Write(FG, BG,ltoa(dec, str, 16));
                    }
                    else if (*fmt == '2')
                    {
                        fmt++;
                        if (*fmt != 'x' && *fmt != 'X') { System::Kernel::term.WriteChar(FG, BG,*fmt); }
                        else
                        {
                            uint32_t num = va_arg(args, uint32_t);
                            char str[16];
                            memset(str, 0, 16);
                            System::Kernel::term.Write(FG, BG,strhex(num, str, false, 1));
                        }
                    }
                    else if (*fmt == '4')
                    {
                        fmt++;
                        if (*fmt != 'x' && *fmt != 'X') { System::Kernel::term.WriteChar(FG, BG,*fmt); }
                        else
                        {
                            uint32_t num = va_arg(args, uint32_t);
                            char str[16];
                            memset(str, 0, 16);
                            System::Kernel::term.Write(FG, BG,strhex(num, str, false, 2));
                        }
                    }
                    else if (*fmt == '8')
                    {
                        fmt++;
                        if (*fmt != 'x' && *fmt != 'X') { System::Kernel::term.WriteChar(FG, BG,*fmt); }
                        else
                        {
                            uint32_t num = va_arg(args, uint32_t);
                            char str[16];
                            memset(str, 0, 16);
                            System::Kernel::term.Write(FG, BG,strhex(num, str, false, 4));
                        }
                    }
                    else if (*fmt == 'f') { }
                    else if (*fmt == 's')
                    {
                        char* val = va_arg(args, char*);
                       System::Kernel::term.Write(FG, BG,val);
                    }
                    else { System::Kernel::term.WriteChar(FG, BG,*fmt); }
                }
                else { System::Kernel::term.WriteChar(FG, BG,*fmt); }
                fmt++;
            }
}
//end Color
static inline int term_vsprintf(const char* fmt, va_list args)
{
    char buffer[1024];
            while (*fmt != 0)
            {
                if (*fmt == '%')
                {
                    fmt++;
                    if (*fmt == '%') { System::Kernel::term.WriteChar('%'); fmt++; continue; }
                    if (*fmt == 'c') { System::Kernel::term.WriteChar((char)va_arg(args, int)); }
                    else if (*fmt == 'd')
                    {
                        int dec = va_arg(args, int);
                        char str[16];
                        memset(str, 0, 16);
                        System::Kernel::term.Write(itoa(dec, str, 10));
                    }
                    else if (*fmt == 'u')
                    {
                        uint32_t dec = va_arg(args, uint32_t);
                        char str[16];
                        memset(str, 0, 16);
                        System::Kernel::term.Write(ltoa(dec, str, 10));
                    }
                    else if (*fmt == 'x')
                    {
                        uint32_t dec = va_arg(args, uint32_t);
                        char str[16];
                        memset(str, 0, 16);
                        System::Kernel::term.Write(ltoa(dec, str, 16));
                    }
                    else if (*fmt == '2')
                    {
                        fmt++;
                        if (*fmt != 'x' && *fmt != 'X') { System::Kernel::term.WriteChar(*fmt); }
                        else
                        {
                            uint32_t num = va_arg(args, uint32_t);
                            char str[16];
                            memset(str, 0, 16);
                            System::Kernel::term.Write(strhex(num, str, false, 1));
                        }
                    }
                    else if (*fmt == '4')
                    {
                        fmt++;
                        if (*fmt != 'x' && *fmt != 'X') { System::Kernel::term.WriteChar(*fmt); }
                        else
                        {
                            uint32_t num = va_arg(args, uint32_t);
                            char str[16];
                            memset(str, 0, 16);
                            System::Kernel::term.Write(strhex(num, str, false, 2));
                        }
                    }
                    else if (*fmt == '8')
                    {
                        fmt++;
                        if (*fmt != 'x' && *fmt != 'X') { System::Kernel::term.WriteChar(*fmt); }
                        else
                        {
                            uint32_t num = va_arg(args, uint32_t);
                            char str[16];
                            memset(str, 0, 16);
                            System::Kernel::term.Write(strhex(num, str, false, 4));
                        }
                    }
                    else if (*fmt == 'f') { }
                    else if (*fmt == 's')
                    {
                        char* val = va_arg(args, char*);
                       System::Kernel::term.Write(val);
                    }
                    else { System::Kernel::term.WriteChar(*fmt); }
                }
                else { System::Kernel::term.WriteChar(*fmt); }
                fmt++;
            }
}
static inline int serial_vsprintf(const char* fmt, va_list args)
{
    char buffer[1024];
            while (*fmt != 0)
            {
                if (*fmt == '%')
                {
                    fmt++;
                    if (*fmt == '%') { System::Kernel::serial.WriteChar('%'); fmt++; continue; }
                    if (*fmt == 'c') { System::Kernel::serial.WriteChar((char)va_arg(args, int)); }
                    else if (*fmt == 'd')
                    {
                        int dec = va_arg(args, int);
                        char str[16];
                        memset(str, 0, 16);
                        System::Kernel::serial.Write(itoa(dec, str, 10));
                    }
                    else if (*fmt == 'u')
                    {
                        uint32_t dec = va_arg(args, uint32_t);
                        char str[16];
                        memset(str, 0, 16);
                        System::Kernel::serial.Write(ltoa(dec, str, 10));
                    }
                    else if (*fmt == 'x')
                    {
                        uint32_t dec = va_arg(args, uint32_t);
                        char str[16];
                        memset(str, 0, 16);
                        System::Kernel::serial.Write(ltoa(dec, str, 16));
                    }
                    else if (*fmt == '2')
                    {
                        fmt++;
                        if (*fmt != 'x' && *fmt != 'X') { System::Kernel::serial.WriteChar(*fmt); }
                        else
                        {
                            uint32_t num = va_arg(args, uint32_t);
                            char str[16];
                            memset(str, 0, 16);
                            System::Kernel::serial.Write(strhex(num, str, false, 1));
                        }
                    }
                    else if (*fmt == '4')
                    {
                        fmt++;
                        if (*fmt != 'x' && *fmt != 'X') { System::Kernel::serial.WriteChar(*fmt); }
                        else
                        {
                            uint32_t num = va_arg(args, uint32_t);
                            char str[16];
                            memset(str, 0, 16);
                            System::Kernel::serial.Write(strhex(num, str, false, 2));
                        }
                    }
                    else if (*fmt == '8')
                    {
                        fmt++;
                        if (*fmt != 'x' && *fmt != 'X') { System::Kernel::serial.WriteChar(*fmt); }
                        else
                        {
                            uint32_t num = va_arg(args, uint32_t);
                            char str[16];
                            memset(str, 0, 16);
                            System::Kernel::serial.Write(strhex(num, str, false, 4));
                        }
                    }
                    else if (*fmt == 'f') { }
                    else if (*fmt == 's')
                    {
                        char* val = va_arg(args, char*);
                       System::Kernel::serial.Write(val);
                    }
                    else { System::Kernel::serial.WriteChar(*fmt); }
                }
                else { System::Kernel::serial.WriteChar(*fmt); }
                fmt++;
            }
}
static inline int iscntrl(int c)
{ 
    if (c >= 0 && c <= 31) { return true; }
    if (c == 127) { return true; }
    return false;
}
static inline int isprint(int c)
{ 
    if (!iscntrl(c)) { return true; } 
    return false;
}

static inline int sprintf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int res = term_vsprintf(format, args);
    va_end(args);
    return res;
}