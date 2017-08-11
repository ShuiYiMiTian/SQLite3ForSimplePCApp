#include "sqlite3.h"
#include <stdio.h>

typedef sqlite3_uint64 u64;

struct SQLite3Opt
{
  sqlite3 *db;
  char *SQLite3Version;
  char *Filename; //数据库文件名
};

int close_sqlite3(struct SQLite3Opt *SQLOPT);
int update_sqlite3();
int select_sqlite3(struct SQLite3Opt *SQLOPT, char *sql,
                         int num, char ***res, int size);
int delete_sqlite3();
int insert_sqlite3(struct SQLite3Opt *SQLOPT, char *sql);
int sqlite_version(struct SQLite3Opt *SQLOPT, char **version);
int init_sqlite3(struct SQLite3Opt *SQLOPT, const char *fileName);
int open_sqlite3(struct SQLite3Opt *SQLOPT);
int destroy_sqlite3(struct SQLite3Opt *SQLOPT);
int create_table(struct SQLite3Opt *SQLOPt, char *sql);
int Exe_sqlite3(struct SQLite3Opt *SQLOPT, char *zLine);
static int get_sqlite_version(struct SQLite3Opt *SQLOPT);
