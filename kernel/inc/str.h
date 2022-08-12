#ifndef __STR_H__
#define __STR_H__

#ifdef __cplusplus
extern "C"
{
#endif

void longtostr(long num, char *buf, int base);
int strlen(const char *str);

#ifdef __cplusplus
}
#endif

#endif