#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

int main()
{
    //Create a socket

    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock == -1)
    {
        std::cerr<<"Problem z gniazdem";
        return -1;
    }
    //Create a hint stucture for the server we're connecting with

    int port = 54000;
    std::string ipAddress = "127.0.0.1";
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET,ipAddress.c_str(),&hint.sin_addr);

    //Connect to the server on the socket

    int connectRes = connect(sock,(sockaddr *)&hint,sizeof(hint));
    if(connectRes == -1)
    {
        std::cerr<<"Blad w polaczeniu";
        return -2;
    }
    //while loop

   char buf[4096];
   std::string userInput; 
    do{
        std::cout<<">";
        std::getline(std::cin,userInput);
        //Send to server
        int sendRes = send(sock,userInput.c_str(),userInput.size()+1,0);
        //TODO check if that failed
        if(sendRes == -1)
        {
            std::cerr<<"Nie mozna wyslac do serwera";
            continue;
        }
        //Wait for response
        memset(buf,0,4096);
        int bytesRecevied = recv(sock,buf,4096,0);
        std::cout<<" Server> "<<std::string(buf,bytesRecevied)<<"\r\n";
    }while(true);
    close(sock);
    return 0;
}