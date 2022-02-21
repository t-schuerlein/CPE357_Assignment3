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

    //TODO valgrind check


    // cout << "pid " << getpid() << endl;
    signal(SIGINT, overwrite); // ctrl+c
    // signal(SIGTSTP, overwrite); // ctrl+z

    // struct stat st
    // read current directory
    // stat("filenam", est) == -1

    DIR *dir;
                struct dirent *dp;

    char *input = (char *)mmap(NULL, sizeof(char) * 100, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // int* parent_pid = (int*) mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1,0);
    int parent_pid = getpid();
    cleanString(input);

    char path[100];
            getcwd(path, sizeof(path));

    int pid = fork();

    // if(pid > 0){
    //     *parent_pid = getpid();
    // }
    // child process
    if (pid == 0)
    {

        for (;;)
        {
            
            // GetCurrentDir( path, sizeof(path) );
            printf("\x1B[36mstat prog. %s\033[0m$ ", path);

            cin >> input;


            // user gave a filename

            // if file is there, print out info



            if(strcmp(input, "..") == 0){
                cout << "we want to retreat" << endl;
                dir = opendir("..");
                
                int index = strlen(path) - 1;
                while(1){
                    if(path[index] == '/'){
                        path[index] = '\0';
                        break;
                    } 
                    path[index] == '\0';
                    index -= 1;
                }
             
                continue;
            }
            
            // user wrote list
            if (strcmp(input, "list") == 0)
            {


                cout << "contents of directory..." << endl;
                
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
                // closedir(dir);
                continue;
            }

            // entering "q"
            //QUESTION is just sending sigkill good?
            if(input[0] == 'q'){
                cout << "we outta here big boss!" << endl;           
                kill(parent_pid, SIGKILL);
                munmap(input, sizeof(char) * 100);
                return 0;
            }


            struct stat sb;
            int status = stat(input, &sb);
            //TODO check if filename or not
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
        for (;;)
        {
            sleep(10);
            // cout << "done with sleep" << endl;
            // for (int i = 0; i < 5; i++)
            // {
            //     printf("%d\n", input[i]);
            // }
            if (input[0] == '\0')
            {
                cout << "no commands" << endl;
                //QUESTION is this the same as wait??
                kill(pid, SIGKILL);
                
                munmap(input, sizeof(char) * 100);
                return 0;
            }
            cleanString(input);
        }
    }

    munmap(input, sizeof(char) * 100);
}