#include "XEngine.h"
sI18nString::eLan sI18nString::eLan_ = sI18nString::LAN_EN;

double RECTDS::L()const { return x_; }
double RECTDS::T()const { return y_; }
double RECTDS::R()const { return x_ + w_; }
double RECTDS::B()const { return y_ + h_; }
double RECTDS::CX() const { return x_ + w_ / 2; }
double RECTDS::CY() const { return y_ + h_ / 2; }
POINTD RECTDS::C() const { return { CX() ,CY() }; };
void RECTDS::ML(double l) { x_ = l; };
void RECTDS::MT(double t) { y_ = t; };
void RECTDS::MR(double r) { x_ = r - w_; };
void RECTDS::MB(double b) { y_ = b - h_; }
void RECTDS::MCX(double cx) { x_ = cx - w_ / 2; };
void RECTDS::MCY(double cy) { y_ = cy - h_ / 2; }
void RECTDS::MC(double cx, double cy) { MCX(cx); MCY(cy); };
void RECTDS::ADDCX(double dx) { x_ += dx; }
void RECTDS::ADDCY(double dy) { y_ += dy; }
void RECTDS::Zoom(double multi) { w_ *= multi, h_ *= multi; };
RECTDS RECTDS::GenerateOffset(double dx, double dy) {
	RECTDS rt{ *this };
	rt.x_ += dx;
	rt.y_ += dy;
	return rt;
}
void sERectV::SetV(double vx, double vy) {
	vx_ = vx;
	vy_ = vy;
}
void sERectV::EffectV(double dtime) {
	x_ += vx_ * dtime;
	y_ += vy_ * dtime;
}
void sERectV::DecelerateX(double vdx) {
	double avx = abs(vx_);
	double avx_cal = avx - vdx;
	avx_cal = avx_cal < 0 ? 0 : avx_cal;
	vx_ = vx_ < 0 ? -avx_cal : avx_cal;
}