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
    std::string ipAddress = "88.156.138.238";
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    //inet_pton(AF_INET,ipAddress.c_str(),&hint.sin_addr);
    hint.sin_addr.s_addr = inet_addr("88.156.138.238");
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
        
        //Wait for response
        memset(buf,0,4096);
        int bytesRecevied;
        bytesRecevied = recv(sock,buf,4096,0);
        std::cout<<" Server> "<<std::string(buf,bytesRecevied)<<"\r\n";
        std::cout<<"Wpisz wiadomosc: ";
        std::getline(std::cin,userInput);
        //Send to server
        int sendRes = send(sock,userInput.c_str(),userInput.size()+1,0);
        //TODO check if that failed
        if(sendRes == -1)
        {
            std::cerr<<"Nie mozna wyslac do serwera";
            continue;
        }
    }while(true);
    close(sock);
    return 0;
}
