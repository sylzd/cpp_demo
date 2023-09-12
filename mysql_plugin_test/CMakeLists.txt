configure_file(include/mysql_version.h.in include/mysql_version.h)

set(PROJECT_VERSION "1.0.0")
set(DEBUG_ENABLED 1)


MYSQL_ADD_PLUGIN(testpluglib testpluglib.c
  MODULE_ONLY MODULE_OUTPUT_NAME "testpluglib")
