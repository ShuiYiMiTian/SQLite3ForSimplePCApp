#include "sqlite3.h"
#include "sqlite3_io.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "string.h"

int open_sqlite3(struct SQLite3Opt *SQLOPT)
{
    sqlite3_open(SQLOPT->Filename, &SQLOPT->db);
    return 0;
}

int create_table(struct SQLite3Opt *SQLOPt, char *sql)
{
    if (sql == NULL)
    {
        return -1;
    }
    char *temp = sql;
    int res = sqlite3_exec(SQLOPt->db, temp, 0, 0, 0);
    if (res && !strstr(sqlite3_errmsg(SQLOPt->db), "already exists"))
    {
        printf("CREATE TABLE failed: %s\n",
               sqlite3_errmsg(SQLOPt->db));
    }
    return 0;
}

int init_sqlite3(struct SQLite3Opt *SQLOPT, const char *fileName)
{
    int i = 0;
    if (fileName == 0)
    {
        return -1;
    }
    SQLOPT->Filename = (char *)malloc(strlen(fileName) + 1);
    if (SQLOPT->Filename == 0)
    {
        printf("Error: out of memory\n");
        exit(0);
    }
    memset(SQLOPT->Filename, 0, strlen(fileName) + 1);
    for (i = 0; fileName[i]; i++)
    {
        SQLOPT->Filename[i] = fileName[i];
    }
    get_sqlite_version(SQLOPT);
    sqlite3_initialize();
    return 0;
}

int sqlite_version(struct SQLite3Opt *SQLOPT, char **Sversion)
{
    int i = 0;
    if (Sversion == NULL || SQLOPT->SQLite3Version == NULL)
    {
        return -1;
    }
    char *version = (char *)malloc(strlen(SQLOPT->SQLite3Version) + 1);
    if (version == NULL)
    {
        printf("Error: out of memory\n");
        return -2;
    }
    memset(version, 0, strlen(SQLOPT->SQLite3Version) + 1);
    for (i = 0; SQLOPT->SQLite3Version[i]; i++)
    {
        version[i] = SQLOPT->SQLite3Version[i];
    }
    *Sversion = version;
    return 0;
}

static int get_sqlite_version(struct SQLite3Opt *SQLOPT)
{
    int i = 0;
    const char *temp = sqlite3_libversion();
    SQLOPT->SQLite3Version = (char *)malloc(strlen(temp) + 1);
    if (SQLOPT->SQLite3Version == NULL)
    {
        printf("Error: out of memory\n");
        exit(0);
    }
    memset(SQLOPT->SQLite3Version, 0, strlen(temp));
    for (i = 0; temp[i]; i++)
    {
        SQLOPT->SQLite3Version[i] = temp[i];
    }
    return 0;
}

int insert_sqlite3(struct SQLite3Opt *SQLOPT, char *sql)
{
    if (sql == NULL)
    {
        return -1;
    }
    char *temp = sql;
    int res = sqlite3_exec(SQLOPT->db, temp, 0, 0, 0);
    if (res)
    {
        printf("INSERT failed: %s\n",
               sqlite3_errmsg(SQLOPT->db));
    }
    return 0;
}

int delete_sqlite3(struct SQLite3Opt *SQLOPT, char *sql)
{
    if (sql == NULL)
    {
        return -1;
    }
    char *temp = sql;
    int res = sqlite3_exec(SQLOPT->db, temp, 0, 0, 0);
    if (res)
    {
        printf("DELETE failed: %s\n",
               sqlite3_errmsg(SQLOPT->db));
    }
    return 0;
}

int update_sqlite3(struct SQLite3Opt *SQLOPT, char *sql)
{
    if (sql == NULL)
    {
        return -1;
    }
    char *temp = sql;
    int res = sqlite3_exec(SQLOPT->db, temp, 0, 0, 0);
    if (res)
    {
        printf("UPDATE failed: %s\n",
               sqlite3_errmsg(SQLOPT->db));
    }
    return 0;
}

int select_sqlite3(struct SQLite3Opt *SQLOPT, char *sql,
                         int num, char ***res, int size)
{
    if (sql == NULL)
    {
        return -1;
    }
    sqlite3_stmt *pStmt = 0;
    char *temp = sql;
    char **finalres = NULL;
    int k = 0;
    int re = sqlite3_prepare_v2(SQLOPT->db, temp, -1, &pStmt, 0);
    if (re)
    {
        printf("SELECT failed: %s\n",
               sqlite3_errmsg(SQLOPT->db));
    }
    else
    {
        while (sqlite3_step(pStmt) == SQLITE_ROW)
        {
            k++;
            finalres = (char **)realloc(finalres, sizeof(char **) * k + 1);
            char *stringRes;
            if (size <= 0)
            {
                stringRes = (char *)malloc(50);
            }
            else
            {
                stringRes = (char *)malloc(size);
            }
            char *tempres = stringRes;
            int i = 0;
            for (i = 0; i < num; i++)
            {
                char *parameter = (char *)sqlite3_column_text(pStmt, i);
                //printf("%s\n", parameter);
                while (*parameter)
                {
                    *tempres = *parameter;
                    tempres++;
                    parameter++;
                }
                *tempres = '|';
                tempres++;
                *tempres = '\0';
            }
            finalres[k - 1] = stringRes;
            finalres[k] = 0;
        }
    }
    *res = finalres;
    return 0;
}

int close_sqlite3(struct SQLite3Opt *SQLOPT)
{
    sqlite3_close(SQLOPT->db);
    return 0;
}

int destroy_sqlite3(struct SQLite3Opt *SQLOPT)
{
    free(SQLOPT->Filename);
    free(SQLOPT->SQLite3Version);
    return 0;
}


// test
int main()
{
    struct SQLite3Opt s;
    init_sqlite3(&s, "test.db");
    open_sqlite3(&s);
    //char *version;
    //sqlite_version(&s, &version);
    //printf("%s\n", version);
    //char *sql = "CREATE TABLE COMPANY(ID INT PRIMARY KEY NOT NULL,NAME TEXT NOT NULL, AGE INT NOT NULL,ADDRESS CHAR(50),SALARY REAL);";
    //create_table(&s, sql);
    //Exe_sqlite3(&s, version);
    //char *sqli = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) VALUES (3, 'Paul', 32, 'California', 20000.00 );";
    //insert_sqlite3(&s, sqli);
    //sqli = "UPDATE COMPANY SET ADDRESS = 'Texas' WHERE ID = 1;";
    //update_sqlite3(&s, sqli);
    //sql = "DELETE FROM COMPANY WHERE ID = 2;";
    //delete_sqlite3(&s, sql);
    //free(version);
    char *sql = "SELECT NAME FROM COMPANY;";
    char **res = NULL;
    select_sqlite3(&s, sql, 1, &res, 0);
    while (res && *res)
    {
        printf("*** %s\n", *res);
        res++;
    }
    close_sqlite3(&s);
    destroy_sqlite3(&s);
    return 0;
}