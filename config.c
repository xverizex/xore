#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

extern struct list_xore *object;
extern struct nodes *nodes;
static struct list_xore *o;
static struct nodes *n;

static FILE *fp_config_file;
char *config_file;

#define STATE_OFF               0
#define STATE_GROUP             1

static int state = STATE_OFF;

#define GROUP_STATE_SOURCE      0
#define GROUP_STATE_DATA_ITEM   1
#define GROUP_STATE_ITEM        2
#define GROUP_STATE_NODE        3
static int group_state = -1;

static char group[255];
static int index_group = 0;

struct object {
	int number;
	int data_type;
	void *data;
	struct object *next;
} *obj;

struct object *ob;

static struct nodes *get_nodes ( void ) {
	if ( nodes == NULL ) nodes = calloc ( 1, sizeof ( struct nodes ) );
	return nodes;
}

static void check_access_config_file ( void ) {

	if ( access ( config_file, F_OK ) ) {
		perror ( "access: " );
		exit ( EXIT_FAILURE );
	}
}

static unsigned long get_size_of_config_file ( void ) {
	struct stat st;
	stat ( config_file, &st );
	return st.st_size;
}

static void open_config_file ( void ) {
	fp_config_file = fopen ( config_file, "r" );
}

static char *line;
static char *read_line ( void ) {
	if ( !line ) line = calloc ( 1024, 1 );

	return fgets ( line, 1024, fp_config_file );
}

static struct list_xore *find_id_list_xore ( const int id ) {
	struct list_xore *l = object;
	while ( l->next ) {
		if ( l->id == id ) return l;

		l = l->next;
	}
	return NULL;
}

static void copy_sx_node ( const char *s ) {
	n->sx = atoi ( s );
}

static void copy_sy_node ( const char *s ) {
	n->sy = atoi ( s );
}

static void copy_dx_node ( const char *s ) {
	n->dx = atoi ( s );
}

static void copy_dy_node ( const char *s ) {
	n->dy = atoi ( s );
}

static void copy_in_node ( const char *s ) {
	n->in = find_id_list_xore ( atoi ( s ) );
}

static void copy_out_node ( const char *s ) {
	n->out = find_id_list_xore ( atoi ( s ) );
}

struct {
	char name[255];
	void (*copy) ( const char *s );
} group_node[] = {
	{ "sx:", copy_sx_node },
	{ "sy:", copy_sy_node },
	{ "dx:", copy_dx_node },
	{ "dy:", copy_dy_node },
	{ "in:", copy_in_node },
	{ "out:", copy_out_node }
};
static const int group_node_size = 6;

static void int_node ( void ) {
	if ( line[0] == '}' ) {
		/* здесь node нужен */
		state = STATE_OFF;
		n->next = calloc ( 1, sizeof ( struct nodes ) );
		n = n->next;
		return;
	}

	char *pline = line;
	while ( *pline < 32 ) pline++;

	for ( int i = 0; i < group_node_size; i++ ) {
		int len = strlen ( group_node[i].name );
		if ( !strncmp ( group_node[i].name, pline, len ) ) {
			char *s = &line[len + 1];
			while ( *s == ' ' ) s++;
			char *end = strchr ( s, '\n' );
			if ( end ) *end = 0;
			int length = strlen ( s );
			group_node[i].copy ( s );
		}
	}
}

static void copy_x_main ( const char *s ) {
	o->x = atoi ( s );
}

static void copy_y_main ( const char *s ) {
	o->y = atoi ( s );
}
static void copy_width_main ( const char *s ) {
	o->width = atoi ( s );
}

static void copy_height_main ( const char *s ) {
	o->height = atoi ( s );
}
static void copy_data_type_main ( const char *s ) {
	o->data_type = atoi ( s );
}

static void copy_data_id ( const char *s ) {
	o->data_id = atoi ( s );
}

static void copy_id_main ( const char *s ) {
	o->id = atoi ( s );
}

struct {
	char name[255];
	void (*copy) ( const char *s );
} group_data_item[] = {
	{ "id:", copy_id_main },
	{ "x:", copy_x_main },
	{ "y:", copy_y_main },
	{ "width:", copy_width_main },
	{ "height:", copy_height_main },
	{ "data_type:", copy_data_type_main },
	{ "data_id:", copy_data_id }
};
static const int group_data_item_size = 7;


static void int_data_item ( void ) {
	if ( line[0] == '}' ) {
		o->type = TYPE_IS_DATA_ITEM;
		state = STATE_OFF;
		o->next = calloc ( 1, sizeof ( struct list_xore ) );
		o = o->next;
		return;
	}

	char *pline = line;
	while ( *pline < 32 ) pline++;

	for ( int i = 0; i < group_data_item_size; i++ ) {
		int len = strlen ( group_data_item[i].name );
		if ( !strncmp ( group_data_item[i].name, pline, len ) ) {
			char *s = &line[len + 1];
			while ( *s == ' ' ) s++;
			char *end = strchr ( s, '\n' );
			if ( end ) *end = 0;
			int length = strlen ( s );
			group_data_item[i].copy ( s );
		}
	}
}

