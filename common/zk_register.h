#ifndef _ZK_REGISTER_H_
#define _ZK_REGISTER_H_


#include<memory>

#include"zk_NodeInfo.h"
#include"zk_handle.h"

class ZKRegister{

public:
	ZKRegister(std::shared_ptr<ZkHandle> zkHandle =nullptr)
		:m_zkHandle(zkHandle){}
	~ZKRegister(){}
	ZKRegister(const ZKRegister&) = delete;
	ZKRegister(ZKRegister&& ) = delete;
	ZKRegister& operator=(const ZKRegister&) = delete;


public:
	//注册节点
	int zkDoRegister();
    int zkUnDoRegister();

    void setZkNode(const ZkNodeInfo& zkNode);

public:
	std::shared_ptr<ZkHandle> m_zkHandle;
	ZkNodeInfo m_zkNode;
};


#endif 