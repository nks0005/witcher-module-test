#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h> // read 함수를 사용하기 위해 추가

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include <stdbool.h>

bool pattern_in_bytes(unsigned char *target, int target_len, unsigned char *pattern, int pattern_len);
static ssize_t (*real_recv)(int sockfd, void *buf, size_t len, int flags) = NULL;

ssize_t recv(int sockfd, void *buf, size_t len, int flags)
{

    real_recv = dlsym(RTLD_NEXT, "recv");
    ssize_t result = real_recv(sockfd, buf, len, flags);

    unsigned char *cptr = (unsigned char *)(buf);

    FILE *file = fopen("/tmp/hook", "a");
    if (file)
    {
        unsigned char *mysql_msg = "You have an error i";
        int error_msg_len = strlen(mysql_msg);
        if (pattern_in_bytes(cptr, len, mysql_msg, error_msg_len))
        {
            fwrite(buf, 1, result, file);

            // 시그널 전송
            FILE *pidFile = fopen("/tmp/httpreqr.pid", "r");
            if (pidFile)
            {
                char pidStr[10];
                if (fgets(pidStr, sizeof(pidStr), pidFile))
                {
                    int pid = atoi(pidStr);

                    // PID에 Segment Fault 신호 보내기
                    kill(pid, SIGSEGV);
                    printf("Sent SIGSEGV signal to PID %d\n", pid);
                }
                fclose(pidFile);
            }
            else
            {
                perror("Failed to open /tmp/httpreqr.pid");
            }
        }
    }
    return result;
}

static ssize_t (*real_read)(int fd, void *buf, size_t count) = NULL;
ssize_t read(int fd, void *buf, size_t count)
{
    // 동적 링크를 사용하여 원래 read 함수 포인터를 가져옵니다.
    real_read = dlsym(RTLD_NEXT, "read");
    if (!real_read)
    {
        // 에러 처리
        perror("dlsym failed");
        return -1; // 실패 처리
    }

    // 실제 read 함수를 호출하여 데이터를 읽어옵니다.
    ssize_t result = real_read(fd, buf, count);

    // 특정 파일 디스크립터(예: fd == 19)에서 읽은 데이터를 파일에 기록합니다.

    FILE *file = fopen("/tmp/hook", "a");
    if (file)
    {
        // 읽은 데이터를 파일에 쓰기 전에 문자열 검색 수행

        unsigned char *mysql_msg = "You have an error i";
        int error_msg_len = strlen(mysql_msg);
        unsigned char *cptr = (unsigned char *)(buf);

        if (pattern_in_bytes(cptr, count, mysql_msg, error_msg_len))
        {
            fprintf(file, "detected!!  %d: ", fd);
            fwrite(buf, 1, result, file);

            // 시그널 전송
            FILE *pidFile = fopen("/tmp/httpreqr.pid", "r");
            if (pidFile)
            {
                char pidStr[10];
                if (fgets(pidStr, sizeof(pidStr), pidFile))
                {
                    int pid = atoi(pidStr);

                    // PID에 Segment Fault 신호 보내기
                    kill(pid, SIGSEGV);
                    printf("Sent SIGSEGV signal to PID %d\n", pid);
                }
                fclose(pidFile);
            }
            else
            {
                perror("Failed to open /tmp/httpreqr.pid");
            }
        }
        else
        {
            // 검색된 문자열이 없으면 그냥 읽은 데이터를 씀
            fprintf(file, "Read data from file descriptor %d: ", fd);
            fwrite(buf, 1, result, file);
        }
        fprintf(file, "\n");
        fclose(file);
    }
    else
    {
        perror("Failed to open /tmp/hook for writing");
    }

    return result;
}

bool pattern_in_bytes(unsigned char *target, int target_len, unsigned char *pattern, int pattern_len)
{
    if (target_len <= pattern_len)
    {
        return false;
    }
    for (int i = 0; i < target_len - pattern_len; i++)
    {
        bool found = true;
        for (int j = 0; j < pattern_len; j++)
        {

            if (pattern[j] == '.')
            {
                i++;
                continue;
            }
            else if (pattern[j] == '~')
            {
                if (target[i] >= 0x20 && target[i] < 0x7f)
                {
                    while (target[i] >= 0x20 && target[i] < 0x7f)
                    {
                        i++;
                    }
                    continue;
                    found = false;
                    break;
                }
            }

            if (target[i] != pattern[j])
            {
                found = false;
                break;
            }

            i++;
        }
        if (found)
        {
            return true;
        }
    }

    return false;
}

// gcc -shared -o hook_recv.so testhook.c -ldl -fPIC

// LD_PRELOAD=./hook_recv.so node app.js