#pragma once

#ifndef DIB_H
#define DIB_H

#ifdef DLL_FILE
#define EX_IM_PORT _declspec(dllexport) //导出类point
#else
#define EX_IM_PORT _declspec(dllimport) //导入类point
#endif

#include <afx.h>
#include <afxwin.h>

class EX_IM_PORT CDib
{
private:
	CString m_fileName;
    BITMAPFILEHEADER* m_lpBmpFileHeader;
    LPBYTE m_lpDib;   
    LPBITMAPINFO m_lpBmpInfo;
    LPBITMAPINFOHEADER m_lpBmpInfoHeader;
    LPRGBQUAD m_lpRgbQuad;
    LPBYTE m_lpData;
    HPALETTE m_hPalette;
    BOOL m_bHasRgbQuad;
    BOOL m_bValid;
	LONG m_rgbsum;

public:
	CDib(void);
	~CDib(void);

	//=======================================================
	// 函数功能： 从文件加载位图
	// 输入参数： LPCTSTR lpszPath-待加载位图文件路径
	// 返回值：   BOOL-TRUE 成功；FALSE 失败
	//=======================================================
	BOOL LoadFromFile(LPCTSTR lpszPath);
	//=======================================================
	// 函数功能： 将位图保存到文件
	// 输入参数： LPCTSTR lpszPath-位图文件保存路径
	// 返回值：   BOOL-TRUE 成功；FALSE 失败
	//=======================================================
	/*BOOL CDib::SaveToFile(LPCTSTR lpszPath)
	{
    // 以写模式打开文件

    HDC hdc,hdcMem;
    HBITMAP hBitMap = NULL;
    CBitmap *pBitMap = NULL;
    CDC *pMemDC = NULL;
    BYTE *pBits;

    hdc = CreateIC(TEXT("DISPLAY"),NULL,NULL,NULL);
    hdcMem = CreateCompatibleDC(hdc);
    hBitMap = CreateDIBSection(hdcMem,m_lpBmpInfo,DIB_PAL_COLORS,(void **)&pBits,NULL,0);
    pBitMap = new CBitmap;
    pBitMap->Attach(hBitMap);
    pMemDC = new CDC;
    pMemDC->Attach(hdcMem);
    pMemDC->SelectObject(pBitMap);
	pMemDC->SetBkMode(TRANSPARENT);
	//pMemDC->SetBkMode(OPAQUE);
    //添加自绘图形 

	m_myview->DrawLine(pMemDC);

	CFile dibFile;
	if(!dibFile.Open(lpszPath, CFile::modeCreate | CFile::modeReadWrite 
		| CFile::shareExclusive))
    {
        return FALSE;
    }

    // 记录位图文件名
    strcpy(m_fileName, lpszPath);

    // 将文件头结构写进文件
    dibFile.Write(m_lpBmpFileHeader, sizeof(BITMAPFILEHEADER));

    // 将文件信息头结构写进文件
    dibFile.Write(m_lpBmpInfoHeader, sizeof(BITMAPINFOHEADER));

    // 计算颜色表长度
    DWORD dwRgbQuadlength = CalcRgbQuadLength();

    // 如果有颜色表的话，将颜色表写进文件
    if(dwRgbQuadlength != 0)
    {
        dibFile.Write(m_lpRgbQuad, dwRgbQuadlength);
    }                                                        

    // 将位图数据写进文件
    DWORD dwDataSize = GetLineByte() * GetHeight();
    dibFile.Write(m_lpData, dwDataSize);

    // 关闭文件
    dibFile.Close();
		
    return TRUE;
}
*/
	//=======================================================
	// 函数功能： 获取位图文件名
	// 输入参数： 无
	// 返回值：   LPCTSTR-位图文件名
	//=======================================================
	LPCTSTR GetFileName();
	//=======================================================
	// 函数功能： 获取位图宽度
	// 输入参数： 无
	// 返回值：   LONG-位图宽度
	//=======================================================
	LONG GetWidth();
	//=======================================================
	// 函数功能： 获取位图高度
	// 输入参数： 无
	// 返回值：   LONG-位图高度
	//=======================================================
	LONG GetHeight();
	//=======================================================
	// 函数功能： 获取位图的宽度和高度
	// 输入参数： 无
	// 返回值：   CSize-位图的宽度和高度
	//=======================================================
	CSize GetDimension();
	//=======================================================
	// 函数功能： 获取位图大小
	// 输入参数： 无
	// 返回值：   DWORD-位图大小
	//=======================================================
	DWORD GetSize();
	//=======================================================
	// 函数功能： 获取单个像素所占位数
	// 输入参数： 无
	// 返回值：   WORD-单个像素所占位数
	//=======================================================
	WORD GetBitCount();
	//=======================================================
	// 函数功能： 获取每行像素所占字节数
	// 输入参数： 无
	// 返回值：   UINT-每行像素所占字节数
	//=======================================================
	UINT GetLineByte();
	//=======================================================
	// 函数功能： 获取位图颜色数
	// 输入参数： 无
	// 返回值：   DWORD-位图颜色数
	//=======================================================
	DWORD GetNumOfColor();
	//=======================================================
	// 函数功能： 计算位图颜色表长度
	// 输入参数： 无
	// 返回值：   DWORD-位图颜色表长度
	//=======================================================
	DWORD CalcRgbQuadLength();
	//=======================================================
	// 函数功能： 计算位图像素的起始位置
	// 输入参数： 无
	// 返回值：   LPBYTE-位图像素的起始位置
	//=======================================================
	LPBYTE GetBits();
	//=======================================================
	// 函数功能： 获取位图颜色表
	// 输入参数： 无
	// 返回值：   LPRGBQUAD-位图颜色表指针
	//=======================================================
	LPRGBQUAD GetRgbQuad();
	//=======================================================
	// 函数功能： 获取位图数据
	// 输入参数： 无
	// 返回值：   LPBYTE-位图数据指针
	//=======================================================
	LPBYTE GetData();
	//=======================================================
	// 函数功能： 根据颜色表生成调色板
	// 输入参数： 无
	// 返回值：   BOOL-TRUE 成功；FALSE 失败
	//=======================================================
	BOOL MakePalette();
	//=======================================================
	// 函数功能： 显示位图
	// 输入参数：
	//            CDC *pDC-设备环境指针
	//            CPoint origin-显示矩形区域的左上角
	//            CSize size-显示矩形区域的尺寸
	// 返回值：
	//            BOOL-TRUE 成功；FALSE 失败
	//=======================================================
	BOOL Draw(CDC *pDC, CPoint origin, CSize size);
	//=======================================================
	// 函数功能： 24位彩色位图转8位灰度位图
	// 输入参数： 无
	// 返回值：   BOOL-TRUE 成功；FALSE 失败
	//=======================================================
	BOOL RgbToGrade();  
	 //=======================================================
	// 函数功能： 8位彩色位图转24位彩色图
	// 输入参数： 无
	// 返回值：   BOOL-TRUE 成功；FALSE 失败
	//=======================================================
	BOOL GraphToRgb();
	//=======================================================
	// 函数功能： 判断是否含有颜色表
	// 输入参数： 无
	// 返回值：   判断结果：TRUE-含有颜色表；FALSE-不含颜色表
	//=======================================================
	BOOL HasRgbQuad();
	//=======================================================
	// 函数功能： 判断是否是灰度图
	// 输入参数： 无
	// 返回值：   判断结果：TRUE-是灰度图；FALSE-是彩色图
	//=======================================================
	BOOL IsGrade();
	//=======================================================
	// 函数功能： 判断位图是否有效
	// 输入参数： 无
	// 返回值：   判断结果：TRUE-位图有效；FALSE-位图无效
	//=======================================================
	BOOL IsValid();
	//=======================================================
	// 函数功能： 清理空间
	// 输入参数： BOOL bFlag-TRUE 全部清空；FALSE 部分清空
	// 返回值：   无
	//=======================================================
	void Empty(BOOL bFlag); 
	BOOL CountClr(int r,int g,int b,int fanwei);  
	BOOL TranslationRight(long lXOffset,long lYOffset);
	BOOL TranslationLeft(long lXOffset,long lYOffset);

private:

};

#endif