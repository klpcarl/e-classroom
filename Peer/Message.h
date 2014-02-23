/*这个文件是我自己写的，专门用于自定义消息的宏定义，方便类间消息传递*/

#define WM_MY_SOCKCLOSE WM_USER+101		//控制台主动断开连接
#define WM_MY_STARTVIEWER WM_USER+102	//控制台发出的命令
#define WM_MY_STOPVIEWER WM_USER+103
#define WM_MY_LOCK WM_USER+107			//锁定学生端
#define WM_MY_UNLOCK WM_USER+108		//解锁学生端
#define WM_MY_RDPCONTROL WM_USER+112	//使viewer端发出控制级别请求
#define WM_MY_CONFIG WM_USER+120		//配置命令，用于配置客户端，如界面的修改等