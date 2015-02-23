#define VERSION_NUMBER 2

struct arguments{
	int x;
};

program CODA{
	version CODA_VERSION{
		int update_file(string) = 1;
		int check_connection(int) = 2;
	} = 1;
} = 0x3987632;
