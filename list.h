#ifndef __LIST_XORE__
#define __LIST_XORE__
#define IMAGE_XORE_SOURCE            "xore_t_source.png"
#define IMAGE_XORE_DATA_ITEM         "xore_t_data_item.png"
#define IMAGE_XORE_INPUT             "xore_t_input.png"

#define TYPE_IS_SOURCE          0
#define TYPE_IS_DATA_ITEM       1
#define TYPE_IS_NODE            2
#define TYPE_IS_INPUT           3

struct source_mysql {
	int id;
	char *login;
	char *password;
	char *host;
	char *port;
	char *db;
	char *table;
};

struct data_item {
	int id;
	char *item;
};

struct data_window {
	int id;
	char *tab;
};

struct data_console {
	int id;
};

struct nodes {
	double sx;
	double sy;
	double dx;
	double dy;
	struct list_xore *in;
	struct list_xore *out;
	struct nodes *next;
};

#define DATA_MYSQL            1
#define DATA_ITEM             2
#define DATA_NODE             3
#define DATA_WINDOW           4
#define DATA_CONSOLE          5

struct list_xore {
	int id;
	int type;
	char *file;
	int x;
	int y;
	int sl_x;
	int sl_y;
	int width;
	int height;
	int data_type;
	int data_id;
	void *data;
	void ( *open_menu ) ( struct list_xore *l );
	struct list_xore *next;
};
#endif
