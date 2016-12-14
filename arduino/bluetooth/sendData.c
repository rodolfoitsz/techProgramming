#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <unistd.h>


#define USART_BAUDRATE 9600
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

int arrayEnciende[8]={'E','N','C','I','E','N','D','E'};
int arrayApaga[5]={'A','P','A','G','A'};
//int arrayEnciende[8]={69,78,67,73,69,78,68,69}

int enciende(char  * stringReceived){

int i =0;
int coincidences=0;
while(arrayEnciende[i] != 0){

if(stringReceived[i]==arrayEnciende[i]){
coincidences++;
    }     
i++;
}
return coincidences;

}


int apaga(char  * stringReceived){

int i =0;
int coincidences=0;
while(arrayApaga[i] != 0){

  printf("hola %d",i);

   if(stringReceived[i]==arrayApaga[i]){
    coincidences++;
    }     
   i++;
}
return coincidences;

}


 
int main(int argc, char **argv)
{

struct sockaddr_rc addr = { 0 };
    int s, status,bytes_read;
    char dest[18] = "98:D3:31:30:71:08";
   
    char bufr[1024] = { 0 };
   

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( dest, &addr.rc_bdaddr );


    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));


    // send a message

uint8_t recivedData;


    while( status >= 0 ) {

        
         unsigned char buf[4];
        char stringReceived[256];
         printf("Send a command\n");
        char tmpChar[256];
        scanf("%s",tmpChar);
        getchar(); // To consume the newline  
  
      int i =0;
      while ( tmpChar[i] != 0){
        buf[0] = tmpChar[i];

        status = write(s,buf, 4);
        
         unsigned char buf2[4];
         sleep(2);

        int status2 = read(s, buf2,4);
        stringReceived[i]=buf2[0];

        printf("Status %c\n",stringReceived[i]);
         
         i ++;
  
         }

      
          if(enciende (stringReceived)==8){
            char send[1024] = { '/' };
            write(s,send, 4);
           }

         else{
           char send[1024] = { '+' };
             write(s,send, 4);

            }
         

    }

    if( status < 0 ) perror("uh oh");

    close(s);
    return 0;
}

//gcc sendData.c -lbluetooth -o sendData



