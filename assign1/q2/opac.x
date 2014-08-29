#define VERSION_NUMBER 1

struct book{
	int id;
	char *name;
	char *author;
	char *category;
	int copies;
};


program OPAC_PROG{
	version OPAC_VERSION{
		int search(book) = 1;
		int reserve(book) = 2;
		int renew(book) = 3;
		int add(book) = 4;
	} = 1;
} = 0x3985632;
