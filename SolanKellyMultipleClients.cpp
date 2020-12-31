#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <sys/select.h>
int main()
{
    int client_socket[5],max_clients = 5,activity,i,valread,sd,sd2,new_socket;
    int max_sd;
    fd_set master;
    char * message = "Witamy w chacie ";
    char buffer[1025];
    //Create a socket
    int listening = socket(AF_INET,SOCK_STREAM,0);
    if(listening == -1)
    {
        std::cerr<<"Nie mozna stworzyc gniazda";
        return -1;
    }
    for(i = 0;i<max_clients;i++)
    {
        client_socket[i] = 0;
    }

    //Bind the socket to a IP / port

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);//htons = host to network short
    inet_pton(AF_INET,"0.0.0.0",&hint.sin_addr);//internet command pointer to string to a number

    if(bind(listening,(sockaddr *) &hint,sizeof(hint))==-1)
    {
        std::cerr<<"nie mozna uzyc polaczyc sie z portem ";
        return -2;
    }

    //Mark the socket for listening in

    if(listen(listening,SOMAXCONN)==-1)
    {
        std::cerr <<"Cant listen";
        return -3;
    }
    if(listen(listening,3)<0)
    {
        std::cerr<<"Za malo osob";
        return -4;
    }
    int addrlen = sizeof(hint);
    while(true)
    {
        
        //clear the socket set  
        FD_ZERO(&master);   
     
        //add master socket to set  
        FD_SET(listening, &master);   
        max_sd = listening;   
             
        //add child sockets to set  
        for ( i = 0 ; i < max_clients ; i++)   
        {   
            //socket descriptor  
            sd = client_socket[i];   
                 
            //if valid socket descriptor then add to read list  
            if(sd > 0)   
                FD_SET( sd , &master);   
                 
            //highest file descriptor number, need it for the select function  
            if(sd > max_sd)   
                max_sd = sd;   
        }   
     
        //wait for an activity on one of the sockets , timeout is NULL ,  
        //so wait indefinitely  
        activity = select( max_sd + 1 , &master , NULL , NULL , NULL);   
       
        if ((activity < 0) && (errno!=EINTR))   
        {   
            printf("select error");   
        }   
             
        //If something happened on the master socket ,  
        //then its an incoming connection  
        if (FD_ISSET(listening, &master))   
        {   
            if ((new_socket = accept(listening,  
                    (struct sockaddr *)&hint, (socklen_t*)&addrlen))<0)   
            {   
                perror("accept");   
                exit(EXIT_FAILURE);   
            }   
             

           
            //send new connection greeting message  
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )   
            {   
                perror("send");   
            }   
                 
            std::cout<<"Welcome message sent successfully \n";   
                 
            //add new socket to array of sockets  
            for (i = 0; i < max_clients; i++)   
            {   
                //if position is empty  
                if( client_socket[i] == 0 )   
                {   
                    client_socket[i] = new_socket;   
                    printf("Adding to list of sockets as %d\n" , i);   
                         
                    break;   
                }   
            }   
        
        }
        else
        {
            /* code */
        
        
            //else its some IO operation on some other socket 
            for (i = 0; i < max_clients; i++)   
            {   
                sd = client_socket[i];   
                    
                if (FD_ISSET( sd , &master))   
                {   
                    //Check if it was for closing , and also read the  
                    //incoming message  
                    if ((valread = read( sd , buffer, 1024)) == 0)   
                    {   
                        std::cout<<"Rozlaczono klienta"<<std::endl;   
                            
                        //Close the socket and mark as 0 in list for reuse  
                        close( sd );   
                        client_socket[i] = 0;   
                    }   
                        
                    //Echo back the message that came in  
                    else 
                    {   
                        for(i = 0;i<max_clients;i++)
                        {
                            sd2=client_socket[i];
                            if(sd2!=listening&&sd2!=sd)
                            {
                                //set the string terminating NULL byte on the end  
                                //of the data read  
                                buffer[valread] = '\0';   
                                send(sd2, buffer , strlen(buffer) , 0 ); 
                            }
                        }
                        
                    }   
                }
            }
        }
          
    } 
        // fd_set copy = master;
        // int socketCount = select(FD_SETSIZE,&copy,NULL,NULL,NULL);
        // if(socketCount == -1)
        // {
        //     std::cerr<<"Blad z zliczaniem";
        //     return -5;
        // }
        // for(int i=0;i<FD_SETSIZE;i++)
        // {
        //     if(FD_ISSET(i,&copy))
        //     {
        //         if(i == listening)
        //         {
        //             sockaddr_in client;
        //             socklen_t clientSize = sizeof(client);
        //             char host[NI_MAXHOST];
        //             char srv[NI_MAXSERV];
        //             int clientSocket = accept(listening,(sockaddr *)&client,&clientSize);
        //             if(clientSocket ==-1)
        //             {
        //                 std::cerr<<"Problem z Klientem";
        //                 return -4;
        //             }
        //             FD_SET(clientSocket,&master);
                    
        //             std::cout<<"siadlo"<<std::endl;
        //             send(clientSocket,WelcomeMessage.c_str(),WelcomeMessage.size()+1,0);
                    
        //         }
        //         else
        //         {
        //             std::cout<<"Czy jestesmy chociaz tu? ";
        //             char buf[4096];
        //             memset(buf,0,4096);
        //             int bytesRecv = recv(i,buf,4096,0);
        //             std::cout<<"TEST123:: "<<std::string(buf,bytesRecv+1);
        //             //if(bytesRecv<=0)
        //             //{
        //             //    close(i);
        //             //    FD_CLR(i,&master);
        //             //}
        //             //else
        //             //{
        //                 // if(buf[0]=='\\')
        //                 // {
        //                 //     std::string cmd = std::string(buf,bytesRecv);
        //                 //     if(cmd == "\\")
        //                 //     {
        //                 //         running = false;
        //                 //         break;
        //                 //     }

        //                 //     continue;
        //                 // }
        //                 for(int j = 0;j<FD_SETSIZE;j++)
        //                 {
        //                     if(FD_ISSET(j,&master))
        //                     {
        //                         if(j!=listening)
        //                         {
        //                             send(j,buf,bytesRecv+1,0);
        //                         }
        //                     }
        //                 }
        //             //}
                    
        //         }
                
        //     }
        // }
    
    //Close socket
    close(listening);
    return 0; 
}