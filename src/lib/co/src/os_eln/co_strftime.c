/* co_strftime.c -- strftime for ELN.

   PROVIEW/R
*/

#include ctype
#include stdio
#include string
#include limits
#include time



#define TYPE_BIT(type)	(sizeof (type) * CHAR_BIT)

#define TYPE_SIGNED(type) (((type) -1) < 0)

/*
** 302 / 1000 is log10(2.0) rounded up.
** Subtract one for the sign bit if the type is signed;
** add one for integer division truncation;
** add one more for a minus sign if the type is signed.
*/
#define INT_STRLEN_MAXIMUM(type) \
    ((TYPE_BIT(type) - TYPE_SIGNED(type)) * 302 / 100 + 1 + TYPE_SIGNED(type))

#define SECSPERMIN	60
#define MINSPERHOUR	60
#define HOURSPERDAY	24
#define DAYSPERWEEK	7
#define DAYSPERNYEAR	365
#define DAYSPERLYEAR	366
#define SECSPERHOUR	(SECSPERMIN * MINSPERHOUR)
#define SECSPERDAY	((long) SECSPERHOUR * HOURSPERDAY)
#define MONSPERYEAR	12

#define TM_SUNDAY	0
#define TM_MONDAY	1
#define TM_TUESDAY	2
#define TM_WEDNESDAY	3
#define TM_THURSDAY	4
#define TM_FRIDAY	5
#define TM_SATURDAY	6

#define TM_JANUARY	0
#define TM_FEBRUARY	1
#define TM_MARCH	2
#define TM_APRIL	3
#define TM_MAY		4
#define TM_JUNE		5
#define TM_JULY		6
#define TM_AUGUST	7
#define TM_SEPTEMBER	8
#define TM_OCTOBER	9
#define TM_NOVEMBER	10
#define TM_DECEMBER	11

#define TM_YEAR_BASE	1900

#define EPOCH_YEAR	1970
#define EPOCH_WDAY	TM_THURSDAY

/*
** Accurate only for the past couple of centuries;
** that will probably do.
*/

#define isleap(y) (((y) % 4) == 0 && (((y) % 100) != 0 || ((y) % 400) == 0))

typedef struct {
	const char *	mon[12];
	const char *	month[12];
	const char *	wday[7];
	const char *	weekday[7];
	const char *	X_fmt;
	const char *	x_fmt;
	const char *	c_fmt;
	const char *	am;
	const char *	pm;
	const char *	date_fmt;
} lc_time_T;

static char * add(const char *, char *, const char *);
static char * conv(int, const char *, char *, const char *);
static char * fmt(const char *, const struct tm *, char *, const char *);
static char * secs(const struct tm *, char *, const char *);


size_t
strftime (
	char *const s,
	const size_t maxsize,
	const char *const format,
	const struct tm *const t
)
{
	char *p;

	tzset();
	p = fmt(((format == NULL) ? "%c" : format), t, s, s + maxsize);
	if (p == s + maxsize)
		return 0;
	*p = '\0';
	return p - s;
}

