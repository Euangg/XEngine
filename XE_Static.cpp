#include"XEngine.h"
//私有声明
std::random_device rd;
std::mt19937 gen(rd());


ID2D1SolidColorBrush* _scBrush_ = nullptr;//纯色画刷
void DrawLine(ID2D1RenderTarget* prt, const POSF& p1, const POSF& p2, const COLORF& color, float width) {
	_scBrush_->SetColor(color);
	prt->DrawLine(p1, p2, _scBrush_, width);
}
void DrawRect(ID2D1RenderTarget* prt, const RECTF& rect, const COLORF& color, float width) {
	_scBrush_->SetColor(color);
	prt->DrawRectangle(rect, _scBrush_, width);
}
void FillRect(ID2D1RenderTarget* prt, const RECTF& rect, const COLORF& color) {
	_scBrush_->SetColor(color);
	prt->FillRectangle(rect, _scBrush_);
}
void FillDrawRect(ID2D1RenderTarget* prt, const RECTF& rect, const COLORF& colrf, const COLORF& colrd, float width) {
	FillRect(prt, rect, colrf);
	DrawRect(prt, rect, colrd, width);
}
void DrawEllipse(ID2D1RenderTarget* prt, const ELLIPSEF& ellipse, const COLORF& color, float width) {
	_scBrush_->SetColor(color);
	prt->DrawEllipse(ellipse, _scBrush_, width);
}
void FillEllipse(ID2D1RenderTarget* prt, const ELLIPSEF& ellipse, const COLORF& color) {
	_scBrush_->SetColor(color);
	prt->FillEllipse(ellipse, _scBrush_);
}
void FillDrawEllipse(ID2D1RenderTarget* prt, const ELLIPSEF& ellipse, const COLORF& colrf, const COLORF& colrd, float width) {
	FillEllipse(prt, ellipse, colrf);
	DrawEllipse(prt, ellipse, colrd, width);
}
void DrawCircle(ID2D1RenderTarget* prt, const CIRCLED& c, const COLORF& color, float width) {
	ELLIPSEF e{ {c.p_.x_,c.p_.y_},c.r_,c.r_ };
	DrawEllipse(prt, e, color, width);
};
void FillCircle(ID2D1RenderTarget* prt, const CIRCLED& c, const COLORF& color) {
	ELLIPSEF e{ {c.p_.x_,c.p_.y_},c.r_,c.r_ };
	FillEllipse(prt, e, color);
};
void FillDrawCircle(ID2D1RenderTarget* prt, const CIRCLED& c, const COLORF& colorf, const COLORF& colord, float width) {
	ELLIPSEF e{ {c.p_.x_,c.p_.y_},c.r_,c.r_ };
	FillDrawEllipse(prt, e, colorf, colord, width);
};


