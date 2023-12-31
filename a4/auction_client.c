#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


#define BUF_SIZE 128

#define MAX_AUCTIONS 5
#ifndef VERBOSE
#define VERBOSE 0
#endif

#define ADD 0
#define SHOW 1
#define BID 2
#define QUIT 3

/* Auction struct - this is different than the struct in the server program
 */
struct auction_data {
    int sock_fd;
    char item[BUF_SIZE];
    int current_bid;
};

/* Displays the command options available for the user.
 * The user will type these commands on stdin.
 */

void print_menu() {
    printf("The following operations are available:\n");
    printf("    show\n");
    printf("    add <server address> <port number>\n");
    printf("    bid <item index> <bid value>\n");
    printf("    quit\n");
}

/* Prompt the user for the next command 
 */
void print_prompt() {
    printf("Enter new command: ");
    fflush(stdout);
}


/* Unpack buf which contains the input entered by the user.
 * Return the command that is found as the first word in the line, or -1
 * for an invalid command.
 * If the command has arguments (add and bid), then copy these values to
 * arg1 and arg2.
 */
int parse_command(char *buf, int size, char *arg1, char *arg2) {
    int result = -1;
    char *ptr = NULL;
    if(strncmp(buf, "show", strlen("show")) == 0) {
        return SHOW;
    } else if(strncmp(buf, "quit", strlen("quit")) == 0) {
        return QUIT;
    } else if(strncmp(buf, "add", strlen("add")) == 0) {
        result = ADD;
    } else if(strncmp(buf, "bid", strlen("bid")) == 0) {
        result = BID;
    } 
    ptr = strtok(buf, " "); // first word in buf

    ptr = strtok(NULL, " "); // second word in buf
    if(ptr != NULL) {
        strncpy(arg1, ptr, BUF_SIZE);
    } else {
        return -1;
    }
    ptr = strtok(NULL, " "); // third word in buf
    if(ptr != NULL) {
        strncpy(arg2, ptr, BUF_SIZE);
        return result;
    } else {
        return -1;
    }
    return -1;
}

/* Connect to a server given a hostname and port number.
 * Return the socket for this server
 */
int add_server(char *hostname, int port) {
        // Create the socket FD.
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("client: socket");
        exit(1);
    }
    
    // Set the IP and port of the server to connect to.
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    struct addrinfo *ai;
    
    /* this call declares memory and populates ailist */
    if(getaddrinfo(hostname, NULL, NULL, &ai) != 0) {
        close(sock_fd);
        return -1;
    }
    /* we only make use of the first element in the list */
    server.sin_addr = ((struct sockaddr_in *) ai->ai_addr)->sin_addr;

    // free the memory that was allocated by getaddrinfo for this list
    freeaddrinfo(ai);

    // Connect to the server.
    if (connect(sock_fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("client: connect");
        close(sock_fd);
        return -1;
    }
    if(VERBOSE){
        fprintf(stderr, "\nDebug: New server connected on socket %d.  Awaiting item\n", sock_fd);
    }
    return sock_fd;
}
/* ========================= Add helper functions below ========================
 * Please add helper functions below to make it easier for the TAs to find the 
 * work that you have done.  Helper functions that you need to complete are also
 * given below.
 */

/* Print to standard output information about the auction
 */
void print_auctions(struct auction_data *a, int size) {
    printf("Current Auctions:\n");

    /* TODO Print the auction data for each currently connected 
     * server.  Use the follosing format string:
     *     "(%d) %s bid = %d\n", index, item, current bid
     * The array may have some elements where the auction has closed and
     * should not be printed.
     */
    int index;
    for (index = 0; index < size; index++) {
        a[index].item[strlen(a[index].item) + 1] = '\0';
        printf("(%d) %s bid = %d\n", index, a[index].item, a[index].current_bid);
    }
}

/* Process the input that was sent from the auction server at a[index].
 * If it is the first message from the server, then copy the item name
 * to the item field.  (Note that an item cannot have a space character in it.)
 */
void update_auction(char *buf, int size, struct auction_data *a, int index) {
    
    // TODO: Complete this function
    char *item = NULL;
    char *bid = NULL;
    char *time = NULL;
    item = strtok(buf, " ");
    if (item == NULL) {
        fprintf(stderr, "ERROR malformed bid: %s", buf);
        exit(1);
    }
    bid = strtok(NULL, " ");
    if (bid == NULL) {
        fprintf(stderr, "ERROR malformed bid: %s", buf);
        exit(1);
    }
    time = strtok(NULL, " ");
    if (time == NULL) {
        fprintf(stderr, "ERROR malformed bid: %s", buf);
        exit(1);
    }
    if (a[index].item[0] == '\0') {
        strncpy(a[index].item, item, strlen(item));
        a[index].item[strlen(item) + 1] = '\0';    
    }
    a[index].current_bid = atoi(bid);
    printf("New bid for %s [%d] is %d (%d seconds left)\n", item, index, atoi(bid), atoi(time));
}

