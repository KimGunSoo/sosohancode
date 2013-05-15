#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

/* address to connect */
#define HWCONTROL_ADDR "hwcontrol_UDS"

int connect_server(char* conn_msg)
{
    int server_sock, client_sock;
    int len;
    struct sockaddr_un server_addr;


    /* Create a socket.
     * This socket will be UDS.*/
    if( (server_sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0 ){
        printf("socket() error!\n");
        return -1;
    }
    printf("socket() success!\n");



    /* Set the address. */
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, HWCONTROL_ADDR);
    len = sizeof(server_addr.sun_family) + strlen(server_addr.sun_path);



    /* Connect the socket. */
    printf("connect() ...\n");
    if( connect(server_sock,(const struct sockaddr*)&server_addr, len) < 0){
        printf("connect() error!\n");
        return -1;
    }
    printf("connect() success!\n");



    /* Send client msg to server. */
    int nbyte;
    nbyte = send(server_sock, conn_msg, strlen(conn_msg) + 1, 0);
    printf("client msg[%d] = %s\n",nbyte, conn_msg);

}

int main(void)
{
    printf("Start client main\n");

    connect_server("1-client : Hi");
    connect_server("2-client : Bye");

    printf("End client main\n");
}
