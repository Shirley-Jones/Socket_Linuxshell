#!/bin/bash
#Zero Socket服务启动文件


Start_Zero_Server()
{
	Load_profile
	
	
	#启动
	if [ ! -f "$Server_File" ]; then
		#失败
		echo "$Server_Name Binary file does not exist，Please check if the path is correct ($Server_File)";
		exit 1;
		#mkdir /Super
	else
		#启动
		$Server_File >/dev/null 2>&1
		Check_Proxy_PID=`ps -ef |grep "$Server_Name" |grep -v "grep" |awk '{print $2}'`;
		if [[ ! -z ${Check_Proxy_PID} ]]; then
			echo "$Server_Name Started....."
		else
			echo "$Server_Name Fail to start.....";
			exit 1;
		fi
	fi
	
	
	exit 0;
	
}

Stop_Zero_Server()
{
	Load_profile
	
	#停止
	killall -9 $Server_Name >/dev/null 2>&1
	sleep 3
	echo "$Server_Name Stopped....."
	
	
	exit 0;
	
	
	
	
	
}

Restart_Zero_Server()
{
	Load_profile
	
	#启动
	if [ ! -f "$Server_File" ]; then
		#失败
		echo "$Server_Name Binary file does not exist，Please check if the path is correct ($Server_File)";
		exit 1;
		#mkdir /Super
	else
		#停止
		killall -9 $Server_Name >/dev/null 2>&1
		sleep 3
		echo "$Server_Name Stopped....."
		#启动
		$Server_File >/dev/null 2>&1
		Check_Proxy_PID=`ps -ef |grep "$Server_Name" |grep -v "grep" |awk '{print $2}'`
		if [[ ! -z ${Check_Proxy_PID} ]]; then
			echo "$Server_Name Started....."
		else
			echo "$Server_Name Fail to start.....";
			exit 1;
		fi
	fi
	
	
	
	
	exit 0;
	
}


Check_Zero_Server_Run()
{
	
	Load_profile
	
	printf "%-70s"  "$Server_Name PID"
	Check_Proxy_PID=$(echo `ps -ef |grep "$Server_Name" |grep -v "grep" |awk '{print $2}'` | tr -d '\n') 
	if [[ ! -z ${Check_Proxy_PID} ]]; then
		echo -e "[ \033[32m Running \033[0m ]"
	else
		echo -e "[ \033[31m Not running \033[0m ]"
	fi
	
	
	exit 0;
	
}

Load_profile()
{
	Server_File="/Zero/Core/Socket.bin";
	Server_Name="Socket.bin";
}



case $1 in
	"start")
		Start_Zero_Server
	;;
	"restart")
		Restart_Zero_Server
	;;
	"stop")
		Stop_Zero_Server
	;;	
	"status")
		Check_Zero_Server_Run
	;;
	"state")
		Check_Zero_Server_Run
	;;
	
	*) 
		echo "Please execute the following command $0 [start|restart|stop|status|state] For example, restart Server service command $0 restart";
		exit 0;
    ;;
esac 