struct {
	char name[255];
	void (*copy) ( const char *s );
} group_source[] = {
	{ "id:", copy_id_main },
	{ "x:", copy_x_main },
	{ "y:", copy_y_main },
	{ "width:", copy_width_main },
	{ "height:", copy_height_main },
	{ "data_type:", copy_data_type_main },
	{ "data_id:", copy_data_id }
};
static const int group_source_size = 7;

static void int_source ( void ) {
	if ( line[0] == '}' ) {
		o->type = TYPE_IS_SOURCE;
		state = STATE_OFF;
		o->next = calloc ( 1, sizeof ( struct list_xore ) );
		o = o->next;
		return;
	}

	char *pline = line;
	while ( *pline < 32 ) pline++;

	for ( int i = 0; i < group_source_size; i++ ) {
		int len = strlen ( group_source[i].name );
		if ( !strncmp ( group_source[i].name, pline, len ) ) {
			char *s = &line[len + 1];
			while ( *s == ' ' ) s++;
			char *end = strchr ( s, '\n' );
			if ( end ) *end = 0;
			int length = strlen ( s );
			group_source[i].copy ( s );
		}
	}
}


static struct object *get_obj ( void ) {
	if ( obj == NULL ) obj = calloc ( 1, sizeof ( struct object ) );
	return obj;
}

static void copy_mysql_login ( const char *s ) {
	struct source_mysql *m = ( struct source_mysql * ) ob->data;
	m->login = strdup ( s );
}
static void copy_mysql_password ( const char *s ) {
	struct source_mysql *m = ( struct source_mysql * ) ob->data;
	m->password = strdup ( s );
}
static void copy_mysql_db ( const char *s ) {
	struct source_mysql *m = ( struct source_mysql * ) ob->data;
	m->db = strdup ( s );
}
static void copy_mysql_host ( const char *s ) {
	struct source_mysql *m = ( struct source_mysql * ) ob->data;
	m->host = strdup ( s );
}

static void copy_mysql_port ( const char *s ) {
	struct source_mysql *m = ( struct source_mysql * ) ob->data;
	m->port = strdup ( s );
}

static void copy_id ( const char *s ) {
	int id = atoi ( s );
	switch ( id ) {
		case DATA_MYSQL: {
			struct source_mysql *m = calloc ( 1, sizeof ( struct source_mysql ) );
			m->id = id;
			ob->data = m;
			ob->number = atoi ( group );
			ob->data_type = id;
				 }
			break;
		case DATA_ITEM: {
			struct data_item *m = calloc ( 1, sizeof ( struct data_item ) );
			m->id = id;
			ob->data = m;
			ob->number = atoi ( group );
			ob->data_type = id;
				}
				break;
	}
}

struct {
	char name[255];
	void (*copy) ( const char *s );
} group_mysql[] = {
	{ "login:", copy_mysql_login },
	{ "password:", copy_mysql_password },
	{ "db:", copy_mysql_db },
	{ "host:", copy_mysql_host },
	{ "port:", copy_mysql_port }
};
static const int group_mysql_size = 5;


static void int_mysql ( void ) {
	if ( line[0] == '}' ) {
		state = STATE_OFF;
		return;
	}

	char *pline = line;
	while ( *pline < 32 ) pline++;

	for ( int i = 0; i < group_mysql_size; i++ ) {
		int len = strlen ( group_mysql[i].name );
		if ( !strncmp ( group_mysql[i].name, pline, len ) ) {
			char *s = &line[len + 1];
			while ( *s == ' ' ) s++;
			char *end = strchr ( s, '\n' );
			if ( end ) *end = 0;
			int length = strlen ( s );
			group_mysql[i].copy ( s );
		}
	}
}

static void copy_item_data_item ( const char *s ) {
	struct data_item *m = ( struct data_item * ) ob->data;
	m->item = strdup ( s );
}

struct {
	char name[255];
	void (*copy) ( const char *s );
} group_items[] = {
	{ "item:", copy_item_data_item }
};

static const int group_item_size = 1;

static void int_items ( void ) {
	if ( line[0] == '}' ) {
		state = STATE_OFF;
		return;
	}

	char *pline = line;
	while ( *pline < 32 ) pline++;

	for ( int i = 0; i < group_item_size; i++ ) {
		int len = strlen ( group_items[i].name );
		if ( !strncmp ( group_items[i].name, pline, len ) ) {
			char *s = &line[len + 1];
			while ( *s == ' ' ) s++;
			char *end = strchr ( s, '\n' );
			if ( end ) *end = 0;
			int length = strlen ( s );
			group_items[i].copy ( s );
		}
	}
}

