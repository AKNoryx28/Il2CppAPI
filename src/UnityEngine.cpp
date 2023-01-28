#include "Unity/Quaternion.h"
#include "Unity/Vector3.h"
#include <Il2CppAPI.h>
#include <map>
#include <android/log.h>
#include <string>
#include <obfuscate.h>
#include <unistd.h>

#if defined(__arm__)
#define CAST_FUNC(ret, args, void_ptr) reinterpret_cast<ret(__fastcall *) args>(void_ptr)
#elif defined(__aarch64__)
#define CAST_FUNC(ret, args, void_ptr) reinterpret_cast<ret(__fastcall *) args>(void_ptr)
#elif defined(__i386__) || defined(__x86_64__)
#define CAST_FUNC(ret, args, void_ptr) reinterpret_cast<ret(__fastcall *) args>(void_ptr)
#endif

namespace Il2CppAPI {
namespace UnityEngine {
    static std::map<const char *, void *> _Methods;
    static bool valid;

    void Init() {
        valid = false;
        if (!Il2CppAPI::IsAttached())
            Il2CppAPI::Attach();
        sleep(2);

        std::string imageNamespace(AY_OBFUSCATE("UnityEngine:UnityEngine::"));

        _Methods["Component::get_transform"] = Il2CppAPI::GetMethodByName(imageNamespace + "Component", "get_transform", 0);
        _Methods["Component::get_gameObject"] = Il2CppAPI::GetMethodByName(imageNamespace + "Component", "get_gameObject", 0);
        _Methods["Component::GetComponent"] = Il2CppAPI::GetMethodByName(imageNamespace + "Component", "GetComponent", 1);

        _Methods["Transform::get_position"] = Il2CppAPI::GetMethodByName(imageNamespace + "Transform", "get_position", 0);
        _Methods["Transform::set_position"] = Il2CppAPI::GetMethodByName(imageNamespace + "Transform", "set_position", 1);
        _Methods["Transform::get_rotation"] = Il2CppAPI::GetMethodByName(imageNamespace + "Transform", "get_rotation", 0);
        _Methods["Transform::set_rotation"] = Il2CppAPI::GetMethodByName(imageNamespace + "Transform", "set_rotation", 1);
        _Methods["Transform::get_localScale"] = Il2CppAPI::GetMethodByName(imageNamespace + "Transform", "get_localScale", 0);
        _Methods["Transform::set_localScale"] = Il2CppAPI::GetMethodByName(imageNamespace + "Transform", "set_localScale", 1);

        _Methods["Camera::get_main"] = Il2CppAPI::GetMethodByName(imageNamespace + "Camera", "get_main", 0);
        _Methods["Camera::get_fieldOfView"] = Il2CppAPI::GetMethodByName(imageNamespace + "Camera", "get_fieldOfView", 0);
        _Methods["Camera::set_fieldOfView"] = Il2CppAPI::GetMethodByName(imageNamespace + "Camera", "set_fieldOfView", 1);
        _Methods["Camera::WorldToScreenPoint"] = Il2CppAPI::GetMethodByName(imageNamespace + "Camera", "WorldToScreenPoint", 1);
        _Methods["Camera::WorldToViewportPoint"] = Il2CppAPI::GetMethodByName(imageNamespace + "Camera", "WorldToViewportPoint", 1);
        _Methods["Camera::ViewportToWorldPoint"] = Il2CppAPI::GetMethodByName(imageNamespace + "Camera", "ViewportToWorldPoint", 1);
        _Methods["Camera::ScreenToWorldPoint"] = Il2CppAPI::GetMethodByName(imageNamespace + "Camera", "ScreenToWorldPoint", 1);
        _Methods["Camera::ScreenToViewportPoint"] = Il2CppAPI::GetMethodByName(imageNamespace + "Camera", "ScreenToViewportPoint", 1);
        _Methods["Camera::ViewportToScreenPoint"] = Il2CppAPI::GetMethodByName(imageNamespace + "Camera", "ViewportToScreenPoint", 1);

        _Methods["Screen::SetResolution"] = Il2CppAPI::GetMethodByName(imageNamespace + "Screen", "SetResolution", 3);
        _Methods["Screen::get_width"] = Il2CppAPI::GetMethodByName(imageNamespace + "Screen", "get_width", 0);
        _Methods["Screen::get_height"] = Il2CppAPI::GetMethodByName(imageNamespace + "Screen", "get_height", 0);
        _Methods["Screen::get_dpi"] = Il2CppAPI::GetMethodByName(imageNamespace + "Screen", "get_dpi", 0);

        Il2CppAPI::Detach();
        for (auto const &[a, b] : _Methods) {
            if (b == nullptr) {
                __android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG,"UnityEngine: NULL at %s", a);
                valid = false;
                break;
            }
        }
        valid = true;
    }
    bool isValid() {return valid;}

