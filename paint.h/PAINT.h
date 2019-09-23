// Writen By FoolFish 2465741507
// Last Update on 2019/5/4
// Unfinished

#ifndef __GUI_PAINT
#define __GUI_PAINT

#include<windows.h>
#include<bits/stdc++.h>
using namespace std;

const double PI = acos(-1);
void RotateAnyAngle(HDC hdcDest,int placex,int placey,int SrcWidth,int SrcHeight,HDC hdcSrc,int nXOriginSrc,int nYOriginSrc,float Angle,bool Middle,COLORREF clrBack,bool Trans);
void Char_Print(HDC hdcDest,int nXOriginDest,int nYOriginDest,int nWidthDest,int nHeightDest,HFONT hf,COLORREF crTransparent,const char* wanna_char);
float Get_FPS();

class IMG {
	private :
		bool ready;
	public :
		int width, height;
		HBITMAP img;
		IMG();
		~IMG();
		static IMG* CreateIMG(const char* file);
		static IMG* CreateIMG(const char* file, int width, int height);
		void remove();
		void set(int w, int h);
		void set(char* file);
};

class DC {
	private :
		bool ready;
	public :
		int width, height;
		HDC dc;
		DC();
		~DC();
		static DC* CreateDC(DC* dest);
		static DC* CreateDC(DC* dest, int w, int h);
		static DC* CreateDC(HWND hwnd, int w, int h);
		void remove();
		void set(int w, int h);
		void set(DC* t);
		
		void select(IMG* t);
		
		void draw(DC* t, int x, int y, int px = 0, int py = 0);
		void draw(DC* t, int x, int y, int w, int h, int px = 0, int py = 0);
		void draw(DC* t, int x, int y, IMG* i);
		
		void trans(DC* t, int x, int y, int w, int h, COLORREF c, int px = 0, int py = 0);
		void trans(DC* t, int x, int y, COLORREF c, int px = 0, int py = 0);
		void trans(DC* t, int x, int y, IMG* i, COLORREF c);
		
		void rotate(DC* t, int x, int y, int w, int h, float angle, COLORREF c, int px = 0, int py = 0);
		void rotate(DC* t, int x, int y, int w, int h, float angle, int px = 0, int py = 0);
		void rotate(DC* t, int x, int y, float angle, int px = 0, int py = 0);
		void rotate(DC* t, int x, int y, float angle, COLORREF c, int px = 0, int py = 0);
		void rotate(DC* t, int x, int y, IMG* i, float angle);
		void rotate(DC* t, int x, int y, IMG* i, float angle, COLORREF c);
		
		void scale(DC* t, int x, int y, int w, int h, float mul, int px = 0, int py = 0);
		void scale(DC* t, int x, int y, float mul, int px = 0, int py = 0);
		void scale(DC* t, int x, int y, IMG* i, float mul);
		void scale(DC* t, int x, int y, int w, int h, float mul, COLORREF c, int px = 0, int py = 0);
		void scale(DC* t, int x, int y, float mul, COLORREF c, int px = 0, int py = 0);
		void scale(DC* t, int x, int y, IMG* i, float mul, COLORREF c);
		
		void alpha(DC* t, int x, int y, int w, int h, int a, int px = 0, int py = 0);
		void alpha(DC *t, int x, int y, int a, int px = 0, int py = 0);
		void alpha(DC *t, int x, int y, IMG* i, int a, int px = 0, int py = 0);
		
		void brush(COLORREF c);
		void brush(HBRUSH c);
		
		void print(int x1, int y1, int x2, int y2, char* p, COLORREF c, int size = 24);
};

class GDI_Paint {
	private :
		HWND hwnd;
		DC* main_dc;
		int width, height;
		bool ready;
	public :
		DC* hdc;
		GDI_Paint();
		~GDI_Paint();
		void init(HWND wnd, int w, int h);
		void end();
		
		DC* CreateDC(int w, int h);
		DC* CreateDC();
};

