#pragma once


char *w2c(char *pcstr, const WCHAR *pwstr, size_t len);
void c2w(wchar_t *pwstr, size_t len, const char *str);