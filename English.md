# Requirement #

  * win xp or  above (x86 and x64)
  * IP Packet Redirector Driver (link see: main page)


# Export Function #

  * redirIP : five input parameters ,1. net adapter information string
(MAC or IP or LowDeviceName) ,2.ip string you want to be redirected，3.ip string to redirect ,4.protocol 1=TCP 2=UDP 0=BOTH ,byte type, 5.Port unsigned short type
> > return type is UINT 0=succeed，otherwise return error code
  * Free : free all resource and clear net filter hook
> > one parameter byte type 1=freelibrary 0=no


> return type is UINT 0=succeed，otherwise return error code

  * DllInit: call when you  use loadlibraryex load\_as\_data flag
> > it also will be called by dllentry
> > > no parameter  no return

# How to Use #

  * Download  IP Packet Redirector Driver
  * Copy PcaFilterApi.dll file to windows/system32

> > or and add that directory into path variable
  * run binary file , you will see all support adapters
> > copy the  LowDeviceName (such as \DEVICE\xxxxx)
> > if exists \DEVICE\NDISWANIP and you use WAN(pppoe) use it
  * run binary file with three(or five) parameters,
> > such as xx.exe [-p protocol] [-o port] \DEVICE\yyyy ip\_you\_want\_to\_be\_redirected ip\_you\_want\_to\_redirect\_to


> default port=0 All port protocol=0 BOTH
> the newest version support domain name
  * Remember Loopback adapter never support by ndis

# Additional #
  * In newest version , "noport" environment  parameters can make special ports (separate by ,) not redirect