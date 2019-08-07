#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static double deg2rad(double degrees) 
{
	return degrees * M_PI / 180;
}

static double convert_dist(double lat1, double lng1, double lat2, double lng2)
{
	double lat = deg2rad(lat2 - lat1);
	double lng = deg2rad(lng2 - lng1);
	double a = sin(lat / 2) * sin(lat / 2) + 
		cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * 
		sin(lng/2) * sin(lng/2);
	double c = 2 * atan2(sqrt(a), sqrt(1-a));
	return c * 3958.75 * 1609.00;
}

static void convert(double lat1, double lng1, double lat2, double lng2, long *dx, long *dy)
{
	long dx0 = (long) convert_dist(lat1, lng1, lat2, lng1);
	long dy0 = (long) convert_dist(lat1, lng1, lat1, lng2);
	*dx = lat1 < lat2 ? dx0 : -dx0;
	*dy = lng1 < lng2 ? dy0 : -dy0;
}

static char *readtok(char *s, char *d)
{
	char *chop = ",-:,\r \n";
	while (*s && strchr(chop, *s) != NULL)
		s++;
	while (*s && !strchr(chop, *s))
		*d++ = *s++;
	*d = '\0';
	return s;
}

#define MAXN		(1 << 20)

static void output(FILE *fp)
{
	char line[1024];
	char tok[1024];
	double lat0 = 116.51172, lng0 = 39.92123;
	long ts0 = (2008 - 1970) * 365 * 24 * 3600;
	int i, n;
	long *xs = malloc(MAXN * sizeof(xs[0]));
	long *ys = malloc(MAXN * sizeof(ys[0]));
	long *tss = malloc(MAXN * sizeof(tss[0]));
	for (i = 0; i < MAXN; i++) {
		char *s = line;
		struct tm tm = {0};
		long x, y, ts;
		double lat, lng;
		if (!fgets(line, sizeof(line), fp))
			break;
		s = readtok(s, tok);		/* taxi ID */
		s = readtok(s, tok);		/* year */
		tm.tm_year = atoi(tok) - 1900;
		s = readtok(s, tok);		/* month */
		tm.tm_mon = atoi(tok);
		s = readtok(s, tok);		/* day */
		tm.tm_mday = atoi(tok);
		s = readtok(s, tok);		/* hour */
		tm.tm_hour = atoi(tok);
		s = readtok(s, tok);		/* minute */
		tm.tm_min = atoi(tok);
		s = readtok(s, tok);		/* second */
		tm.tm_sec = atoi(tok);
		s = readtok(s, tok);		/* latitude */
		lat = atof(tok);
		s = readtok(s, tok);		/* longitude */
		lng = atof(tok);
		ts = mktime(&tm);
		convert(lat0, lng0, lat, lng, &x, &y);
		xs[i] = x;
		ys[i] = y;
		tss[i] = ts - ts0;
	}
	n = i;
	printf("%d\n", i);
	for (i = 0; i < n; i++)
		printf("%ld %ld %ld\n", tss[i], xs[i], ys[i]);
	free(xs);
	free(ys);
	free(tss);
}

int main(int argc, char *argv[])
{
	int i;
	if (argc < 2) {
		output(stdin);
	} else {
		printf("%d\n", argc - 1);
		for (i = 1; i < argc; i++) {
			FILE *fp = fopen(argv[i], "r");
			if (!fp) {
				fprintf(stderr, "tdrive: cannot open <%s>\n", argv[i]);
				return 1;
			}
			output(fp);
			fclose(fp);
		}
	}
	return 0;
}
