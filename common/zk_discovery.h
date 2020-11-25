#ifndef _ZK_DISCOVERY_H_
#define _ZK_DISCOVERY_H_

#include<map>
#include<memory>

#include"zk_NodeInfo.h"
#include"zk_handle.h"


//服务发现
//主要是检测节点path， 并将其孩子节点信息保存在m_path_to_nodes中
class ZKDiscovery{
public:
	ZKDiscovery(const std::string& path,std::shared_ptr<ZkHandle> zkHandle =nullptr)
		:m_path(path),m_zkHandle(zkHandle){}
	~ZKDiscovery(){}
	ZKDiscovery(const ZKDiscovery&) = delete;
	ZKDiscovery(ZKDiscovery&& ) = delete;
	ZKDiscovery& operator=(const ZKDiscovery&) = delete;

public:
	int zkDoDiscovery();
	void reset();
	std::map<string, ZkNodeInfo> getPathToNodes(){
		return m_path_to_nodes;
	}
private:
	std::string m_path;
	std::shared_ptr<ZkHandle> m_zkHandle;
	std::map<string, ZkNodeInfo> m_path_to_nodes;
};


#endif
