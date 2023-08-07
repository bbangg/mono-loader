#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <codecvt>
#include <unordered_map>
#include <TlHelp32.h>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <thread>
#include <mono/jit/jit.h>

#include "raw_data.h"
#include "mono.h"

VOID Inject() {

	Mono mono = Mono::instance();

	MonoDomain* root_domain = mono.get_root_domain();
	mono.mono_set_security_mode(MONO_SECURITY_MODE_NONE);
	mono.mono_thread_attach(root_domain);

	MonoImageOpenStatus Status;
	MonoImage* raw_image = mono.mono_image_open_from_data_full(raw_data, sizeof(raw_data), 1, &Status, 0);
	if (!raw_image)
	{
		printf("raw_image is null\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}

	MonoImageOpenStatus Status1;
	MonoAssembly* assembly = mono.mono_assembly_load_from_full(raw_image, "not_required", &Status1, 0);
	if (!assembly) {
		printf("assembly is null\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}

	MonoImage* image = mono.mono_assembly_get_image(assembly);
	if (!image) {
		printf("image is null\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}

	MonoClass* image_class = mono.mono_class_from_name(image, (PCHAR)"HurtworldMono", (PCHAR)"Loader");
	if (!image_class) {
		printf("image_class is null\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}


	MonoMethod* image_method = mono.mono_class_get_method_from_name(image_class, (PCHAR)"Init", 0);
	if (!image_method) {
		printf("image_method is null\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(1500));
	mono.mono_runtime_invoke(image_method, NULL, NULL, NULL);
}

BOOL WINAPI DllMain(HMODULE hDll, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH) {
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		Inject();
	}
	return true;
}