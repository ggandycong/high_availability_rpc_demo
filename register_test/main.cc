
#include<unistd.h>
#include<stdio.h>

#include<string>
#include<memory>
#include<set>

#include"../common/zk_handle.h"
#include"../common/zk_register.h"


using std::string;
using std::set;

const char* host_list = "127.0.0.1:2181";
const int time_out = 3000;
//临时节点,而且不是序列化
std::string nodeMsg = "IP[127.0.0.1]PORT[12345]ZK_PATH[/server/host1]IS_EPHEMERAL[1]IS_SEQUENCE[0]";

int main(){
	zoo_set_debug_level(ZOO_LOG_LEVEL_ERROR);
	struct Stat stat;
    set<string> node_list;
    string path_info;
    std::string raw_name;

	std::shared_ptr<ZkHandle> pZkhandle(new ZkHandle);
	if(pZkhandle->zkInit(host_list, time_out) == 0){
		printf("pZkhandle zkInit ok \n");		
	}

	//创建永久节点
	if(pZkhandle->zkExists("/server", stat)!=0){
		pZkhandle->zkCreateNode("/server", "this is a server", false,false,  raw_name);
	}

	ZKRegister zkRegister(pZkhandle);
	ZkNodeInfo zkNode(nodeMsg);
	zkRegister.setZkNode(zkNode);
	zkRegister.zkDoRegister();

	pZkhandle->zkGetChildren("/",node_list);
	pZkhandle->zkGetNodeInfo(zkNode.m_zk_path.c_str(),path_info,stat);

	//解除注册
/*	zkRegister.zkUnDoRegister();
	pZkhandle->zkGetChildren("/",node_list);*/


	while(1){
		sleep(1);
	}
	
	return 0;
}



