#include <QString>
#include <QtCore>
#ifdef Q_WS_WIN

#include <QtDebug>

#include "usbwin.h"
#include <Windows.h>
#include <WinDef.h>
#include <Dbt.h>
#include <io.h>
#include <errno.h>
#include "StringUtil.h"
#include "resourcestring.h"
#include "debug.h"
#include <certstore/npkipath.h>
#include "usbassistant.h"

char GetDriveLetter(unsigned long ulUnitMask)
{
    char c;
    for (c = 0; c < 26; c++)    {
        if (ulUnitMask & 0x01){
            break;
        }
        ulUnitMask = ulUnitMask >> 1;
    }
    return (c + 'A');
}

char getSystemDrive(void)
{
    string systemNpkiPath = NPKIPath::getLocalNPKIPath();
    systemNpkiPath = StringUtil::toLower(systemNpkiPath);
    char systemDrive = 'c';
    if(systemNpkiPath.length() != 0)
        systemDrive = systemNpkiPath.at(0);
    return systemDrive;
}
bool isSystemDrive(char chDrv)
{
    char systemDrive = getSystemDrive();

    if((chDrv >= 'a' && chDrv <= 'z') && chDrv == systemDrive)
        return true;
    if((chDrv >= 'A' && chDrv <= 'Z') && chDrv == (systemDrive - ('a'-'A')))
        return true;
    return false;

}

//TODO:
vector<string> getUSBDrives()
{
    //Get Logical Drives
	DWORD dwDrives=GetLogicalDrives();

    vector<string> list;
    TCHAR chDriveLabel='A';
    TCHAR szRootpath[5]={0,};
    TCHAR volumeName[100] = {0, };
    TCHAR fileSystemName[100] = {0, };
    TCHAR szNPKI[128] = {0,};


    while (dwDrives != 0) {
        if ((dwDrives & 1)  &&  (isSystemDrive(chDriveLabel) != true)) {

            wsprintf(szRootpath,L"%c:\\",chDriveLabel);
            wsprintf(szNPKI,L"%sNPKI\\",szRootpath);
            int type    = GetDriveType(szRootpath);

            if(DRIVE_REMOVABLE==type || DRIVE_FIXED==type || DRIVE_CDROM==type /*|| DRIVE_REMOTE ==type*/){
                SHFILEINFO sfi;
                memset(&sfi,0,sizeof(sfi));
                UINT       uFlags = SHGFI_USEFILEATTRIBUTES | SHGFI_SYSICONINDEX |SHGFI_DISPLAYNAME;
                DWORD retFileInfo = SHGetFileInfo ( szRootpath, 0, &sfi, sizeof(SHFILEINFO),uFlags );
                QString prettyName;

                if(retFileInfo == 0 || sfi.szDisplayName == NULL || wcslen(sfi.szDisplayName) == 0)
                {
                    GetVolumeInformation(szRootpath, volumeName, 100, NULL,NULL,NULL, fileSystemName, 100);
                    QString drivename = QString::fromUtf16(szRootpath).left(2);

                    //list << QString::fromUtf16(szRootpath);
                    if (wcslen(volumeName) != 0) {
                        QTextStream(&prettyName) << QString::fromUtf16(volumeName) << " (" << drivename << ")";
                    } else {
                        if(DRIVE_FIXED == type)
                            QTextStream(&prettyName) << R.unknown.qstr() << " (" << drivename << ")";
                        if(DRIVE_REMOVABLE == type)
                            QTextStream(&prettyName) << R.removable_disk.qstr() << " (" << drivename << ")";
                        if(DRIVE_CDROM == type)
                            QTextStream(&prettyName) << R.cdrom.qstr() << " (" << drivename << ")";
                    }
                }
                else
                {
					prettyName = QString::fromUtf16(sfi.szDisplayName);
					//드라이브 문자 표시 하지 않음음으로 설정하면 getRootFromLabel() 로 드라이브 Root를 찾을수 없다.
					string rootFromLabel = USBAssistant::getRootFromLabel(toStdString(prettyName));
					string root;
					root += (char)chDriveLabel;
					root += ":";

					if(!boost::iequals(rootFromLabel, root)){
						 prettyName += " (";
						 prettyName += root.c_str();
						 prettyName += ")";
					}
				}

                list.push_back(toStdString(prettyName));
			}
        }
        chDriveLabel++;
        dwDrives = dwDrives >> 1;//next drive
    }

    return list;
}

bool usbEvent(HWND hWnd, MSG *msg,long * result)
{
    int msgType = msg->message;

    if(msgType == WM_DEVICECHANGE)
    {
       qDebug() << "winEvent in MgFrame : WM_DEVICECHANGE" ;
       PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;

       switch(msg->wParam)
       {
          case DBT_DEVICEARRIVAL:
          if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
          {
              PDEV_BROADCAST_VOLUME pVol = (PDEV_BROADCAST_VOLUME) lpdb;
              wchar_t szMessage[80];
              char cDriveLetter = GetDriveLetter(pVol->dbcv_unitmask);
              wsprintf(szMessage, L"Device '%c:' has been inserted.", cDriveLetter);
              //MessageBox(hWnd, szMessage, L"USB Notice", MB_OK);
          }
          break;

          case DBT_DEVICEREMOVECOMPLETE:
          if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
          {
              PDEV_BROADCAST_VOLUME pVol = (PDEV_BROADCAST_VOLUME) lpdb;
              wchar_t szMessage[80];
              char cDriveLetter = GetDriveLetter(pVol->dbcv_unitmask);
              wsprintf(szMessage, L"Device '%c:' has been removed.", cDriveLetter);
              //MessageBox(hWnd, szMessage, L"USB Notice", MB_OK);
          }
          break;
       } // skip the rest

    } // end of if msgType

    return false; // let qt handle the rest
}

void registerUSBEvent(HWND hWnd)
{

    DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
    HDEVNOTIFY hDeviceNotify = NULL;

    static const GUID GuidDevInterfaceList[] =
    {
    { 0xa5dcbf10, 0x6530, 0x11d2, { 0x90, 0x1f, 0x00, 0xc0, 0x4f, 0xb9, 0x51, 0xed } },
    { 0x53f56307, 0xb6bf, 0x11d0, { 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b } },
    { 0x4d1e55b2, 0xf16f, 0x11Cf, { 0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } },
    { 0xad498944, 0x762f, 0x11d0, { 0x8d, 0xcb, 0x00, 0xc0, 0x4f, 0xc3, 0x35, 0x8c } }
    };

    ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));

    NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;

    bool isSucceed = true;
    for (int i = 0; i < sizeof(GuidDevInterfaceList); i++)
    {
        NotificationFilter.dbcc_classguid = GuidDevInterfaceList[i];
        hDeviceNotify = RegisterDeviceNotification(hWnd, &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
        if (hDeviceNotify == NULL)
        {
            MessageBox(hWnd, L"USB Register has failed.", L"USB Notice", MB_OK);
            isSucceed = false;
        }
    }
    if (isSucceed) {
        MessageBox(hWnd, L"USB Register has succeed.", L"USB Notice", MB_OK);
    }
}

#endif

