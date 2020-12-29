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
    int listening = socket(AF_INET,SOCK_STREAM,0);
    if(listening == -1)
    {
        std::cerr<<"Nie mozna stworzyc gniazda";
        return -1;
    }
    //Bind the socket to a IP / port

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);//htons = host to network short
    inet_pton(AF_INET,"0.0.0.0",&hint.sin_addr);//internet command pointer to string to a number

    //(bind(listening,AF_INET,(struct sockaddr *) &hint,sizeof(hint))==-1)
    if(bind(listening,(sockaddr *) &hint,sizeof(hint))==-1)
    {
        std::cerr<<"nie mozna uzyc bind do portu";
        return -2;
    }

    //Mark the socket for listening in

    if(listen(listening,SOMAXCONN)==-1)
    {
        std::cerr <<"Cant listen";
        return -3;
    }

    //Accept a call

    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char srv[NI_MAXSERV];
    int clientSocket = accept(listening,(sockaddr *)&client,&clientSize);
    if(clientSocket ==-1)
    {
        std::cerr<<"Problem z Klientem";
        return -4;
    }
    //Close the listening socket

    close(listening);
    std::cout<<"test";
    memset(host,0,NI_MAXHOST);
    memset(srv,0,NI_MAXSERV);
    int results = getnameinfo((sockaddr *)&client,sizeof(client),host,NI_MAXHOST,srv,NI_MAXSERV,0);
    if(results)
    {
        std::cout<<host<<" polaczony z "<<srv<<std::endl;
    }
    else
    {
        inet_ntop(AF_INET,&client.sin_addr,host,NI_MAXHOST);
        std::cout<<host <<"polaczony z "<<ntohs(client.sin_port)<<std::endl;
    }
       
    //While reciveing display message echo message

    char buf[4096];
    while(true)
    {
        //Clear the buffer
        memset(buf,0,4096);
        //Wait for the message
        int bytesRecv = recv(clientSocket,buf,4096,0);
        if(bytesRecv == -1)
        {
            std::cerr<<" Nastapil blad z polaczeniem"<<std::endl;
        }
        if(bytesRecv == 0)
        {
            std::cout<<"Klient sie rozlaczyl "<<std::endl;
        }
        //Display message
        std::cout<<"Otrzymano: "<<std::string(buf,0,bytesRecv)<<std::endl;

        //Resend message
        send(clientSocket,buf,bytesRecv+1,0);
        
        
    }
    //Close socket
    close(clientSocket);
    return 0;
}