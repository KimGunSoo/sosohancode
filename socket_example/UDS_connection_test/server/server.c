#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

/* address to connect */
#define HWCONTROL_ADDR "hwcontrol_UDS"

int hw_th_ret;

static void* hwcontrol_server_thread_fn(void* arg)
{
    int server_sock, client_sock;
    int len;
    struct sockaddr_un server_addr, client_addr;

    printf("Start hwcontrol server thread\n");


    /* Create a socket.
     * This socket will be UDS.*/
    if( (server_sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0 ){
        printf("socket() error!\n");
        return (void*) &hw_th_ret;
    }
    printf("socket() success!\n");



    /* Set the address. */
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, HWCONTROL_ADDR);
    len = sizeof(server_addr.sun_family) + strlen(server_addr.sun_path);



    /* Bind the socket. */
    if( access(HWCONTROL_ADDR, F_OK) == 0){
        unlink(HWCONTROL_ADDR);
    }
    if( bind(server_sock,(const struct sockaddr*)&server_addr, len) < 0){
        printf("bind() error!\n");
        perror("bind() error!\n");
        return (void*) &hw_th_ret;
    }
    printf("bind() success!\n");



    /* Listen on the socket. */
    if( listen(server_sock, 10) < 0 ){
        printf("listen() error!\n");
        return (void*) &hw_th_ret;
    }
    printf("listen() success!\n");



    /* Connection Loop */
    while(1){

        /* Accept connections. */
        printf("accept() ...\n");
        if( (client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &len)) < 0 ){
            printf("accept() error!\n");
            return (void*) &hw_th_ret;
        }
        printf("accept() success!\n");

        /* Receive connnection msg from client. */
        char buff[128];
        int nbyte;
        nbyte = recv(client_sock, buff, sizeof(buff), 0);
        printf("client msg[%d] = %s\n",nbyte, buff);

    }

    printf("End hwcontrol server thread\n");
}

int main(void)
{
    sigset_t set, oldset;
    pthread_t thread;
    int ret;

    printf("Start server main\n");


    sigfillset(&set);
    pthread_sigmask(SIG_SETMASK, &set, &oldset);
    pthread_create(&thread, NULL, hwcontrol_server_thread_fn, NULL);
    pthread_sigmask(SIG_SETMASK, &oldset, NULL);


    /* wait until hwcontrol thread is terminated. */
    pthread_join(thread, (void **)&ret);


    printf("End server main\n");
}
