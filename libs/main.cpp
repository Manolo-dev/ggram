#include <iostream>
#include <dlfcn.h>

int main() {
    // Charger dynamiquement la bibliothèque partagée
    void* libraryHandle = dlopen("./lib.so", RTLD_LAZY);

    if (libraryHandle == nullptr) {
        std::cerr << "Impossible de charger la bibliothèque partagée : " << dlerror() << std::endl;
        return 1;
    }

    // Récupérer le pointeur de la fonction souhaitée depuis la bibliothèque
    using MyFunctionType = void(*)();
    MyFunctionType myFunction = reinterpret_cast<MyFunctionType>(dlsym(libraryHandle, "print"));

    if (myFunction == nullptr) {
        std::cerr << "Impossible de récupérer la fonction depuis la bibliothèque : " << dlerror() << std::endl;
        dlclose(libraryHandle);
        return 1;
    }

    // Appeler la fonction depuis la bibliothèque
    myFunction();

    // Fermer la bibliothèque
    dlclose(libraryHandle);

    return 0;
}
