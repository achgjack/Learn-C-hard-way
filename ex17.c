#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

// 数据库的单条记录
typedef struct Address
{
	int id;
	int set;
	char name[MAX_DATA];
	char email[MAX_ROWS];
} Address;

// 数据库表
typedef struct Database
{
	Address rows[MAX_ROWS];
} Database;

typedef struct Connection
{
	FILE *file;     // in hard-disk
	Database *db;   // in memory
} Connection;

void die(const char *message)
{
	if (errno)
	{
		perror(message);
	}
	else
	{
		printf("ERROR: %s\n", message);
	}

	exit(1);
}

void Address_print(Address *addr)
{
	printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(Connection *conn)
{
	// read database from the disk file to the memory
	int rc = fread(conn->db, sizeof(Database), 1, conn->file);
	if (rc != 1)
		die("Failed to load database.");
}

Connection *Database_open(const char *filename, char mode)
{
	Connection *conn = malloc(sizeof(Connection));

	if (!conn)
		die("Memory error");

	conn->db = malloc(sizeof(Database));
	if (!conn->db)
		die("Memory error");

	if (mode == 'c')
	{
		conn->file = fopen(filename, "w");
	}
	else
	{
		conn->file = fopen(filename, "r+");

		if (conn->file)
		{
			Database_load(conn);
		}
	}

	if (!conn->file)
	{
		die("Failed to open the file");
	}

	return conn;
}

void Database_close(Connection *conn)
{
	if (conn)
	{
		if (conn->file)
			fclose(conn->file);
		if (conn->db)
			free(conn->db);
		free(conn);
	}
}

void Database_write(Connection *conn)
{
	rewind(conn->file);

	int rc = fwrite(conn->db, sizeof(Database), 1, conn->file);
	if (rc != 1)
	{
		die("Failed to write database");
	}

	rc = fflush(conn->file);
	if (rc == -1)
		die("Cannot flush database");
}

void Database_create(Connection *conn)
{
	int i = 0;

	for (i = 0; i < MAX_ROWS; i++)
	{
		// make a prototype to initialize it
		Address addr = {.id = i, .set = 0};
		//then just assign it
		conn->db->rows[i] = addr;
	}
}

void Database_set(Connection *conn, int id, const char* name, const char *email)
{
	Address *addr = &conn->db->rows[id];
	if (addr->set)
		die("already set, delete it first");

	addr->set = 1;

	//demonstrate the strncpy bug
	char *res = strncpy(addr->name, name, MAX_DATA);
	if (!res)
		die("Name copy failed");

	res = strncpy(addr->email, email, MAX_DATA);
	if (!res)
		die("Email copy failed");
}

void Database_get(Connection *conn, int id)
{
	Address *addr = &conn->db->rows[id];

	if (addr->set)
	{
		Address_print(addr);
	}
	else
	{
		die("ID is not set");
	}
}

void Database_delete(Connection *conn, int id)
{
	Address addr = {.id = id, .set = 0};
	conn->db->rows[id] = addr;
}

void Database_list(Connection *conn)
{
	int i = 0;
	Database *db = conn->db;

	for (i = 0; i < MAX_ROWS; i++)
	{
		Address *cur = &db->rows[i];

		if (cur->set)
		{
			Address_print(cur);
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc < 3)
		die("USAGE: ex17 <dbfile> <action> [action params]");

	char *filename = argv[1];
	char action = argv[2][0];
	Connection *conn = Database_open(filename, action);

	int id = 0;

	if (argc > 3)
		id = atoi(argv[3]);
	if (id >= MAX_ROWS)
		die("There's not that many records");

	switch (action)
	{
		case 'c':
		Database_create(conn);
		Database_write(conn);
		break;

		case 'g':
		if (argc != 4)
			die("Need an id to get");
		Database_get(conn, id);

		case 's':
		if (argc != 6)
			die("Need id, name, email to set");
		Database_set(conn, id, argv[4], argv[5]);
		Database_write(conn);
		break;

		case 'd':
		if (argc != 4)
			die("Need id to delete");
		Database_delete(conn, id);
		Database_write(conn);
		break;

		case 'l':
		Database_list(conn);
		break;

		default:
		die("Invalid action: c=create, g=get, s=set, d=delete, l=list");
	}

	Database_close(conn);
		
	return 0;
}