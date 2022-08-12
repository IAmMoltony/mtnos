#include <str.h>
#include <stdbool.h>

void longtostr(long num, char *buf, int base)
{
    int i = 0;
    bool neg = false;

    if (num == 0)
    {
        buf[i] = '0';
        buf[i + 1] = 0;
        return;
    }

    if (num < 0)
    {
        neg = true;
        num *= -1;
    }

    while (num != 0)
    {
        int rem = num % base;
        buf[i++] = (rem > 9) ? (rem - 10) + 'A' : rem + '0';
        num /= base;
    }

    if (neg)
        buf[i++] = '-';
    buf[i] = 0;
    
    for (int start = 0, end = i - 1; start < end; ++start, --end)
    {
        char tmp = *(buf + start);
        *(buf + start) = *(buf + end);
        *(buf + end) = tmp;
    }
}

int strlen(const char *str)
{
    int i;
    for (i = 0; str[i] != 0; ++i)
        ;
    return i;
}