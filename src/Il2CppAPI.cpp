#include <Il2CppAPI.h>
#include <android/log.h>
#include <codecvt>
#include <cstdint>
#include <cstring>
#include <dlfcn.h>
#include <dobby.h>
#include <jni.h>
#include <locale>
#include <map>
#include <obfuscate.h>
#include "unity/Vector3.h"

using namespace Il2CppTypes;

typedef unsigned short UTF16;
typedef wchar_t UTF32;
typedef char UTF8;

static bool il2cppAttached = false;

void *(*il2cpp_class_from_name)(void *image, const char *namespaze,
                                const char *name);

void *(*il2cpp_domain_get)();
void **(*il2cpp_domain_get_assemblies)(const void *domain, size_t *size);
void *(*il2cpp_domain_assembly_open)(void *domain, const char *name);
void *(*il2cpp_assembly_get_image)(void *assembly);

void *(*il2cpp_class_get_field_from_name)(void *klass, const char *name);
void (*il2cpp_field_get_value)(void *klass, void *field, void *value);
void (*il2cpp_field_set_value)(void *klass, void *field, void *value);
void (*il2cpp_field_static_get_value)(void *field, void *value);
void (*il2cpp_field_static_set_value)(void *field, void *value);
void *(*il2cpp_field_get_type)(void *field);
int (*il2cpp_field_get_flags)(void *field);
size_t (*il2cpp_field_get_offset)(void *field);

char *(*il2cpp_type_get_name)(void *type);
const char *(*il2cpp_image_get_name)(void *image);
// uint32_t (*il2cpp_type_get_attrs)(void *type);

void *(*il2cpp_class_get_methods)(void *klass, void **iter);
void *(*il2cpp_class_get_method_from_name)(void *klass, const char *name,
                                           int argsCount);

const char *(*il2cpp_method_get_name)(void *method);
uint32_t (*il2cpp_method_get_param_count)(void *method);
void *(*il2cpp_method_get_param)(void *method, uint32_t index);

String *(*il2cpp_string_new)(const char *text);
String *(*il2cpp_string_new_utf16)(const wchar_t *text, int32_t len);

Object *(*il2cpp_object_new)(void *klass);
void *(*il2cpp_runtime_object_init)(void *object);
void *(*il2cpp_runtime_class_init)(void *klass);

void *(*il2cpp_runtime_invoke)(void *method, void *obj, void **params,
                               void **exc);
void *(*il2cpp_thread_attach)(void *domain);
void (*il2cpp_thread_detach)(void *thread);

void *(*il2cpp_array_new)(void *type, uintptr_t length);

void (*__il2cpp_init)(const char *domain_name);
void il2cpp_init(const char *domain_name) {
  __android_log_print(ANDROID_LOG_INFO, IL2CPP_LOG_TAG,
                      AY_OBFUSCATE("il2cpp_init: %s"), domain_name);
  return __il2cpp_init(domain_name);
}

void *lib_handle;
uintptr_t absoulute_address;
const char *absoulute_path;
bool attached;
const char *target_lib;

void Il2CppAPI::InitAPI(const char *lib_name) {
  int countTryOpen = 0;
  do {
    if (countTryOpen >= 10) {
      __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,
                          AY_OBFUSCATE("ERRO: Failed to dlopen: %s"), lib_name);
      break;
    }
    lib_handle = dlopen(lib_name, RTLD_LAZY);
    __android_log_print(ANDROID_LOG_INFO, IL2CPP_LOG_TAG, "try dlopen: %s | %p",
                        lib_name, lib_handle);
    countTryOpen += 1;
  } while (lib_handle == nullptr);

  if (lib_handle != nullptr) {
    void *p_il2cpp_init = dlsym(lib_handle, AY_OBFUSCATE("il2cpp_init"));

    Dl_info il2cpp_info;
    dladdr(p_il2cpp_init, &il2cpp_info);
    size_t len = strlen(il2cpp_info.dli_fname) + 1;
    char *lib_path = new char[len];
    memset((void *)lib_path, 0, len);
    strcpy(lib_path, il2cpp_info.dli_fname);

    absoulute_address = (uintptr_t)il2cpp_info.dli_fbase;
    absoulute_path = lib_path;
    attached = false;
    target_lib = lib_name;

    if (p_il2cpp_init != nullptr)
      DobbyHook(p_il2cpp_init, (dobby_dummy_func_t)il2cpp_init,
                (dobby_dummy_func_t *)&__il2cpp_init);
    __android_log_print(ANDROID_LOG_INFO, IL2CPP_LOG_TAG,
                        AY_OBFUSCATE("Il2CppAPI"));
    dlclose(lib_handle);
  } else {
    absoulute_address = 0;
    absoulute_path = AY_OBFUSCATE("failed when initialize Il2CpppAPI");
    attached = false;
    target_lib = lib_name;
    __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,
                        AY_OBFUSCATE("failed when constructing Il2CpppAPI"));
  }
}

