#include "ImageExample.h"
#include<fstream>
#include<vector>
#include<algorithm>
HRESULT ImageExample::Init(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	D2DFramework::Init(hInstance, title, width, height);

	return LoadBMP(L"Images/32.bmp", mspBitmap.GetAddressOf());
}

void ImageExample::Render()
{

		// 3. 그리기
	
		mpRenderTarget->BeginDraw();
	
		mpRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));
		if (mspBitmap)
		{
			mpRenderTarget->DrawBitmap(mspBitmap.Get());
		}
		HRESULT hr = mpRenderTarget->EndDraw();
		if (hr == D2DERR_RECREATE_TARGET)
		{
			CreateDeviceResources();
		}
	
}

HRESULT ImageExample::LoadBMP(LPCWSTR filename, ID2D1Bitmap** ppBitmap)
{	
	// 1. BMP 파일 열기
	std::ifstream file;
	file.open(filename, std::ios::binary); //ifstream \ ios

	BITMAPFILEHEADER bfh; 
	BITMAPINFOHEADER bih;

	// 2. BITMAPFILEHEADER
	file.read((char*)&bfh, sizeof(BITMAPFILEHEADER));
	// 3. BITMAPINFOHEADER
	file.read((char*)&bih, sizeof(BITMAPINFOHEADER));

	if (bfh.bfType != 0x4D42)
	{
		OutputDebugString(L"Wrong Bitmap File!!\n");
		return E_FAIL;
	}
	if (bih.biBitCount != 32)
	{
		OutputDebugString(L"RGBA format not found!!\n");
		return E_FAIL;
	}
	// 4. 픽셀 배열 찾아가기 (offset)
	file.seekg(bfh.bfOffBits);

	std::vector<unsigned char> pixels(bih.biSizeImage);

	// 5. 배열 읽기
	int pitch = bih.biWidth * bih.biBitCount / 8;
	
	for (int y = bih.biHeight - 1; y >= 0; y--)
	{
		file.read((char*)&pixels[y*pitch], pitch);
	}

	//file.read((char*)&pixels[0], bih.biSizeImage);

	file.close();

	// 6. ID2DBitmap 만들기
	HRESULT hr = mpRenderTarget->CreateBitmap(
		D2D1::SizeU(bih.biWidth, bih.biHeight),
		D2D1::BitmapProperties(
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)
		),
		ppBitmap
	);
	ThowIfFailed(hr);

	//pixels -> bitmap

	

	//std::reverse(pixels.begin(), pixels.end());

	(*ppBitmap)->CopyFromMemory(
		nullptr,
		&pixels[0],
		pitch);

	return S_OK;
}