IDWriteFactory* _dwriteFactory_ = nullptr;
void ShowText(ID2D1RenderTarget* prt, const wstring& str,const RECTF& rect, const COLORF& color, double size, DWRITE_TEXT_ALIGNMENT talignment, DWRITE_PARAGRAPH_ALIGNMENT palignment) {
	IDWriteTextFormat* _textFormat_;
	_dwriteFactory_->CreateTextFormat(L"Cambria", NULL, DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		size, L"", &_textFormat_);
	_textFormat_->SetTextAlignment(talignment);//设置文字对齐
	_textFormat_->SetParagraphAlignment(palignment);//设置段落对齐
	_textFormat_->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);//设置为单行文字
	_scBrush_->SetColor(color);
	prt->DrawTextW(str.c_str(), str.length(), _textFormat_, rect, _scBrush_);
	_textFormat_->Release();
}
void ShowTextLT(ID2D1RenderTarget* prt, const wstring& str, const RECTF& rect, const COLORF& color, double size) {
	ShowText(prt, str, rect, color, size, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
}
void ShowTextLC(ID2D1RenderTarget* prt, const wstring& str, const RECTF& rect, const COLORF& color, double size) {
	ShowText(prt, str, rect, color, size, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}
void ShowTextLB(ID2D1RenderTarget* prt, const wstring& str, const RECTF& rect, const COLORF& color, double size) {
	ShowText(prt, str, rect, color, size, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_FAR);
}
void ShowTextCT(ID2D1RenderTarget* prt, const wstring& str, const RECTF& rect, const COLORF& color, double size) {
	ShowText(prt, str, rect, color, size, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
}
void ShowTextCB(ID2D1RenderTarget* prt, const wstring& str, const RECTF& rect, const COLORF& color, double size) {
	ShowText(prt, str, rect, color, size, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_FAR);
}
void ShowTextRT(ID2D1RenderTarget* prt, const wstring& str, const RECTF& rect, const COLORF& color, double size) {
	ShowText(prt, str, rect, color, size, DWRITE_TEXT_ALIGNMENT_TRAILING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
}
void ShowTextRC(ID2D1RenderTarget* prt, const wstring& str, const RECTF& rect, const COLORF& color, double size) {
	ShowText(prt, str, rect, color, size, DWRITE_TEXT_ALIGNMENT_TRAILING, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}
void ShowTextRB(ID2D1RenderTarget* prt, const wstring& str, const RECTF& rect, const COLORF& color, double size) {
	ShowText(prt, str, rect, color, size, DWRITE_TEXT_ALIGNMENT_TRAILING, DWRITE_PARAGRAPH_ALIGNMENT_FAR);
}
void ShowText2(ID2D1RenderTarget* prt, const wstring& str, const RECTF& rect, const COLORF& color, double size, DWRITE_TEXT_ALIGNMENT talignment, DWRITE_PARAGRAPH_ALIGNMENT palignment) {
	IDWriteTextFormat* _textFormat_;
	_dwriteFactory_->CreateTextFormat(L"Cambria", NULL, DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		size, L"", &_textFormat_);
	_textFormat_->SetTextAlignment(talignment);//设置文字对齐
	_textFormat_->SetParagraphAlignment(palignment);//设置段落对齐
	_scBrush_->SetColor(color);
	prt->DrawTextW(str.c_str(), str.length(), _textFormat_, rect, _scBrush_);
	_textFormat_->Release();
}

void ShowTextA(ID2D1RenderTarget* prt, const std::string& str, const RECTF& rect, const COLORF& color, double size, DWRITE_TEXT_ALIGNMENT talignment, DWRITE_PARAGRAPH_ALIGNMENT palignment) {
	IDWriteTextFormat* _textFormat_;
	_dwriteFactory_->CreateTextFormat(L"Cambria", NULL, DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		size, L"", &_textFormat_);
	_textFormat_->SetTextAlignment(talignment);//设置文字对齐
	_textFormat_->SetParagraphAlignment(palignment);//设置段落对齐
	_textFormat_->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);//设置为单行文字
	_scBrush_->SetColor(color);
	//prt->DrawTextW(str.c_str(), str.length(), _textFormat_, rect, _scBrush_);
	_textFormat_->Release();
}//目前还不能用

void ShowTextV(ID2D1RenderTarget* prt, const wstring& str, const RECTF& rect, const COLORF& color, double size, DWRITE_TEXT_ALIGNMENT talignment, DWRITE_PARAGRAPH_ALIGNMENT palignment){
	IDWriteTextFormat* _textFormat_;
	_dwriteFactory_->CreateTextFormat(L"Cambria", NULL, DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		size, L"", &_textFormat_);
	//_textFormat_->SetragraphAlignment(palignment);//设置段落对齐
	//_textFormat_->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);//设置为单行文字TextAlignment(talignment);//设置文字对齐
	//_textFormat_->SetPa
	_textFormat_->SetReadingDirection(DWRITE_READING_DIRECTION::DWRITE_READING_DIRECTION_TOP_TO_BOTTOM);
	_textFormat_->SetFlowDirection(DWRITE_FLOW_DIRECTION::DWRITE_FLOW_DIRECTION_TOP_TO_BOTTOM);//设置竖向文字
	_scBrush_->SetColor(color);
	prt->DrawTextW(str.c_str(), str.length(), _textFormat_, rect, _scBrush_);
	_textFormat_->Release();
}

int RandIntIn(int min, int max) {
	std::uniform_int_distribution<int>dist(min, max);
	return dist(gen);
}
float RandFloatIn(float min, float max) {
	std::uniform_real_distribution<float>dist(min, max);
	return dist(gen);
}
bool Chance(double chance1) {
	if (chance1 <= 0)return false;
	else if (chance1 >= 1)return true;
	std::binomial_distribution<int>dist(1, chance1);
	return dist(gen);
}

wstring GetKeyName(int xkindex) {
	wstring keyname = L"";
	switch (xkindex) {
	case DIK_A:keyname = L"A"; break;
	case DIK_B:keyname = L"B"; break;
	case DIK_C:keyname = L"C"; break;
	case DIK_D:keyname = L"D"; break;
	case DIK_E:keyname = L"E"; break;
	case DIK_F:keyname = L"F"; break;
	case DIK_G:keyname = L"G"; break;
	case DIK_H:keyname = L"H"; break;
	case DIK_I:keyname = L"I"; break;
	case DIK_J:keyname = L"J"; break;
	case DIK_K:keyname = L"K"; break;
	case DIK_L:keyname = L"L"; break;
	case DIK_M:keyname = L"M"; break;
	case DIK_N:keyname = L"N"; break;
	case DIK_O:keyname = L"O"; break;
	case DIK_P:keyname = L"P"; break;
	case DIK_Q:keyname = L"Q"; break;
	case DIK_R:keyname = L"R"; break;
	case DIK_S:keyname = L"S"; break;
	case DIK_T:keyname = L"T"; break;
	case DIK_U:keyname = L"U"; break;
	case DIK_V:keyname = L"V"; break;
	case DIK_W:keyname = L"W"; break;
	case DIK_X:keyname = L"X"; break;
	case DIK_Y:keyname = L"Y"; break;
	case DIK_Z:keyname = L"Z"; break;

	case DIK_1:keyname = L"1"; break;
	case DIK_2:keyname = L"2"; break;
	case DIK_3:keyname = L"3"; break;
	case DIK_4:keyname = L"4"; break;
	case DIK_5:keyname = L"5"; break;
	case DIK_6:keyname = L"6"; break;
	case DIK_7:keyname = L"7"; break;
	case DIK_8:keyname = L"8"; break;
	case DIK_9:keyname = L"9"; break;
	case DIK_0:keyname = L"0"; break;

	case DIK_F1:keyname = L"F1"; break;
	case DIK_F2:keyname = L"F2"; break;
	case DIK_F3:keyname = L"F3"; break;
	case DIK_F4:keyname = L"F4"; break;
	case DIK_F5:keyname = L"F5"; break;
	case DIK_F6:keyname = L"F6"; break;
	case DIK_F7:keyname = L"F7"; break;
	case DIK_F8:keyname = L"F8"; break;
	case DIK_F9:keyname = L"F9"; break;
	case DIK_F10:keyname = L"F10"; break;
	case DIK_F11:keyname = L"F11"; break;
	case DIK_F12:keyname = L"F12"; break;

	case DIK_UP:keyname = L"↑"; break;
	case DIK_DOWN:keyname = L"↓"; break;
	case DIK_LEFT:keyname = L"←"; break;
	case DIK_RIGHT:keyname = L"→"; break;
	case DIK_NUMPAD0:keyname = L"NUM 0"; break;
	case DIK_NUMPAD1:keyname = L"NUM 1"; break;
	case DIK_NUMPAD2:keyname = L"NUM 2"; break;
	case DIK_NUMPAD3:keyname = L"NUM 3"; break;
	case DIK_NUMPAD4:keyname = L"NUM 4"; break;
	case DIK_NUMPAD5:keyname = L"NUM 5"; break;
	case DIK_NUMPAD6:keyname = L"NUM 6"; break;
	case DIK_NUMPAD7:keyname = L"NUM 7"; break;
	case DIK_NUMPAD8:keyname = L"NUM 8"; break;
	case DIK_NUMPAD9:keyname = L"NUM 9"; break;
	case DIK_ESCAPE:keyname = L"ESC"; break;
	case DIK_TAB:keyname = L"TAB"; break;
	case DIK_LSHIFT:keyname = L"L SHIFT"; break;
	case DIK_RSHIFT:keyname = L"R SHIFT"; break;
	case DIK_LCONTROL:keyname = L"L CTRL"; break;
	case DIK_RCONTROL:keyname = L"R CTRL"; break;
	case DIK_SPACE:keyname = L"SPACE"; break;
	case DIK_RETURN:keyname = L"ENTER"; break;
	case DIK_NUMPADENTER:keyname = L"NUM ENTER"; break;
	case DIK_BACK:keyname = L"BACK"; break;

	case 256:keyname = L"M1"; break;
	case 256+1:keyname = L"M2"; break;
	case 256+2:keyname = L"M3"; break;
	}
	return keyname;
}

void XSort4(pair<double, char>* List) {//对4个元素排序(从小到大)
#define SWAP(X,Y){t=List[X];List[X]=List[Y];List[Y]=t;}
	pair<double, char> t;
	if (List[0].first > List[2].first)SWAP(0, 2);
	if (List[1].first > List[3].first)SWAP(1, 3);
	if (List[0].first > List[1].first)SWAP(0, 1);
	if (List[2].first > List[3].first)SWAP(2, 3);
	if (List[1].first > List[2].first)SWAP(1, 2);
}
double Distance(const POINTD& point1, const POINTD& point2){
	return sqrt(pow(point1.x_ - point2.x_, 2) + pow(point1.y_ - point2.y_, 2));
}
bool HitRectRect(const RECTF& rrect1, const RECTF& rrect2, RECTF* const p_overlap_rect) {
	//水平方向数据检测
	pair<double, char> hsH[4] = { {rrect1.left,1},{rrect1.right,1},{rrect2.left,2},{rrect2.right,2} };
	XSort4(hsH);
	if (hsH[0].second == hsH[1].second)return false;
	//垂直方向数据检测
	pair<double, char> hsV[4] = { {rrect1.top,1},{rrect1.bottom,1},{rrect2.top,2},{rrect2.bottom,2} };
	XSort4(hsV);
	if (hsV[0].second == hsV[1].second)return false;

	if (p_overlap_rect) {
		p_overlap_rect->left = float(hsH[1].first);
		p_overlap_rect->right = float(hsH[2].first);
		p_overlap_rect->top = float(hsV[1].first);
		p_overlap_rect->bottom = float(hsV[2].first);
	}
	return true;
}
bool HitPointRect(const POINTD& pointd, const RECTF& rectf) {
	if (pointd.x_ >= rectf.left && pointd.x_ <= rectf.right &&
		pointd.y_ >= rectf.top && pointd.y_ <= rectf.bottom)return true;
	return false;
}
bool HitRortRort(RoRECTD& rort1, RoRECTD& rort2) {
	bool hitted = true;

	POINTD p11 = rort1.ROLT();
	POINTD p12 = rort1.RORT();
	POINTD p13 = rort1.RORB();
	POINTD p14 = rort1.ROLB();

	POINTD p21 = rort2.ROLT();
	POINTD p22 = rort2.RORT();
	POINTD p23 = rort2.RORB();
	POINTD p24 = rort2.ROLB();

	POINTD n[4] = {
		{ p12.x_ - p11.x_,p12.y_ - p11.y_ },
		{ p13.x_ - p12.x_,p13.y_ - p12.y_ },
		{ p22.x_ - p21.x_,p22.y_ - p21.y_ },
		{ p23.x_ - p22.x_,p23.y_ - p22.y_ },
	};

	for (int o = 0; o < 4; o++) {
		POINTD& vn = n[o];
		//省略除以向量模长
		double shadow1[4] = {
			(p11.x_ * vn.x_ + p11.y_ * vn.y_),
			(p12.x_ * vn.x_ + p12.y_ * vn.y_),
			(p13.x_ * vn.x_ + p13.y_ * vn.y_),
			(p14.x_ * vn.x_ + p14.y_ * vn.y_),
		};
		double shadow2[4] = {
			(p21.x_ * vn.x_ + p21.y_ * vn.y_),
			(p22.x_ * vn.x_ + p22.y_ * vn.y_),
			(p23.x_ * vn.x_ + p23.y_ * vn.y_),
			(p24.x_ * vn.x_ + p24.y_ * vn.y_),
		};


		double max1 = shadow1[0], min1 = shadow1[0], max2 = shadow2[0], min2 = shadow2[0];
		for (int i = 1; i < 4; i++) {
			max1 < shadow1[i] ? max1 = shadow1[i] : 0;
			min1 > shadow1[i] ? min1 = shadow1[i] : 0;

			max2 < shadow2[i] ? max2 = shadow2[i] : 0;
			min2 > shadow2[i] ? min2 = shadow2[i] : 0;
		}
		min1 >= max2 ? hitted = false : 0;
		min2 >= max1 ? hitted = false : 0;
	}
	return hitted;
}
bool HitRortCircle(RoRECTD& rort1, CIRCLED& circle) {
	bool hitted = true;

	POINTD o{ circle.p_.x_ - rort1.roPoint_.x_,circle.p_.y_ - rort1.roPoint_.y_ };
	double radian = -rort1.roAngle_ * M_PI / 180;
	POINTD newPos{ o.x_ * cos(radian) - o.y_ * sin(radian) + rort1.roPoint_.x_,
		o.x_ * sin(radian) + o.y_ * cos(radian) + rort1.roPoint_.y_ };

	if (newPos.x_ <= rort1.L() - circle.r_) { return false; }
	else if (newPos.x_ <= rort1.L()) {
		if (newPos.y_ <= rort1.T() - circle.r_) { return false; }
		if (newPos.y_ <= rort1.T()) { if (Distance(newPos, { rort1.L(),rort1.T() }) < circle.r_)return true; return false; }
		else if (newPos.y_ < rort1.B()) { return true; }
		else if (newPos.y_ < rort1.B() + circle.r_) { if (Distance(newPos, { rort1.L(),rort1.B() }) < circle.r_)return true; return false; }
		else { return false; }
	}
	else if (newPos.x_ < rort1.R()) {
		if (newPos.y_ <= rort1.T() - circle.r_) { return false; }
		if (newPos.y_ <= rort1.T()) { return true; }
		else if (newPos.y_ < rort1.B()) { return true; }
		else if (newPos.y_ < rort1.B() + circle.r_) { return true; }
		else { return false; }
	}
	else if (newPos.x_ < rort1.R() + circle.r_) {
		if (newPos.y_ <= rort1.T() - circle.r_) { return false; }
		if (newPos.y_ <= rort1.T()) { if (Distance(newPos, { rort1.R(),rort1.T() }) < circle.r_)return true; return false; }
		else if (newPos.y_ < rort1.B()) { return true; }
		else if (newPos.y_ < rort1.B() + circle.r_) { if (Distance(newPos, { rort1.R(),rort1.B() }) < circle.r_)return true; return false; }
		else { return false; }
	}
	else { return false; }

	return false;
}
bool HitPointPixel(const POINTD& pointd, const RECTF& xrc, const DIB& dib) {
	if (pointd.x_ >= xrc.left && pointd.x_ <= xrc.right && pointd.y_ >= xrc.top && pointd.y_ <= xrc.bottom) {
		POINTD pointonrect = { pointd.x_ - xrc.left ,pointd.y_ - xrc.top };

		pointonrect.x_ = pointonrect.x_ * dib.w_ / (xrc.right - xrc.left);
		pointonrect.y_ = pointonrect.y_ * dib.h_ / (xrc.bottom - xrc.top);

		byte alpha = dib.GetAlpha(pointonrect.x_, pointonrect.y_);
		return alpha;
	}
	return false;
}
bool HitPixelPixel(const RECTF& rrect1, const DIB& dib1, const RECTF& rrect2, const DIB& dib2) {
	int flag = 0;//

	RECTF outRect = { 0 };
	if (!HitRectRect(rrect1, rrect2, &outRect))return false;

	//计算出重合的区域的在原图1中的位置
	RECTF overrect = { 0 };
	overrect.left = outRect.left - rrect1.left;
	overrect.right = outRect.right - rrect1.left;
	overrect.top = outRect.top - rrect1.top;
	overrect.bottom = outRect.bottom - rrect1.top;

	//计算图像相对原图的XY缩放倍率
	double scaleX1 = (rrect1.right - rrect1.left) / dib1.w_;
	double scaleY1 = (rrect1.bottom - rrect1.top) / dib1.h_;
	double scaleX2 = (rrect2.right - rrect2.left) / dib2.w_;
	double scaleY2 = (rrect2.bottom - rrect2.top) / dib2.h_;

	//用于存放同一对应位置，1和2的颜色透明度
	byte alpha1 = 0;
	byte alpha2 = 0;
	int ox1 = 0, oy1 = 0;//对应到原图1中的位置
	int ox2 = 0, oy2 = 0;//对应到原图2中的位置
	for (int j = overrect.top; j < overrect.bottom; j++) {
		for (int i = overrect.left; i < overrect.right; i++) {
			//1
			ox1 = i / scaleX1;
			if (ox1 >= dib1.w_)ox1 = dib1.w_ - 1;
			if (ox1 < 0)ox1 = 0;

			oy1 = j / scaleY1;
			if (oy1 >= dib1.h_)oy1 = dib1.h_ - 1;
			if (oy1 < 0)oy1 = 0;

			alpha1 = dib1.GetAlpha(ox1, oy1);
			if (alpha1 == 0)continue;
			//2
			int check2x = i + (INT)(rrect1.left - rrect2.left);
			int check2y = j + (INT)(rrect1.top - rrect2.top);

			ox2 = check2x / scaleX2;
			if (ox2 >= dib2.w_)ox2 = dib2.w_ - 1;
			if (ox2 < 0)ox2 = 0;

			oy2 = check2y / scaleY2;
			if (oy2 >= dib2.h_)oy2 = dib2.h_ - 1;
			if (oy2 < 0)oy2 = 0;

			alpha2 = dib2.GetAlpha(ox2, oy2);
			if (alpha2 == 0)continue;
			else return true;
		}
	}
	return false;
}

void LoadPng(const wchar_t* filepath, ID2D1Bitmap** ppd2dpng) {
	bool flag = false;
	IWICBitmapDecoder* wicDecoder = nullptr;	//解码器，能够从不同格式的图像文件中读取数据并转换为位图的程序
												//（BMP、PNG、ICO、JPEG、GIF、TIFF、WDP）
	IWICBitmapFrameDecode* wicFrame = nullptr;	//帧解码器，可以访问位图的大小、像素格式、分辨率等
	IWICFormatConverter* wicConverter = nullptr;
	//1.创建图像解码器，根据图像文件名
	HRESULT hr = _wicFactory->CreateDecoderFromFilename(filepath,//文件名
		NULL,													//特定解码器厂商，NULL-使用平台提供的默认解码器
		GENERIC_READ,											//文件访问权限
		WICDecodeMetadataCacheOnLoad,							//元数据缓存选项
		&wicDecoder);
	if (hr != S_OK)goto tag_redress;
	//2.创建帧解码器，基于图像解码器
	hr = wicDecoder->GetFrame(0,//要检索的帧的索引
		&wicFrame);
	if (hr != S_OK)goto tag_redress;
	//3.
	hr = _wicFactory->CreateFormatConverter(&wicConverter);
	if (hr != S_OK)goto tag_redress;
	//4.
	hr = wicConverter->Initialize(wicFrame, GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);
	if (hr != S_OK)goto tag_redress;
	//5.
	hr = _d2dDeviceContext->CreateBitmapFromWicBitmap(wicConverter, NULL, ppd2dpng);

	if (hr != S_OK)goto tag_redress;
	flag = true;
tag_redress:
	if (wicDecoder)wicDecoder->Release();
	if (wicConverter)wicConverter->Release();
	if (wicFrame)wicFrame->Release();
	if (flag == false) {
		MessageBox(NULL, filepath, L"资源加载失败", MB_OK);
	}
}
void LoadPng_ReverseFrom(ID2D1Bitmap* source, ID2D1Bitmap** ppd2dpng) {
	UINT width = source->GetPixelSize().width;
	UINT height = source->GetPixelSize().height;

	D2D1_BITMAP_PROPERTIES properties;
	source->GetDpi(&properties.dpiX, &properties.dpiY);
	properties.pixelFormat = source->GetPixelFormat();

	HRESULT hr = _d2dDeviceContext->CreateBitmap(D2D1::SizeU(width, height), properties, ppd2dpng);
	if (hr != S_OK) {
		MessageBoxW(NULL, L"LoadPng_ReverseFrom Failed", L"err", MB_OK);
		return;
	}
	for (UINT x = 0; x < width; x++) {
		D2D1_RECT_U srcRect = D2D1::RectU(x, 0, x + 1, height);
		D2D1_POINT_2U dstPoint = D2D1::Point2U(width - x - 1, 0);
		(*ppd2dpng)->CopyFromBitmap(&dstPoint, source, &srcRect);
	}
}
void LoadDib(const wchar_t* filepath, DIB& rdib) {
	bool flag = false;

	IWICBitmapDecoder* wicDecoder = nullptr;	//解码器，能够从不同格式的图像文件中读取数据并转换为位图的程序
	IWICBitmapFrameDecode* wicFrame = nullptr;	//帧解码器，可以访问位图的大小、像素格式、分辨率等
	IWICFormatConverter* wicConverter = nullptr;
	//1.创建图像解码器，根据图像文件名
	HRESULT hr = _wicFactory->CreateDecoderFromFilename(filepath,//文件名
		NULL,													//特定解码器厂商，NULL-使用平台提供的默认解码器
		GENERIC_READ,											//文件访问权限
		WICDecodeMetadataCacheOnLoad,							//元数据缓存选项
		&wicDecoder);
	if (hr != S_OK)goto tag_redress;
	//2.创建帧解码器，基于图像解码器
	hr = wicDecoder->GetFrame(0,//要检索的帧的索引
		&wicFrame);
	if (hr != S_OK)goto tag_redress;
	//3.创建格式转换器
	hr = _wicFactory->CreateFormatConverter(&wicConverter);
	if (hr != S_OK)goto tag_redress;
	//4.格式转换器关联解码帧
	hr = wicConverter->Initialize(wicFrame, GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);
	if (hr != S_OK)goto tag_redress;
	//载入DIB
	wicFrame->GetSize(&rdib.w_, &rdib.h_);
	rdib.data_ = new byte[rdib.w_ * rdib.h_ * 4];//RGBA
	wicConverter->CopyPixels(nullptr, rdib.w_ * 4, rdib.w_ * rdib.h_ * 4, rdib.data_);

	if (hr != S_OK)goto tag_redress;
	flag = true;
tag_redress:
	if (wicDecoder)wicDecoder->Release();
	if (wicConverter)wicConverter->Release();
	if (wicFrame)wicFrame->Release();
	if (flag == false)MessageBox(NULL, filepath, L"资源加载失败", MB_OK);
}
void LoadDib_ReverseFrom(DIB& rdibsource, DIB& rdib) {
	int w = rdib.w_ = rdibsource.w_;
	int h = rdib.h_ = rdibsource.h_;

	rdib.data_ = new byte[w * h * 4];
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++) {
			rdib.data_[(y * w + x) * 4] = rdibsource.data_[(y * w + w - x - 1) * 4];
			rdib.data_[(y * w + x) * 4 + 1] = rdibsource.data_[(y * w + w - x - 1) * 4 + 1];
			rdib.data_[(y * w + x) * 4 + 2] = rdibsource.data_[(y * w + w - x - 1) * 4 + 2];
			rdib.data_[(y * w + x) * 4 + 3] = rdibsource.data_[(y * w + w - x - 1) * 4 + 3];
		};
};
void LoadPngDib(const wchar_t* filepath, ID2D1Bitmap** ppd2dpng, DIB& rdib) {
	bool flag = false;

	IWICBitmapDecoder* wicDecoder = nullptr;	//解码器，能够从不同格式的图像文件中读取数据并转换为位图的程序
	IWICBitmapFrameDecode* wicFrame = nullptr;	//帧解码器，可以访问位图的大小、像素格式、分辨率等
	IWICFormatConverter* wicConverter = nullptr;
	//1.创建图像解码器，根据图像文件名
	HRESULT hr = _wicFactory->CreateDecoderFromFilename(filepath,//文件名
		NULL,													//特定解码器厂商，NULL-使用平台提供的默认解码器
		GENERIC_READ,											//文件访问权限
		WICDecodeMetadataCacheOnLoad,							//元数据缓存选项
		&wicDecoder);
	if (hr != S_OK)goto tag_redress;
	//2.创建帧解码器，基于图像解码器
	hr = wicDecoder->GetFrame(0,//要检索的帧的索引
		&wicFrame);
	if (hr != S_OK)goto tag_redress;
	//3.创建格式转换器
	hr = _wicFactory->CreateFormatConverter(&wicConverter);
	if (hr != S_OK)goto tag_redress;
	//4.格式转换器关联解码帧
	hr = wicConverter->Initialize(wicFrame, GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);
	if (hr != S_OK)goto tag_redress;
	//d2dBitmap
	hr = _d2dDeviceContext->CreateBitmapFromWicBitmap(wicConverter, NULL, ppd2dpng);
	//载入DIB
	wicFrame->GetSize(&rdib.w_, &rdib.h_);
	rdib.data_ = new byte[rdib.w_ * rdib.h_ * 4];//RGBA
	wicConverter->CopyPixels(nullptr, rdib.w_ * 4, rdib.w_ * rdib.h_ * 4, rdib.data_);

	if (hr != S_OK)goto tag_redress;
	flag = true;
tag_redress:
	if (wicDecoder)wicDecoder->Release();
	if (wicConverter)wicConverter->Release();
	if (wicFrame)wicFrame->Release();
	if (flag == false)MessageBox(NULL, filepath, L"资源加载失败", MB_OK);
}