void Il2CppAPI::InitAPI(JNIEnv *env, const char *lib_name) {
  jclass activityThread = env->FindClass("android/app/ActivityThread");
  jobject currentActivityThread = env->CallStaticObjectMethod(
      activityThread,
      env->GetStaticMethodID(activityThread, "currentActivityThread",
                             "()Landroid/app/ActivityThread;"));
  jobject context = env->CallObjectMethod(
      currentActivityThread, env->GetMethodID(activityThread, "getApplication",
                                              "()Landroid/app/Application;"));

  jmethodID getAppInfo = env->GetMethodID(
      env->GetObjectClass(context), AY_OBFUSCATE("getApplicationInfo"),
      AY_OBFUSCATE("()Landroid/content/pm/ApplicationInfo;"));
  jobject appInfo = env->CallObjectMethod(context, getAppInfo);

  jfieldID nativeLibraryDir = env->GetFieldID(
      env->GetObjectClass(appInfo), AY_OBFUSCATE("nativeLibraryDir"),
      AY_OBFUSCATE("Ljava/lang/String;"));
  const char *lib_path = env->GetStringUTFChars(
      (jstring)env->GetObjectField(appInfo, nativeLibraryDir), nullptr);

  absoulute_path = lib_path;
  lib_handle = dlopen(lib_path, RTLD_LAZY);
  if (lib_handle != nullptr) {
    void *p_il2cpp_init = dlsym(lib_handle, AY_OBFUSCATE("il2cpp_init"));
    if (p_il2cpp_init != nullptr)
      DobbyHook(p_il2cpp_init, (dobby_dummy_func_t)il2cpp_init,
                (dobby_dummy_func_t *)&__il2cpp_init);
    __android_log_print(ANDROID_LOG_INFO, IL2CPP_LOG_TAG,
                        AY_OBFUSCATE("Il2CppAPI"));

    Dl_info il2cpp_info;
    dladdr(p_il2cpp_init, &il2cpp_info);
    size_t len = strlen(il2cpp_info.dli_fname) + 1;
    char *lib_path = new char[len];
    memset((void *)lib_path, 0, len);
    strcpy(lib_path, il2cpp_info.dli_fname);

    absoulute_address = (uintptr_t)il2cpp_info.dli_fbase;
    absoulute_path = lib_path;
    attached = false;
    target_lib = lib_name;
    dlclose(lib_handle);
  } else {
    absoulute_address = 0;
    absoulute_path = AY_OBFUSCATE("failed when initialize Il2CpppAPI");
    attached = false;
    target_lib = lib_name;
    __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,
                        AY_OBFUSCATE("failed when initialize Il2CpppAPI"));
  }
}

void *Il2CppAPI::GetLibHandle() { return lib_handle; }

uintptr_t Il2CppAPI::GetAbsolouteAddress() { return absoulute_address; }

const char *Il2CppAPI::GetAbsoloutePath() { return absoulute_path; }

bool Il2CppAPI::IsAttached() { return attached; }