GDI_Paint::GDI_Paint() { width = height = 0; ready = 0;}
GDI_Paint::~GDI_Paint() {
	if(ready) {
		hdc->remove();
		main_dc->remove();
	}
}
void GDI_Paint::init(HWND wnd, int w, int h) {
	if(ready) main_dc->remove(), hdc->remove();
	width = w, height = h;
	hwnd = wnd;
	main_dc = DC::CreateDC(wnd, w, h);
	hdc = DC::CreateDC(main_dc, w, h);
	ready = 1;
}
void GDI_Paint::end() {
	main_dc->draw(hdc, 0, 0);
}

DC* GDI_Paint::CreateDC(int w, int h) {
	return DC::CreateDC(hdc, w, h);
}
DC* GDI_Paint::CreateDC() { return DC::CreateDC(hdc, width, height);}

IMG::IMG() { width = height = 0; img = NULL; ready = 0;}
IMG::~IMG() { if(ready) DeleteObject(img);}
IMG* IMG::CreateIMG(const char* file) {
	IMG* p = new IMG;
	p->img = (HBITMAP)LoadImage(GetModuleHandle(0), file, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	BITMAP temp;
	GetObject(p->img, sizeof(BITMAP), &temp);
	p->width = temp.bmWidth;
	p->height = temp.bmHeight;
	p->ready = 1;
	return p;
}
IMG* IMG::CreateIMG(const char* file, int width, int height) {
	IMG* p = new IMG;
	p->img = (HBITMAP)LoadImage(GetModuleHandle(0), file, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	p->width = width;
	p->height = height;
	p->ready = 1;
	return p;
}
void IMG::remove() {
	if(ready) DeleteObject(img);
	width = height = 0;
	ready = 0;
}
void IMG::set(int w, int h) { width = w, height = h;}
void IMG::set(char* file) { if(ready) remove(); *this = *CreateIMG(file);}

DC::DC() { width = height = 0; dc = NULL; ready = 0;}
DC::~DC() { if(ready) DeleteObject(dc);}
DC* DC::CreateDC(DC* dest) {
	DC* p = new DC;
	p->width = dest->width;
	p->height = dest->height;
	HBITMAP t = CreateCompatibleBitmap(dest->dc, dest->width, dest->height);
	p->dc = CreateCompatibleDC(dest->dc);
	SelectObject(p->dc, t);
	DeleteObject(t);
	p->ready = 1;
	return p;
}
DC* DC::CreateDC(DC* dest, int w, int h) {
	DC* p = new DC;
	p->width = w;
	p->height = h;
	p->dc = CreateCompatibleDC(dest->dc);
	HBITMAP t = CreateCompatibleBitmap(dest->dc, w, h);
	SelectObject(p->dc, t);
	DeleteObject(t);
	p->ready = 1;
	return p;
}
DC* DC::CreateDC(HWND hwnd, int w, int h) {
	DC* p = new DC;
	p->width = w;
	p->height = h;
	p->dc = GetDC(hwnd);
	p->ready = 1;
	return p;
}
void DC::remove() { if(ready) DeleteObject(dc); width = height = 0; ready = 0;}
void DC::set(int w, int h) { width = w, height = h;}
void DC::set(DC* t) { if(ready) remove(); *this = *CreateDC(t, t->width, t->height);}

void DC::select(IMG* t) { SelectObject(dc, t->img);}

void DC::draw(DC* t, int x, int y, int px, int py) {
	BitBlt(dc, x, y, t->width, t->height, t->dc, px, py, SRCCOPY);
}
void DC::draw(DC* t, int x, int y, int w, int h, int px, int py) {
	BitBlt(dc, x, y, w, h, t->dc, px, py, SRCCOPY);
}
void DC::draw(DC* t, int x, int y, IMG* i) {
	t->select(i);
	BitBlt(dc, x, y, i->width, i->height, t->dc, 0, 0, SRCCOPY);
}

void DC::trans(DC* t, int x, int y, int w, int h, COLORREF c, int px, int py) {
	TransparentBlt(dc, x, y, w, h, t->dc, px, py, w, h, c);
}
void DC::trans(DC* t, int x, int y, COLORREF c, int px, int py) {
	trans(t, x, y, t->width, t->height, c, px, py);
}
void DC::trans(DC* t, int x, int y, IMG* i, COLORREF c) {
	t->select(i);
	trans(t, x, y, i->width, i->height, c, 0, 0);
}

void DC::rotate(DC* t, int x, int y, int w, int h, float angle, COLORREF c, int px, int py) {
	RotateAnyAngle(dc, x, y, w, h, t->dc, px, py, angle, 1, c, 1);
}
void DC::rotate(DC* t, int x, int y, int w, int h, float angle, int px, int py) {
	RotateAnyAngle(dc, x, y, w, h, t->dc, px, py, angle, 1, RGB(0, 0, 0), 0);
}
void DC::rotate(DC* t, int x, int y, float angle, int px, int py) {
	rotate(t, x, y, t->width, t->height, angle, px, py);
}
void DC::rotate(DC* t, int x, int y, float angle, COLORREF c, int px, int py) {
	rotate(t, x, y, t->width, t->height, angle, c, px, py);
}
void DC::rotate(DC* t, int x, int y, IMG* i, float angle) {
	t->select(i);
	rotate(t, x, y, i->width, i->height, angle);
}
void DC::rotate(DC* t, int x, int y, IMG* i, float angle, COLORREF c) {
	t->select(i);
	rotate(t, x, y, i->width, i->height, angle, c);
}

void DC::scale(DC* t, int x, int y, int w, int h, float mul, int px, int py) {
	StretchBlt(dc, x, y, (int)(w * mul), (int)(h * mul), t->dc, px, py, w, h, SRCCOPY);
}
void DC::scale(DC* t, int x, int y, float mul, int px, int py) {
	scale(t, x, y, t->width, t->height, mul, px, py);
}
void DC::scale(DC* t, int x, int y, IMG* i, float mul) {
	t->select(i);
	scale(t, x, y, i->width, i->height, mul, 0, 0);
}
void DC::scale(DC* t, int x, int y, int w, int h, float mul, COLORREF c, int px, int py) {
	TransparentBlt(dc, x, y, (int)(w * mul), (int)(h * mul), t->dc, px, py, w, h, c);
}
void DC::scale(DC* t, int x, int y, float mul, COLORREF c, int px, int py) {
	scale(t, x, y, t->width, t->height, mul, c, px, py);
}
void DC::scale(DC* t, int x, int y, IMG* i, float mul, COLORREF c) {
	t->select(i);
	scale(t, x, y, i->width, i->height, mul, c, 0, 0);
}

void DC::alpha(DC* t, int x, int y, int w, int h, int a, int px, int py) {
	BLENDFUNCTION b;
	b.BlendOp = AC_SRC_OVER;
	b.BlendFlags = 0;
	b.SourceConstantAlpha = a;
	b.AlphaFormat = 0;
	AlphaBlend(dc, x, y, w, h, t->dc, px, py, w, h, b);
}
void DC::alpha(DC *t, int x, int y, int a, int px, int py) {
	alpha(t, x, y, t->width, t->height, a, px, py);
}
void DC::alpha(DC *t, int x, int y, IMG* i, int a, int px, int py) {
	t->select(i);
	alpha(t, x, y, i->width, i->height, a, px, py);
}

void DC::brush(COLORREF c) {
	HBRUSH b = CreateSolidBrush(c);
	DC* t = CreateDC(this);
	SelectObject(t->dc, b);
	draw(t, 0, 0);
	DeleteObject(b);
	delete t;
}
void DC::brush(HBRUSH c) {
	DC* t = CreateDC(this);
	SelectObject(t->dc, c);
	draw(t, 0, 0);
	delete t;
}

void DC::print(int x1, int y1, int x2, int y2, char* p, COLORREF c, int size) {
	Char_Print(dc, x1, y1, x2 - x1, y2 - y1,
	           CreateFont(size, 0, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, "Î¢ÈíÑÅºÚ")
	           , c, p);
}

void RotateAnyAngle(HDC hdcDest,int placex,int placey,int SrcWidth,int SrcHeight,HDC hdcSrc,int nXOriginSrc,int nYOriginSrc,float Angle,bool Middle,COLORREF clrBack,bool Trans=true) {
	if(!Angle) {
		if(Trans) TransparentBlt(hdcDest,placex,placey,SrcWidth,SrcHeight,hdcSrc,0,0,SrcWidth,SrcHeight,clrBack);
		else BitBlt(hdcDest,placex,placey,SrcWidth,SrcHeight,hdcSrc,0,0,SRCCOPY);
		return;
	}

	HDC destDC = CreateCompatibleDC( NULL );

	float cosine = (float)cos(Angle/180 * PI);
	float sine = (float)sin(Angle/180 * PI);
	int x1 = (int)(SrcHeight * sine);
	int y1 = (int)(SrcHeight * cosine);
	int x2 = (int)(SrcWidth * cosine + SrcHeight * sine);
	int y2 = (int)(SrcHeight * cosine - SrcWidth * sine);
	int x3 = (int)(SrcWidth * cosine);
	int y3 = (int)(-SrcWidth * sine);
	int minx = min(0,min(x1, min(x2,x3)));
	int miny = min(0,min(y1, min(y2,y3)));
	int maxx = max(0,max(x1, max(x2,x3)));
	int maxy = max(0,max(y1, max(y2,y3)));
	int w = maxx - minx;
	int h = maxy - miny;

	if(Middle) {
		placex -= (w-SrcWidth)/2;
		placey -= (h-SrcHeight)/2;
	}

	HBITMAP hbmResult = CreateCompatibleBitmap(GetDC(NULL), w, h);
	HBITMAP hbmOldDest = (HBITMAP)::SelectObject( destDC, hbmResult );

	if(clrBack != RGB(0,0,0)) {
		HBRUSH hbrBack = CreateSolidBrush( clrBack );
		HBRUSH hbrOld = (HBRUSH)::SelectObject( destDC, hbrBack );
		PatBlt(destDC,0, 0, w, h, PATCOPY );

		DeleteObject( ::SelectObject( destDC, hbrOld ) );
		DeleteObject(hbrBack);
		DeleteObject(hbrOld);
	}

	SetGraphicsMode(destDC, GM_ADVANCED);
	XFORM xform;
	xform.eM11 = cosine;
	xform.eM12 = -sine;
	xform.eM21 = sine;
	xform.eM22 = cosine;
	xform.eDx = (float)-minx;
	xform.eDy = (float)-miny;
	SetWorldTransform( destDC, &xform );

	BitBlt(destDC,0,0,SrcWidth, SrcHeight, hdcSrc, 0, 0, SRCCOPY );

	DeleteObject(hbmOldDest);
	DeleteObject(destDC);

	SelectObject(hdcSrc,hbmResult);
	DeleteObject(hbmResult);

	if(Trans) TransparentBlt(hdcDest,placex,placey,w,h,hdcSrc,0,0,w,h,clrBack);
	else BitBlt(hdcDest,placex,placey,w,h,hdcSrc,0,0,SRCCOPY);
}

void Char_Print(HDC hdcDest,int nXOriginDest,int nYOriginDest,int nWidthDest,int nHeightDest,HFONT hf,COLORREF crTransparent,const char* wanna_char) {
	RECT rect;

	SetRect(&rect,nXOriginDest,nYOriginDest,nXOriginDest+nWidthDest,nYOriginDest+nHeightDest);

	int Last_Mode = SetBkMode(hdcDest,TRANSPARENT);
	COLORREF Last_Color = SetTextColor(hdcDest,crTransparent);

	SelectObject(hdcDest,hf);
	DrawText(hdcDest, wanna_char , -1, &rect, DT_NOCLIP);

	SetBkMode(hdcDest,Last_Mode);
	SetTextColor(hdcDest,Last_Color);

	return;
}

float Get_FPS() {
	static float  fps = 0;
	static int    frameCount = 0;
	static float  currentTime = 0.0f;
	static float  lastTime = 0.0f;

	frameCount++;
	currentTime = clock()*0.001f;

	if(currentTime - lastTime > 1.0f) {
		fps = (float)frameCount /(currentTime - lastTime);
		lastTime = currentTime;
		frameCount    = 0;
	}

	return fps;
}

#endif
