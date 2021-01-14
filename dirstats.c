#include <stdio.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <sys/sysmacros.h>

#define _MAX_PATHS 256
#define _EXT_LEN 265
#define _FILENAME_LEN 256
#define _DIRNAME_LEN 256

void dirstats(char dirpath[]);
void getExt(char filename[], char ext[]);
void stat_ex(char *path[]);

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
        }
    }
    return 0;
}

void dirstats(char *dirpath[])
{
    struct dirent *file;
    DIR *ddir;
    ddir = opendir(dirpath);
    if(ddir == NULL)
    {
        printf("Unable to open directory %s", dirpath);
        return;                
    }

    /* read the directory */
    while( (file=readdir(ddir)) != NULL )
    {
        stat_ex(file->d_name);   
        /*
        struct stat fs;                
        char ext[_EXT_LEN];
        stat(file->d_name, &fs);

        switch (fs.st_mode & __S_IFMT)
        {
            case __S_IFBLK: printf("%17s\t", "block device"); break;
            case __S_IFCHR: printf("%17s\t", "character device"); break;
            case __S_IFDIR:
                printf("%17s\t%s\n","directory", file->d_name);
                break;                     
            case __S_IFIFO: printf("%17s\t", "FIFO/pipe"); break;
            case __S_IFLNK: printf("%17s\t","symlink"); break;
            case __S_IFREG:
                getExt(file->d_name, ext);
                printf("%17s\t%15s\t%10lld\t%10ld\t%10ld\t%s\n",
                    "regular file",                     
                    ext,
                    (long long) fs.st_size, 
                    fs.st_blocks, 
                    fs.st_blksize,
                    file->d_name);
                break;
            case __S_IFSOCK:    printf("socket\t");                  break;
            default:            printf("unknown?\t");                break;
        }
        */
    }
    
}

void getExt(char filename[], char ext[])
{
    // find the dot looking from end
    char *ptr =  strrchr( filename, '.');
    // now get chars from ptr
    int i = 0;
    while(*ptr++ |= '\0')
    {
        if(strlen(ext) > _EXT_LEN)
        {
            printf("Exceeds maximum extension length (%d)", _EXT_LEN);
            break;
        }
        ext[i++] = *ptr;  
    }
}

void stat_ex(char *path[])
{
        struct stat sb;

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