void Il2CppAPI::Detach() {

  il2cpp_image_get_name = nullptr;
  il2cpp_domain_get_assemblies = nullptr;
  il2cpp_class_from_name = nullptr;
  il2cpp_domain_get = nullptr;
  il2cpp_domain_assembly_open = nullptr;
  il2cpp_assembly_get_image = nullptr;
  il2cpp_class_get_field_from_name = nullptr;
  il2cpp_field_get_offset = nullptr;
  il2cpp_field_get_value = nullptr;
  il2cpp_field_set_value = nullptr;
  il2cpp_field_static_get_value = nullptr;
  il2cpp_field_static_set_value = nullptr;
  il2cpp_field_get_type = nullptr;
  il2cpp_field_get_flags = nullptr;
  il2cpp_type_get_name = nullptr;
  // il2cpp_type_get_attrs = nullptr;
  il2cpp_class_get_methods = nullptr;
  il2cpp_class_get_method_from_name = nullptr;
  il2cpp_method_get_name = nullptr;
  il2cpp_method_get_param_count = nullptr;
  il2cpp_method_get_param = nullptr;
  il2cpp_string_new = nullptr;
  il2cpp_string_new_utf16 = nullptr;
  il2cpp_object_new = nullptr;
  il2cpp_runtime_object_init = nullptr;
  il2cpp_runtime_class_init = nullptr;
  il2cpp_runtime_invoke = nullptr;
  il2cpp_thread_attach = nullptr;
  il2cpp_thread_detach = nullptr;
  il2cpp_array_new = nullptr;

  attached = false;
  il2cppAttached = false;
}

void Il2CppAPI::Attach() {
  void *il2cpp = nullptr;
  while (il2cpp == nullptr) {
    il2cpp = dlopen(target_lib, 4);
  }

  il2cpp_image_get_name =
      (const char *(*)(void *))dlsym(il2cpp, "il2cpp_image_get_name");
  il2cpp_domain_get_assemblies = (void **(*)(const void *, size_t *))dlsym(
      il2cpp, "il2cpp_domain_get_assemblies");
  il2cpp_class_from_name = (void *(*)(void *, const char *, const char *))dlsym(
      il2cpp, "il2cpp_class_from_name");
  il2cpp_domain_get = (void *(*)(void))dlsym(il2cpp, "il2cpp_domain_get");
  il2cpp_domain_assembly_open = (void *(*)(void *, const char *))dlsym(
      il2cpp, "il2cpp_domain_assembly_open");
  il2cpp_assembly_get_image =
      (void *(*)(void *))dlsym(il2cpp, "il2cpp_assembly_get_image");
  il2cpp_class_get_field_from_name = (void *(*)(void *, const char *))dlsym(
      il2cpp, "il2cpp_class_get_field_from_name");
  il2cpp_field_get_offset =
      (size_t(*)(void *))dlsym(il2cpp, "il2cpp_field_get_offset");
  il2cpp_field_get_value =
      (void (*)(void *, void *, void *))dlsym(il2cpp, "il2cpp_field_get_value");
  il2cpp_field_set_value =
      (void (*)(void *, void *, void *))dlsym(il2cpp, "il2cpp_field_set_value");
  il2cpp_field_static_get_value =
      (void (*)(void *, void *))dlsym(il2cpp, "il2cpp_field_static_get_value");
  il2cpp_field_static_set_value =
      (void (*)(void *, void *))dlsym(il2cpp, "il2cpp_field_static_set_value");
  il2cpp_field_get_type =
      (void *(*)(void *))dlsym(il2cpp, "il2cpp_field_get_type");
  il2cpp_field_get_flags =
      (int (*)(void *))dlsym(il2cpp, "il2cpp_field_get_flags");
  il2cpp_type_get_name =
      (char *(*)(void *))dlsym(il2cpp, "il2cpp_type_get_name");
  // il2cpp_type_get_attrs = (uint32_t (*)(void *))dlsym(il2cpp,
  // "il2cpp_type_get_attrs");
  il2cpp_class_get_methods =
      (void *(*)(void *, void **))dlsym(il2cpp, "il2cpp_class_get_methods");
  il2cpp_class_get_method_from_name =
      (void *(*)(void *, const char *, int))dlsym(
          il2cpp, "il2cpp_class_get_method_from_name");
  il2cpp_method_get_name =
      (const char *(*)(void *))dlsym(il2cpp, "il2cpp_method_get_name");
  il2cpp_method_get_param_count =
      (uint32_t(*)(void *))dlsym(il2cpp, "il2cpp_method_get_param_count");
  il2cpp_method_get_param =
      (void *(*)(void *, uint32_t))dlsym(il2cpp, "il2cpp_method_get_param");
  il2cpp_string_new =
      (String * (*)(const char *)) dlsym(il2cpp, "il2cpp_string_new");
  il2cpp_string_new_utf16 = (String * (*)(const wchar_t *, int32_t))
      dlsym(il2cpp, "il2cpp_string_new_utf16");
  il2cpp_object_new = (Object * (*)(void *)) dlsym(il2cpp, "il2cpp_object_new");
  il2cpp_runtime_object_init =
      (void *(*)(void *))dlsym(il2cpp, "il2cpp_runtime_object_init");
  il2cpp_runtime_class_init =
      (void *(*)(void *))dlsym(il2cpp, "il2cpp_runtime_class_init");
  il2cpp_runtime_invoke = (void *(*)(void *, void *, void **, void **))dlsym(
      il2cpp, "il2cpp_runtime_invoke");
  il2cpp_thread_attach =
      (void *(*)(void *))dlsym(il2cpp, "il2cpp_thread_attach");
  il2cpp_thread_detach =
      (void (*)(void *))dlsym(il2cpp, "il2cpp_thread_detach");
  il2cpp_array_new =
      (void *(*)(void *, uintptr_t))dlsym(il2cpp, "il2cpp_array_new");

  dlclose(il2cpp);
  attached = true;
  il2cppAttached = true;
}

