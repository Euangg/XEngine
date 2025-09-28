#pragma once
#define _USE_MATH_DEFINES

#include<d2d1_1.h>
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"d3d10_1.lib")
#include<dwrite.h>
#pragma comment(lib,"dwrite.lib")
#include<wincodec.h>
#pragma comment(lib,"windowscodecs.lib")
#include<xaudio2.h>
#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include<ShlObj.h>
#include<math.h>

#include<string>
#include<array>
#include<vector>
#include<list>
#include<set>
#include<bitset>

#include<fstream>
#include<sstream>
#include<random>

using std::array;
using std::pair;
using std::vector;
using std::list;
using std::set;
using std::wstring;
using std::shared_ptr;
using std::make_shared;
using std::static_pointer_cast;
using std::clamp;
typedef D2D1_COLOR_F COLORF;
typedef D2D_POINT_2F POSF;
typedef D2D_RECT_F RECTF;
typedef D2D1_ELLIPSE ELLIPSEF;
typedef void (*Func)();
typedef void (*Func2)(double);
struct GamePhase {
	Func _create_;//程序启动时
	Func _load_;//进入阶段
	Func _init_;//主动初始化
	Func2 _update_;//帧逻辑
	Func _render_;//帧渲染
	Func _leave_;//离开阶段
	Func _destroy_;//程序结束
	void Bind(Func create, Func load, Func init, Func2 update, Func render, Func leave, Func destroy) {
		_create_ = create; _load_ = load; _init_ = init; _update_ = update;
		_render_ = render; _leave_ = leave; _destroy_ = destroy;
	}

	static GamePhase* _currentPhase;
	static void StartFromPhase(GamePhase* phase) {
		GamePhase::_currentPhase = phase;
		GamePhase::_currentPhase->_load_();
		GamePhase::_currentPhase->_init_();
	}
	static void SwitchToPhase(GamePhase* phase, bool init) {
		GamePhase::_currentPhase->_leave_();
		GamePhase::_currentPhase = phase;
		GamePhase::_currentPhase->_load_();
		if (init)GamePhase::_currentPhase->_init_();
	}
};

struct sXKeyState {
	bool bPushed = false;
	bool bHeld = false;
	bool bReleased = false;
	void Init() { *this = { false,false,false }; };
};
struct POINTD {
	double x_{ 0 }, y_{ 0 };
	POINTD() {};
	POINTD(double x, double y) :x_(x), y_(y) {};
};
struct CIRCLED {
	POINTD p_;double r_;
};
struct RECTD {
	virtual operator RECTF() { return { 0,0,0,0 }; };
};
struct RECTDE :RECTD {
	double l_, t_, r_, b_;
	operator RECTF()override {
		RECTF rectf = { l_ ,t_ ,r_ ,b_ };
		return rectf;
	}
};
struct RECTDS :RECTD {
	double x_{ 0 }, y_{ 0 }, w_{ 0 }, h_{ 0 };
	operator RECTF()override {
		RECTF rect = { x_,y_,x_ + w_,y_ + h_ };
		return rect;
	};
	RECTDS() {};
	RECTDS(double x, double y, double w, double h) :x_{ x }, y_{ y }, w_{ w }, h_{ h }{};
	RECTDS(int x, int y, int w, int h) :x_{ double(x) }, y_{ double(y) }, w_{ double(w) }, h_{ double(h) }{};
	double L()const;
	double T()const;
	double R()const;
	double B()const;
	double CX()const;
	double CY()const;
	POINTD C()const;
	void ML(double l);
	void MT(double t);
	void MR(double r);
	void MB(double b);
	void MCX(double cx);
	void MCY(double cy);
	void MC(double cx, double cy);
	void ADDCX(double cx);
	void ADDCY(double cy);
	void Zoom(double multi);
	RECTDS GenerateOffset(double dx, double dy);
};
struct RoRECTD :RECTDS {
	POINTD roPoint_{ 0,0 };
	double roAngle_{ 0 };

	RoRECTD(double x, double y, double w, double h, POINTD p, double ang) :RECTDS(x, y, w, h), roPoint_{ p }, roAngle_{ ang } {};

