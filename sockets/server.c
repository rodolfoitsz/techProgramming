 #include <stdio.h>
          #include <sys/socket.h>
          #include <arpa/inet.h>
          #include <stdlib.h>
          #include <string.h>
          #include <unistd.h>
          #include <netinet/in.h>

          #define MAXPENDING 5    /* Max connection requests */
          #define BUFFSIZE 32
          void Die(char *mess) { perror(mess); exit(1); }
 


          void HandleClient(int sock) {
            char buffer[BUFFSIZE];
            int received = -1;

            
            /* Receive message */
            if ((received = recv(sock, buffer, BUFFSIZE, 0)) < 0) {
              Die("Failed to receive initial bytes from client");
            }
             
            /* Send bytes and check for more incoming data in loop */
            while (received > 0) {
              /* Send back received data */
              if (send(sock, buffer, received, 0) != received) {
                Die("Failed to send bytes to client");
              }
              /* Check for more data */
              if ((received = recv(sock, buffer, BUFFSIZE, 0)) < 0) {
                Die("Failed to receive additional bytes from client");
              }

               printf("%s\n",buffer);
 
             
            }

            close(sock);
          }



          int main(int argc, char *argv[]) {
            int serversock, clientsock;
            struct sockaddr_in echoserver, echoclient;

            if (argc != 2) {
              fprintf(stderr, "USAGE: echoserver <port>\n");
              exit(1);
            }
            /* Create the TCP socket */
            if ((serversock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
              Die("Failed to create socket");
            }
            /* Construct the server sockaddr_in structure 

           You usually want to use the special constant INADDR_ANY 
           to enable receipt of client requests on any IP address 
           the server supplies; in principle, such as in a
            multi-hosting server, you could specify a particular IP address instead.
            */
            memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
            echoserver.sin_family = AF_INET;                  /* Internet/IP */
            echoserver.sin_addr.s_addr = htonl(INADDR_ANY);   /* Incoming addr */
            echoserver.sin_port = htons(atoi(argv[1]));       /* server port */



             /* Bind the server socket 

             La llamada bind() se usa cuando los puertos locales de nuestra
              máquina están en nuestros planes 
              (usualmente cuando utilizamos la llamada listen()). 
              Su función esencial es asociar un socket con un puerto (de nuestra máquina).
              Análogamente socket(), devolverá -1 en caso de error.*/
          if (bind(serversock, (struct sockaddr *) &echoserver,
                                       sizeof(echoserver)) < 0) {
            Die("Failed to bind the server socket");
          }
          /* Listen on the server socket 
      La función listen() se usa si se están esperando conexiones entrantes, 
       o cual significa, si se quiere, que alguien pueda conectarse a nuestra máquina.*/
          if (listen(serversock, MAXPENDING) < 0) {
            Die("Failed to listen on server socket");
          }



          /* Run until cancelled 
Después de llamar a listen(), se deberá llamar a accept(), para así aceptar 
las conexiones entrantes. La secuencia resumida de llamadas al sistema es:

socket()

bind()

listen()

accept()/* En la próxima sección se explicará como usar esta llamada */

            while (1) {
              unsigned int clientlen = sizeof(echoclient);
              /* Wait for client connection */
              if ((clientsock =
                   accept(serversock, (struct sockaddr *) &echoclient,
                          &clientlen)) < 0) {
                Die("Failed to accept client connection");
              }
              fprintf(stdout, "Client connected: %s\n",
                              inet_ntoa(echoclient.sin_addr));
              HandleClient(clientsock);
            }
          }

          /*
Un aspecto importante sobre los puertos y la llamada bind() 
es que todos los puertos menores que 1024 están reservados.
 Se podrá establecer un puerto, siempre que esté entre 1024 y 65535
  (y siempre que no estén siendo usados por otros programas).

          */

//gcc server.c -o server
