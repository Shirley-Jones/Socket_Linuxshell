
## 程序说明
* 这是一个使用Socket接口来执行shell命令的程序
* 根据Zero Panel项目适配而来，兼容FAS流控（编译后替换 /root/res/fas-service 即可正常使用）
* Socket默认监听8989端口，您可以自己修改，请注意不要在公网上开放8989端口，有安全风险！
* 还望各位大虾自己研究


## 编译说明
* 先安装GCC: yum -y install gcc gcc++ gdb 
* 编译 gcc -o Socket.bin Socket.c
* 执行 ./Socket.bin