	POINTD ROLT() {
		POINTD o{ L() - roPoint_.x_,T() - roPoint_.y_ }; double radian = roAngle_ * M_PI / 180;
		return{ o.x_ * cos(radian) - o.y_ * sin(radian) + roPoint_.x_,o.x_ * sin(radian) + o.y_ * cos(radian) + roPoint_.y_ };
	}
	POINTD RORT() {
		POINTD o{ R() - roPoint_.x_,T() - roPoint_.y_ }; double radian = roAngle_ * M_PI / 180;
		return{ o.x_ * cos(radian) - o.y_ * sin(radian) + roPoint_.x_,o.x_ * sin(radian) + o.y_ * cos(radian) + roPoint_.y_ };
	}
	POINTD ROLB() {
		POINTD o{ L() - roPoint_.x_,B() - roPoint_.y_ }; double radian = roAngle_ * M_PI / 180;
		return{ o.x_ * cos(radian) - o.y_ * sin(radian) + roPoint_.x_,o.x_ * sin(radian) + o.y_ * cos(radian) + roPoint_.y_ };
	}
	POINTD RORB() {
		POINTD o{ R() - roPoint_.x_,B() - roPoint_.y_ };  double radian = roAngle_ * M_PI / 180;
		return{ o.x_ * cos(radian) - o.y_ * sin(radian) + roPoint_.x_,o.x_ * sin(radian) + o.y_ * cos(radian) + roPoint_.y_ };
	}
	POINTD ROC() {
		POINTD o{ CX() - roPoint_.x_,CY() - roPoint_.y_ };  double radian = roAngle_ * M_PI / 180;
		return{ o.x_ * cos(radian) - o.y_ * sin(radian) + roPoint_.x_,o.x_ * sin(radian) + o.y_ * cos(radian) + roPoint_.y_ };
	}
};
struct sERectV :RECTDS {
	double vx_ = 0;
	double vy_ = 0;

	void SetV(double vx, double vy);
	void EffectV(double dtime);
	void DecelerateX(double vdx);
};
//资源
struct sI18nString {
	enum eLan {
		LAN_START = 0,

		LAN_EN = 0,
		LAN_CH = 1,

		LAN_NUM
	};
	static eLan eLan_;
	static void ChangeLanguageNext() {
		int lan = eLan_ + 1;
		lan %= LAN_NUM;
		eLan_ = static_cast<eLan>(lan);
	}

	std::array<std::wstring, LAN_NUM>wstr_{ L"",L"" };//内容

	//构造
	sI18nString() {};//默认无参构造
	sI18nString(const sI18nString&) = default;//拷贝构造
	sI18nString(std::wstring en, std::wstring ch) { wstr_[0] = en; wstr_[1] = ch; }

	//重载
	void operator()(std::wstring wstr, eLan lan) { wstr_[lan] = wstr; };
	void operator()(std::wstring wstren, std::wstring wstrch) { wstr_[0] = wstren; wstr_[1] = wstrch; };
	void operator()(sI18nString sms) { (*this)(sms.wstr_[0], sms.wstr_[1]); };
	void operator()(std::wstring wstr) { wstr_[0] = wstr; wstr_[1] = wstr; };

	const std::wstring& wstr() const { return wstr_[eLan_]; };//获取字符串
};
struct DIB {
	unsigned w_ = 0;
	unsigned h_ = 0;
	byte* data_ = nullptr;
	byte GetAlpha(unsigned x, unsigned y)const { return data_[(y * w_ + x) * 4 + 3]; };
};

//声音
struct sSoundData {
	WAVEFORMATEXTENSIBLE waveFormatEx_ = { 0 };
	XAUDIO2_BUFFER buffer_ = { 0 };
};
//Static
using std::to_wstring;
#define PI acos(-1)
#define SAFE_RELEASE(x)if(x)x->Release();x=nullptr;

const COLORF cBlack{ D2D1::ColorF(D2D1::ColorF::Black) };
const COLORF cBlue{ D2D1::ColorF(D2D1::ColorF::Blue) };
const COLORF cChartreuse{ D2D1::ColorF(D2D1::ColorF::Chartreuse) };
const COLORF cCornflowerBlue = D2D1::ColorF(D2D1::ColorF::CornflowerBlue);
const COLORF cCrimson = D2D1::ColorF(D2D1::ColorF::Crimson);
const COLORF cCyan = D2D1::ColorF(D2D1::ColorF::Cyan);
const COLORF cFirebrick = D2D1::ColorF(D2D1::ColorF::Firebrick);
const COLORF cGhostWhite{ D2D1::ColorF(D2D1::ColorF::GhostWhite) };
const COLORF cGold{ D2D1::ColorF(D2D1::ColorF::Gold) };
const COLORF cGray{ D2D1::ColorF(D2D1::ColorF::Gray) };
const COLORF cGreen{ D2D1::ColorF(D2D1::ColorF::Green) };
const COLORF cOrange{ D2D1::ColorF(D2D1::ColorF::Orange) };
const COLORF cPurple = D2D1::ColorF(D2D1::ColorF::Purple);
const COLORF cRed{ D2D1::ColorF(D2D1::ColorF::Red) };
const COLORF cWhite{ D2D1::ColorF(D2D1::ColorF::White) };
const COLORF cYellow{ D2D1::ColorF(D2D1::ColorF::Yellow) };

