// ResourceManager.h
#ifndef ResourceManagerH
#define ResourceManagerH

// --- INCLUDERI OBLIGATORII PENTRU VCL ---
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.ExtCtrls.hpp> // <--- Aici este definit TImage
#include <pngimage.hpp>

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    // Functiile care copiaza logica ta veche 1:1
    TPngImage* GetCardImage(UnicodeString imgName);
    TPngImage* GetBackImage();

    // Functie pentru fundal
    void LoadBackground(TImage* targetImage);
};

#endif
