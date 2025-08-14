#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define DEBUG

#define MAX_DIRNAME_LEN 1024
#define MAX_FILENAME_LEN 512
#define CMD_LEN_WITH_SPACE 4

typedef enum CATONE_STATUS {
  CATONE_SUCCESS,
  CATONE_COULD_NOT_OPEN_ERR,
  CATONE_DIRNAME_TOO_LONG_ERR,
  CATONE_FILENAME_TOO_LONG_ERR,
  CATONE_SPRINTF_ERR,
  CATONE_CMD_ERR,
  CATONE_NO_FILES_ERR
} CATONE_STATUS;

int usage(void)
{
  return printf("Usage: catonce <path>\n");
}

int is_file(const char *path) //Hat tip to response on https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file
{
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISREG(path_stat.st_mode);
}

int cat_file(const char *path)
{
  if (strlen(path) > MAX_DIRNAME_LEN)
  {
    fprintf(stderr, "Path to directory too long.\n");
    return CATONE_DIRNAME_TOO_LONG_ERR;
  }

  DIR *dir = opendir(path);
  if (dir == NULL)
  {
    fprintf(stderr, "Could not open directory.\n");
    return CATONE_COULD_NOT_OPEN_ERR;
  }

  struct dirent *entry;
  size_t dir_count = 0;

  while ((entry = readdir(dir)) != NULL)
  {
    #ifdef DEBUG
    printf("%s\n", entry->d_name);
    #endif
    if (strlen(entry->d_name) > MAX_FILENAME_LEN)
    {
      fprintf(stderr, "Filename too long. Skipping.\n");
      continue;
    }
    char full_path[MAX_DIRNAME_LEN + MAX_FILENAME_LEN + 1];
    sprintf(full_path, "%s/%s", path, entry->d_name);
    if (is_file(full_path))
    {
      printf("catone: Found file %s\nContents:\n\n", entry->d_name);
      char cmd[CMD_LEN_WITH_SPACE + MAX_DIRNAME_LEN + MAX_FILENAME_LEN + 1];
      sprintf(cmd, "cat %s", full_path);
      if (system(cmd) != 0)
      {
        return CATONE_CMD_ERR;
      }
      return CATONE_SUCCESS;
    }
  }
  printf("No files found.\n");
  return CATONE_NO_FILES_ERR;
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    return usage();
  }
  return cat_file(argv[1]);
}
