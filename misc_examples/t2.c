#include "../common/helper.h"

#define MAXLINE 512

int main()
{
	char s1[MAXLINE];

	getstr(s1, MAXLINE);
	printf("%s\n", s1);

	return 0;
}
