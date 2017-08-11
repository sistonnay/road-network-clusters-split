//=====================================================
// �ļ��� Dib.cpp
// ���ݣ� �豸�޹�λͼ��-Դ�ļ�
// ���ܣ� ��1��λͼ�ļ����뱣�棻
//        ��2��λͼ��Ϣ�Ļ�ȡ��
//        ��3��λͼ���ݵĻ�ȡ��
//        ��3��λͼ����ʾ��
//        ��4��λͼ��ת����
//        ��5��λͼ����жϣ�
//======================================================

#ifndef DLL_FILE
#define DLL_FILE
#endif

#include "Dib.h"

//=======================================================
// �������ܣ� ���캯������ʼ�����ݳ�Ա
// ��������� ��
// ����ֵ��   ��
//=======================================================
CDib::CDib(void)
{
    // ���ݳ�Ա��ʼ��
    m_fileName="";
    m_lpBmpFileHeader = NULL;
    m_lpDib = NULL;   
    m_lpBmpInfo = NULL;
    m_lpBmpInfoHeader = NULL;
    m_lpRgbQuad = NULL;
    m_lpData = NULL;
    m_hPalette = NULL;
    m_bHasRgbQuad = FALSE;
    m_bValid = FALSE;
	m_rgbsum=0;
}

//=======================================================
// �������ܣ� �����������ͷ��ڴ�ռ�
// ��������� ��
// ����ֵ��   ��
//=======================================================
CDib::~CDib(void)
{
    // ����ռ�
    Empty(TRUE);
}

//=======================================================
// �������ܣ� ���ļ�����λͼ
// ��������� LPCTSTR lpszPath-������λͼ�ļ�·��
// ����ֵ��   BOOL-TRUE �ɹ���FALSE ʧ��
//=======================================================
BOOL CDib::LoadFromFile(LPCTSTR lpszPath)
{
    // ��¼λͼ�ļ���
    m_fileName=lpszPath;

    // �Զ�ģʽ��λͼ�ļ�
    CFile dibFile;
    if(!dibFile.Open(m_fileName, CFile::modeRead | CFile::shareDenyWrite))
    {
        return FALSE;
    }

    // ����ռ�
    Empty(FALSE); 
    
    // Ϊλͼ�ļ�ͷ����ռ䣬����ʼ��Ϊ0
    m_lpBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
    memset(m_lpBmpFileHeader, 0, sizeof(BITMAPFILEHEADER)); 

    // ��ȡλͼ�ļ�ͷ

    int nCount = dibFile.Read((void *)m_lpBmpFileHeader, sizeof(BITMAPFILEHEADER));
    if(nCount != sizeof(BITMAPFILEHEADER)) 
    {
        return FALSE;
    } 

    // �жϴ��ļ��ǲ���λͼ�ļ�����0x4d42������BM����
    if(m_lpBmpFileHeader->bfType == 0x4d42)
    {
        // ��λͼ�ļ�
        //AfxMessageBox("m_lpBmpFileHeader->bfType == 0x4d42");
        // �����λͼ�ļ�ͷ�Ŀռ��С������ռ䲢��ʼ��Ϊ0
        DWORD dwDibSize = dibFile.GetLength() - sizeof(BITMAPFILEHEADER);
        m_lpDib = new BYTE[dwDibSize];
        //(LPBYTE) new BYTE[lHeight*lineBytes24]
		//m_lpOldDib = new BYTE[dwDibSize];

        memset(m_lpDib, 0, dwDibSize);

        //memset(m_lpOldDib, 0, dwDibSize);
        // ��ȡ��λͼ�ļ�ͷ����������
        dibFile.Read(m_lpDib, dwDibSize);
        //dibFile.Read(m_lpOldDib, dwDibSize);

        // �ر�λͼ�ļ�
        dibFile.Close();

        // ����λͼ��Ϣָ��
        m_lpBmpInfo = (LPBITMAPINFO)m_lpDib;

        // ����λͼ��Ϣͷָ��
        m_lpBmpInfoHeader = (LPBITMAPINFOHEADER)m_lpDib;

        // ����λͼ��ɫ��ָ��
        m_lpRgbQuad = (LPRGBQUAD)(m_lpDib + m_lpBmpInfoHeader->biSize);

        // ���λͼû������λͼʹ�õ���ɫ����������
        if(m_lpBmpInfoHeader->biClrUsed == 0)
        {
            m_lpBmpInfoHeader->biClrUsed = GetNumOfColor();
            
			//AfxMessageBox("24λ");

			
			//CString strColor;
			//strColor.Format("m_lpBmpInfoHeader->biClrUsed :%d",m_lpBmpInfoHeader->biClrUsed);
			//AfxMessageBox(strColor);

        }

        // ������ɫ����
        DWORD dwRgbQuadLength = CalcRgbQuadLength();

        //m_lpBmpInfoHeader->biPlanes = 1;

		//CString strColor;
		//strColor.Format("m_lpBmpInfoHeader->biCompression :%d", m_lpBmpInfoHeader->biPlanes);
		//AfxMessageBox(strColor);


        // ����λͼ����ָ��
        //m_lpData = m_lpDib + m_lpBmpInfoHeader->biSize + dwRgbQuadLength;
		m_lpData = m_lpDib + m_lpBmpInfoHeader->biSize + dwRgbQuadLength;
        //m_lpOldData= m_lpOldDib + ((LPBITMAPINFOHEADER)m_lpOldDib)->biSize + dwRgbQuadLength;
        // �ж��Ƿ�����ɫ��
        if(m_lpRgbQuad == (LPRGBQUAD)m_lpData)
        {
            m_lpRgbQuad = NULL;    // ��λͼ��ɫ��ָ���ÿ�
            m_bHasRgbQuad = FALSE; // ����ɫ��
	        //AfxMessageBox("����ɫ��");
        }
        else
        {
            m_bHasRgbQuad = TRUE;  // ����ɫ��
            MakePalette();         // ������ɫ�����ɵ�ɫ��

			//AfxMessageBox("�������ɵ�ɫ��");
        }        

        // ����λͼ��С����Ϊ�ܶ�λͼ�ļ��������ô��
        m_lpBmpInfoHeader->biSizeImage = GetSize();

        // λͼ��Ч
        m_bValid = TRUE;

        return TRUE;
    }
    else
    {
        // ����λͼ�ļ�
		AfxMessageBox("��ͼ����λͼ��");
        m_bValid = FALSE;

        return FALSE;
    }     

}

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
LPCTSTR CDib::GetFileName()
{
    return m_fileName;
}

