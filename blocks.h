// Modify this file to change what commands output to your statusbar, and
// recompile using the make command.

#include <stdio.h>

void batterystatus(char *output);
void temperature(char *output);
void date(char *output);
void showtime(char *output);

static const Block blocks[] = {
	{"/\x02", "volumeicon", NULL, 0, 10, 1},
	{"/\x07", NULL, batterystatus, 30, 11, 0},
	{"/\x02", NULL, temperature, 2, 0, 0},
	{"/\x07", NULL, date, 1, 0, 0},
	{"/\x02", NULL, showtime, 1, 0, 0},
};

// sets delimeter between status commands. NULL character ('\0') means no
// delimeter.
static char delim[] = "";
static unsigned int delimLen = 0;

void batterystatus(char *output) {
	FILE *f = fopen("/sys/class/power_supply/BAT0/status", "rb");
	char status = fgetc(f);
	fclose(f);

	f = fopen("/sys/class/power_supply/BAT0/capacity", "rb");
	char level[4] = {[3] = 0};
	fgets(level, 3, f);
	fclose(f);

	int ilvl = atoi(level);

	printf("%c\n", status);

	switch (status) {
		case 'C':
			sprintf(output, " %.3s%%", level);
			break;
		case 'D':
			if (ilvl < 12)
				sprintf(output, "  %.3s%%", level);
			else if (ilvl < 37)
				sprintf(output, "  %.3s%%", level);
			else if (ilvl < 62)
				sprintf(output, "  %.3s%%", level);
			else if (ilvl < 87)
				sprintf(output, "  %.3s%%", level);
			else
				sprintf(output, "  %.3s%%", level);
			break;
		case 'N':
			sprintf(output, "NC %.3s%%", level);
			break;
	}
}

void temperature(char *output) {
	FILE *f = fopen("/sys/class/hwmon/hwmon6/temp1_input", "rb");
	char temp[7];
	fgets(temp, 7, f);
	sprintf(output, " %.2s°C", temp);
}

void date(char* output){
	time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(output, 24, "%a, %d %b %Y", tm);
}

void showtime(char* output){
	time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(output, 24, "%r", tm);
}