void *Il2CppAPI::GetImageByName(const char *image) {
  if (!attached)
    return nullptr;
  size_t size;
  void **assemblies = il2cpp_domain_get_assemblies(il2cpp_domain_get(), &size);

  for (int i = 0; i < size; ++i) {
    void *img = (void *)il2cpp_assembly_get_image(assemblies[i]);
    const char *img_name = il2cpp_image_get_name(img);

    if (strcmp(img_name, image) == 0) {
      return img;
    }
  }
  return nullptr;
}

void *Il2CppAPI::GetClassType(const char *image, const char *namespaze,
                              const char *clazz) {
  if (!attached)
    return nullptr;
  static std::map<std::string, void *> cache;
  std::string s = image;
  s += namespaze;
  s += clazz;
  if (cache.count(s) > 0)
    return cache[s];
  void *img = GetImageByName(image);
  if (!img) {
    __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,
                        "Can't find image %s!", image);
    return 0;
  }
  void *klass = il2cpp_class_from_name(img, namespaze, clazz);
  if (!klass) {
    __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,
                        "Can't find class %s!", clazz);
    return 0;
  }
  cache[s] = klass;
  return klass;
}
Class *Il2CppAPI::CreateClassInstance(const char *image, const char *namespaze,
                                      const char *clazz) {
  if (!attached)
    return nullptr;
  void *img = GetImageByName(image);
  if (!img) {
    __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,
                        "Can't find image %s!", image);
    return 0;
  }
  void *klass = GetClassType(image, namespaze, clazz);
  if (!klass) {
    __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,
                        "Can't find class %s!", clazz);
    return 0;
  }
  void *obj = il2cpp_object_new(klass);
  if (!obj) {
    __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,
                        "Can't create object %s!", clazz);
    return 0;
  }
  return (Class *)obj;
}

bool IsStaticField(void *field) {
  return (il2cpp_field_get_flags(field) & 0x10) != 0;
}

void Il2CppAPI::GetFieldValue(const char *image, const char *namespaze,
                              const char *clazz, const char *name,
                              void *output) {
  if (!attached)
    return;
  void *img = GetImageByName(image);
  if (!img) {
    __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,
                        "Can't find image %s!", image);
    return;
  }
  void *klass = GetClassType(image, namespaze, clazz);
  if (!klass) {
    __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,
                        "Can't find class %s!", clazz);
    return;
  }
  void *field = il2cpp_class_get_field_from_name(klass, name);
  if (!field) {
    __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,
                        "Can't find field %s in class %s!", name, clazz);
    return;
  }
  if (IsStaticField(field)) {
    il2cpp_field_static_get_value(field, output);
  } else {
    il2cpp_field_get_value(klass, field, output);
  }
}

