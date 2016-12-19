#include <string.h>

/* Probably not necessary to have a seperate file just for this */
void shift_left(char *s)
{
	int i, n, temp;

	n = strlen(s);
	/* -1 to remove the null terminator, which is included for some reason */
	n--;
	for (i = 0; i != n; i++) {
		temp = s[i];
		s[i]   = s[i + 1];
		s[i+1] = temp;
	}
	s[n] = '\0';
}