extern ID2D1SolidColorBrush* _scBrush_;
void DrawLine(ID2D1RenderTarget* prt, const POSF& p1, const POSF& p2, const COLORF& color, float width);
void DrawRect(ID2D1RenderTarget* prt, const RECTF& rect, const COLORF& color, float width);
void FillRect(ID2D1RenderTarget* prt, const RECTF& rect, const COLORF& color);
void FillDrawRect(ID2D1RenderTarget* prt, const RECTF& rect, const COLORF& colrf, const COLORF& colrd, float width);
void DrawEllipse(ID2D1RenderTarget* prt,const ELLIPSEF& ellipse, const COLORF& color, float width);
void FillEllipse(ID2D1RenderTarget* prt, const ELLIPSEF& ellipse, const COLORF& color);
void FillDrawEllipse(ID2D1RenderTarget* prt, const ELLIPSEF& ellipse, const COLORF& colrf, const COLORF& colrd, float width);
void DrawCircle(ID2D1RenderTarget* prt, const CIRCLED& c, const COLORF& color, float width);
void FillCircle(ID2D1RenderTarget* prt, const CIRCLED& c, const COLORF& color);
void FillDrawCircle(ID2D1RenderTarget* prt, const CIRCLED& c, const COLORF& colorf, const COLORF& colord, float width);
extern IDWriteFactory* _dwriteFactory_;
void ShowText(ID2D1RenderTarget* prt, const wstring& str,const RECTF& rect, const COLORF& color, double size = 15., DWRITE_TEXT_ALIGNMENT talignment = DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT palignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
void ShowTextLT(ID2D1RenderTarget* prt, const wstring& str,const RECTF& rect, const COLORF& color, double size = 15.);
void ShowTextLC(ID2D1RenderTarget* prt, const wstring& str,const RECTF& rect, const COLORF& color, double size = 15.);
void ShowTextLB(ID2D1RenderTarget* prt, const wstring& str,const RECTF& rect, const COLORF& color, double size = 15.);
void ShowTextCT(ID2D1RenderTarget* prt, const wstring& str,const RECTF& rect, const COLORF& color, double size = 15.);
void ShowTextCB(ID2D1RenderTarget* prt, const wstring& str,const RECTF& rect, const COLORF& color, double size = 15.);
void ShowTextRT(ID2D1RenderTarget* prt, const wstring& str,const RECTF& rect, const COLORF& color, double size = 15.);
void ShowTextRC(ID2D1RenderTarget* prt, const wstring& str,const RECTF& rect, const COLORF& color, double size = 15.);
void ShowTextRB(ID2D1RenderTarget* prt, const wstring& str,const RECTF& rect, const COLORF& color, double size = 15.);
void ShowText2(ID2D1RenderTarget* prt, const wstring& str, const RECTF& rect, const COLORF& color, double size, DWRITE_TEXT_ALIGNMENT talignment = DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT palignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

void ShowTextV(ID2D1RenderTarget* prt, const wstring& str, const RECTF& rect, const COLORF& color, double size = 15., DWRITE_TEXT_ALIGNMENT talignment = DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT palignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

int RandIntIn(int min, int max);
float RandFloatIn(float min, float max);//2位小数
bool Chance(double chance1);


double Distance(const POINTD& point1, const POINTD& point2);
bool HitRectRect(const RECTF& rrect1, const RECTF& rrect2, RECTF* const p_overlap_rect = nullptr);
bool HitPointRect(const POINTD& pointd, const RECTF& rectf);
bool HitRortRort(RoRECTD& rort1, RoRECTD& rort2);
bool HitRortCircle(RoRECTD& rort1, CIRCLED& circle);
bool HitPointPixel(const POINTD& pointd, const RECTF& xrc, const DIB& dib);
bool HitPixelPixel(const RECTF& rrect1, const DIB& dib1, const RECTF& rrect2, const DIB& dib2);


//Dynamic
extern bool fullScreenMode_;//全屏模式

namespace EnginePreset {
	extern SIZE resolution;
	extern SIZE clientSize;
	extern HCURSOR cursor;
}

extern bool isMouseMoved;
extern POINTD mousePositionScene;

extern ID2D1DeviceContext* _d2dDeviceContext;
extern IWICImagingFactory* _wicFactory;
void ClientInit();
void EngineRun(PCWSTR wndname);//准备好上面的函数，运行引擎

void LoadPng(const wchar_t* filepath, ID2D1Bitmap** ppd2dpng);
void LoadPng_ReverseFrom(ID2D1Bitmap* source, ID2D1Bitmap** ppd2dpng);
void LoadDib(const wchar_t* filepath, DIB& rdib);
void LoadDib_ReverseFrom(DIB& rdibsource, DIB& rdib);
void LoadPngDib(const wchar_t* filepath, ID2D1Bitmap** ppd2dpng, DIB& rdib);

extern int soundVolume_;//音效大小
extern int musicVolume_;//音效大小
void SoundVolumeAdd();
void SoundVolumeDec();
void PlayXSound(sSoundData& xsounddata);
void PlayXBgm(sSoundData& xsounddata, bool loop = true);
void StopXBgm();

extern sXKeyState key[256];
extern sXKeyState mouseKey[8];
extern sXKeyState* key2[256 + 8];//键鼠按键合集
extern int dMouseZ;
wstring GetKeyName(int xkindex);
int TakeSignalPushedKey();
int TakeSignalUpKey();

//Old
namespace nEngine {
	bool IsKeyPushed();//是否有键按下

	void NormalScreen();
	void FullScreen();
	void NormalFullScreen();

	void LoadSoundData(wchar_t* filepath, sSoundData& xsounddata);
};