//libraries to jess
#include <jni.h>
#include <string.h>

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

/* Destroy the JVM */
int shutDown() { 
    (*jvm)->DestroyJavaVM(jvm); 
    return 0; 
}

/* Instantiate the JVM, find the class and instantiate it */
 int instantiateVM() {
   
   options[0].optionString = "-Djava.class.path=.";
   memset(&vm_args, 0, sizeof(vm_args));
   vm_args.version = JNI_VERSION_1_2;
   vm_args.nOptions = 1;
   vm_args.options = options;
   status = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);

    // Virtual Machine creation
   if (status == JNI_ERR) { printf("problem in VM creation\n\n"); return -1; }  

    // Looking for the class
   cls = (*env)->FindClass(env, "jess/Rete");     
   if (cls == 0) { printf("problem in finding class\n\n"); return shutDown(); } 
    
    // Finding class constructor
   jmethodID constructor = (*env)->GetMethodID(env, cls, "<init>", "()V");
   if (constructor == 0) { printf("problem with constructor\n\n"); return shutDown(); }

    // Creating object
   obj = (*env)->NewObject(env, cls, constructor);
   if (obj == 0) { printf("problem with the object creation\n\n"); return shutDown(); }
   
   return 1;
}
 
/* Call a RESET command on the ES */
int reset() {
    jmethodID mid = (*env)->GetMethodID(env, cls, "reset", "()V");
    
    if (mid == 0) { 
        printf("problem detected\n\n"); 
        return shutDown();
    }
    else {
        // calling the function
        (*env)->CallVoidMethod(env, obj, mid, NULL);
        printf("reset called\n");
    }
    
    return 1;
}


char evalBuffer[512];
/* Call a EVAL command and place the result on the buffer */
int eval(char* command) {
   
    jmethodID mid = (*env)->GetMethodID(env, cls, "eval", "(Ljava/lang/String;)Ljess/Value;");
    
    if (mid == 0) { 
        printf("problem detected\n\n"); 
        return shutDown();
    }
    else {
        // creating temporary buffer
        //const char tmpBuffer[1024];
        // filling it
        //strcpy(tmpBuffer, command);
        // converting to jstring
        jstring arg = (*env)->NewStringUTF(env, command); 
        // calling the function and storing the result
       (*env)->CallVoidMethod(env, obj, mid, arg);
        printf("eval called\n");
    }
    
    return 1;
}



int main(int argc, char **argv)
{

   instantiateVM();
    reset();


eval("(defrule 1 (left_a) => (printout t \"tienes gripa\"))");
   eval("(assert (left_a))");
   eval("(run)");
   eval("(facts)");
  /*   eval("(defrule f (forw a)");
    eval("(defrule r (right a)");
    eval("(defrule b (back a)");

    eval("deffunction max (?a ?b ?c)(if (> ?a ?b ?c) then ?a else if (> ?b ?a ?c) then ?b else if (> ?c ?a ?b) then ?c )) TRUE ");
     


     (forw b) (right c) (back c) => (assert (tienes gripa)))");

    eval("(assert (left a))");
    eval("(assert (hecho b))");
    eval("(assert (hecho c))");*/


   shutDown();

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


    // send a message

uint8_t recivedData;


    while( status >= 0 ) {

        
         unsigned char buf[4];
        // char stringReceived[256];
        printf("Send a command\n");
        char tmpChar;
        scanf("%c",&tmpChar);
         getchar(); // To consume the newline  
        buf[0] = tmpChar;

        status = write(s,buf, 4);

        sleep(4);
        
         unsigned char buf0[4];
         
    
        read(s, buf0,5);
       
       //values with bigger number means  superfice is white
        printf("point 1  %d\n",buf0[0] );
        printf("point 2  %d\n",buf0[1] );
        printf("point 3  %d\n",buf0[2] );
        printf("point 4  %d\n",buf0[3] );
       

    }

    if( status < 0 ) perror("uh oh");

    close(s);
    return 0;
}

//export LD_LIBRARY_PATH=/usr/lib/jvm/java-8-openjdk-i386/jre/lib/i386/server

// gcc -I/usr/lib/jvm/java-8-openjdk-i386/include/ -I/usr/lib/jvm/java-8-openjdk-i386/include/linux -L/usr/lib/jvm/java-8-openjdk-i386/jre/lib/i386/server sendData.c -lbluetooth -o sendData  -ljvm


//gcc sendData.c -lbluetooth -o sendData



