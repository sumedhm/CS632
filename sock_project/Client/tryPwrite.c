#define _X_SOURCE 500
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

int main(int argc, char* argv[]){
	

    
    int fd;
    int res;
    char* buf = (char* )malloc(sizeof(char)*1024);

    FILE* logff;
    logff = fopen("/home/shujaat/Desktop/CODA/Client/readfrom", "r+");
    if(logff==NULL)printf("Couldn't open readfrom file\n");


    size_t size;
	off_t offset = 0;
    size = 16;
    char path[] = "/home/shujaat/Desktop/CODA/Client/tets";
    fd = open(path, O_WRONLY);
    if (fd == -1){
        printf("Couldn't open file");
        return 0;
    }
    printf("fd integral value %d\n",fd);
    fread(buf, size, sizeof(char), logff);
    //printf("Buffer %s %d\n",res);


    res = pwrite(fd, buf, size, offset);
    if (res == -1){
        printf("Couldn't pWrite\n");
        return 0;
    }
    printf("res integral value %d\n",res);
    close(fd);
    fclose(logff);
	/*
	//logfile = fopen("/home/shujaat/Desktop/CODA/loggedfs-0.5/examplelog.txt", "r+");
	//logff = fopen("/home/shujaat/Desktop/CODA/loggedfs-0.5/loglog.txt", "r+");
	//if(logfile==NULL)printf("haya examplelog na khuleyi\n");
	//if(logff==NULL)printf("haya loglog na khuleyi\n");
	//printf("Opening the files\n");
	while(fscanf(logff, "%s %d %d", aPath, &size, &offset)==3){
    //    printf("entry made in file %s of size %d at offset %d\n", aPath, size, offset);
    //    char ident[2];
        //char buf[1024];
        char *buf = malloc(sizeof(char) * size);
        //char *puff = malloc(sizeof(char) * ((int)100));
        
//char buf[] = "Winter is Coming";
        fread(buf, size, 1, logfile);
        //sprintf(buf,"winter is coming");
        printf("Read into buf ::\n%s\n",buf);
        printf("Strlen of buf ::\n%d\n",strlen(buf));
        //sprintf(puff,"%s",buf);
        //fread(ident, 3, 1, logfile);
        //strcpy(buff,buf);
        //char fname[100];

        char ffname[100];
        int len = strlen(aPath);
        int i = 0,count=0;
        for(i = len;i > 0;i--){
        	if(aPath[i]=='/')
        		break;
        	count++;
        }
        count--;
        char fname[count];
        for(i = 0;i < count;i++){
        	fname[i] = aPath[len-count+i];
        }
        fname[count]='\0';
        
        //char* aPath11 = malloc(sizeof(char)*100);
        //char* token;
        //char* tofree;
        //strcpy(aPath11,aPath);
        //tofree = strdup(aPath11);

        //char pp[]= "wget";
        // while ((token = strsep(&aPath11, "/")) != NULL){
                //printf("%s\n", token);
        //        strcpy(fname,token);

        //}
        strcpy(ffname,"/home/shujaat/Desktop/CODA/loggedfs-0.5/");
        strcat(ffname,fname);
        printf("Mirror file name :: %s \n",ffname);
        
        FILE* fd = fopen(ffname, "r+");
        if(fd==NULL)printf("Couldn't open file\n");
        fseek(fd, offset, SEEK_SET);

        //char buf[] = "Winter is Coming";
        //fread(buf, size, 1, logfile);
        //sprintf(buf,"winter is coming");
        //printf("Read into buf ::\n%s\n",buf);
        //printf("Strlen of buf ::\n%d\n",strlen(buf));
        //fread(ident, 3, 1, logfile);
        //strcpy(buff,buf);
        //char fname[100];
        //printf("Opened mirror file name :: %s \n",ffname);
        //char buff[] = "This is to be written";
        //char *buff = malloc(sizeof(char) * size);
        //strcpy(buff,"Some random shit");
        int res = fwrite(buf, 1, size, fd);
        //int res = pwrite(fd, buf, size, offset);
        //sleep(2);
    	if(res == -1){
        	printf("fwrite failed\n");
    	}
    	printf("No of bytes written :: %d\n",res);
        fclose(fd);
        //logfile = fopen("/home/shujaat/Desktop/CODA/loggedfs-0.5/mirror/", "");
        //fwrite(buf, size, 1, fd);
        
        //free(buf);
        //free(puff);
        //*/
        //free(buf);
        //free(aPath11);
        //fwrite(ident, 3, 1, stdout);
        //loop++;
    //}
    //fclose(logfile);
    //fclose(logff);
    return 1;

}