// Modify this file to change what commands output to your statusbar, and
// recompile using the make command.


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
	char level[5];
	fgets(level, 5, f);
	fclose(f);

	int ilvl = atoi(level);

	switch (status) {
		case 'C':
			sprintf(output, " %i%%", ilvl);
			break;
		case 'D':
			if (ilvl < 12)
				sprintf(output, "  %i%%", ilvl);
			else if (ilvl < 37)
				sprintf(output, "  %i%%", ilvl);
			else if (ilvl < 62)
				sprintf(output, "  %i%%", ilvl);
			else if (ilvl < 87)
				sprintf(output, "  %i%%", ilvl);
			else
				sprintf(output, "  %i%%", ilvl);
			break;
		case 'N':
			sprintf(output, "NC %i%%", ilvl);
			break;
	}
}

void temperature(char *output) {
	static char filename[64] = "/sys/class/hwmon/";
	static bool haveFile = false;
	if (!haveFile) {
		DIR *d;
		struct dirent *dir;
		d = opendir("/sys/class/hwmon/");
		if (d) {
			while ((dir = readdir(d)) != NULL) {
				if (dir->d_type != DT_LNK)
					continue;
				strcpy(filename + 17 /* length of '/sys/class/hwmon' */, dir->d_name);
				int i = strlen(filename);
				sprintf(filename + i, "/temp7_input");
				FILE* f = fopen(filename, "rb");
				if(f != NULL){
					fclose(f);
					sprintf(filename, "/sys/class/hwmon/%s/temp1_input", dir->d_name);
					break;
				}

			}
			closedir(d);
		}
		haveFile = true;
	}

	FILE *f = fopen(filename, "rb");
	char temp[7];
	fgets(temp, 7, f);
	sprintf(output, " %.2s°C", temp);
	fclose(f);
}

void date(char *output) {
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	strftime(output, 24, "%a, %d %b %Y", tm);
}

void showtime(char *output) {
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	strftime(output, 24, "%r", tm);
}