//=======================================================
// �������ܣ� ��ȡλͼ���
// ��������� ��
// ����ֵ��   LONG-λͼ���
//=======================================================
LONG CDib::GetWidth()
{
    return m_lpBmpInfoHeader->biWidth;
}

//=======================================================
// �������ܣ� ��ȡλͼ�߶�
// ��������� ��
// ����ֵ��   LONG-λͼ�߶�
//=======================================================
LONG CDib::GetHeight()
{
    return m_lpBmpInfoHeader->biHeight;
}

//=======================================================
// �������ܣ� ��ȡλͼ�Ŀ�Ⱥ͸߶�
// ��������� ��
// ����ֵ��   CSize-λͼ�Ŀ�Ⱥ͸߶�
//=======================================================
CSize CDib::GetDimension()
{
    return CSize(GetWidth(), GetHeight());
}

//=======================================================
// �������ܣ� ��ȡλͼ��С
// ��������� ��
// ����ֵ��   DWORD-λͼ��С
//=======================================================
DWORD CDib::GetSize()
{
    if(m_lpBmpInfoHeader->biSizeImage != 0)
    {
        return m_lpBmpInfoHeader->biSizeImage;
    }
    else
    {       
        return GetWidth() * GetHeight();
    }
}

//=======================================================
// �������ܣ� ��ȡ����������ռλ��
// ��������� ��
// ����ֵ��   WORD-����������ռλ��
//=======================================================
WORD CDib::GetBitCount()
{
    return m_lpBmpInfoHeader->biBitCount;
}       

//=======================================================
// �������ܣ� ��ȡÿ��������ռ�ֽ���
// ��������� ��
// ����ֵ��   UINT-ÿ��������ռ�ֽ���
//=======================================================
UINT CDib::GetLineByte()
{ 
    return (GetWidth() * GetBitCount() /8 + 3) / 4 * 4;
}

//=======================================================
// �������ܣ� ��ȡλͼ��ɫ��
// ��������� ��
// ����ֵ��   DWORD-λͼ��ɫ��
//=======================================================
DWORD CDib::GetNumOfColor()
{
    UINT dwNumOfColor;     

    if ((m_lpBmpInfoHeader->biClrUsed == 0) 
        && (m_lpBmpInfoHeader->biBitCount < 9))
	{
		switch (m_lpBmpInfoHeader->biBitCount)
		{
		    case 1: dwNumOfColor = 2; break;
		    case 4: dwNumOfColor = 16; break;
		    case 8: dwNumOfColor = 256;
		}
	}
    else
    {
        dwNumOfColor = m_lpBmpInfoHeader->biClrUsed;
		
		//CString strColor;
		//strColor.Format("dwNumOfColor:%d",dwNumOfColor);
		//AfxMessageBox(strColor);
    }  		

    return dwNumOfColor; 
}

