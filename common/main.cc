
#include"zk_handle.h"

#include<unistd.h>
#include<stdio.h>

#include<string>

const char* host_list = "127.0.0.1:2181";
const int time_out = 3000;

int main(){

	zoo_set_debug_level(ZOO_LOG_LEVEL_ERROR);
	ZkHandle zkhandle;
	
	if(zkhandle.zkInit(host_list, time_out) == 0){
		printf(" zkInit ok \n");		
	}
	struct Stat stat;
    set<string> node_list;
    string node_info;
    string raw_name;		

    printf("=======create node: /server, /server/host1, /server/host2\n");
	//创建一个PERSISTENT 永久节点/server(永久节点才可以创建子节点)
	//首先判断节点/server是否存在， 不存在则创建/server
	if(zkhandle.zkExists("/server", stat)!=0){
		zkhandle.zkCreateNode("/server", "this is a server", false,false,  raw_name);
	}
	//在/server创建2个临时节点host1,host2
	zkhandle.zkCreateNode("/server/host1", "127.0.0.1:12345", true,false, raw_name);
	zkhandle.zkCreateNode("/server/host2", "127.0.0.1:12346", true,false, raw_name);
	zkhandle.zkGetChildren("/",node_list);
	zkhandle.zkGetChildren("/server",node_list);

	printf("=======get the value of node: /server, /server/host1, /server/host2\n");
	std::string value;
	zkhandle.zkGetNodeInfo("/server",value,stat);
	zkhandle.zkGetNodeInfo("/server/host1",value,stat);
	zkhandle.zkGetNodeInfo("/server/host1",value,stat);



	printf("=======delete node: /server ,  删除失败，有孩子节点无法直接删除\n");
	//有孩子节点的话，无法直接删除
	zkhandle.zkDeleteNode("/server");
	zkhandle.zkGetChildren("/",node_list);
	zkhandle.zkGetChildren("/server",node_list);

	printf("=======delete node: /server/host1\n");
	//删除一个节点/server/host1
	zkhandle.zkDeleteNode("/server/host1");
	zkhandle.zkGetChildren("/",node_list);
	zkhandle.zkGetChildren("/server",node_list);

	//删除剩余节点
	printf("=======delete node: /server/host2, /server\n");
	zkhandle.zkDeleteNode("/server/host2");
	zkhandle.zkDeleteNode("/server");
	zkhandle.zkGetChildren("/",node_list);
	zkhandle.zkGetChildren("/server",node_list);


	//创建临时的有序节点SEQUENCE
	printf("=======create SEQUENCE1 node: /ZOO_SEQUENCE1, /ZOO_SEQUENCE2\n");
	zkhandle.zkCreateNode("/ZOO_SEQUENCE1", "seq1", true,true, raw_name);
	zkhandle.zkCreateNode("/ZOO_SEQUENCE2", "seq2", true,true, raw_name);
	zkhandle.zkGetChildren("/",node_list);


	while(1){
		sleep(1);
	}
	
	zkhandle.zkClose();
	return 0;
}



