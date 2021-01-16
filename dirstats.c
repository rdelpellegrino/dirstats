#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <sys/sysmacros.h>
#include <time.h>

//#include <limits.h>
//#include <sys/types.h>

void dirstats(char dirpath[]);
void stat_ex(char path[]);

int main(int argc, char *argv[])
{
    if(argc > 1) // check for args
    {        
        for(int x=1; x<argc; x++)
        {
            //struct dirent *file;
            //DIR *dir;
            //char dirpath[_DIRNAME_LEN];
            //strcpy(dirpath, argv[x]);
            printf("Argument %d is %s\n",x,argv[x] );
            dirstats(argv[x]);
            puts("===");
        }
    }
    return 0;
}

void dirstats(char dirpath[])
{    
    struct dirent *file;
    DIR *ddir;
    ddir = opendir(dirpath);
    if(ddir == NULL)
    {
        struct stat ds;
        if (lstat(dirpath, &ds) == -1) 
            printf("lstat - invalid path: %s", dirpath);
        else
            printf("lstat - not a directory?: %s", dirpath);        
        exit(EXIT_FAILURE); // return? if we want to continue?                      
    }

    /* read the directory */
    while( (file=readdir(ddir)) != NULL )
    {
        char fullpath[256];
        strcpy(fullpath, dirpath);
        strcat(fullpath, "/");
        strcat(fullpath, file->d_name);
        stat_ex(fullpath);
        puts("\n");          
    }    
}

void stat_ex(char path[])
{
        struct stat sb;
        /*
        lstat()  is  identical to stat(), except that if pathname is a symbolic link,
        then it returns information about the link  itself,  not  the  file  that  it
        refers to.
        */
        if (lstat(path, &sb) == -1) 
        {
            perror("lstat");
            exit(EXIT_FAILURE);
        }       

        printf("Filename: %s\n", path);
        printf("ID of containing device:  [%lx,%lx]\n",
                (long) major(sb.st_dev), (long) minor(sb.st_dev));
        printf("File type:                ");

        switch (sb.st_mode & S_IFMT)
         {
            case S_IFBLK:  printf("block device\n");            break;
            case S_IFCHR:  printf("character device\n");        break;
            case S_IFDIR:  printf("directory\n");               break;
            case S_IFIFO:  printf("FIFO/pipe\n");               break;
            case S_IFLNK:  printf("symlink\n");                 break;
            case S_IFREG:  printf("regular file\n");            break;
            case S_IFSOCK: printf("socket\n");                  break;
            default:       printf("unknown?\n");                break;
        }
        printf("I-node number:            %ld\n", (long) sb.st_ino);
        printf("Mode:                     %lo (octal)\n",
                (unsigned long) sb.st_mode);
        printf("Link count:               %ld\n", (long) sb.st_nlink);
        printf("Ownership:                UID=%ld   GID=%ld\n",
                (long) sb.st_uid, (long) sb.st_gid);
        printf("Preferred I/O block size: %ld bytes\n",
                (long) sb.st_blksize);
        printf("File size:                %lld bytes\n",
                (long long) sb.st_size);
        printf("Blocks allocated:         %lld\n",
                (long long) sb.st_blocks);
        printf("Last status change:       %s", ctime(&sb.st_ctime));
        printf("Last file access:         %s", ctime(&sb.st_atime));
        printf("Last file modification:   %s", ctime(&sb.st_mtime));             
}