//=======================================================
// �������ܣ� ����λͼ��ɫ����
// ��������� ��
// ����ֵ��   DWORD-λͼ��ɫ����
//=======================================================
DWORD CDib::CalcRgbQuadLength()
{
    DWORD dwNumOfColor = GetNumOfColor();
    if(dwNumOfColor > 256)
    {
        dwNumOfColor = 0;
    }

	CString strColor;
	strColor.Format("dwNumOfColor:%d",dwNumOfColor);
	//AfxMessageBox(strColor);
    return  dwNumOfColor * sizeof(RGBQUAD);
}

//=======================================================
// �������ܣ� ����λͼ���ص���ʼλ��
// ��������� ��
// ����ֵ��   LPBYTE-λͼ���ص���ʼλ��
//=======================================================
LPBYTE CDib::GetBits()
{
   return (m_lpDib+((LPBITMAPINFOHEADER)m_lpDib)->biSize+CalcRgbQuadLength());
}

//=======================================================
// �������ܣ� ��ȡλͼ��ɫ��
// ��������� ��
// ����ֵ��   LPRGBQUAD-λͼ��ɫ��ָ��
//=======================================================
LPRGBQUAD CDib::GetRgbQuad()
{
    return m_lpRgbQuad;
}

//=======================================================
// �������ܣ� ��ȡλͼ����
// ��������� ��
// ����ֵ��   LPBYTE-λͼ����ָ��
//=======================================================
LPBYTE CDib::GetData()
{
    return m_lpData;
}

//=======================================================
// �������ܣ� ������ɫ�����ɵ�ɫ��
// ��������� ��
// ����ֵ��   BOOL-TRUE �ɹ���FALSE ʧ��
//=======================================================
BOOL CDib::MakePalette()
{
    // ������ɫ����
    DWORD dwRgbQuadLength = CalcRgbQuadLength();



	CString strColor;
		strColor.Format("dwRgbQuadLength:%d",dwRgbQuadLength);
		//AfxMessageBox(strColor);



    // �����ɫ����Ϊ0���������߼���ɫ��
	if(dwRgbQuadLength == 0) 
    {
        return FALSE;
    }

	//ɾ���ɵĵ�ɫ�����
	if(m_hPalette != NULL) 
    {
        DeleteObject(m_hPalette);
        m_hPalette = NULL;
    }

	// ���뻺��������ʼ��Ϊ0
    DWORD dwNumOfColor = GetNumOfColor();
    DWORD dwSize = 2 * sizeof(WORD) + dwNumOfColor * sizeof(PALETTEENTRY);
	LPLOGPALETTE lpLogPalette = (LPLOGPALETTE) new BYTE[dwSize];
    memset(lpLogPalette, 0, dwSize);

    // �����߼���ɫ��
	lpLogPalette->palVersion = 0x300;
	lpLogPalette->palNumEntries = dwNumOfColor;
	LPRGBQUAD lpRgbQuad = (LPRGBQUAD) m_lpRgbQuad;
	for(int i = 0; i < dwNumOfColor; i++) 
    {
		lpLogPalette->palPalEntry[i].peRed = lpRgbQuad->rgbRed;
		lpLogPalette->palPalEntry[i].peGreen = lpRgbQuad->rgbGreen;
		lpLogPalette->palPalEntry[i].peBlue = lpRgbQuad->rgbBlue;
		lpLogPalette->palPalEntry[i].peFlags = 0;
		lpRgbQuad++;
	}

	// �����߼���ɫ��
	m_hPalette = CreatePalette(lpLogPalette);

	// �ͷŻ�����
	delete [] lpLogPalette;

    return TRUE;
}

//=======================================================
// �������ܣ� ��ʾλͼ
// ���������
//            CDC *pDC-�豸����ָ��
//            CPoint origin-��ʾ������������Ͻ�
//            CSize size-��ʾ��������ĳߴ�
// ����ֵ��
//            BOOL-TRUE �ɹ���FALSE ʧ��
//=======================================================
BOOL CDib::Draw(CDC *pDC, CPoint origin, CSize size)
{
    // λͼ��Ч���޷����ƣ����ش���
    if(!IsValid())
    {
        return FALSE;
    }

    // �ɵĵ�ɫ����
	HPALETTE hOldPalette = NULL;

	// ���λͼָ��Ϊ�գ��򷵻�FALSE
	if(m_lpDib == NULL) 
    {
        return FALSE;
    }

	// ���λͼ�е�ɫ�壬��ѡ���豸������
	if(m_hPalette != NULL) 
    {
		hOldPalette = SelectPalette(pDC->GetSafeHdc(), m_hPalette, TRUE);
	}

	// ����λͼ����ģʽ
	pDC->SetStretchBltMode(COLORONCOLOR);

	// ��λͼ��pDC��ָ����豸�Ͻ�����ʾ
	StretchDIBits(pDC->GetSafeHdc(), origin.x, origin.y, size.cx, size.cy,
		0, 0, GetWidth(), GetHeight(), m_lpData, m_lpBmpInfo, DIB_RGB_COLORS, SRCCOPY);

	// �ָ��ɵĵ�ɫ��
	if(hOldPalette != NULL)
    {
        SelectPalette(pDC->GetSafeHdc(), hOldPalette, TRUE);
    }

    return TRUE;
}

