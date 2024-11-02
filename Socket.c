#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h> // For wait()

#define PORT 8989
#define BUFFER_SIZE 1024

void execute_command(char *command, char *output) {
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        strcpy(output, "运行命令失败\n");
        return;
    }
    char result[BUFFER_SIZE];
    strcpy(output, ""); // Clear the output buffer
    while (fgets(result, sizeof(result), fp) != NULL) {
        strcat(output, result);
    }
    pclose(fp);
}

int handle_client(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    char output[BUFFER_SIZE * 10] = {0}; // Larger buffer to hold command output

    int valread = read(client_socket, buffer, BUFFER_SIZE);
    buffer[valread] = '\0'; // Null-terminate the received command
    printf("收到命令: %s\n", buffer);

    execute_command(buffer, output);

    send(client_socket, output, strlen(output), 0);
    printf("向客户端发送输出\n");

    close(client_socket);
    return 0;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pid_t pid, pid1;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("创建套接字文件描述符失败");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("监听端口绑定失败");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
	
	
	// 创建第一个子进程 
    pid1 = fork();
    if (pid1 < 0) {
        // fork失败
        perror("创建子进程失败");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        if (setsid() < 0) {
            perror("setsid");
            exit(0);
        }
		printf("\nZero Panel Socket系统 测试版\n");
		printf("Socket监听端口 %d\n", PORT);
		printf("Socket监控服务     [已启动]\nPID:        %d\n",getpid());
		while (1) { // 无限循环，让服务器永远运行
			if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
				perror("accept");
				continue; // 即使accept失败，也要继续循环
			}

			pid = fork(); // 创建子进程
			if (pid < 0) {
				perror("创建子进程失败");
				close(new_socket);
				continue;
			} else if (pid == 0) {
				// 子进程，关闭服务器套接字并处理客户端
				close(server_fd);
				handle_client(new_socket);
				exit(0); // 处理客户端后退出子进程
			} else {
				// 父进程，关闭客户端套接字并继续监听
				close(new_socket);
			}
		}
    }
	
	exit(EXIT_SUCCESS);
}