void Il2CppAPI::SetFieldValue(const char *image, const char *namespaze,
                              const char *clazz, const char *name,
                              void *value) {
  if (!attached)
    return;
  void *img = GetImageByName(image);
  if (!img) {
    __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,
                        "Can't find image %s!", image);
    return;
  }
  void *klass = GetClassType(image, namespaze, clazz);
  if (!klass) {
    __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,
                        "Can't find class %s!", clazz);
    return;
  }
  void *field = il2cpp_class_get_field_from_name(klass, name);
  if (!field) {
    __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,
                        "Can't find field %s in class %s!", name, clazz);
    return;
  }
  if (IsStaticField(field)) {
    il2cpp_field_static_set_value(field, value);
  } else {
    il2cpp_field_set_value(klass, field, value);
  }
}
void *Il2CppAPI::GetMethodByName(const char *image, const char *namespaze,
                                 const char *clazz, const char *name,
                                 int argsCount) {
  if (!attached)
    return nullptr;
  void *img = GetImageByName(image);
  if (!img) {
    __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,
                        "Can't find image %s!", image);
    return 0;
  }
  void *klass = GetClassType(image, namespaze, clazz);
  if (!klass) {
    __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,
                        "Can't find method %s!", clazz, name);
    return 0;
  }
  void **method =
      (void **)il2cpp_class_get_method_from_name(klass, name, argsCount);
  if (!method) {
    __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,
                        "Can't find method %s in class %s!", name, clazz);
    return 0;
  }
  __android_log_print(ANDROID_LOG_DEBUG, IL2CPP_LOG_TAG, "%s - [%s] %s::%s: %p",
                      image, namespaze, clazz, name, *method);
  return *(void**)method;
}
void *Il2CppAPI::GetMethodByName(const char *image, const char *namespaze,
                                 const char *clazz, const char *name,
                                 char **args, int argsCount) {
  if (!attached)
    return nullptr;
  void *img = GetImageByName(image);
  if (!img) {
    __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,
                        "Can't find image %s!", image);
    return nullptr;
  }
  void *klass = GetClassType(image, namespaze, clazz);
  if (!klass) {
    __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,
                        "Can't find class %s for method %s!", clazz, name);
    return nullptr;
  }
  void *iter = 0;
  int score = 0;
  void **method = (void **)il2cpp_class_get_methods(klass, &iter);

  while (method) {
    const char *fname = il2cpp_method_get_name(method);
    if (strcmp(fname, name) == 0) {
      for (int i = 0; i < argsCount; i++) {
        void *arg = il2cpp_method_get_param(method, i);
        if (arg) {
          const char *tname = il2cpp_type_get_name(arg);
          if (strcmp(tname, args[i]) == 0) {
            score++;
          } else {
            __android_log_print(ANDROID_LOG_INFO, IL2CPP_LOG_TAG,
                                "Argument at index %d didn't matched requested "
                                "argument!\n\tRequested: %s\n\tActual: "
                                "%s\nnSkipping function...",
                                i, args[i], tname);
            score = 0;
            goto skip;
          }
        }
      }
    }
  skip:
    if (score == argsCount) {
      __android_log_print(ANDROID_LOG_DEBUG, IL2CPP_LOG_TAG,
                          "%s - [%s] %s::%s: %p", image, namespaze, clazz, name,
                          *method);
      return *method;
    }
    method = (void **)il2cpp_class_get_methods(klass, &iter);
  }
  __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,
                      "Cannot find function %s in class %s!", name, clazz);
  return nullptr;
}

