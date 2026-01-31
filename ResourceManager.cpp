#pragma hdrstop
#include "ResourceManager.h"
#include <vcl.h>
#include <memory>

ResourceManager::ResourceManager() {}
ResourceManager::~ResourceManager() {}

TPngImage* ResourceManager::GetCardImage(UnicodeString imgName) {
    // 1. Incarcam DLL-ul
    HINSTANCE hLocalDLL = LoadLibrary(L"Library.dll");

    if (!hLocalDLL) {
        ShowMessage("CRITIC: Nu gasesc 'Library.dll'!");
        return nullptr;
    }

    TPngImage* png = nullptr;
    HRSRC hRes = nullptr;


    hRes = FindResource(hLocalDLL, imgName.c_str(), RT_RCDATA);


    if (!hRes) {
        hRes = FindResource(hLocalDLL, imgName.c_str(), L"PNG");
    }


    if (!hRes) {

        FreeLibrary(hLocalDLL);
        return nullptr;
    }

    // Daca am gasit resursa, o incarcam
    HGLOBAL hResData = LoadResource(hLocalDLL, hRes);
    if (hResData)
    {
        void* pData = LockResource(hResData);
        DWORD size = SizeofResource(hLocalDLL, hRes);

        if (pData && size > 0)
        {
            TMemoryStream* ms = new TMemoryStream();
            try
            {
                ms->WriteBuffer(pData, size);
                ms->Position = 0; // CRITIC: Resetam pozitia la inceput!

                png = new TPngImage();
                png->LoadFromStream(ms);
            }
            catch (Exception &e)
            {
                // ShowMessage("Eroare la conversia PNG: " + e.Message);
                delete png;
                png = nullptr;
            }
            delete ms;
        }
    }

    FreeLibrary(hLocalDLL);
    return png;
}

TPngImage* ResourceManager::GetBackImage() {
    // Incearca sa incarce spatele.
    // Daca in DLL se numeste altfel (ex: "back"), modifica aici string-ul.
	TPngImage* img = GetCardImage("Backcard");

	return img;
}

void ResourceManager::LoadBackground(TImage* targetImage) {
    if (!targetImage) return;

    HINSTANCE hLocalDLL = LoadLibrary(L"Library.dll");
    if (!hLocalDLL) return;

    typedef void (__stdcall *TGetDllBackground)(TBitmap*);
    TGetDllBackground GetImg = (TGetDllBackground)GetProcAddress(hLocalDLL, "GetDllBackground");

    if (GetImg) {
        TBitmap *bmp = new TBitmap();
        try {
            GetImg(bmp);
            targetImage->Picture->Assign(bmp);
        }
        __finally {
            delete bmp;
        }
    }
    FreeLibrary(hLocalDLL);
}
