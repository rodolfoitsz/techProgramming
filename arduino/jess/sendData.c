//libraries to jess
#include <jni.h>
#include <string.h>
#include <stdlib.h>

 #ifdef _WIN32
 #define PATH_SEPARATOR ';'
 #else
 #define PATH_SEPARATOR ':'
 #endif

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <unistd.h>


#define USART_BAUDRATE 9600
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)


JavaVMOption options[1];
 JNIEnv *env;
 JavaVM *jvm;
 JavaVMInitArgs vm_args;
 long status;
 jobject obj;
 jclass cls;

//id frommethod
 jmethodID mid;


/* Destroy the JVM */
int shutDown() { 
    (*jvm)->DestroyJavaVM(jvm); 
    return 0; 
}

/* Instantiate the JVM, find the class and instantiate it */
 int instantiateVM(  ) {
   
   options[0].optionString = "-Djava.class.path=.";
   memset(&vm_args, 0, sizeof(vm_args));
   vm_args.version = JNI_VERSION_1_2;
   vm_args.nOptions = 1;
   vm_args.options = options;
   status = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
  


    // Virtual Machine creation
   if (status == JNI_ERR) { printf("problem in VM creation\n\n"); return -1; }  


    // Looking for the class
   cls = (*env)->FindClass(env, "JessJava");     
   if (cls == 0) { printf("problem in finding class\n\n"); return shutDown(); } 
    
    // Finding class constructor
    mid = (*env)->GetStaticMethodID(env, cls, "dataJessMethod", "([I)C");
   if (mid == 0) { printf("problem with static method\n\n"); return shutDown(); }

       
     


   return 1;
}
 

char evaluateNumbers( int  *values ) { 

 jintArray intArray= (*env)->NewIntArray(env,4);
       
       (*env)->SetIntArrayRegion(env,intArray,0,4,values);

       

   jchar resultChar= (*env)->CallStaticCharMethod(env, cls, mid, intArray);
        printf("Result of StaticMethod: %c\n\n", resultChar);

        return resultChar;


}


int main(int argc, char **argv)
{

 struct sockaddr_rc addr = { 0 };
    int s, status,bytes_read;
   //new bluetooth 
   //char dest[18] = "98:D3:32:10:4B:79";

    //oldbluetooth
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




    instantiateVM(); 

    //char i to initialize 
    char tmpChar='i';

    while( status >= 0 ) {

        unsigned char buf[4];
        // char stringReceived[256];
        printf("Send a command\n");
      // scanf("%c",&tmpChar);
      //getchar(); // To consume the newline  
        buf[0] = tmpChar;


        // send a message
        status = write(s,buf, 4);

        sleep(4);
        
        unsigned char dataNumbers[4];

        read(s, dataNumbers,4);
       
       //values with bigger number means  superfice is white
        printf("point 1  %d\n",dataNumbers[0] );
        printf("point 2  %d\n",dataNumbers[1] );
        printf("point 3  %d\n",dataNumbers[2] );
        printf("point 4  %d\n",dataNumbers[3] );

        unsigned int bufConvToInt[4];
        bufConvToInt[0]=dataNumbers[0];
        bufConvToInt[1]=dataNumbers[1];
        bufConvToInt[2]=dataNumbers[2];
        bufConvToInt[3]=dataNumbers[3];
        
        tmpChar=evaluateNumbers(bufConvToInt);
       

    }

    
    shutDown();

    if( status < 0 ) perror("uh oh");

    close(s);
    return 0;
}

//export LD_LIBRARY_PATH=/usr/lib/jvm/java-8-openjdk-i386/jre/lib/i386/server

// gcc -I/usr/lib/jvm/java-8-openjdk-i386/include/ -I/usr/lib/jvm/java-8-openjdk-i386/include/linux -L/usr/lib/jvm/java-8-openjdk-i386/jre/lib/i386/server sendData.c -lbluetooth -o sendData  -ljvm


//gcc sendData.c -lbluetooth -o sendData