size_t Il2CppAPI::GetFieldByName(const char *image, const char *namespaze,
                                 const char *clazz, const char *name) {
  if (!attached)
    return -1;
  void *img = GetImageByName(image);
  if (!img) {
    __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,
                        "Can't find image %s!", image);
    return -1;
  }
  void *klass = GetClassType(image, namespaze, clazz);
  if (!klass) {
    __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,
                        "Can't find field %s!", clazz, name);
    return -1;
  }
  void *field = il2cpp_class_get_field_from_name(klass, name);
  if (!field) {
    __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,
                        "Can't find field %s in class %s!", clazz, name);
    return -1;
  }
  auto result = il2cpp_field_get_offset(field);
  __android_log_print(ANDROID_LOG_DEBUG, IL2CPP_LOG_TAG, "%s - [%s] %s::%s: %p",
                      image, namespaze, clazz, name, (void *)result);
  return result;
}

int is_surrogate(UTF16 uc) { return (uc - 0xd800u) < 2048u; }

int is_high_surrogate(UTF16 uc) { return (uc & 0xfffffc00) == 0xd800; }

int is_low_surrogate(UTF16 uc) { return (uc & 0xfffffc00) == 0xdc00; }

UTF32 surrogate_to_utf32(UTF16 high, UTF16 low) {
  return (high << 10) + low - 0x35fdc00;
}

const char *utf16_to_utf8(const UTF16 *source, size_t len) {
  std::u16string s(source, source + len);
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
  const char *ret = convert.to_bytes(s).c_str();
  return ret;
}

const wchar_t *utf16_to_utf32(const UTF16 *source, size_t len) {
  auto output = new UTF32[len + 1];

  for (int i = 0; i < len; i++) {
    const UTF16 uc = source[i];
    if (!is_surrogate(uc)) {
      output[i] = uc;
    } else {
      if (is_high_surrogate(uc) && is_low_surrogate(source[i]))
        output[i] = surrogate_to_utf32(uc, source[i]);
      else
        output[i] = L'?';
    }
  }

  output[len] = L'\0';
  return output;
}

template <class T>
Il2CppArray<T> *Il2CppArray<T>::Create(void *type, uintptr_t length) {
  return *(Il2CppArray<T> *)il2cpp_array_new(type, length);
}

const char *Il2CppString::CString() {
  return utf16_to_utf8(&this->start_char, this->length);
}

const wchar_t *Il2CppString::WCString() {
  return utf16_to_utf32(&this->start_char, this->length);
}

Il2CppString *Il2CppString::Create(const char *s) {
  if (!il2cppAttached)
    return nullptr;
  return il2cpp_string_new(s);
}

Il2CppString *Il2CppString::Create(const wchar_t *s, int len) {
  if (!il2cppAttached)
    return nullptr;
  return il2cpp_string_new_utf16(s, len);
}

void *Il2CppAPI::GetClassType(const std::string &imageNamespaceClass) {
  std::string imageName;
  std::string nameSpace;
  std::string className;

  std::size_t pos1 = imageNamespaceClass.find(':');
  if (pos1 != std::string::npos) {
    imageName = imageNamespaceClass.substr(0, pos1).append(".dll");
    std::size_t pos2 = imageNamespaceClass.find(':', pos1 + 1);
    if (pos2 != std::string::npos) {
      nameSpace = imageNamespaceClass.substr(pos1 + 1, pos2 - pos1 - 1);
      className = imageNamespaceClass.substr(pos2 + 2);
    }
  }

  return GetClassType(imageName.c_str(), nameSpace.c_str(), className.c_str());
}

Il2CppTypes::Class *
Il2CppAPI::CreateClassInstance(const std::string &imageNamespaceClass) {
  std::string imageName;
  std::string nameSpace;
  std::string className;

  std::size_t pos1 = imageNamespaceClass.find(':');
  if (pos1 != std::string::npos) {
    imageName = imageNamespaceClass.substr(0, pos1).append(".dll");
    std::size_t pos2 = imageNamespaceClass.find(':', pos1 + 1);
    if (pos2 != std::string::npos) {
      nameSpace = imageNamespaceClass.substr(pos1 + 1, pos2 - pos1 - 1);
      className = imageNamespaceClass.substr(pos2 + 2);
    }
  }

  return CreateClassInstance(imageName.c_str(), nameSpace.c_str(),
                             className.c_str());
}

