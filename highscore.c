#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "highscore.h"
#include "error.h"
#define HIGHSCORE_DIRECTORY (getPath())
#define HIGHSCORE_FILE  (getFilename())

static char* getPath()
{
	static char pathname[256] = "\0";
	if(pathname[0] != '\0') return pathname;

	const char*const PATH_TEMPLATE = "%s/.local/share/terminal-snake-game";

	char* home = getenv("HOME");
	errorIf(home == NULL, "Could not locate home folder");
	snprintf(pathname, sizeof(pathname), PATH_TEMPLATE, home);
	
	return pathname;
}
static char* getFilename()
{
	static char filename[256] = "\0";
	if(filename[0] != '\0') return filename;
	const char*const FILENAME_TEMPLATE = "%s/highscore";
	snprintf(filename, sizeof(filename), FILENAME_TEMPLATE, getPath());
	return filename;
}


static FILE* hs_open(const char* mode, const char* error_msg)
{
	FILE* rv = fopen(HIGHSCORE_FILE, mode);
	errorIf(rv == NULL,
			"%s\n\tfopen(\"%s\",\"%s\")\n\terrno: %d\n", error_msg,
			HIGHSCORE_FILE, mode, errno);
	return rv;
}

static void hs_close(FILE* fp, const char* error_msg)
{
	errorIf(fclose(fp),
			"%s\n\tfclose(%p)}\n\terrno: %d\n", error_msg, fp, errno);
}

// Set high score to so and so
static void setHighScore(int score)
{
	const char*const FAIL_MESSAGE = "Failed to set high score";

	FILE* fp = hs_open("w", FAIL_MESSAGE);
	errorIf(EOF == fprintf(fp, "%d", score), FAIL_MESSAGE);
	hs_close(fp, FAIL_MESSAGE);
}

// Create the High Score file if it doesn't exist
static void createIfNotExists(void)
{
	struct stat st = {0};

	if(stat(HIGHSCORE_FILE, &st) == -1)
	{
		errorIf(mkdir(HIGHSCORE_DIRECTORY, 0700),
				"mkdir() failed in createIfNotExists() with errno %d", errno);

		setHighScore(0);
	}
}


int getHighScore(void)
{
	const char*const FAIL_MESSAGE = "Failed to extract high score";
	int hs;
	FILE* fp;

	createIfNotExists();

	fp = hs_open("r", FAIL_MESSAGE);
	errorIf(0>fscanf(fp, "%d", &hs), FAIL_MESSAGE);
	hs_close(fp, FAIL_MESSAGE);

	return hs;
}


bool isHighScore(int score)
{
	if(score>getHighScore())
	{
		setHighScore(score);
		return true;
	}
	return false;
}