//=======================================================
// �������ܣ� 24λ��ɫλͼת8λ�Ҷ�λͼ
// ��������� ��
// ����ֵ��   BOOL-TRUE �ɹ���FALSE ʧ��
//=======================================================
BOOL CDib::RgbToGrade()
{
    // λͼ��Ч��ʧ�ܷ���
    if(!IsValid())
    {
        AfxMessageBox("��λͼ��Ч��");
		return FALSE;
    }

    // ��ѹ��λͼ��ʧ�ܷ���
    if(m_lpBmpInfoHeader->biCompression != BI_RGB)
    {
        return FALSE;
    }

    // ������ǻҶ�λͼ������Ҫת��
    if(!IsGrade())
    {
        // ��ȡԭλͼ��Ϣ
        LONG lHeight = GetHeight();
        LONG lWidth = GetWidth();
        UINT uLineByte = GetLineByte();

        // ����Ҷ�λͼ��������ռ�
        UINT uGradeBmpLineByte = (lWidth * 8 / 8 + 3) / 4 * 4;
        DWORD dwGradeBmpDataSize = uGradeBmpLineByte * lHeight; 

        // ����Ҷ�λͼ����ռ�
        DWORD dwGradeBmpSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 + dwGradeBmpDataSize;

        // ���ûҶ�λͼ�ļ�ͷ
        LPBITMAPFILEHEADER lpGradeBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
        memset(lpGradeBmpFileHeader, 0, sizeof(BITMAPFILEHEADER));
        lpGradeBmpFileHeader->bfType = 0x4d42;
        lpGradeBmpFileHeader->bfSize = sizeof(BITMAPFILEHEADER) + dwGradeBmpSize;
        lpGradeBmpFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
                                          + sizeof(RGBQUAD) * 256;
        lpGradeBmpFileHeader->bfReserved1 = 0;
        lpGradeBmpFileHeader->bfReserved2 = 0;            

        // Ϊ�Ҷ�λͼ����ռ䣬����ʼ��Ϊ0
        LPBYTE lpGradeBmp = (LPBYTE)new BYTE[dwGradeBmpSize];
        memset(lpGradeBmp, 0, dwGradeBmpSize);

        // ���ûҶ�λͼ��Ϣͷ
        LPBITMAPINFOHEADER lpGradeBmpInfoHeader = (LPBITMAPINFOHEADER)(lpGradeBmp);
        lpGradeBmpInfoHeader->biBitCount = 8;
        lpGradeBmpInfoHeader->biClrImportant = 0;
        lpGradeBmpInfoHeader->biClrUsed = 256;
        lpGradeBmpInfoHeader->biCompression = BI_RGB;
        lpGradeBmpInfoHeader->biHeight = lHeight;
        lpGradeBmpInfoHeader->biPlanes = 1;
        lpGradeBmpInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
        lpGradeBmpInfoHeader->biSizeImage = dwGradeBmpDataSize;
        lpGradeBmpInfoHeader->biWidth = lWidth;
        lpGradeBmpInfoHeader->biXPelsPerMeter = m_lpBmpInfoHeader->biXPelsPerMeter;
        lpGradeBmpInfoHeader->biYPelsPerMeter = m_lpBmpInfoHeader->biYPelsPerMeter;

        // ���ûҶ�λͼ��ɫ��
        LPRGBQUAD lpGradeBmpRgbQuad = (LPRGBQUAD)(lpGradeBmp + sizeof(BITMAPINFOHEADER));

        // ��ʼ��8λ�Ҷ�ͼ�ĵ�ɫ����Ϣ
        LPRGBQUAD lpRgbQuad;               
        for(int k = 0; k < 256; k++)
        {
            lpRgbQuad = (LPRGBQUAD)(lpGradeBmpRgbQuad + k);
            lpRgbQuad->rgbBlue = k; 
            lpRgbQuad->rgbGreen = k;
            lpRgbQuad->rgbRed = k;
            lpRgbQuad->rgbReserved = 0;
        }

        // �Ҷ�λͼ���ݴ���
        BYTE r, g, b; 
        LPBYTE lpGradeBmpData = (LPBYTE)(lpGradeBmp + sizeof(BITMAPINFOHEADER) 
                                         + sizeof(RGBQUAD) * 256);
        // ������ɫת��
        for(int i = 0; i < lHeight; i++)
        {
            for(int j = 0; j < lWidth; j++)
            {
                b = m_lpData[i * uLineByte + 3 * j];
                g = m_lpData[i * uLineByte + 3 * j + 1];
                r = m_lpData[i * uLineByte + 3 * j + 2];
                lpGradeBmpData[i * uGradeBmpLineByte + j] = (BYTE)(0.299 * r + 0.587 * g + 0.114 * b); 
            }
        }

        // �ͷ�ԭ��λͼ�ռ�
        Empty(FALSE);

        // �����趨ԭλͼָ��ָ��
        m_lpBmpFileHeader = lpGradeBmpFileHeader;
        m_lpDib = lpGradeBmp;
        m_lpBmpInfo = (LPBITMAPINFO)(lpGradeBmp);
        m_lpBmpInfoHeader = lpGradeBmpInfoHeader;
        m_lpRgbQuad = lpGradeBmpRgbQuad;
        m_lpData = lpGradeBmpData;

        // ������ɫ���־
        m_bHasRgbQuad = TRUE;  
        // ����λͼ��Ч��־
        m_bValid = TRUE;
        // ���ɵ�ɫ��
        MakePalette();

    }
    AfxMessageBox("ת���ɹ����뱣��8λ�Ҷ�ͼ��");
    return TRUE;   
}   
 //=======================================================
