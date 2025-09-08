#include "XEngine.h"

IXAudio2* pXAudio2 = nullptr;//XAudio2
IXAudio2SourceVoice* _bgmSource_ = nullptr;
list<IXAudio2SourceVoice*>listSound_{};
int soundCounter{ 0 };

IDirectInputDevice8W* _keyboard_ = nullptr;
IDirectInputDevice8W* _mouse_ = nullptr;

long long startTime_ = 0;
long long lastCallToUpdate_ = 0;
long long currentCallToUpdate_ = 0;
long long frequency1000_ = 0;

//私有方法
void SetKeyState_EndFrame();//所有按键帧结束
void UpdateInput();
HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);

//对外访问定义
bool fullScreenMode_;
bool isXAudioOK_;
int soundVolume_;
int musicVolume_;

sXKeyState key[256];
sXKeyState mouseKey[8];
sXKeyState* key2[256 + 8];
int dMouseZ{0};

namespace EnginePreset {
	SIZE resolution{ 1920,1080 };
	SIZE clientSize{ 1920,1080 };
	HCURSOR cursor{ NULL };
}

SIZE clientSize;
bool isMouseMoved;
POINTD mousePositionScene;

HWND hwnd;
IDXGISwapChain* _dxgiSwapChain;
ID2D1DeviceContext* _d2dDeviceContext;
ID2D1Bitmap1* _d2dBitmap1Temp;
ID2D1Bitmap1* _d2dBitmap1Dxgi;
IWICImagingFactory* _wicFactory;
void EngineRun(PCWSTR wndname) {
	WNDCLASSEX wnd{};//准备WNDCLASSEX
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.hCursor = EnginePreset::cursor ? EnginePreset::cursor : LoadCursor(NULL, IDC_ARROW);
	wnd.lpszClassName = L"XClass";//窗口类名
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wnd.lpfnWndProc = [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)->LRESULT {
		switch (msg) {
		case WM_SIZE: {
			clientSize = { LOWORD(lp), HIWORD(lp) };
			_d2dDeviceContext->SetTarget(nullptr);
			_d2dBitmap1Dxgi->Release();
			_dxgiSwapChain->ResizeBuffers(1, clientSize.cx, clientSize.cy, DXGI_FORMAT_B8G8R8A8_UNORM, 0);
			IDXGISurface* _dxgiSurface;
			_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&_dxgiSurface));
			_d2dDeviceContext->CreateBitmapFromDxgiSurface(_dxgiSurface, nullptr, &_d2dBitmap1Dxgi);
			_dxgiSurface->Release();
			break;
		}
		case WM_MOUSEMOVE: {
			isMouseMoved = true;
			POINTS cpos_wind = MAKEPOINTS(lp);

			double hMulti = clientSize.cx ? static_cast<double>(EnginePreset::resolution.cx) / clientSize.cx : 0;
			double vMulti = clientSize.cy ? static_cast<double>(EnginePreset::resolution.cy) / clientSize.cy : 0;

			mousePositionScene.x_ = cpos_wind.x * hMulti;
			mousePositionScene.y_ = cpos_wind.y * vMulti;
			break;
		}
		case WM_QUIT:
		case WM_CLOSE:
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
		return DefWindowProcW(hwnd, msg, wp, lp);
	};

	if (!RegisterClassExW(&wnd)) {
		MessageBox(NULL, L"register window class failed", L"err", MB_OK);
		return;
	}

	RECT rect = { 0,0,EnginePreset::clientSize.cx,EnginePreset::clientSize.cy };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);//客户区尺寸反推窗口尺寸

	hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"XClass", wndname, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, NULL, 0);
	if (!hwnd) {
		MessageBox(NULL, L"create window failed", L"err", MB_OK);
		return;
	}
	
	fullScreenMode_ = false;

	//创建D3dDevice、DxgiSwapChain、DxgiDevice
	ID3D10Device1* _d3d10Device;
	
	DXGI_SWAP_CHAIN_DESC scd{};
	scd.OutputWindow = hwnd;
	scd.Windowed = true;
	scd.BufferCount = 1;//后台缓冲区数量
	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//缓冲区用于输出
	scd.SampleDesc.Count = 1;//多重采样次数，至少为1
	D3D10CreateDeviceAndSwapChain1(nullptr, D3D10_DRIVER_TYPE_HARDWARE, 0, D3D10_CREATE_DEVICE_BGRA_SUPPORT,
		D3D10_FEATURE_LEVEL_10_1, D3D10_1_SDK_VERSION, &scd, &_dxgiSwapChain, &_d3d10Device);
	IDXGIDevice* _dxgiDevice;
	_dxgiSwapChain->GetDevice(IID_PPV_ARGS(&_dxgiDevice));

	//创建D2dFactory、D2dDevice、D2dDeviceContext、d2dBitmap1Temp（离屏渲染区）
	ID2D1Factory1* _d2dFactory;
	HRESULT hres = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_PPV_ARGS(&_d2dFactory));
	if (hres != S_OK)return;
	ID2D1Device* _d2dDevice;
	hres = _d2dFactory->CreateDevice(_dxgiDevice, &_d2dDevice);
	if (hres != S_OK)return;
	_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &_d2dDeviceContext);
	_d2dDeviceContext->CreateBitmap(D2D1::SizeU(EnginePreset::resolution.cx, EnginePreset::resolution.cy), nullptr, 0, D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)), &_d2dBitmap1Temp);
	hres = _d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 0), &_scBrush_);
	if (hres != S_OK)return;

	//获取DxgiSurface，创建关联d2dBitmap1Dxgi
	IDXGISurface* _dxgiSurface;
	_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&_dxgiSurface));
	_d2dDeviceContext->CreateBitmapFromDxgiSurface(_dxgiSurface, nullptr, &_d2dBitmap1Dxgi);
	_dxgiSurface->Release();

	//初始化文字工厂
	hres = DWriteCreateFactory(DWRITE_FACTORY_TYPE::DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&_dwriteFactory_));
	if (hres != S_OK)return;

	//WIC与XAudio2要用的
	hres = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (hres != S_OK)return;

	//初始化WIC工厂
	hres = CoCreateInstance(CLSID_WICImagingFactory, NULL,
		CLSCTX_INPROC_SERVER, IID_IWICImagingFactory,
		(LPVOID*)&_wicFactory);
	if (hres != S_OK)return;

	//创建XAudio2引擎实例
	isXAudioOK_ = true;
	soundVolume_ = 10; musicVolume_ = 10;
	hres = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if (FAILED(hres)) {
		isXAudioOK_ = false;
		soundVolume_ = 0; musicVolume_ = 0;
		MessageBox(hwnd, L"XAudio2 initialize failed!", L"err", 0);
	};

	//创建主音源
	IXAudio2MasteringVoice* pMasterVoice = nullptr;
	hres = pXAudio2->CreateMasteringVoice(&pMasterVoice);
	if (FAILED(hres)) {
		isXAudioOK_ = false;
		soundVolume_ = 0; musicVolume_ = 0;
		MessageBox(hwnd, L"XAudio2 voice initialize failed!", L"err", 0);
	};

	//创建dinput接口
	IDirectInput8W* _dinput;
	hres = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_dinput, 0);
	if (FAILED(hres))return;

	hres = _dinput->CreateDevice(GUID_SysKeyboard, &_keyboard_, 0); if (FAILED(hres))return;
	_keyboard_->SetDataFormat(&c_dfDIKeyboard);
	_keyboard_->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	hres = _dinput->CreateDevice(GUID_SysMouse, &_mouse_, 0); if (FAILED(hres))return;
	_mouse_->SetDataFormat(&c_dfDIMouse2);
	_mouse_->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	
	for (int i = 0; i < 256; ++i) key2[i] = &(key[i]);
	for (int i = 0; i < 8; ++i) key2[i + 256] = &(mouseKey[i]);

	ClientInit();

	//置计时器
	LARGE_INTEGER t;
	QueryPerformanceFrequency(&t);
	frequency1000_ = t.QuadPart / 1000;
	QueryPerformanceCounter(&t);
	startTime_ = t.QuadPart;
	currentCallToUpdate_ = t.QuadPart;
	lastCallToUpdate_ = t.QuadPart;

	ShowWindow(hwnd, SW_SHOW);//显示窗口
	SetFocus(hwnd);//聚焦窗口

	MSG msg{};//准备接收消息
	while (msg.message != WM_QUIT) {
		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) DispatchMessage(&msg);
		else {
			//===========计时刷新===========
			lastCallToUpdate_ = currentCallToUpdate_;
			QueryPerformanceCounter(&t);
			currentCallToUpdate_ = t.QuadPart;
			//=========update逻辑刷新=========
			UpdateInput();
			long long dtick = currentCallToUpdate_ - lastCallToUpdate_;
			ClientUpdate((double)dtick / frequency1000_);
			SetKeyState_EndFrame();
			////=========Render图像渲染=========
			_d2dDeviceContext->SetTarget(_d2dBitmap1Temp);
			_d2dDeviceContext->BeginDraw();
			ClientRender();
			_d2dDeviceContext->EndDraw();

			_d2dDeviceContext->SetTarget(_d2dBitmap1Dxgi);
			_d2dDeviceContext->BeginDraw();
			_d2dDeviceContext->DrawBitmap(_d2dBitmap1Temp, D2D1::RectF(0, 0, _d2dBitmap1Dxgi->GetSize().width, _d2dBitmap1Dxgi->GetSize().height),
				1, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
			_d2dDeviceContext->EndDraw();

			_dxgiSwapChain->Present(0, 0);
		}
	}
}

