#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>

const char* APP_ID = "WriterApp";

int main(int argc, char *argv[]) {
  openlog(APP_ID, 0, LOG_USER);

  if (argc < 2) {
    syslog(LOG_ERR, "Two positional arguments required: file to be written, "
                    "string to be written into the file");
    return 1;
  }

  const char* write_file = argv[1];
  const char* write_str = argv[2];
  syslog(LOG_DEBUG, "Writing %s to %s", write_str, write_file);

  FILE *fd = fopen(write_file, "w");
  if (!fd) {
    syslog(LOG_ERR, "Could not open file %s, error: %s", write_file, strerror(errno));
    return 1;
  }

  fprintf(fd, "%s\n", write_str);

  fclose(fd);
  return 0;
}