void Il2CppAPI::GetFieldValue(const std::string &imageNamespaceClass,
                              const char *fieldName, void *output) {
  std::string imageName;
  std::string nameSpace;
  std::string className;

  std::size_t pos1 = imageNamespaceClass.find(':');
  if (pos1 != std::string::npos) {
    imageName = imageNamespaceClass.substr(0, pos1).append(".dll");
    std::size_t pos2 = imageNamespaceClass.find(':', pos1 + 1);
    if (pos2 != std::string::npos) {
      nameSpace = imageNamespaceClass.substr(pos1 + 1, pos2 - pos1 - 1);
      className = imageNamespaceClass.substr(pos2 + 2);
    }
  }

  return GetFieldValue(imageName.c_str(), nameSpace.c_str(), className.c_str(),
                       fieldName, output);
}

void Il2CppAPI::SetFieldValue(const std::string &imageNamespaceClass,
                              const char *fieldName, void *value) {
  std::string imageName;
  std::string nameSpace;
  std::string className;

  std::size_t pos1 = imageNamespaceClass.find(':');
  if (pos1 != std::string::npos) {
    imageName = imageNamespaceClass.substr(0, pos1).append(".dll");
    std::size_t pos2 = imageNamespaceClass.find(':', pos1 + 1);
    if (pos2 != std::string::npos) {
      nameSpace = imageNamespaceClass.substr(pos1 + 1, pos2 - pos1 - 1);
      className = imageNamespaceClass.substr(pos2 + 2);
    }
  }

  return SetFieldValue(imageName.c_str(), nameSpace.c_str(), className.c_str(),
                       fieldName, value);
}

void *Il2CppAPI::GetMethodByName(const std::string &imageNamespaceClass,
                                 const char *methodName, int argsCount) {
  std::string imageName;
  std::string nameSpace;
  std::string className;

  std::size_t pos1 = imageNamespaceClass.find(':');
  if (pos1 != std::string::npos) {
    imageName = imageNamespaceClass.substr(0, pos1).append(".dll");
    std::size_t pos2 = imageNamespaceClass.find(':', pos1 + 1);
    if (pos2 != std::string::npos) {
      nameSpace = imageNamespaceClass.substr(pos1 + 1, pos2 - pos1 - 1);
      className = imageNamespaceClass.substr(pos2 + 2);
    }
  }

  return GetMethodByName(imageName.c_str(), nameSpace.c_str(),
                         className.c_str(), methodName, argsCount);
}

void *Il2CppAPI::GetMethodByName(const std::string &imageNamespaceClass,
                                 const char *methodName, char **args,
                                 int argsCount) {
  std::string imageName;
  std::string nameSpace;
  std::string className;

  std::size_t pos1 = imageNamespaceClass.find(':');
  if (pos1 != std::string::npos) {
    imageName = imageNamespaceClass.substr(0, pos1).append(".dll");
    std::size_t pos2 = imageNamespaceClass.find(':', pos1 + 1);
    if (pos2 != std::string::npos) {
      nameSpace = imageNamespaceClass.substr(pos1 + 1, pos2 - pos1 - 1);
      className = imageNamespaceClass.substr(pos2 + 2);
    }
  }

  return GetMethodByName(imageName.c_str(), nameSpace.c_str(),
                         className.c_str(), methodName, args, argsCount);
}

size_t Il2CppAPI::GetFieldByName(const std::string &imageNamespaceClass,
                                 const char *fieldName) {
  std::string imageName;
  std::string nameSpace;
  std::string className;

  std::size_t pos1 = imageNamespaceClass.find(':');
  if (pos1 != std::string::npos) {
    imageName = imageNamespaceClass.substr(0, pos1).append(".dll");
    std::size_t pos2 = imageNamespaceClass.find(':', pos1 + 1);
    if (pos2 != std::string::npos) {
      nameSpace = imageNamespaceClass.substr(pos1 + 1, pos2 - pos1 - 1);
      className = imageNamespaceClass.substr(pos2 + 2);
    }
  }

  return GetFieldByName(imageName.c_str(), nameSpace.c_str(), className.c_str(),
                        fieldName);
}

