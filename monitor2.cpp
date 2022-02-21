#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <csignal>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>


//TODO monitor1 and monitor2 are not equal in parts they should be

using namespace std;

void cleanString(char *input)
{
    input[0] = '\0';
}

void overwrite(int i)
{
    cout << "unable to kill program from command line" << endl;
}

int main()
{

    signal(SIGINT, overwrite); // ctrl+c
    // signal(SIGTSTP, overwrite); // ctrl+z

    // struct stat st
    // read current directory
    // stat("filenam", est) == -1

    char *input = (char *)mmap(NULL, sizeof(char) * 100, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    for (;;)
        {

    cleanString(input);

    int pid = fork();
    // child process
    if (pid == 0)
    {

        for (;;)
        {

            printf("\x1B[36mstat prog\033[0m$ ");

            cin >> input;

            // user gave a filename

            
            // user wrote list
            if (strcmp(input, "list") == 0)
            {
                cout << "contents of directory..." << endl;
                DIR *dir;
                struct dirent *dp;
                if ((dir = opendir(".")) == NULL)
                {
                    perror("not opening");
                    exit(1);
                }
                while ((dp = readdir(dir)) != NULL)
                {
                    cout << "filename: " << dp->d_name << "\n";
                    //     if(dp->d_type == DT_REG) cout << "type: file" << endl;
                    //     else cout << "type: folder" << endl;
                }
                cout << "...done printing" << endl;
                closedir(dir);
            }

            // entering "q"
            //FIXME how to immediately exit program
                // idea, mmap parent pid ???
            if(input[0] == 'q'){
                
                return 0;
            }

            // if file is there, print out info

            struct stat sb;
            int status = stat(input, &sb);

            if (status != -1)
            {

                printf("File type:                ");

                switch (sb.st_mode & S_IFMT)
                {
                case S_IFBLK:
                    printf("block device\n");
                    break;
                case S_IFCHR:
                    printf("character device\n");
                    break;
                case S_IFDIR:
                    printf("directory\n");
                    break;
                case S_IFIFO:
                    printf("FIFO/pipe\n");
                    break;
                case S_IFLNK:
                    printf("symlink\n");
                    break;
                case S_IFREG:
                    printf("regular file\n");
                    break;
                case S_IFSOCK:
                    printf("socket\n");
                    break;
                default:
                    printf("unknown?\n");
                    break;
                }

                printf("I-node number:            %ju\n", (uintmax_t)sb.st_ino);

                printf("Mode:                     %jo (octal)\n",
                       (uintmax_t)sb.st_mode);

                printf("Link count:               %ju\n", (uintmax_t)sb.st_nlink);
                printf("Ownership:                UID=%ju   GID=%ju\n",
                       (uintmax_t)sb.st_uid, (uintmax_t)sb.st_gid);

                printf("Preferred I/O block size: %jd bytes\n",
                       (intmax_t)sb.st_blksize);
                printf("File size:                %jd bytes\n",
                       (intmax_t)sb.st_size);
                printf("Blocks allocated:         %jd\n",
                       (intmax_t)sb.st_blocks);

                printf("Last status change:       %s", ctime(&sb.st_ctime));
                printf("Last file access:         %s", ctime(&sb.st_atime));
                printf("Last file modification:   %s", ctime(&sb.st_mtime));

                continue;
            }

            else{
                cout << "invalid file name" << endl;
            }


        }
        return 0;
    }
    else
    {
        
            wait(0);
            cout<< "wait is over" << endl;
            sleep(10);
            // cout << "done with sleep" << endl;
            // for (int i = 0; i < 5; i++)
            // {
            //     printf("%d\n", input[i]);
            // }
            
            cout << "done waiting" << endl;
            
            
        }
    }

    munmap(input, sizeof(char) * 100);
}