static char *
fmt (
	const char *format,
	const struct tm *const t,
	char *pt,
	const char *const ptlim
)
{
	static lc_time_T C_time_locale = {
	{
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	}, {
		"January", "February", "March", "April", "May", "June",
		"July", "August", "September", "October", "November", "December"
	}, {
		"Sun", "Mon", "Tue", "Wed",
		"Thu", "Fri", "Sat"
	}, {
		"Sunday", "Monday", "Tuesday", "Wednesday",
		"Thursday", "Friday", "Saturday"
	},

	/* X_fmt */
	"%H:%M:%S",

	/*
	** x_fmt
	** Since the C language standard calls for
	** "date, using locale's date format," anything goes.
	** Using just numbers (as here) makes Quakers happier;
	** it's also compatible with SVR4.
	*/
	"%m/%d/%y",

	/*
	** c_fmt (ctime-compatible)
	** Note that
	**	"%a %b %d %H:%M:%S %Y"
	** is used by Solaris 2.3.
	*/
	"%a %b %e %X %Y",

	/* am */
	"AM",

	/* pm */
	"PM",

	/* date_fmt */
	"%a %b %e %X %Z %Y"
	};

	lc_time_T * Locale = &C_time_locale;

	for ( ; *format; ++format) {
		if (*format == '%') {
label:
			switch (*++format) {
			case '\0':
				--format;
				break;
			case 'A':
				pt = add((t->tm_wday < 0 || t->tm_wday > 6) ?
					"?" : Locale->weekday[t->tm_wday],
					pt, ptlim);
				continue;
			case 'a':
				pt = add((t->tm_wday < 0 || t->tm_wday > 6) ?
					"?" : Locale->wday[t->tm_wday],
					pt, ptlim);
				continue;
			case 'B':
				pt = add((t->tm_mon < 0 || t->tm_mon > 11) ?
					"?" : Locale->month[t->tm_mon],
					pt, ptlim);
				continue;
			case 'b':
			case 'h':
				pt = add((t->tm_mon < 0 || t->tm_mon > 11) ?
					"?" : Locale->mon[t->tm_mon],
					pt, ptlim);
				continue;
			case 'C':
				/*
				** %C used to do a...
				**	fmt("%a %b %e %X %Y", t);
				** ...whereas now POSIX 1003.2 calls for
				** something completely different.
				** (ado, 5/24/93)
				*/
				pt = conv((t->tm_year + TM_YEAR_BASE) / 100,
					"%02d", pt, ptlim);
				continue;
			case 'c':
				pt = fmt(Locale->c_fmt, t, pt, ptlim);
				continue;
			case 'D':
				pt = fmt("%m/%d/%y", t, pt, ptlim);
				continue;
			case 'd':
				pt = conv(t->tm_mday, "%02d", pt, ptlim);
				continue;
			case 'E':
			case 'O':
				/*
				** POSIX locale extensions, a la
				** Arnold Robbins' strftime version 3.0.
				** The sequences
				**	%Ec %EC %Ex %Ey %EY
				**	%Od %oe %OH %OI %Om %OM
				**	%OS %Ou %OU %OV %Ow %OW %Oy
				** are supposed to provide alternate
				** representations.
				** (ado, 5/24/93)
				*/
				goto label;

			case 'e':
				pt = conv(t->tm_mday, "%2d", pt, ptlim);
				continue;
			case 'H':
				pt = conv(t->tm_hour, "%02d", pt, ptlim);
				continue;
			case 'I':
				pt = conv((t->tm_hour % 12) ?
					(t->tm_hour % 12) : 12,
					"%02d", pt, ptlim);
				continue;
			case 'j':
				pt = conv(t->tm_yday + 1, "%03d", pt, ptlim);
				continue;
			case 'k':
				/*
				** This used to be...
				**	conv(t->tm_hour % 12 ?
				**		t->tm_hour % 12 : 12, 2, ' ');
				** ...and has been changed to the below to
				** match SunOS 4.1.1 and Arnold Robbins'
				** strftime version 3.0.  That is, "%k" and
				** "%l" have been swapped.
				** (ado, 5/24/93)
				*/
				pt = conv(t->tm_hour, "%2d", pt, ptlim);
				continue;
#ifdef KITCHEN_SINK
			case 'K':
				/*
				** After all this time, still unclaimed!
				*/
				pt = add("kitchen sink", pt, ptlim);
				continue;
#endif /* defined KITCHEN_SINK */
			case 'l':
				/*
				** This used to be...
				**	conv(t->tm_hour, 2, ' ');
				** ...and has been changed to the below to
				** match SunOS 4.1.1 and Arnold Robbin's
				** strftime version 3.0.  That is, "%k" and
				** "%l" have been swapped.
				** (ado, 5/24/93)
				*/
				pt = conv((t->tm_hour % 12) ?
					(t->tm_hour % 12) : 12,
					"%2d", pt, ptlim);
				continue;
			case 'M':
				pt = conv(t->tm_min, "%02d", pt, ptlim);
				continue;
			case 'm':
				pt = conv(t->tm_mon + 1, "%02d", pt, ptlim);
				continue;
			case 'n':
				pt = add("\n", pt, ptlim);
				continue;
			case 'p':
				pt = add((t->tm_hour >= 12) ?
					Locale->pm :
					Locale->am,
					pt, ptlim);
				continue;
			case 'R':
				pt = fmt("%H:%M", t, pt, ptlim);
				continue;
			case 'r':
				pt = fmt("%I:%M:%S %p", t, pt, ptlim);
				continue;
			case 'S':
				pt = conv(t->tm_sec, "%02d", pt, ptlim);
				continue;
			case 's':
				pt = secs(t, pt, ptlim);
				continue;
			case 'T':
				pt = fmt("%H:%M:%S", t, pt, ptlim);
				continue;
			case 't':
				pt = add("\t", pt, ptlim);
				continue;
			case 'U':
				pt = conv((t->tm_yday + 7 - t->tm_wday) / 7,
					"%02d", pt, ptlim);
				continue;
			case 'u':
				/*
				** From Arnold Robbins' strftime version 3.0:
				** "ISO 8601: Weekday as a decimal number
				** [1 (Monday) - 7]"
				** (ado, 5/24/93)
				*/
				pt = conv((t->tm_wday == 0) ? 7 : t->tm_wday,
					"%d", pt, ptlim);
				continue;
			case 'V':	/* ISO 8601 week number */
			case 'G':	/* ISO 8601 year (four digits) */
			case 'g':	/* ISO 8601 year (two digits) */
/*
** From Arnold Robbins' strftime version 3.0:  "the week number of the
** year (the first Monday as the first day of week 1) as a decimal number
** (01-53)."
** (ado, 1993-05-24)
**
** From "http://www.ft.uni-erlangen.de/~mskuhn/iso-time.html" by Markus Kuhn:
** "Week 01 of a year is per definition the first week which has the
** Thursday in this year, which is equivalent to the week which contains
** the fourth day of January. In other words, the first week of a new year
** is the week which has the majority of its days in the new year. Week 01
** might also contain days from the previous year and the week before week
** 01 of a year is the last week (52 or 53) of the previous year even if
** it contains days from the new year. A week starts with Monday (day 1)
** and ends with Sunday (day 7).  For example, the first week of the year
** 1997 lasts from 1996-12-30 to 1997-01-05..."
** (ado, 1996-01-02)
*/
				{
					int	year;
					int	yday;
					int	wday;
					int	w;

					year = t->tm_year + TM_YEAR_BASE;
					yday = t->tm_yday;
					wday = t->tm_wday;
					for ( ; ; ) {
						int	len;
						int	bot;
						int	top;

						len = isleap(year) ?
							DAYSPERLYEAR :
							DAYSPERNYEAR;
						/*
						** What yday (-3 ... 3) does
						** the ISO year begin on?
						*/
						bot = ((yday + 11 - wday) %
							DAYSPERWEEK) - 3;
						/*
						** What yday does the NEXT
						** ISO year begin on?
						*/
						top = bot -
							(len % DAYSPERWEEK);
						if (top < -3)
							top += DAYSPERWEEK;
						top += len;
						if (yday >= top) {
							++year;
							w = 1;
							break;
						}
						if (yday >= bot) {
							w = 1 + ((yday - bot) /
								DAYSPERWEEK);
							break;
						}
						--year;
						yday += isleap(year) ?
							DAYSPERLYEAR :
							DAYSPERNYEAR;
					}
#ifdef XPG4_1994_04_09
					if ((w == 52
					     && t->tm_mon == TM_JANUARY)
					    || (w == 1
						&& t->tm_mon == TM_DECEMBER))
						w = 53;
#endif /* defined XPG4_1994_04_09 */
					if (*format == 'V')
						pt = conv(w, "%02d",
							pt, ptlim);
					else if (*format == 'g') {
						pt = conv(year % 100, "%02d",
							pt, ptlim);
					} else	pt = conv(year, "%04d",
							pt, ptlim);
				}
				continue;
			case 'v':
				/*
				** From Arnold Robbins' strftime version 3.0:
				** "date as dd-bbb-YYYY"
				** (ado, 5/24/93)
				*/
				pt = fmt("%e-%b-%Y", t, pt, ptlim);
				continue;
			case 'W':
				pt = conv((t->tm_yday + 7 -
					(t->tm_wday ?
					(t->tm_wday - 1) : 6)) / 7,
					"%02d", pt, ptlim);
				continue;
			case 'w':
				pt = conv(t->tm_wday, "%d", pt, ptlim);
				continue;
			case 'X':
				pt = fmt(Locale->X_fmt, t, pt, ptlim);
				continue;
			case 'x':
				pt = fmt(Locale->x_fmt, t, pt, ptlim);
				continue;
			case 'y':
				pt = conv((t->tm_year + TM_YEAR_BASE) % 100,
					"%02d", pt, ptlim);
				continue;
			case 'Y':
				pt = conv(t->tm_year + TM_YEAR_BASE, "%04d",
					pt, ptlim);
				continue;
#if 0 /* Time zone is not implemented on ELN, ML */
			case 'Z':
				if (t->tm_zone != NULL)
					pt = add(t->tm_zone, pt, ptlim);
				else
				if (t->tm_isdst == 0 || t->tm_isdst == 1) {
					pt = add(tzname[t->tm_isdst],
						pt, ptlim);
				} else  pt = add("?", pt, ptlim);
				continue;
#endif
			case '+':
				pt = fmt(Locale->date_fmt, t, pt, ptlim);
				continue;
			case '%':
			/*
			 * X311J/88-090 (4.12.3.5): if conversion char is
			 * undefined, behavior is undefined.  Print out the
			 * character itself as printf(3) also does.
			 */
			default:
				break;
			}
		}
		if (pt == ptlim)
			break;
		*pt++ = *format;
	}
	return pt;
}

static char *
conv (
	const int n,
	const char *const format,
	char *const pt,
	const char *const ptlim
)
{
	char	buf[INT_STRLEN_MAXIMUM(int) + 1];

	(void) sprintf(buf, format, n);
	return add(buf, pt, ptlim);
}

static char *
secs (
	const struct tm *t,
	char *pt,
	const char *ptlim
)
{
	char    buf[INT_STRLEN_MAXIMUM(int) + 1];
	register time_t s;
	struct tm tmp;

	/* Make a copy, mktime(3) modifies the tm struct. */
	tmp = *t;
	s = mktime(&tmp);
	(void) sprintf(buf, "%ld", s);
	return add(buf, pt, ptlim);
}

static char *
add (
	const char *str,
	char *pt,
	const char *const ptlim
)
{
	while (pt < ptlim && (*pt = *str++) != '\0')
		++pt;
	return pt;
}
