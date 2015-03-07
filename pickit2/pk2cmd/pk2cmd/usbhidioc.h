// usbhidiocDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUsbhidiocDlg dialog

class CUsbhidioc //: public CDialog
{
public:
	CUsbhidioc(void);
	char* GetPK2UnitID(void);
	bool FindTheHID(int unitIndex);
    bool ReadReport (char *);
    bool WriteReport(char *, unsigned int);
    void CloseReport ();

protected:
    void GetDeviceCapabilities();
    void PrepareForOverlappedTransfer();

	char m_UnitID[32];
};


