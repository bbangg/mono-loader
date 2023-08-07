#pragma once
#include <Windows.h>
#include <minwindef.h>
#include <iostream>

#include <mono/metadata/metadata.h>
#include <mono/metadata/class.h>

typedef MonoDomain* (__cdecl* mono_jit_init_t)(const char* file);
typedef int(__cdecl* mono_jit_exec_t)(MonoDomain* domain, MonoAssembly* assembly, int argc, char* argv[]);
typedef MonoDomain* (__cdecl* mono_get_root_domain_t)();
typedef MonoDomain* (__cdecl* mono_domain_get_t)();

typedef void* (__cdecl* mono_thread_attach_t)(MonoDomain* domain);
typedef void* (__cdecl* mono_thread_detach_t)(MonoDomain* domain);

typedef MonoAssembly* (__cdecl* mono_assembly_open_t)(const char* filename, MonoImageOpenStatus* status);
typedef MonoAssembly* (__cdecl* mono_assembly_load_from_full_t)(MonoImage* image, const char* fname, MonoImageOpenStatus* status, mono_bool refonly);
typedef MonoImage* (__cdecl* mono_assembly_get_image_t)(MonoAssembly* assembly);
typedef MonoImage* (__cdecl* mono_image_open_from_data_full_t)(uint8_t* data, uint32_t data_len, mono_bool need_copy, MonoImageOpenStatus* status, mono_bool refonly);
typedef MonoClass* (__cdecl* mono_class_from_name_t)(MonoImage* image, const char* name_space, const char* name);
typedef MonoMethod* (__cdecl* mono_class_get_method_from_name_t)(MonoClass* mclass, const char* name, int param_count);
typedef MonoObject* (__cdecl* mono_runtime_invoke_t)(MonoMethod* method, void* obj, void** params, MonoObject** exc);
typedef MonoProperty* (__cdecl* mono_class_get_property_from_name_t)(MonoClass* mclass, const char* name);
typedef MonoMethod* (__cdecl* mono_property_get_set_method_t)(MonoProperty* prop);
typedef MonoMethod* (__cdecl* mono_property_get_get_method_t)(MonoProperty* prop);

typedef void(__cdecl* mono_security_set_mode_t)(MonoSecurityMode Mode);

class Mono {
private:
    bool initialized = false;
    HMODULE hMono;
    mono_jit_init_t mono_jit_init_;
    mono_jit_exec_t mono_jit_exec_;
    mono_get_root_domain_t mono_get_root_domain_;
    mono_domain_get_t mono_domain_get_;
    mono_thread_attach_t mono_thread_attach_;
    mono_thread_detach_t mono_thread_detach_;
    mono_assembly_open_t mono_assembly_open_;
    mono_assembly_load_from_full_t mono_assembly_load_from_full_;
    mono_assembly_get_image_t mono_assembly_get_image_;
    mono_image_open_from_data_full_t mono_image_open_from_data_full_;
    mono_class_from_name_t mono_class_from_name_;
    mono_class_get_method_from_name_t mono_class_get_method_from_name_;
    mono_runtime_invoke_t mono_runtime_invoke_;
    mono_class_get_property_from_name_t mono_class_get_property_from_name_;
    mono_property_get_set_method_t mono_property_get_set_method_;
    mono_property_get_get_method_t mono_property_get_get_method_;
    mono_security_set_mode_t mono_security_set_mode_;

public:
    static Mono& instance() {
        static Mono _instance;

        if (!_instance.initialized) {
            _instance.Initialize();
        }

        return _instance;
    }

