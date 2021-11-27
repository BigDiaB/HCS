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

    while(1)
    {

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
            LSD_Log(LSD_ltMESSAGE,"%s", right_line);
            sscanf(right_line, "%c %c %c %c %c %c %d %d",&HCS_Input_A_down, &HCS_Input_A_pressed, &HCS_Input_A_released, &HCS_Input_B_down, &HCS_Input_B_pressed, &HCS_Input_B_released, &HCS_Input_Pad.x, &HCS_Input_Pad.y);
        }



        write(new_socket, message, strlen(message));

        free(message);

        close(new_socket);
    }