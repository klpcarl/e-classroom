/* 这个文件专门用于自定义消息的宏定义 */
//PeerTreeDlg的消息
#define WM_MY_LOGNAME WM_USER+100//用户登录并验证
#define WM_MY_SOCKCLOSE WM_USER+101//用户下线

//ShareSessionDlg的消息
#define WM_MY_SHARERSTARTED WM_USER+110//通知RDP服务端已开启
#define WM_MY_SHARERNOTSTARTED WM_USER+111
#define WM_MY_SHARERSTOPPED WM_USER+112

//MonitorSessionDlg的消息
#define WM_MY_MONITOROPENED WM_USER+120
#define WM_MY_MONITORNOTOPENED WM_USER+121
#define WM_MY_MONITORCLOSED WM_USER+122

//HandDlg的消息
#define WM_MY_HANDUP WM_USER+130