void SoundVolumeAdd() {
	soundVolume_ += 1;
	if (soundVolume_ > 10)soundVolume_ = 10;
};
void SoundVolumeDec() {
	soundVolume_ -= 1;
	if (soundVolume_ < 0)soundVolume_ = 0;
};
void PlayXSound(sSoundData& xsounddata) {
	if (isXAudioOK_&&soundVolume_) {
		IXAudio2SourceVoice* _sourceVoice;
		pXAudio2->CreateSourceVoice(&_sourceVoice, (WAVEFORMATEX*)&xsounddata.waveFormatEx_);//创建源语音
		_sourceVoice->SubmitSourceBuffer(&xsounddata.buffer_);
		_sourceVoice->SetVolume(0.1*soundVolume_);
		_sourceVoice->Start(0);
		//内存管理
		listSound_.push_back(_sourceVoice);
		if (++soundCounter > 100) {
			for (auto& it = listSound_.begin(); it != listSound_.end();) {
				IXAudio2SourceVoice* pSourceVoice = *it;
				if(pSourceVoice){
					XAUDIO2_VOICE_STATE voiceState;
					pSourceVoice->GetState(&voiceState);
					if (voiceState.BuffersQueued > 0) {}
					else { 
						pSourceVoice->DestroyVoice();
						it = listSound_.erase(it); 
						continue;
					}
				}
				else { it = listSound_.erase(it); continue; }
				++it;
			}
			soundCounter = 0;
		};
	}
}
void PlayXBgm(sSoundData& xsounddata, bool loop) {
	if (isXAudioOK_) {
		if (_bgmSource_) {
			_bgmSource_->Stop(0);
			_bgmSource_->DestroyVoice();
		}
		if (musicVolume_) {
			pXAudio2->CreateSourceVoice(&_bgmSource_, (WAVEFORMATEX*)&xsounddata.waveFormatEx_);//创建源语音
			_bgmSource_->SubmitSourceBuffer(&xsounddata.buffer_);
			_bgmSource_->SetVolume(0.1 * musicVolume_);
			_bgmSource_->Start(0);
		}
	}
}
void StopXBgm() {if (_bgmSource_)_bgmSource_->Stop(0);}