    Transform *Component::get_transform() {
        auto _ = CAST_FUNC(Transform*,(Component*),_Methods["Component::get_transform"]);
        return _(this);
    }
    void *Component::get_gameObject() {
        auto _ = CAST_FUNC(void*,(Component*),_Methods["Component::get_gameObject"]);
        return _(this);
    }
    Component *Component::GetComponent(void *type) {
        auto _ = CAST_FUNC(Component*,(Component*,void*),_Methods["Component::GetComponent"]);
        return _(this,type);
    }

    Vector3 Transform::get_position() {
        auto _ = CAST_FUNC(Vector3,(Transform*),_Methods["Transform::get_position"]);
        return _(this);
    }
    void Transform::set_position(Vector3 value) {
        auto _ = CAST_FUNC(void,(Transform*,Vector3),_Methods["Transform::set_position"]);
        return _(this,value);
    }
    Quaternion Transform::get_rotation() {
        auto _ = CAST_FUNC(Quaternion,(Transform*),_Methods["Transform::get_rotation"]);
        return _(this);
    }
    void Transform::set_rotation(Quaternion value) {
        auto _ = CAST_FUNC(void,(Transform*,Quaternion),_Methods["Transform::set_rotation"]);
        return _(this,value);
    }
    Vector3 Transform::get_localScale() {
        auto _ = CAST_FUNC(Vector3,(Transform*),_Methods["Transform::get_localScale"]);
        return _(this);
    }
    void Transform::set_localScale(Vector3 value) {
        auto _ = CAST_FUNC(void,(Transform*,Vector3),_Methods["Transform::set_localScale"]);
        return _(this,value);
    }

    Camera *Camera::get_main() {
        auto _ = CAST_FUNC(Camera*,(),_Methods["Camera::get_main"]);
        return _();
    }
    float Camera::get_fieldOfView() {
        auto _ = CAST_FUNC(float,(Camera*),_Methods["Camera::get_fieldOfView"]);
        return _(this);
    }
    void Camera::set_fieldOfView(float value) {
        auto _ = CAST_FUNC(void,(Camera*,float),_Methods["Camera::set_fieldOfView"]);
        return _(this,value);
    }
    Vector3 Camera::WorldToScreenPoint(Vector3 value) {
        auto _ = CAST_FUNC(Vector3,(Camera*,Vector3),_Methods["Camera::WorldToScreenPoint"]);
        return _(this,value);
    }
    Vector3 Camera::WorldToViewportPoint(Vector3 value) {
        auto _ = CAST_FUNC(Vector3,(Camera*,Vector3),_Methods["Camera::WorldToViewportPoint"]);
        return _(this, value);
    }
    Vector3 Camera::ViewportToWorldPoint(Vector3 value) {
        auto _ = CAST_FUNC(Vector3,(Camera*,Vector3),_Methods["Camera::ViewportToWorldPoint"]);
        return _(this, value);
    }
    Vector3 Camera::ScreenToWorldPoint(Vector3 value) {
        auto _ = CAST_FUNC(Vector3,(Camera*,Vector3),_Methods["Camera::ScreenToWorldPoint"]);
        return _(this, value);
    }
    Vector3 Camera::ScreenToViewportPoint(Vector3 value) {
        auto _ = CAST_FUNC(Vector3,(Camera*,Vector3),_Methods["Camera::ScreenToViewportPoint"]);
        return _(this, value);
    }
    Vector3 Camera::ViewportToScreenPoint(Vector3 value) {
        auto _ = CAST_FUNC(Vector3,(Camera*,Vector3),_Methods["Camera::ViewportToScreenPoint"]);
        return _(this, value);
    }

    int Screen::get_width() {
        auto _ = CAST_FUNC(int,(),_Methods["Screen::get_width"]);
        return _();
    }
    int Screen::get_height() {
        auto _ = CAST_FUNC(int,(),_Methods["Screen::get_height"]);
        return _();
    }
    float Screen::get_dpi() {
        auto _ = CAST_FUNC(float,(),_Methods["Screen::get_dpi"]);
        return _();
    }
    void Screen::SetResolution(int width, int height, bool fullscreen) {
        auto _ = CAST_FUNC(void,(int,int,bool),_Methods["Screen::SetResolution"]);
        return _(width,height,fullscreen);
    }

}
}
