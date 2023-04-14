#include <irrlicht.h>
#include <iostream>
#include "sim.h"
#include <sstream>
#include <string>


using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif


int main() {
    IrrlichtDevice* device =
        createDevice(video::EDT_DIRECT3D9, dimension2d<u32>(640, 480), 16,
            false, false, false, 0);

    if (!device) {
        std::cerr << "Could not initialize Irrlicht Device" << std::endl;
        return 1;
    }

    device->setWindowCaption(L"Aerodinamical simulator");

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();

    IGUIStaticText* textX = guienv->addStaticText(L"x", rect<s32>(10, 10, 260, 22), true);
    IGUIStaticText* textY = guienv->addStaticText(L"y", rect<s32>(10, 30, 260, 42), true);
    IGUIStaticText* textZ = guienv->addStaticText(L"z", rect<s32>(10, 50, 260, 62), true);

    IGUIStaticText* textTime = guienv->addStaticText(L"t", rect<s32>(10, 70, 260, 82), true);


    IAnimatedMesh* mesh = smgr->getMesh("../../assets/lowpoly-airplane/airplane.obj");
    if (!mesh)
    {
        device->drop();
        return 1;
    }

    IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);

    if (node) {
        node->setMaterialFlag(EMF_LIGHTING, false);

        ITexture* texture = driver->getTexture("../../assets/lowpoly-airplane/textures/diffuse.png");

        if (!texture) {
            std::cout << "Could not load texture!" << std::endl;
        }

        node->setMaterialTexture(0, texture);
    }
    

    smgr->addCameraSceneNode(0, vector3df(- 80, 50, -90), vector3df(0, 5, 0));

    /////////////////////////////////////
    // Initial conditions for simulator
    /////////////////////////////////////


    const int m = 12;
    //double Pn, Pe, Pd, u, v, w, phi, theeta, psi, dphi, dtheeta, dpsi, u0 = 2;

    /*Vector de Estados*/
    //double x[12] = { Pn, Pe, Pd, u, v, w, phi, theeta, psi, dphi, dtheeta, dpsi };
    
    double u0 = 2;
    double x[12] = {};
    double x_0[12] = { 0, 0, 0, u0, 0, 0, 0, 0, 0, 0, 0, 0 };
    /////////////////////////////////////

    float lastTime = device->getTimer()->getTime() / 1000.;

    std::wstring strX, strY, strZ, strT;

    while (device->run())
    {
        driver->beginScene(true, true, SColor(255, 100, 101, 140));

        float now = device->getTimer()->getTime() / 1000.;
        float deltaTimeSeconds = now - lastTime;
        
        /////////////////////////////////////
        // Simulator step
        /////////////////////////////////////

        Crank_Nicolson(x_0, x, m, lastTime, now);

        for (int i = 0; i < m; ++i) {
            x_0[i] = x[i];
        }
        /////////////////////////////////////

        strX = std::wstring(L"x: ") + std::to_wstring(x[0]);
        textX->setText(strX.c_str());

        strY = std::wstring(L"y: ") + std::to_wstring(x[1]);
        textY->setText(strY.c_str());

        strZ = std::wstring(L"z: ") + std::to_wstring(x[2]);
        textZ->setText(strZ.c_str());

        strT = std::wstring(L"t: ") + std::to_wstring(now);
        textTime->setText(strT.c_str());

        node->setRotation(vector3df(x[6], x[7], x[8]));

        // Para mover el avion
        // node->setPosition(vector3df(0, 0, 0))

        smgr->drawAll();
        guienv->drawAll();

        driver->endScene();

        lastTime = now;
    }

    device->drop();


	return 0;
}