int main(void) {

    char name[BUF_SIZE];

    // Declare and initialize necessary variables
    // TODO
    int auction_num = 0;
    struct auction_data auctions[MAX_AUCTIONS];
    fd_set all_fds;
    fd_set read_fds;
    FD_ZERO(&all_fds); 
    FD_SET(STDIN_FILENO, &all_fds);
    int max_fd = STDIN_FILENO;

    // Get the user to provide a name.
    printf("Please enter a username: ");
    fflush(stdout);
    int num_read = read(STDIN_FILENO, name, BUF_SIZE);
    if(num_read <= 0) {
        fprintf(stderr, "ERROR: read from stdin failed\n");
        exit(1);
    }
    print_menu();

    // TODO
    name[num_read - 1] = '\0';
    while(1) {
        print_prompt();
        // TODO
        read_fds = all_fds;
        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("client: select");
            exit(1);
        }
        for (int i = 0; i < auction_num; i++) {
            if (auctions[i].sock_fd != -1) {
                int curr_fd = auctions[i].sock_fd;
                char buf[BUF_SIZE];
                if (FD_ISSET(curr_fd, &read_fds)) {
                    int num_read = read(curr_fd, buf, BUF_SIZE);
                    if(num_read <= 0) {
                        FD_CLR(curr_fd, &all_fds);
                        close(curr_fd);
                        fprintf(stderr, "ERROR: read from server failed\n");
                        exit(1);
                    }
                    if (strncmp("Auction closed:", buf, 15) == 0) {   
                        printf("%s", buf);
                        fflush(stdout);                
                        FD_CLR(curr_fd, &all_fds);
                        close(curr_fd);
                        auctions[i].sock_fd = -1;
                    } 
                    else {
                        update_auction(buf, BUF_SIZE, auctions, i);
                    }
                }
            }
        }
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            char typed_command[BUF_SIZE];
            int num_read = read(STDIN_FILENO, typed_command, BUF_SIZE);
            if (num_read <= 0) {
                FD_CLR(STDIN_FILENO, &all_fds);
                close(STDIN_FILENO);
                fprintf(stderr, "ERROR: read from stdin failed\n");
                exit(1);
            }
            char arg1[BUF_SIZE];
            char arg2[BUF_SIZE];
            int command;
            command = parse_command(typed_command, BUF_SIZE, arg1, arg2);
            if (command == ADD) {
                int port = atoi(arg2);
                int sock_fd = add_server(arg1, port);
                struct auction_data a;
                a.sock_fd = sock_fd;
                a.current_bid = -1;
                int closed = 0;
                for (int i = 0; i < auction_num; i++) {
                    if(auctions[i].sock_fd == -1){
                        closed = 1;
                        auctions[i] = a;
                        int num_write = write(sock_fd, name, strlen(name));
                        if(num_write <= 0){
                            fprintf(stderr, "ERROR: write to server failed\n");
                            close(sock_fd);
                            exit(1);
                        }
                    }
                }
                if (closed == 0) {
                    if (auction_num > MAX_AUCTIONS) {
                        fprintf(stderr, "ERROR: auction capacity overloaded\n");
                        close(sock_fd);
                        exit(1);
                    }
                    auctions[auction_num] = a;
                    int num_write = write(sock_fd, name, strlen(name));
                    if (num_write <= 0) {
                        fprintf(stderr, "ERROR: write to server failed\n");
                        close(sock_fd);
                        exit(1);
                    }
                    auction_num++;   
                }
                max_fd = sock_fd > max_fd ? sock_fd : max_fd;
                FD_SET(sock_fd, &all_fds);
                FD_SET(sock_fd, &read_fds);    
            }
            
            else if (command == SHOW) {
                print_auctions(auctions, auction_num);
                continue;
            }
            else if (command == BID) {
                if (atoi(arg1) >= auction_num) {
                printf("There is no auction open at %s\n", arg1);
                continue;
                }
                int num_write = write(auctions[atoi(arg1)].sock_fd, arg2, strlen(arg2));
                if(num_write <= 0){
                    fprintf(stderr, "ERROR: write to server failed\n");
                    close(auctions[atoi(arg1)].sock_fd);
                    exit(1);
                }
            }
            else if (command == QUIT) {
                for (int i = 0; i < auction_num; i++) {
                    close(auctions[i].sock_fd);
                }
                exit(0);
            }
            else {
                print_menu();
                continue;
            }
        }
    
    }
    return 0; // Shoud never get here
}