// �������ܣ� 8λ��ɫλͼת24λ��ɫͼ
// ��������� ��
// ����ֵ��   BOOL-TRUE �ɹ���FALSE ʧ��
//=======================================================
BOOL CDib::GraphToRgb()
{
    // λͼ��Ч��ʧ�ܷ���
    if(!IsValid())
    {
        AfxMessageBox("��λͼ��Ч��");
		return FALSE;
    }

    // ��ѹ��λͼ��ʧ�ܷ���
    if(m_lpBmpInfoHeader->biCompression != BI_RGB)
    {
        return FALSE;
    }

	if(GetBitCount()==24)
	{
		AfxMessageBox("��λͼ����24λ������ת��!");
	    return FALSE;
	}
	if(GetBitCount()!=8)
	{
		AfxMessageBox("��λͼ����8λ������ת�������û�ͼ����������Ϊ��24λͼ!");
	    return FALSE;
	}
    // �������24λͼ������Ҫת��
    
        // ��ȡԭλͼ��Ϣ
        LONG lHeight = GetHeight();
        LONG lWidth = GetWidth();
        UINT uLineByte = GetLineByte();

        // ����24λͼ��������ռ�
		//return (GetWidth() * GetBitCount() /8 + 3) / 4 * 4;

        UINT uGradeBmpLineByte = (lWidth * 24/8  + 3) / 4 * 4;
        DWORD dwGradeBmpDataSize = uGradeBmpLineByte * lHeight; 

        // ����24λͼ����ռ�
        DWORD dwGradeBmpSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 0 + dwGradeBmpDataSize;

        // ����24λͼ�ļ�ͷ
        LPBITMAPFILEHEADER lpGradeBmpFileHeader = (LPBITMAPFILEHEADER)new BYTE[sizeof(BITMAPFILEHEADER)];
        memset(lpGradeBmpFileHeader, 0, sizeof(BITMAPFILEHEADER));

        lpGradeBmpFileHeader->bfType = 0x4d42;
        lpGradeBmpFileHeader->bfSize = sizeof(BITMAPFILEHEADER) + dwGradeBmpSize;

        lpGradeBmpFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
                      
        lpGradeBmpFileHeader->bfReserved1 = 0;
        lpGradeBmpFileHeader->bfReserved2 = 0;            

        // Ϊ24λͼ����ռ䣬����ʼ��Ϊ0
        LPBYTE lpGradeBmp = (LPBYTE)new BYTE[dwGradeBmpSize];
        memset(lpGradeBmp, 0, dwGradeBmpSize);

        // ����24λͼ��Ϣͷ
        LPBITMAPINFOHEADER lpGradeBmpInfoHeader = (LPBITMAPINFOHEADER)(lpGradeBmp);
        lpGradeBmpInfoHeader->biBitCount = 24;
        lpGradeBmpInfoHeader->biClrImportant = 0;
        lpGradeBmpInfoHeader->biClrUsed = 0;
        lpGradeBmpInfoHeader->biCompression = BI_RGB;
        lpGradeBmpInfoHeader->biHeight = lHeight;
        lpGradeBmpInfoHeader->biPlanes = 1;
        lpGradeBmpInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
        lpGradeBmpInfoHeader->biSizeImage = dwGradeBmpDataSize;
        lpGradeBmpInfoHeader->biWidth = lWidth;
        lpGradeBmpInfoHeader->biXPelsPerMeter = 0;
        lpGradeBmpInfoHeader->biYPelsPerMeter = 0;

        // ����24λͼ��ɫ��
        //LPRGBQUAD lpGradeBmpRgbQuad = (LPRGBQUAD)(lpGradeBmp + sizeof(BITMAPINFOHEADER));

        // ��ʼ��8λ�Ҷ�ͼ�ĵ�ɫ����Ϣ
        /*LPRGBQUAD lpRgbQuad;               
        for(int k = 0; k < 256; k++)
        {
            lpRgbQuad = (LPRGBQUAD)(lpGradeBmpRgbQuad + k);
            lpRgbQuad->rgbBlue = k; 
            lpRgbQuad->rgbGreen = k;
            lpRgbQuad->rgbRed = k;
            lpRgbQuad->rgbReserved = 0;
        }*/

        // 24λͼ���ݴ���


        //BYTE r, g, b; 
        //LPBYTE lpGradeBmpData = (LPBYTE)(lpGradeBmp + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 0);
                     
        
	
        // ������ɫת��
        LONG lineBytes24=(lWidth * 24 / 8 + 3) / 4 * 4;
		LONG lineBytes8=(lWidth * 8 / 8 + 3) / 4 * 4;

		//LONG lineBytes24=(lWidth * 24 + 31) / 32 * 4;
		//LONG lineBytes8=(lWidth * 8  + 31) / 32 * 4;



		//CString strColor;
		//strColor.Format("lineBytes24 :%d,lineBytes8 :%d", lineBytes24,lineBytes8);
		//AfxMessageBox(strColor);




        LPBYTE lpGradeBmpData =(LPBYTE) new BYTE[lHeight*lineBytes24];

		for(int i = 0; i < lHeight; i++)
        {
            for(int j = 0,n=0; j < lineBytes8; j++,n++)
            {
                BYTE gray=*(m_lpData+lineBytes8*i+j);
 
				*(lpGradeBmpData+lineBytes24*i+n)=gray;
				n++;
				*(lpGradeBmpData+lineBytes24*i+n)=gray;
				n++;
				*(lpGradeBmpData+lineBytes24*i+n)=gray;
			    
               	/*lpGradeBmpData[lineBytes24*i+j  ]=  m_lpData[lineBytes8*i+j];
				lpGradeBmpData[lineBytes24*i+j+1]=  m_lpData[lineBytes8*i+j];
				lpGradeBmpData[lineBytes24*i+j+2]=  m_lpData[lineBytes8*i+j];*/



            }
        }
        // �ͷ�ԭ��λͼ�ռ�
        	//AfxMessageBox("ת���ɹ�");
		
		
		Empty(FALSE);

        // �����趨ԭλͼָ��ָ��
        m_lpBmpFileHeader = lpGradeBmpFileHeader;
        m_lpDib = lpGradeBmp;
        m_lpBmpInfo = (LPBITMAPINFO)(lpGradeBmp);
        m_lpBmpInfoHeader = lpGradeBmpInfoHeader;
        //m_lpRgbQuad = lpGradeBmpRgbQuad;
        m_lpData = lpGradeBmpData;

        // ������ɫ���־
        //m_bHasRgbQuad = TRUE;  
        // ����λͼ��Ч��־
        m_bValid = TRUE;
        // ���ɵ�ɫ��
        //MakePalette();

    AfxMessageBox("ת���ɹ����뱣��24λͼ��");
    return TRUE;   
}   
//=======================================================
// �������ܣ� �ж��Ƿ�����ɫ��
// ��������� ��
// ����ֵ��   �жϽ����TRUE-������ɫ��FALSE-������ɫ��
//=======================================================
BOOL CDib::HasRgbQuad()
{
    return m_bHasRgbQuad;
}