void *Il2CppAPI::UnityEngine::GetTransform(void *object) {
    if (!object || !attached) return nullptr;
    void *ptrGetTF = Il2CppAPI::GetMethodByName("UnityEngine.CoreModule:UnityEngine::Component", "get_transform", 0);
    if (!ptrGetTF) return nullptr;
    static const auto get_transform = reinterpret_cast<uintptr_t(__fastcall *)(void *)>(*(void**)ptrGetTF);
    return (void*)get_transform(object);
}

Vector3 Il2CppAPI::UnityEngine::GetPossition(void *transform) {
    if (!transform || !attached) return Vector3();
    void *ptrGetPos = Il2CppAPI::GetMethodByName("UnityEngine.CoreModule:UnityEngine::Transform", "get_position_Injected", 1);
    if (!ptrGetPos) return Vector3();
    Vector3 retPos;
    static const auto get_position = reinterpret_cast<uintptr_t(__fastcall *)(void *,Vector3 &)>(*(void**)ptrGetPos);
    get_position(transform, retPos);
    return retPos;
}

Vector3 Il2CppAPI::UnityEngine::WorldToScreenPoint(void *cam, Vector3 pos, MonoOrStereoscopicEye eye) {
    if (!cam || !attached) return Vector3();
    void *ptrWorld2ScreenPnt = Il2CppAPI::GetMethodByName("UnityEngine.CoreModule:UnityEngine::Camera", "WorldToScreenPoint_Injected", 3);
    if (!ptrWorld2ScreenPnt) return Vector3();
    Vector3 retPos;
    static const auto WorldToScreenPoint = reinterpret_cast<uintptr_t(__fastcall *)(void *,Vector3, MonoOrStereoscopicEye, Vector3 &)>(*(void**)ptrWorld2ScreenPnt);
    WorldToScreenPoint(cam, pos, eye, retPos);
    return retPos;
}

Vector3 Il2CppAPI::UnityEngine::ScreenToWorldPoint(void *cam, Vector3 pos, MonoOrStereoscopicEye eye) {
    if (!cam || !attached) return Vector3();
    void *ptrScreen2WorldPnt = Il2CppAPI::GetMethodByName("UnityEngine.CoreModule:UnityEngine::Camera", "ScreenToWorldPoint_Injected", 3);
    if (!ptrScreen2WorldPnt) return Vector3();
    Vector3 retPos;
    static const auto ScreenToWorldPoint = reinterpret_cast<uintptr_t(__fastcall *)(void *,Vector3, MonoOrStereoscopicEye, Vector3 &)>(*(void**)ptrScreen2WorldPnt);
    ScreenToWorldPoint (cam, pos, eye, retPos);
    return retPos;
}

Vector3 Il2CppAPI::UnityEngine::ScreenToViewportPoint(void *cam, Vector3 pos) {
    if (!cam || !attached) return Vector3();
    void *ptrScreenToViewportPnt = Il2CppAPI::GetMethodByName("UnityEngine.CoreModule:UnityEngine::Camera", "ScreenToViewportPoint_Injected", 2);
    if (!ptrScreenToViewportPnt) return Vector3();
    Vector3 retPos;
    static const auto ScreenToViewportPoint = reinterpret_cast<uintptr_t(__fastcall *)(void *,Vector3, Vector3 &)>(*(void**)ptrScreenToViewportPnt);
    ScreenToViewportPoint(cam, pos, retPos);
    return retPos;

}

void *Il2CppAPI::UnityEngine::GetCamera() {
    if (!attached) return nullptr;
    void *ptrGet_main = Il2CppAPI::GetMethodByName("UnityEngine.CoreModule:UnityEngine::Camera", "get_main", 0); 
    static const auto get_cam_main = reinterpret_cast<uintptr_t(__fastcall *)()>(*(void**)ptrGet_main);
    return (void*)get_cam_main();
}
