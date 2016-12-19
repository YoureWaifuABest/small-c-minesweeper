#include <string.h>

void shift_left(char *s)
{
	int i, n, temp;

	n = strlen(s);
	n -= 1;
	for (i = 0; i != n; i++) {
		temp = s[i];
		s[i]   = s[i + 1];
		s[i+1] = temp;
	}
	s[n] = '\0';
}
