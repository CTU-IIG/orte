#include <stdlib.h>
#include <jni.h>

static jmethodID findClassM;
static jmethodID findLoadedClassM;
static jobject classLoader;

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved)
{
  JNIEnv *env;
  jclass testCl;
  jclass clClass;
  jclass clClassLoader;
  jmethodID getClassLoader;

  (*vm)->GetEnv(vm, (void **)&env, JNI_VERSION_1_6);

  testCl = (*env)->FindClass(env, "org/ocera/orte/JOrte");
  clClass = (*env)->FindClass(env, "java/lang/Class");
  clClassLoader = (*env)->FindClass(env, "java/lang/ClassLoader");
  getClassLoader = (*env)->GetMethodID(env, clClass, "getClassLoader", "()Ljava/lang/ClassLoader;");
  classLoader = (*env)->CallObjectMethod(env, testCl, getClassLoader);
  classLoader = (*env)->NewGlobalRef(env, classLoader);
  findClassM = (*env)->GetMethodID(env, clClassLoader, "findClass", "(Ljava/lang/String;)Ljava/lang/Class;");
  findLoadedClassM = (*env)->GetMethodID(env, clClassLoader, "findLoadedClass", "(Ljava/lang/String;)Ljava/lang/Class;");

  return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL
JNI_OnUnload(JavaVM *vm, void *reserved)
{
  JNIEnv *env;

  (*vm)->GetEnv(vm, (void **)&env, JNI_VERSION_1_6);

  (*env)->DeleteGlobalRef(env, classLoader);
}

jclass
findClass(JNIEnv *env, const char *name)
{
  jclass cl;

  if ((cl = (*env)->CallObjectMethod(env, classLoader, findLoadedClassM, (*env)->NewStringUTF(env, name))) == NULL) {
    cl = (*env)->CallObjectMethod(env, classLoader, findClassM, (*env)->NewStringUTF(env, name));
  }

  return cl;
}