bool nEngine::IsKeyPushed(){
	for (auto& k : key)if (k.bPushed)return true;
	return false;
}

void nEngine::NormalScreen() {
	RECT rect = { 0,0,1600,900 };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);//根据绘图区尺寸生成窗口尺寸

	SetWindowLongW(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
	SetWindowPos(hwnd, HWND_TOP, 0, 0,
		rect.right-rect.left, rect.bottom-rect.top,
		SWP_SHOWWINDOW);
	SetForegroundWindow(hwnd);
	ShowWindow(hwnd, SW_SHOWNORMAL);
	fullScreenMode_ = false;
}
void nEngine::FullScreen(){
	HWND hDesk;
	RECT rectDesk;
	hDesk = GetDesktopWindow();
	GetWindowRect(hDesk, &rectDesk);
	SetWindowLongW(hwnd, GWL_STYLE, WS_BORDER);
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0,
		rectDesk.right, rectDesk.bottom,
		SWP_SHOWWINDOW);
	fullScreenMode_ = true;
}
void nEngine::NormalFullScreen(){
	if (fullScreenMode_) { nEngine::NormalScreen(); }
	else { nEngine::FullScreen(); }
}


int TakeSignalPushedKey() {
	for (int order = 0; order < 256+8; order++)
		if (key2[order]->bPushed) {
			key2[order]->bPushed = false;
			return order;
		}
	return 0;
}
int TakeSignalUpKey() {
	for (int order = 0; order < 256 + 8; order++)
		if (key2[order]->bReleased) {
			key2[order]->bReleased = false;
			return order;
		}
	return 0;
}

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
void nEngine::LoadSoundData(wchar_t* filepath, sSoundData& xsounddata) {
	WAVEFORMATEXTENSIBLE& wfx = xsounddata.waveFormatEx_;
	wfx = { 0 };
	//打开音频文件
	HANDLE hFile = CreateFileW(filepath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)return;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))return;
	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	//在音频文件中找到RIFF区块，并检查文件类型应为fourccWAVE或'XWMA'
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD filetype;
	ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
	if (filetype != fourccWAVE)return;
	//MessageBox(NULL, L"找到RIFF", L"Tip", MB_OK);

	//找到fmt区块
	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

	//找到“数据”区块，并将其内容读入缓冲区。
	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE* pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);
	//填充 XAUDIO2_BUFFER 结构。
	xsounddata.buffer_.AudioBytes = dwChunkSize;//音频buffer的大小（bytes）
	xsounddata.buffer_.pAudioData = pDataBuffer;//包含音频数据的buffer
	xsounddata.buffer_.Flags = XAUDIO2_END_OF_STREAM;//告诉源语音在此缓冲区之后不要期待任何数据
}