struct {
	char name[255];
	void (*copy) ( const char *s );
} group_item[] = {
	{ "id:", copy_id }
};

static void int_item ( void ) {
	if ( line[0] == '}' ) {
		state = STATE_OFF;
		ob->next = calloc ( 1, sizeof ( struct object ) );
		ob = ob->next;
		return;
	}

	char *pline = line;
	while ( *pline < 32 ) pline++;

	for ( int i = 0; i < 1; i++ ) {
		int len = strlen ( group_item[i].name );
		if ( !strncmp ( group_item[i].name, pline, len ) ) {
			char *s = &line[len + 1];
			while ( *s == ' ' ) s++;
			char *end = strchr ( s, '\n' );
			if ( end ) *end = 0;
			int length = strlen ( s );
			group_item[i].copy ( s );
		}
	}

	switch ( ob->data_type ) {
		case DATA_MYSQL: {
					 int_mysql ( );
				 }
				 break;
		case DATA_ITEM: {
					int_items ( );
				}
				break;
	}
}

struct {
	char name[255];
	void (*interrupt) ( void );
	int state;
} groups[] = {
	{ "source", int_source, GROUP_STATE_SOURCE },
	{ "*", int_item, GROUP_STATE_ITEM },
	{ "data_item", int_data_item, GROUP_STATE_DATA_ITEM },
	{ "node", int_node, GROUP_STATE_NODE }
};

static int groups_size = 4;

static void (*interrupt) ( void );


static int check_is_number ( ) {
	int len = strlen ( group );
	for ( int i = 0; i < len; i++ ) {
		if ( group[i] >= '0' && group[i] <= '9' ) continue;
		return -1;
	}
	return 0;
}

static int check_groups ( void ) {
	for ( int i = 0; i < groups_size; i++ ) {
		if ( !strncmp ( groups[i].name, group, strlen ( group ) + 1 ) ) return 0;
		if ( check_is_number ( group ) == 0 && !strncmp ( groups[i].name, "*", 2 ) ) return 0;
		
	}
	return -1;
}

static void set_group_state ( void ) {
	int group_is_number = check_is_number ( );

	for ( int i = 0; i < groups_size; i++ ) {
		if ( !strncmp ( groups[i].name, "*", 2 ) && !group_is_number ) {
			interrupt = groups[i].interrupt;
			group_state = groups[i].state;
			break;
		}
		if ( !strncmp ( groups[i].name, group, strlen ( group ) + 1 ) ) {
			interrupt = groups[i].interrupt;
			group_state = groups[i].state;
			break;
		}
	}
}

static void read_group ( void ) {
	int len = strlen ( line );

	index_group = 0;
	for ( int i = 0; i < len; i++ ) {
		if ( line[i] == ' ' ) break;
		if ( line[i] == '\n' ) break;
		group[index_group++] = line[i];
	}
	group[index_group] = 0;
	if ( index_group == 0 ) return;
	
	if ( check_groups ( ) ) {
		fprintf ( stderr, "Not found group\n" );
		exit ( EXIT_FAILURE );
	}

	state = STATE_GROUP;
	set_group_state ( );

	index_group = 0;
}

static void read_param ( void ) {
	interrupt ( );
}

static void parse_config ( void ) {

	check_access_config_file ( );

	unsigned long size_of_config_file = get_size_of_config_file ( );

	open_config_file ( );

	while ( read_line ( ) ) {

		switch ( state ) {
			case STATE_OFF:
				read_group ( );
				break;
			case STATE_GROUP:
				read_param ( );
				break;
			default:
				printf ( "alternate: %s\n", group );
				break;
		}
	}
}

static void close_config_file ( void ) {
	fclose ( fp_config_file );
}

static struct object *find_data ( const int num ) {
	struct object *oz = obj;
	while ( oz ) {
		if ( oz->number == num ) return oz;

		oz = oz->next;
	}
	return NULL;
}

void read_config ( char *file ) {
	if ( object == NULL ) object = calloc ( 1, sizeof ( struct list_xore ) );
	o = object;
	ob = get_obj();
	n = get_nodes();

	config_file = file;

	parse_config ( );

	close_config_file ( );

	o = object;
	while ( o ) {
		if ( o->data_type > 0 ) {
			struct object *oz = find_data ( o->data_id );
			if ( oz ) o->data = oz->data;
		}
		switch ( o->type ) {
			case TYPE_IS_SOURCE: o->file = IMAGE_XORE_SOURCE; break;
			case TYPE_IS_DATA_ITEM: o->file = IMAGE_XORE_DATA_ITEM; break;
		}

		o = o->next;
	}

}
