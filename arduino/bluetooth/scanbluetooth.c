#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>


/**
 * Decode device class
 */
static void classinfo(uint8_t dev_class[3]) {
	int flags = dev_class[2];
	int major = dev_class[1];
	int minor = dev_class[0] >> 2;

	
switch (major) {
	case 1:
		 printf(" %s", " type : computer ");
		break;
	case 2:
		 printf(" %s", "type : phone ");
		break;
        default :

               printf(" %s", "type: none ");
	}

}


int main(int argc, char **argv)
{
    inquiry_info *ii = NULL;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    int i;
    char addr[19] = { 0 };
    char name[248] = { 0 };
   
    dev_id = hci_get_route(NULL);
    sock = hci_open_dev( dev_id );
    if (dev_id < 0 || sock < 0) {
        perror("opening socket");
        exit(1);
    }

    len  = 8;
    max_rsp = 255;
    flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));
    
    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    if( num_rsp < 0 ) perror("hci_inquiry");

 
    for (i = 0; i < num_rsp; i++) {
      ba2str(&(ii+i)->bdaddr, addr);

      classinfo(&(ii+i)->dev_class);


       /*uint8_t type [3];
	type = ii[i]->dev_class;*/

      /* type[0]=&(ii+i)->dev_class[0];
       type[1]=&(ii+i)->dev_class[1];
       type[2]=&(ii+i)->dev_class[2];*/

        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name), name, 0) < 0)    
        strcpy(name, "[unknown]");
        printf("%s  %s\n", addr, name);
       //printf("%d %d %d the type\n",type[0],type[1],type[2]);
    }



    free( ii );
    close( sock );
    return 0;
}



//gcc scanbluetooth.c -lbluetooth -o scanbluetooth
