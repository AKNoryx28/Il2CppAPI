#ifndef IL2CPP_API_H
#define IL2CPP_API_H

#include <cstddef>
#include <cstdint>
#include <jni.h>
#include <inttypes.h>
#include <string>

#define IL2CPP_LOG_TAG "IL2CPP_LOG"

template<typename T> struct Il2CppArray {
    void *klass;
    void *monitor;
    void *bounds;
    int max_length;
    T m_Items[0xFFFF];

    // Untested, may not work
    static Il2CppArray<T> *Create(void *type, uintptr_t length);

    int getLength() {
        return max_length;
    }

    T *getPointer() {
        return (T *)m_Items;
    }

    T &operator[](int i) {
        return m_Items[i];
    }

    T &operator[](int i) const {
        return m_Items[i];
    }
};

struct Il2CppString {
    void *klass;
    void *monitor;
    int32_t length;
    uint16_t start_char;

    const char *CString();

    const wchar_t *WCString();

    static Il2CppString *Create(const char *s);
    static Il2CppString *Create(const wchar_t *s, int len);

    int getLength() {
        return length;
    }
};

template<typename T> struct Il2CppList {
    void *klass;
    void *unk1;
    Il2CppArray<T> *items;
    int size;
    int version;

    T *getItems() {
        return items->getPointer();
    }

    int getSize() {
        return size;
    }

    int getVersion() {
        return version;
    }

    T &operator[](int i) {
        return items->m_Items[i];
    }

    T &operator[](int i) const {
        return items->m_Items[i];
    }
};

template<typename K, typename V> struct Il2CppDictionary {
    void *klass;
    void *unk1;
    Il2CppArray<int **> *table;
    Il2CppArray<void **> *linkSlots;
    Il2CppArray<K> *keys;
    Il2CppArray<V> *values;
    int touchedSlots;
    int emptySlot;
    int size;

    K *getKeys() {
        return keys->getPointer();
    }

    V *getValues() {
        return values->getPointer();
    }

    int getNumKeys() {
        return keys->getLength();
    }

    int getNumValues() {
        return values->getLength();
    }

    int getSize() {
        return size;
    }
};

struct Il2CppClass;
struct Il2CppObject
{
    Il2CppClass *klass;
    void *monitor;
};

namespace Il2CppTypes {
typedef Il2CppClass Class;
typedef Il2CppObject Object;
typedef Il2CppString String;
template<typename T>
using Array = Il2CppArray<T>;
template<typename T>
using List = Il2CppList<T>;
template<typename K, typename V>
using Dictionary = Il2CppDictionary<K, V>;
}

namespace Il2CppAPI {
    void InitAPI(const char *lib_name="libil2cpp.so");
    void InitAPI(JNIEnv *env, const char *lib_name="libil2cpp.so");

    void *GetLibHandle();
    uintptr_t GetAbsolouteAddress();
    const char *GetAbsoloutePath();

    // IMPORTANT Call this before use any functions even in Il2CppTypes Struct functions
    void Attach();
    void Detach();
    bool IsAttached();

    void *GetImageByName(const char *image);
	void *GetClassType(const char *image, const char *namespaze, const char *clazz);
	Il2CppTypes::Class *CreateClassInstance(const char *image, const char *namespaze, const char *clazz);
	void GetFieldValue(const char *image, const char *namespaze, const char *clazz, const char *name, void *output);
	void SetFieldValue(const char *image, const char *namespaze, const char *clazz, const char *name, void* value);
    void *GetMethodByName(const char *image, const char *namespaze, const char *clazz, const char *name, int argsCount);
    void *GetMethodByName(const char *image, const char *namespaze, const char *clazz, const char *name, char **args, int argsCount);
	size_t GetFieldByName(const char *image, const char *namespaze, const char *clazz, const char *name);

    /**
     * @brief Get the Class Type object
     * 
     * @param imageNamespaceClass format: "ImageName:NamespaceName::ClassName"
     * @return void* 
     */
	void *GetClassType(const std::string &imageNamespaceClass);
    /**
     * @brief Create a Class Instance object
     * 
     * @param imageNamespaceClass format: "ImageName:NamespaceName::ClassName"
     * @return Il2CppTypes::Class* 
     */
	Il2CppTypes::Class *CreateClassInstance(const std::string &imageNamespaceClass);
    /**
     * @brief Get the Field Value object, static/dynamic
     * 
     * @param imageNamespaceClass format: "ImageName:NamespaceName::ClassName"
     * @param fieldName 
     * @param output 
     */
	void GetFieldValue(const std::string &imageNamespaceClass, const char *fieldName, void *output);
    /**
     * @brief Set the Field Value object, static/dynamic
     * 
     * @param imageNamespaceClass format: "ImageName:NamespaceName::ClassName"
     * @param fieldName 
     * @param value 
     */
	void SetFieldValue(const std::string &imageNamespaceClass, const char *fieldName, void* value);
    /**
     * @brief Get the Method By Name object, static/dynamic
     * 
     * @param imageNamespaceClass format: "ImageName:NamespaceName::ClassName"
     * @param methodName 
     * @param argsCount 
     * @return Il2CppTypes::Method* 
     */
    void *GetMethodByName(const std::string &imageNamespaceClass, const char *methodName, int argsCount);
    /**
     * @brief Get the Method By Name object, static/dynamic
     * 
     * @param imageNamespaceClass format: "ImageName:NamespaceName::ClassName"
     * @param methodName
     * @param args
     * @param argsCount 
     * @return Il2CppTypes::Method* 
     */
    void *GetMethodByName(const std::string &imageNamespaceClass, const char *methodName, char **args, int argsCount);
    /**
     * @brief Get the Field By Name object, static/dynamic
     * 
     * @param imageNamespaceClass format: "ImageName:NamespaceName::ClassName"
     * @param fieldName 
     * @return size_t 
     */
	size_t GetFieldByName(const std::string &imageNamespaceClass, const char *fieldName);
};

#endif //IL2CPP_API_H