//=======================================================
// �������ܣ� �ж��Ƿ��ǻҶ�ͼ
// ��������� ��
// ����ֵ��   �жϽ����TRUE-�ǻҶ�ͼ��FALSE-�ǲ�ɫͼ
//=======================================================
BOOL CDib::IsGrade()
{
    return (GetBitCount() < 9 && GetBitCount() > 0);
}

//=======================================================
// �������ܣ� �ж�λͼ�Ƿ���Ч
// ��������� ��
// ����ֵ��   �жϽ����TRUE-λͼ��Ч��FALSE-λͼ��Ч
//=======================================================
BOOL CDib::IsValid()
{
    return m_bValid;
}

//=======================================================
// �������ܣ� ����ռ�
// ��������� BOOL bFlag-TRUE ȫ����գ�FALSE �������
// ����ֵ��   ��
//=======================================================
void CDib::Empty(BOOL bFlag)
{
    // �ļ������
    if(bFlag)
    {
        m_fileName="";
    }      

    // �ͷ�λͼ�ļ�ͷָ��ռ�
    if(m_lpBmpFileHeader != NULL)
    {
        delete [] m_lpBmpFileHeader;
        m_lpBmpFileHeader = NULL;
    }    

    // �ͷ�λͼָ��ռ�
    if(m_lpDib != NULL)
    {
        delete [] m_lpDib;
        m_lpDib = NULL;
        m_lpBmpInfo = NULL;
        m_lpBmpInfoHeader = NULL;
        m_lpRgbQuad = NULL;
        m_lpData = NULL;           
    }       

    // �ͷŵ�ɫ��
    if(m_hPalette != NULL)
    {
        DeleteObject(m_hPalette);
        m_hPalette = NULL;
    }    

    // ���ò�����ɫ��
    m_bHasRgbQuad = FALSE;
    
    // ����λͼ��Ч
    m_bValid = FALSE;

}  

