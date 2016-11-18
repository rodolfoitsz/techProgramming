nclude <jni.h>
#include <stdio.h>
#include <string.h>

int main(){
/*Here start all the configuration options*/
   JavaVMOption options[1];
   JNIEnv *env;/*pointer to a native method interface*/
   JavaVM *jvm; /* denotes a Java VM */
   JavaVMInitArgs vm_args; /* JDK/JRE  VM initialization arguments */
   long status;/*Variable to save the status returned in the linked*/ 
   jclass cls; /*variable to save the class found in Java*/
   jmethodID mid;/*Variable to knows the id to the method*/
   jint square ;/*Variable to save the itn returned by java*/   

 options[0].optionString = "-Djava.class.path=.";/* the option as a string in the default platform encoding */
 memset(&vm_args, 0, sizeof(vm_args));
vm_args.version = JNI_VERSION_1_2;/*The Version of JNI*/
vm_args.nOptions = 1;
vm_args.options = options;

/*Here finish all the configuration options*/
status = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);/*Create the link between platforms*/

if(status!=JNI_ERR){
cls = (*env)->FindClass(env, "Sample2");

     if(cls !=0){

}


}


