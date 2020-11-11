#ifndef __LIST_XORE__
#define __LIST_XORE__
#define IMAGE_XORE_SOURCE            "xore_t_source.png"
#define IMAGE_XORE_DATA_ITEM         "xore_t_data_item.png"

#define TYPE_IS_SOURCE          0
#define TYPE_IS_DATA_ITEM       1

struct source_mysql {
	int id;
	char *login;
	char *password;
	char *host;
	char *port;
	char *db;
};

#define DATA_MYSQL            1

struct list_xore {
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
