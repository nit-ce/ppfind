#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(void)
{
	char line[1024];
	char tok[1024];
	double lat0 = 0, lng0 = 0;
	long ts0;
	int i, n;
	long *xs = malloc(MAXN * sizeof(xs[0]));
	long *ys = malloc(MAXN * sizeof(ys[0]));
	long *tss = malloc(MAXN * sizeof(tss[0]));
	for (i = 0; i < MAXN; i++) {
		char *s = line;
		int year, mon, day, hour, min, sec;
		long x, y, ts;
		double lat, lng;
		if (!fgets(line, sizeof(line), stdin))
			break;
		s = readtok(s, tok);		/* taxi ID */
		s = readtok(s, tok);		/* year */
		year = atoi(tok);
		s = readtok(s, tok);		/* month */
		mon = atoi(tok);
		s = readtok(s, tok);		/* day */
		day = atoi(tok);
		s = readtok(s, tok);		/* hour */
		hour = atoi(tok);
		s = readtok(s, tok);		/* minute */
		min = atoi(tok);
		s = readtok(s, tok);		/* second */
		sec = atoi(tok);
		s = readtok(s, tok);		/* latitude */
		lat = atof(tok);
		s = readtok(s, tok);		/* longitude */
		lng = atof(tok);
		ts = ((year - 1970) * 400 + mon * 31 + day) * 24 * 3600 +
			hour * 3600 + min * 60 + sec;
		if (i == 0) {
			lat0 = lat;
			lng0 = lng;
			ts0 = ts;
		}
		convert(lat0, lng0, lat, lng, &x, &y);
		xs[i] = x;
		ys[i] = y;
		tss[i] = ts - ts0;
	}
	n = i;
	printf("%d\n", i);
	for (i = 0; i < n; i++)
		printf("%ld %ld %ld\n", tss[i], xs[i], ys[i]);
	return 0;
}
