#include <cstdlib>
#include <cstring>

// Shaders.dll importa servicios del motor aunque la lectura de descriptores y
// ensamblador no los ejecuta. Este host implementa únicamente la ABI mínima
// necesaria para cargar la DLL sin inicializar el cliente ni Direct3D.
extern "C" void* __cdecl EngineStub()
{
	return nullptr;
}

extern "C" void* __cdecl CTStringCtor(void* self)
{
	if (self != nullptr)
		*static_cast<char**>(self) = nullptr;
	return self;
}

extern "C" void __cdecl CTStringDtor(void* self)
{
	if (self == nullptr)
		return;

	char*& value = *static_cast<char**>(self);
	std::free(value);
	value = nullptr;
}

extern "C" char* __cdecl StringDuplicateStub(const char* value)
{
	if (value == nullptr || value[0] == '\0')
		return nullptr;

	const size_t length = std::strlen(value);
	char* copy = static_cast<char*>(std::malloc(length + 1));
	if (copy != nullptr)
		std::memcpy(copy, value, length + 1);
	return copy;
}

extern "C" void __cdecl StringFreeStub(char* value)
{
	std::free(value);
}

extern "C" void* GfxPointer = nullptr;
