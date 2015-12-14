# 需要 #

  * win xp包括起操作系统x86和x64都支持
  * IP Packet Redirector Driver 驱动


# 导出函数说明 #

  * redirIP 函数 有 五个 输入参数 包括 网卡接口信息字符(MAC或IP或设备名)
要重定向的源ip，目标ip，均以字符方式输入,协议（TCP 1 UDP 2 0不限） byte类型，
端口号 无符号 short
> > 返回类型为UINT 0表示成功，其他为错误代码

  * Free 函数 释放重定向设置
一个输入参数 byte 类型，0为不释放 PcaFilterApi.dll 资源 ，1为释放
返回类型为UINT 0表示成功，其他为错误代码
  * DllInit 函数
为Dll无法初始化静态变量所使用，当以不执行DllEntry 方式来加载dll时，请调用本函数初始化

> 无参数，无返回

# 具体使用方法 #
  * 下载对应的 IP Packet Redirector Driver 地址见 介绍那里
  * 把 PcaFilterApi.dll 文件拷贝到 windows 的system32 目录下
> > 或把 PcaFilterApi.dll 所在目录加到 path 变量中
  * 直接运行本程序 可获得支持的网络接口的信息
> > 有\DEVICE\NDISWANIP 的，可直接使用这个
  * 以3个参数的方式 运行本 程序，第一个参数 网卡接口信息字符(MAC或IP或设备名)
第二个参数 要重定向的源ip，第三个参数 目标ip
> > 如 xxx.exe  [-p 协议] [-o 端口] \DEVICE\xxxx ip ip
> > 默认 协议=0 tcp+udp 端口 0 所有端口
> > 最新版本支持域名

# 新功能 #

  * 在最新版本中,环境变量 noport 可以让指定的多个端口不转发 用逗号隔离