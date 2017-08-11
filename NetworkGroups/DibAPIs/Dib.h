#pragma once

#ifndef DIB_H
#define DIB_H

#ifdef DLL_FILE
#define EX_IM_PORT _declspec(dllexport) //������point
#else
#define EX_IM_PORT _declspec(dllimport) //������point
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
	// �������ܣ� ���ļ�����λͼ
	// ��������� LPCTSTR lpszPath-������λͼ�ļ�·��
	// ����ֵ��   BOOL-TRUE �ɹ���FALSE ʧ��
	//=======================================================
	BOOL LoadFromFile(LPCTSTR lpszPath);
	//=======================================================
	// �������ܣ� ��λͼ���浽�ļ�
	// ��������� LPCTSTR lpszPath-λͼ�ļ�����·��
	// ����ֵ��   BOOL-TRUE �ɹ���FALSE ʧ��
	//=======================================================
	/*BOOL CDib::SaveToFile(LPCTSTR lpszPath)
	{
    // ��дģʽ���ļ�

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
    //����Ի�ͼ�� 

	m_myview->DrawLine(pMemDC);

	CFile dibFile;
	if(!dibFile.Open(lpszPath, CFile::modeCreate | CFile::modeReadWrite 
		| CFile::shareExclusive))
    {
        return FALSE;
    }

    // ��¼λͼ�ļ���
    strcpy(m_fileName, lpszPath);

    // ���ļ�ͷ�ṹд���ļ�
    dibFile.Write(m_lpBmpFileHeader, sizeof(BITMAPFILEHEADER));

    // ���ļ���Ϣͷ�ṹд���ļ�
    dibFile.Write(m_lpBmpInfoHeader, sizeof(BITMAPINFOHEADER));

    // ������ɫ����
    DWORD dwRgbQuadlength = CalcRgbQuadLength();

    // �������ɫ��Ļ�������ɫ��д���ļ�
    if(dwRgbQuadlength != 0)
    {
        dibFile.Write(m_lpRgbQuad, dwRgbQuadlength);
    }                                                        

    // ��λͼ����д���ļ�
    DWORD dwDataSize = GetLineByte() * GetHeight();
    dibFile.Write(m_lpData, dwDataSize);

    // �ر��ļ�
    dibFile.Close();
		
    return TRUE;
}
*/
	//=======================================================
	// �������ܣ� ��ȡλͼ�ļ���
	// ��������� ��
	// ����ֵ��   LPCTSTR-λͼ�ļ���
	//=======================================================
	LPCTSTR GetFileName();
	//=======================================================
	// �������ܣ� ��ȡλͼ���
	// ��������� ��
	// ����ֵ��   LONG-λͼ���
	//=======================================================
	LONG GetWidth();
	//=======================================================
	// �������ܣ� ��ȡλͼ�߶�
	// ��������� ��
	// ����ֵ��   LONG-λͼ�߶�
	//=======================================================
	LONG GetHeight();
	//=======================================================
	// �������ܣ� ��ȡλͼ�Ŀ�Ⱥ͸߶�
	// ��������� ��
	// ����ֵ��   CSize-λͼ�Ŀ�Ⱥ͸߶�
	//=======================================================
	CSize GetDimension();
	//=======================================================
	// �������ܣ� ��ȡλͼ��С
	// ��������� ��
	// ����ֵ��   DWORD-λͼ��С
	//=======================================================
	DWORD GetSize();
	//=======================================================
	// �������ܣ� ��ȡ����������ռλ��
	// ��������� ��
	// ����ֵ��   WORD-����������ռλ��
	//=======================================================
	WORD GetBitCount();
	//=======================================================
	// �������ܣ� ��ȡÿ��������ռ�ֽ���
	// ��������� ��
	// ����ֵ��   UINT-ÿ��������ռ�ֽ���
	//=======================================================
	UINT GetLineByte();
	//=======================================================
	// �������ܣ� ��ȡλͼ��ɫ��
	// ��������� ��
	// ����ֵ��   DWORD-λͼ��ɫ��
	//=======================================================
	DWORD GetNumOfColor();
	//=======================================================
	// �������ܣ� ����λͼ��ɫ����
	// ��������� ��
	// ����ֵ��   DWORD-λͼ��ɫ����
	//=======================================================
	DWORD CalcRgbQuadLength();
	//=======================================================
	// �������ܣ� ����λͼ���ص���ʼλ��
	// ��������� ��
	// ����ֵ��   LPBYTE-λͼ���ص���ʼλ��
	//=======================================================
	LPBYTE GetBits();
	//=======================================================
	// �������ܣ� ��ȡλͼ��ɫ��
	// ��������� ��
	// ����ֵ��   LPRGBQUAD-λͼ��ɫ��ָ��
	//=======================================================
	LPRGBQUAD GetRgbQuad();
	//=======================================================
	// �������ܣ� ��ȡλͼ����
	// ��������� ��
	// ����ֵ��   LPBYTE-λͼ����ָ��
	//=======================================================
	LPBYTE GetData();
	//=======================================================
	// �������ܣ� ������ɫ�����ɵ�ɫ��
	// ��������� ��
	// ����ֵ��   BOOL-TRUE �ɹ���FALSE ʧ��
	//=======================================================
	BOOL MakePalette();
	//=======================================================
	// �������ܣ� ��ʾλͼ
	// ���������
	//            CDC *pDC-�豸����ָ��
	//            CPoint origin-��ʾ������������Ͻ�
	//            CSize size-��ʾ��������ĳߴ�
	// ����ֵ��
	//            BOOL-TRUE �ɹ���FALSE ʧ��
	//=======================================================
	BOOL Draw(CDC *pDC, CPoint origin, CSize size);
	//=======================================================
	// �������ܣ� 24λ��ɫλͼת8λ�Ҷ�λͼ
	// ��������� ��
	// ����ֵ��   BOOL-TRUE �ɹ���FALSE ʧ��
	//=======================================================
	BOOL RgbToGrade();  
	 //=======================================================
	// �������ܣ� 8λ��ɫλͼת24λ��ɫͼ
	// ��������� ��
	// ����ֵ��   BOOL-TRUE �ɹ���FALSE ʧ��
	//=======================================================
	BOOL GraphToRgb();
	//=======================================================
	// �������ܣ� �ж��Ƿ�����ɫ��
	// ��������� ��
	// ����ֵ��   �жϽ����TRUE-������ɫ��FALSE-������ɫ��
	//=======================================================
	BOOL HasRgbQuad();
	//=======================================================
	// �������ܣ� �ж��Ƿ��ǻҶ�ͼ
	// ��������� ��
	// ����ֵ��   �жϽ����TRUE-�ǻҶ�ͼ��FALSE-�ǲ�ɫͼ
	//=======================================================
	BOOL IsGrade();
	//=======================================================
	// �������ܣ� �ж�λͼ�Ƿ���Ч
	// ��������� ��
	// ����ֵ��   �жϽ����TRUE-λͼ��Ч��FALSE-λͼ��Ч
	//=======================================================
	BOOL IsValid();
	//=======================================================
	// �������ܣ� ����ռ�
	// ��������� BOOL bFlag-TRUE ȫ����գ�FALSE �������
	// ����ֵ��   ��
	//=======================================================
	void Empty(BOOL bFlag); 
	BOOL CountClr(int r,int g,int b,int fanwei);  
	BOOL TranslationRight(long lXOffset,long lYOffset);
	BOOL TranslationLeft(long lXOffset,long lYOffset);

private:

};

#endif