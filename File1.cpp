#include <vcl.h>
#include <windows.h>
#include <memory>
#include <System.Types.hpp>

#pragma hdrstop

// NU adăuga cp3000.lib aici.
#pragma link "rtl.lib"
#pragma link "vcl.lib"
#pragma link "vclimg.lib"



#pragma resource "Win64x\\Release\\New1.res" // Atenție la Release vs Debug

// Punct de intrare obligatoriu pentru VCL pe 64-bit
extern "C" int _libmain(unsigned long reason) {
    return 1;
}

int WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    return 1;
}

extern "C" __declspec(dllexport) void __stdcall GetDllBackground(TBitmap* DestBitmap)
{
    if (!DestBitmap) return;
    try
    {
        // Folosim HInstance-ul global al DLL-ului
        std::unique_ptr<TResourceStream> resStream(
           new TResourceStream((NativeUInt)HInstance, L"Background", (System::WideChar*)RT_RCDATA)
        );
        DestBitmap->LoadFromStream(resStream.get());
    }
    catch (...) { }
}