//私有定义
using namespace nEngine;


void SetKeyState_EndFrame() {
	for (auto& k : key) {
		k.bHeld = false;
		k.bPushed = false;
		k.bReleased = false;
	}
	for (auto& k : mouseKey) {
		k.bHeld = false;
		k.bPushed = false;
		k.bReleased = false;
	}
	isMouseMoved = false;
}

void UpdateInput(){
	static char stateKeyBoard[2][256] = { 0 };
	static DIMOUSESTATE2 stateMouse[2] = { 0 };
	static char tick = 0;

	_keyboard_->Acquire();
	_keyboard_->Poll();
	if (FAILED(_keyboard_->GetDeviceState(256, &(stateKeyBoard[tick]))))for (auto& d : stateKeyBoard[tick])d = 0;

	_mouse_->Acquire();
	_mouse_->Poll();
	if (FAILED(_mouse_->GetDeviceState(sizeof(DIMOUSESTATE2), &(stateMouse[tick]))))
		for (auto& d : stateMouse[tick].rgbButtons)d = 0;

	int lastTick = tick ? 0 : 1;
	for (int i = 0; i < 256;i++) {
		bool cdown = stateKeyBoard[tick][i] & 0x80;
		bool ldown = stateKeyBoard[lastTick][i] & 0x80;
		if (cdown) {
			key[i].bHeld = true;
			if (!ldown)key[i].bPushed = true;
		}
		else {
			if (ldown)key[i].bReleased = true;
		}
	}
	for (int i = 0; i < 8; i++) {
		bool cdown = stateMouse[tick].rgbButtons[i] & 0x80;
		bool ldown = stateMouse[lastTick].rgbButtons[i] & 0x80;
		if (cdown) {
			mouseKey[i].bHeld = true;
			if (!ldown)mouseKey[i].bPushed = true;
		}
		else {
			if (ldown)mouseKey[i].bReleased = true;
		}
		//dMouseZ = stateMouse[tick].lZ - stateMouse[lastTick].lZ;
		dMouseZ = stateMouse[tick].lZ;
	}

	tick = tick ? 0 : 1;
}

HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition) {
	HRESULT hr = S_OK;
	//文件指针置开头
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytestRead = 0;
	DWORD dwOffset = 0;
	
	while (hr == S_OK) {
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, 4, &dwRead, NULL))hr = HRESULT_FROM_WIN32(GetLastError());
		if (0 == ReadFile(hFile, &dwChunkDataSize, 4, &dwRead, NULL))hr = HRESULT_FROM_WIN32(GetLastError());
		switch (dwChunkType) {
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, 4, &dwRead, NULL))hr = HRESULT_FROM_WIN32(GetLastError());
			break;
		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))return HRESULT_FROM_WIN32(GetLastError());
		}
		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc) {
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytestRead >= dwRIFFDataSize)return S_FALSE;
	}

	return S_OK;
}
HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset) {
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}