    MonoDomain* Initialize() {
        if (!initialized) {
            hMono = GetModuleHandle("mono.dll");
            if (hMono == NULL) return nullptr;

            mono_jit_init_ = reinterpret_cast<mono_jit_init_t>(GetProcAddress(hMono, "mono_jit_init"));
            mono_jit_exec_ = reinterpret_cast<mono_jit_exec_t>(GetProcAddress(hMono, "mono_jit_exec"));
            mono_get_root_domain_ = reinterpret_cast<mono_get_root_domain_t>(GetProcAddress(hMono, "mono_get_root_domain"));
            mono_domain_get_ = reinterpret_cast<mono_domain_get_t>(GetProcAddress(hMono, "mono_domain_get"));
            mono_thread_attach_ = reinterpret_cast<mono_thread_attach_t>(GetProcAddress(hMono, "mono_thread_attach"));
            mono_thread_detach_ = reinterpret_cast<mono_thread_detach_t>(GetProcAddress(hMono, "mono_thread_detach"));
            mono_assembly_open_ = reinterpret_cast<mono_assembly_open_t>(GetProcAddress(hMono, "mono_assembly_open"));
            mono_assembly_load_from_full_ = reinterpret_cast<mono_assembly_load_from_full_t>(GetProcAddress(hMono, "mono_assembly_load_from_full"));
            mono_assembly_get_image_ = reinterpret_cast<mono_assembly_get_image_t>(GetProcAddress(hMono, "mono_assembly_get_image"));
            mono_image_open_from_data_full_ = reinterpret_cast<mono_image_open_from_data_full_t>(GetProcAddress(hMono, "mono_image_open_from_data_full"));
            mono_class_from_name_ = reinterpret_cast<mono_class_from_name_t>(GetProcAddress(hMono, "mono_class_from_name"));
            mono_class_get_method_from_name_ = reinterpret_cast<mono_class_get_method_from_name_t>(GetProcAddress(hMono, "mono_class_get_method_from_name"));
            mono_runtime_invoke_ = reinterpret_cast<mono_runtime_invoke_t>(GetProcAddress(hMono, "mono_runtime_invoke"));
            mono_class_get_property_from_name_ = reinterpret_cast<mono_class_get_property_from_name_t>(GetProcAddress(hMono, "mono_class_get_property_from_name"));
            mono_property_get_set_method_ = reinterpret_cast<mono_property_get_set_method_t>(GetProcAddress(hMono, "mono_property_get_set_method"));
            mono_property_get_get_method_ = reinterpret_cast<mono_property_get_get_method_t>(GetProcAddress(hMono, "mono_property_get_get_method"));
            mono_security_set_mode_ = reinterpret_cast<mono_security_set_mode_t>(GetProcAddress(hMono, "mono_security_set_mode"));

            initialized = true;
        }
    }

    void mono_jit_exec(MonoDomain* domain, MonoAssembly* assembly, int argc, char* argv[]) {
        mono_jit_exec_(domain, assembly, argc, argv);
    }

    MonoDomain* get_root_domain() {
        return mono_get_root_domain_();
    }

    void mono_set_security_mode(MonoSecurityMode mode) {
        mono_security_set_mode_(mode);
    }

    MonoDomain* mono_domain_get() {
        return mono_domain_get_();
    }

    void* mono_thread_attach(MonoDomain* domain) {
        return mono_thread_attach_(domain);
    }

    void* mono_thread_detach(MonoDomain* domain) {
        return mono_thread_detach_(domain);
    }

    MonoAssembly* mono_assembly_open(const char* filename, MonoImageOpenStatus* status) {
        return mono_assembly_open_(filename, status);
    }

    MonoAssembly* mono_assembly_load_from_full(MonoImage* image, const char* fname, MonoImageOpenStatus* status, mono_bool refonly) {
        return mono_assembly_load_from_full_(image, fname, status, refonly);
    }

    MonoImage* mono_assembly_get_image(MonoAssembly* assembly) {
        return mono_assembly_get_image_(assembly);
    }

    MonoImage* mono_image_open_from_data_full(uint8_t* data, uint32_t data_len, mono_bool need_copy, MonoImageOpenStatus* status, mono_bool refonly) {
        return mono_image_open_from_data_full_(data, data_len, need_copy, status, refonly);
    }

    MonoClass* mono_class_from_name(MonoImage* image, const char* name_space, const char* name) {
        return mono_class_from_name_(image, name_space, name);
    }

    MonoMethod* mono_class_get_method_from_name(MonoClass* mclass, const char* name, int param_count) {
        return mono_class_get_method_from_name_(mclass, name, param_count);
    }

    MonoObject* mono_runtime_invoke(MonoMethod* method, void* obj, void** params, MonoObject** exc) {
        return mono_runtime_invoke_(method, obj, params, exc);
    }

    MonoProperty* mono_class_get_property_from_name(MonoClass* mclass, const char* name) {
        return mono_class_get_property_from_name_(mclass, name);
    }

    MonoMethod* mono_property_get_set_method(MonoProperty* prop) {
        return mono_property_get_set_method_(prop);
    }

    MonoMethod* mono_property_get_get_method(MonoProperty* prop) {
        return mono_property_get_get_method_(prop);
    }
};
