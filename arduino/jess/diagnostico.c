#include <jni.h>
#include <string.h>

 #ifdef _WIN32
 #define PATH_SEPARATOR ';'
 #else
 #define PATH_SEPARATOR ':'
 #endif

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

int main() {
	
	options[0].optionString = "-Djava.class.path=.";
	memset(&vm_args, 0, sizeof(vm_args));
	vm_args.version = JNI_VERSION_1_2;
	vm_args.nOptions = 1;
	vm_args.options = options;
	status = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);

	// Virtual Machine creation
	if (status == JNI_ERR) { printf("problem in VM creation\n\n"); return -1; }	

	// Looking for the class
	cls = (*env)->FindClass(env, "Actividad2");	  
	if (cls == 0) { printf("problem in finding class\n\n"); return shutDown(); }	
	
	jmethodID mid = (*env)->GetStaticMethodID(env, cls, "work", "()V");
	
	if (mid != 0) { printf("executing...\n\n"); (*env)->CallVoidMethod(env, cls, mid); }
	else { printf("problem detected\n\n"); return shutDown(); }
  	
}


//gcc -I/usr/lib/jvm/java-8-openjdk-i386/include/ -I/usr/lib/jvm/java-8-openjdk-i386/include/linux -L/usr/lib/jvm/java-8-openjdk-i386/jre/lib/i386/server diagnostico.c -ljvm

//export LD_LIBRARY_PATH=/usr/lib/jvm/java-8-openjdk-i386/jre/lib/i386/server