BOOL CDib::CountClr(int r,int g,int b,int fanwei)
{
    m_rgbsum=0;
	
	// λͼ��Ч��ʧ�ܷ���
    if(!IsValid())
    {
        AfxMessageBox("��λͼ��Ч��");
		return FALSE;
    }

	if(GetBitCount()!=24)
	{
		
		CString str;
		str.Format("��λͼ��%dλ���޷�ͳ�ƣ����û�ͼ����������Ϊ��24λͼ!",GetBitCount());
		AfxMessageBox(str);
	    return FALSE;
	}


    // ��ѹ��λͼ��ʧ�ܷ���
    if(m_lpBmpInfoHeader->biCompression != BI_RGB)
    {
        return FALSE;
    }

    //LoadFromFile((LPCTSTR)m_fileName);
    // ��ȡԭλͼ��Ϣ
    LONG lHeight=GetHeight();
	LONG lWidth=GetWidth();
	UINT uLineByte=GetLineByte();

	// �Ҷ�λͼ���ݴ���
	int R=0, G=0, B=0;
	//int R1=0, G1=0, B1=0;
	// ������ɫת��
    int clrfanwei;
	int clrjizhun=0.299 * r + 0.587 * g + 0.114 * b;//��׼�Ҷ�


	for(int i = 0; i < lHeight; i++)
	{
		for(int j = 0; j < lWidth; j++)
		{
			//(BYTE)(0.299 * r + 0.587 * g + 0.114 * b); 
			B = *(m_lpData+i * uLineByte + 3 * j);
			G = *(m_lpData+i * uLineByte + 3 * j + 1);
			R = *(m_lpData+i * uLineByte + 3 * j + 2);

			int someonegray=0.299 * R + 0.587 * G + 0.114 * B;

			if(clrjizhun<128)//�궨��ɫ����Ϊȱ��
			{
	            clrfanwei=clrjizhun+fanwei;
                if(clrfanwei>255)
	            clrfanwei=255;
				if(someonegray<=clrfanwei && someonegray>=clrjizhun)
				{
				   if(r==255 && g==0 && b==0)
				   {
				      *(m_lpData+i * uLineByte + 3 * j)=255;
			          *(m_lpData+i * uLineByte + 3 * j + 1)=0;
			          *(m_lpData+i * uLineByte + 3 * j + 2)=0;
				   }
				   else
				   {
				   	  *(m_lpData+i * uLineByte + 3 * j)=0;
			          *(m_lpData+i * uLineByte + 3 * j + 1)=0;
			          *(m_lpData+i * uLineByte + 3 * j + 2)=255;
				   }
                   m_rgbsum++;					   
				}
 	    
			}
 

           else//�궨ǳɫ����Ϊȱ��
		   {
	        	clrfanwei=clrjizhun-fanwei;
            	if(clrfanwei<0)
                clrfanwei=0;
				if(someonegray>=clrfanwei && someonegray<=clrjizhun)
				{
			       if(r==255 && g==0 && b==0)
				   {
				      *(m_lpData+i * uLineByte + 3 * j)=255;
			          *(m_lpData+i * uLineByte + 3 * j + 1)=0;
			          *(m_lpData+i * uLineByte + 3 * j + 2)=0;
				   }
				   else
				   {
				   	  *(m_lpData+i * uLineByte + 3 * j)=0;
			          *(m_lpData+i * uLineByte + 3 * j + 1)=0;
			          *(m_lpData+i * uLineByte + 3 * j + 2)=255;
				   }
                   m_rgbsum++;					   
				}
		   }   
			
		}
	}
return TRUE;
}   


