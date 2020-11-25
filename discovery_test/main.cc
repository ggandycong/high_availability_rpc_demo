
#include<unistd.h>
#include<stdio.h>

#include<map>
#include<string>

#include"../common/zk_discovery.h"


const char* host_list = "127.0.0.1:2181";
const int time_out = 3000;

const string path = "/server";

int main(){
	zoo_set_debug_level(ZOO_LOG_LEVEL_ERROR);

	std::shared_ptr<ZkHandle> pZkhandle(new ZkHandle);
	if(pZkhandle->zkInit(host_list, time_out) == 0){
		printf("pZkhandle zkInit ok \n");
	}

	ZKDiscovery zkDiscovery(path,pZkhandle);
	zkDiscovery.zkDoDiscovery();
	std::map<string, ZkNodeInfo> nodes = zkDiscovery.getPathToNodes();

	//具体节点信息
	for (auto node : nodes){
		printf("path = %s, value = %s\n",node.first.c_str(), node.second.ToString().c_str());
	}


	while(1){
		sleep(1);
	}

	return 0;
}
