#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3d11.lib")

#define NOMINMAX
//winapi
#include <windows.h>
#include <iostream>
#include <thread>

//hooks
#include "hooks.hpp"
#include "sdk/interfaces/interfaces.hpp"

//features
#include "esp.hpp"

//imgui
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//directx 
#include <d3d11.h>
#include <dxgi.h>

//menu hooking
#include "kiero/kiero.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"
#include "aimbot.hpp"
#include "load_materials.hpp"
#include "chams.hpp"
typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;
Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}
	DWORD64 handle = (DWORD64)GetModuleHandle(NULL);
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("ZeruaS");
	ImGui::Text("mogesalmebit zeruasidan :O");



	//bhop
	ImGui::Checkbox("Luxis Meinebis Shesadzlebloba", &bhop);

	//esp
	ImGui::Checkbox("Eqstrasensoruli Aghqma",&esp::esp_on);
	if (esp::esp_on) {
		ImGui::Checkbox("Daxatos Sicocxle", &esp::health);
		ImGui::Checkbox("Daxatos Chonchxi", &esp::skeleton);
	}

	//aimbot
	ImGui::Checkbox("Miznis Shesworeba", &aimbot::aim_on);
	if (aimbot::aim_on) {
		ImGui::SliderFloat("Shesworebis Areali", &aimbot::legit_fov, 1.f, 180.f, "%.2f");
		ImGui::SliderFloat("Shesworebis Shemsubukeba", &aimbot::smooth, 1.f, 30.f, "%.2f");

		//ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(0, 0), aimbot::legit_fov, ImColor(101.f, 139.f, 201.f), 0, 1.5f);

		ImGui::Checkbox("Srolis Shemowmeba Shesworebamde", &aimbot::check_attack);

		ImGui::Text("Samizne Sxeulis Nawilebi");
		ImGui::Selectable("Tavi", &aimbot::hitboxes[0]);
		ImGui::Selectable("Mkerdi", &aimbot::hitboxes[1]);
		ImGui::Selectable("Xelebi", &aimbot::hitboxes[2]);
		ImGui::Selectable("Fexebi", &aimbot::hitboxes[3]);
	}
	ImGui::SeparatorText("Erteulis Feris Modulacia");

	ImGui::Checkbox("Chartva", &chams::chartuli);
	if (chams::chartuli) {
		//ImGui::Checkbox("Kedlebshi Modulacia", &chams::kedlebshi_xedva);
		ImGui::ColorEdit3("Kedlebshi Modulaciis Feri##1", chams::kedlebis_color, ImGuiColorEditFlags_NoOptions);
		//ImGui::Checkbox("Xilulze Modulacia", &chams::visible_xedva);
		ImGui::ColorEdit3("Xilulze Modulaciis Feri##1", chams::visible_color, ImGuiColorEditFlags_NoOptions);
	}
	ImGui::End();
	esp::exec();
	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}




int main();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL); //thread on top ak gashvebas mainc moveridebi
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

int main() {
    AllocConsole(); // tu rame mouvida didialbatobit amis brali ikneba soz <.< easy to catch
    freopen("CONOUT$", "w", stdout);
	std::cout << R"(                                    ZS       
                                 ZSZ        
                              ZSZSZ  Z      
                             ZSZSZSZS       
                           ZSZSZSZSZ        
                            ZSZSZSZ         
                          ZSZSZSZSZ         
                       ZSZSZSZSZSZ          
                    ZSZSZSZSZSZSZS          
              ZSZSZSZSZSZSZSZSZSZ           
           ZSZSZSZSZSZSZSZSZSZSZS           
         ZSZSZSZSZSZSZSZSZSZSZSZS           
 Z      ZSZSZSZSZS    ZSZSZSZSZSZ           
 ZS    ZSZSZSZSZS           ZS Z            
 ZSZSZS ZSZSZSZSZ            Z Z            
         ZSZSZS              Z Z            
          ZSZSZ              Z Z            
          ZSZ ZS             Z Z            
          ZS   ZS                   
         ZS     Z        
          ZS     ZS  
           ZS  

Tu Amas Xedavt Anu Zeruasis Debug Situaciashi Imkofebit :D
Warmatebebs Gisurvebt, Gaitvalisiwnet Rom Shesadzlebelia Raime Funqcia Ar Mushaobdes
Raime Crashis Agmochenis Shemtxvevashi Daukavshirdit Zeros

E010 5254 F322 DE38 8C86 611B A037 B75B 2E4D 8461
)" << "\n"; // magari mezareba compiler kleobis gaketeba amitomac xelit chxikini eshveleba am prints
    //chavtvirtot hookebi
	interfaces::initialize();
	
    materials::LoadAllMaterials();
    
	//schema_system::initialize();
    hooks::initialize();

	//menu loading
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)&oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);
}
