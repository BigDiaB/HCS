#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <netdb.h>     


void check_host_name(int hostname) { //This function returns host name for local computer
   if (hostname == -1) {
      perror("gethostname");
      exit(1);
   }
}
void check_host_entry(struct hostent * hostentry) { //find host info from host name
   if (hostentry == NULL) {
      perror("gethostbyname");
      exit(1);
   }
}
void IP_formatter(char *IPbuffer) { //convert IP string to dotted decimal format
   if (NULL == IPbuffer) {
      perror("inet_ntoa");
      exit(1);
   }
}

LSD_Thread_function(Controller_Server)
{
    LSD_Thread_init();





    char host[256];
    char *IP;
    struct hostent *host_entry;
    int hostname;
    hostname = gethostname(host, sizeof(host)); //find the host name
    check_host_name(hostname);
    host_entry = gethostbyname(host); //find host information
    check_host_entry(host_entry);
    IP = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));
    //Convert into IP string
    printf("Host Name: %s\n", host);
    printf("Host IP: %s\n", IP);






    int server_fd, new_socket;
    const char* template = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=UTF-8\nContent-Length: %lu\n\n%s";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        LSD_Log(LSD_ltERROR, "Socket wurde nicht erstellt!");
        exit(1);
    }

    struct sockaddr_in address;
    #define PORT 1234
    #define NUM_CONNECTIONS 1

    memset((char*)&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);


    int opt = 1;

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt))<0)
    {
        perror("setsockopt");exit(EXIT_FAILURE);
    }

    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, (char *)&opt, sizeof(opt))<0)
    {
        perror("setsockopt");exit(EXIT_FAILURE);
    }

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        LSD_Log(LSD_ltERROR,"Bind ist fehlgeschlagen!");
        exit(1);
    }

    if (listen(server_fd, NUM_CONNECTIONS) < 0)
    {
        LSD_Log(LSD_ltERROR,"Listen ist fehlgeschlagen!");
        exit(1);
    }



    int addrlen = sizeof(address);

    while(running)
    {
        // nanosleep((const struct timespec[]){{0, 50000000L}}, NULL);
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
        {
            LSD_Log(LSD_ltERROR, "Accept ist fehlgeschlagen!");
            exit(1);
        }


        char buffer[30000] = {0};

        int valread = read(new_socket, buffer, 1024);
        // system("clear");
        // LSD_Log(LSD_ltMESSAGE, "%s", buffer);
        if (valread < 0)
        {
            LSD_Log(LSD_ltWARNING, "Read konnte keine Bytes lesen!");
        }

        char* message = malloc(1);

        if(strstr(buffer, "GET") != NULL)
        {
            buffer[(int)(strstr(buffer, "\n") - buffer) - 10] = 0;

            char path[1024] = "server";
            strcat(path,buffer + 4);

            LSD_File* file = LSD_File_open(path);

            message = realloc(message,strlen(file->data) + strlen(template) + 100);

            sprintf(message,template,strlen(file->data),file->data);

            LSD_File_close(file);
        }
        else if(strstr(buffer, "POST") != NULL)
        {
            // buffer[(int)(strstr(buffer, "\n") - buffer)] = 0;
            // LSD_Log(LSD_ltMESSAGE,"%s", buffer);
            char* right_line = strstr(buffer,"Content-Type: ") + 14;
            right_line[(int)(strstr(right_line, "\n") - right_line)] = 0;
            // LSD_Log(LSD_ltMESSAGE,"%s", right_line);
            // HCS_Input_A_Ldown = HCS_Input_A_down;
            // HCS_Input_B_Ldown = HCS_Input_B_down;

            sscanf(right_line, "%d %d %d %d",&HCS_Input_A_down, &HCS_Input_B_down, &HCS_Input_Pad.x, &HCS_Input_Pad.y);
        }



        write(new_socket, message, strlen(message));

        free(message);

        close(new_socket);
    }
    close(server_fd);

    LSD_Delta_remove("Controller_Server");
    LSD_Thread_finish();
}