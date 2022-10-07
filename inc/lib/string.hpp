#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
static inline void memset(void* ptr, uint8_t value, size_t num)
{
    uint8_t* p = (uint8_t*)ptr;
    for (size_t i = 0; i < num; i++)
    {
        p[i] = value;
    }
}
static inline void memcpy(void* dest, const void* src, size_t num)
{
    uint8_t* d = (uint8_t*)dest;
    uint8_t* s = (uint8_t*)src;
    for (size_t i = 0; i < num; i++)
    {
        d[i] = s[i];
    }
}
static inline int memcmp(const void* ptr1, const void* ptr2, size_t num)
{
    uint8_t* p1 = (uint8_t*)ptr1;
    uint8_t* p2 = (uint8_t*)ptr2;
    for (size_t i = 0; i < num; i++)
    {
        if (p1[i] != p2[i])
        {
            return p1[i] - p2[i];
        }
    }
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
    memcpy(dest, src, len + 1);
    return dest;
}
static inline char* strncpy(char* dest, const char* src, size_t num)
{
    size_t len = strlen(src);
    if (len > num)
    {
        len = num;
    }
    memcpy(dest, src, len);
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
    int res = memcmp(str1, str2, len);
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
    int res = memcmp(str1, str2, len);
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
static inline int vsprintf(char* buffer, const char* format, va_list args)
{
    int written = 0;
    while (*format != '\0')
    {
        size_t maxrem = INT32_MAX - written;
        if (format[0] != '%' || format[1] == '%')
        {
            if (format[0] == '%')
            {
                format++;
            }
            size_t amount = 1;
            while (format[amount] && format[amount] != '%')
            {
                amount++;
            }
            if (maxrem < amount)
            {
                return -1;
            }
            for (size_t i = 0; i < amount; i++)
            {
                buffer[written] = format[i];
                written++;
            }
            format += amount;
            continue;
        }
        const char* format_begun_at = format++;
        if (*format == 'c')
        {
            format++;
            char c = (char)va_arg(args, int);
            if (!maxrem)
            {
                return -1;
            }
            buffer[written] = c;
            written++;
        }
        else if (*format == 's')
        {
            format++;
            const char* str = va_arg(args, const char*);
            size_t len = strlen(str);
            if (maxrem < len)
            {
                return -1;
            }
            memcpy(buffer + written, str, len);
            written += len;
        }
        else if (*format == 'd')
        {
            format++;
            int i = va_arg(args, int);
            if (i == 0)
            {
                if (maxrem < 1)
                {
                    return -1;
                }
                buffer[written] = '0';
                written++;
            }
            else
            {
                bool negative = false;
                if (i < 0)
                {
                    negative = true;
                    i = -i;
                }
                char a[32];
                int j = 0;
                while (i > 0)
                {
                    int rem = i % 10;
                    a[j] = rem + '0';
                    j++;
                    i /= 10;
                }
                if (negative)
                {
                    if (maxrem < 1)
                    {
                        return -1;
                    }
                    buffer[written] = '-';
                    written++;
                }
                for (int k = j - 1; k >= 0; k--)
                {
                    if (maxrem < 1)
                    {
                        return -1;
                    }
                    buffer[written] = a[k];
                    written++;
                }
            }
        }
    }
}
static inline int sprintf(char* buffer, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int res = vsprintf(buffer, format, args);
    va_end(args);
    return res;
}