BOOL CDib::TranslationRight(long lXOffset,long lYOffset)
 {
  
  long i;
  long j;
  long lWidth,lHeight;
  lWidth=GetWidth();
  lHeight=GetHeight();
  //LPBYTE lpSrcDIBBits;
  

  long lLineBytes;
  lLineBytes=GetLineByte();

  if (lXOffset<=0 || lYOffset<=0)
  {
      return FALSE;
  }

  for(i=0;i<lHeight;i+=(2*lYOffset))
  {
	  
	  int index=0;
	  do
	  {
	    for(j=0;j<GetWidth();j++)
		{

           if(index<lYOffset)
		   {
		       if((lWidth-j-lXOffset)>=0)	
			   {
	             	*(m_lpData+(i+index) * lLineBytes + 3 * (lWidth-j))=*(m_lpData+(i+index) * lLineBytes + 3 * (lWidth-j-lXOffset));
		        	*(m_lpData+(i+index) * lLineBytes + 3 * (lWidth-j) + 1)=*(m_lpData+(i+index) * lLineBytes + 3 * (lWidth-j-lXOffset) + 1);
			        *(m_lpData+(i+index) * lLineBytes + 3 * (lWidth-j) + 2)=*(m_lpData+(i+index)* lLineBytes + 3 * (lWidth-j-lXOffset) + 2);

			   }
	           else
			   {
	                break;
			   }
		   }
		   else 
		   {
			  if((i+index)<lHeight)
			  {
			        *(m_lpData+(i+index) * lLineBytes + 3 * j)=*(m_lpData+(i+index) * lLineBytes + 3 * j);
		        	*(m_lpData+(i+index) * lLineBytes + 3 * j + 1)=*(m_lpData+(i+index) * lLineBytes + 3 * j + 1);
			        *(m_lpData+(i+index) * lLineBytes + 3 * j + 2)=*(m_lpData+(i+index) * lLineBytes + 3 * j + 2);
			         
			  }
              else
			   {
	                break;
			   }
		   }
	       
		}
		index++;
		//AfxMessageBox("ƽ���У�");
	  }
	  while(index<2*lYOffset);
	
  }
   return TRUE;
 }

BOOL CDib::TranslationLeft(long lXOffset,long lYOffset)
 {
  
  long i;
  long j;
  long lWidth,lHeight;
  lWidth=GetWidth();
  lHeight=GetHeight();
  //LPBYTE lpSrcDIBBits;
  

  long lLineBytes;
  lLineBytes=GetLineByte();

  if (lXOffset<=0 || lYOffset<=0)
  {
     return FALSE;
  }
	 
  for(i=0;i<lHeight;i+=(2*lYOffset))
  {

	  int index=0;
	  do
	  {
	    for(j=0;j<GetWidth();j++)
		{

           if(index<lYOffset)
		   {
		       if((j+lXOffset)<=lWidth)	
			   {
	             	*(m_lpData+(i+index) * lLineBytes + 3 * j)=*(m_lpData+(i+index) * lLineBytes + 3 * (j+lXOffset));
		        	*(m_lpData+(i+index) * lLineBytes + 3 * j + 1)=*(m_lpData+(i+index) * lLineBytes + 3 * (j+lXOffset) + 1);
			        *(m_lpData+(i+index) * lLineBytes + 3 * j + 2)=*(m_lpData+(i+index)* lLineBytes + 3 * (j+lXOffset) + 2);

			   }
	           else
			   {
	                break;
			   }
		   }
		   else 
		   {
			  if((i+index)<lHeight)
			  {
			        *(m_lpData+(i+index) * lLineBytes + 3 * j)=*(m_lpData+(i+index) * lLineBytes + 3 * j);
		        	*(m_lpData+(i+index) * lLineBytes + 3 * j + 1)=*(m_lpData+(i+index) * lLineBytes + 3 * j + 1);
			        *(m_lpData+(i+index) * lLineBytes + 3 * j + 2)=*(m_lpData+(i+index) * lLineBytes + 3 * j + 2);
			         
			  }
              else
			   {
	                break;
			   }

		   }
	       
		}
		index++;
		//AfxMessageBox("ƽ���У�");
	  }
	  while(index<2*lYOffset);
	
  }
   return TRUE;
 }


