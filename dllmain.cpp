// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>

// YYTK is in this now
#include "MyPlugin.h"
#include "Assets.h"
#include "LHSprites.h"
// Plugin functionality
#include <fstream>
#include <iterator>
#include "LHCore.h"
#include "Config.h"
#define _CRT_SECURE_NO_WARNINGS


CallbackAttributes_t* CodeCallbackAttr;
YYRValue PART_SYSTEM;
YYRValue PART_TYPE;
YYRValue PART_EMITTER;

#define PART_SHAPE_PIXEL 0.0
#define PART_SHAPE_CLOUD 11.0

#define GAME_WIDTH 640.0
#define GAME_HEIGHT 360.0

// Unload function, remove callbacks here
YYTKStatus PluginUnload()
{
    LHCore::pUnregisterModule(gPluginName);
    return YYTK_OK;
}

int CodePostPatch(YYTKCodeEvent* codeEvent, void*)
{
    
    CCode* codeObj = std::get<CCode*>(codeEvent->Arguments());
    CInstance* selfInst = std::get<0>(codeEvent->Arguments());
    CInstance* otherInst = std::get<1>(codeEvent->Arguments());


    // If we have invalid data???
    if (!codeObj)
        return YYTK_INVALIDARG;

    if (!codeObj->i_pName)
        return YYTK_INVALIDARG;

    // Do event specific stuff here.
    if (Misc::StringHasSubstr(codeObj->i_pName, "o_fight_enemy_Draw_0") || Misc::StringHasSubstr(codeObj->i_pName, "o_hero_Draw_0"))
    {
        Binds::CallBuiltinA("part_emitter_burst", { PART_SYSTEM, PART_EMITTER, PART_TYPE, 8.0 });
    }


    return YYTK_OK;

}



void InstallPatches() // Register Pre and Post patches here
{
	if (LHCore::pInstallPostPatch != nullptr)
	{
		LHCore::pInstallPostPatch(CodePostPatch);
        Misc::Print("Installed patch method(s)", CLR_GREEN);
	}

    // Set up particles
    PART_SYSTEM = Binds::CallBuiltinA("part_system_create", {});
    Binds::CallBuiltinA("part_system_depth", { PART_SYSTEM, -100.0 });

    PART_TYPE = Binds::CallBuiltinA("part_type_create", {});
    Binds::CallBuiltinA("part_type_shape", { PART_TYPE, PART_SHAPE_PIXEL  });
    Binds::CallBuiltinA("part_type_size", { PART_TYPE, 0.8, 1.2, 0.0, 0.0 });
    Binds::CallBuiltinA("part_type_color1", { PART_TYPE, 16777215.0 });
    Binds::CallBuiltinA("part_type_alpha3", { PART_TYPE, 0.1, 0.9 ,0.0 });
    Binds::CallBuiltinA("part_type_speed", { PART_TYPE, 0.5, 1.0, 0.0, 0.0 });
    Binds::CallBuiltinA("part_type_direction", { PART_TYPE, 260.0, 280.0, 0.0, 0.0 });
    //Binds::CallBuiltinA("part_type_gravity", { PART_TYPE, 0.02, 270.0 });

    // emitter

    PART_EMITTER = Binds::CallBuiltinA("part_emitter_create", { PART_SYSTEM });

   

    Binds::CallBuiltinA(
        "part_emitter_region",
        {
            PART_SYSTEM ,
            PART_EMITTER,
            0.0, GAME_WIDTH,
            0.0, GAME_HEIGHT ,
            0.0, // ps_shape_rectangle
            0.0  // ps_distr_linear
        }
    );

    
}

// Entry
DllExport YYTKStatus PluginEntry(
    YYTKPlugin* PluginObject // A pointer to the dedicated plugin object
)
{
    LHCore::CoreReadyPack* pack = new LHCore::CoreReadyPack(PluginObject, InstallPatches);
    PluginObject->PluginUnload = PluginUnload;
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LHCore::ResolveCore, (LPVOID)pack, 0, NULL); // Wait for LHCC
    return YYTK_OK; // Successful PluginEntry.
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DllHandle = hModule; // save our module handle
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

