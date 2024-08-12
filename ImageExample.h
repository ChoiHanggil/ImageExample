#pragma once
#include "D2DFramework.h"
class ImageExample : public D2DFramework
{

public:
	HRESULT Init(HINSTANCE hInstance, LPCWSTR title = L"ImageExample",
		UINT width = 1024, UINT height = 768) override;

	void Render() override;

private:
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBitmap;

	HRESULT LoadBMP(LPCWSTR filename, ID2D1Bitmap** ppBitmap);
};

