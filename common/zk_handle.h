#ifndef _ZK_HANDLE_H_
#define _ZK_HANDLE_H_

#include<stdio.h>

#include<string>
#include<set>

#include <zookeeper.h>
#include <zookeeper_log.h>
#include <zookeeper.jute.h>

using std::string;
using std::set;

class ZkHandle{

public:
	ZkHandle();
	~ZkHandle();
	//禁止拷贝
	ZkHandle(const ZkHandle&) = delete;
	ZkHandle(ZkHandle&& ) = delete;
	ZkHandle& operator=(const ZkHandle&) = delete;

public:
	int zkInit(const std::string& host_list,const int time_out);
	int zkClose();
	
	//判断节点path是否存在
	int zkExists(const string& path, struct Stat & stat);
	
	//创建和删除节点，
	//note，此处应该创建零时节点
	int zkCreateNode(const string& path, const string& value, bool is_ephemeral, bool is_sequential, string& raw_node_name);
    int zkDeleteNode(const string& path, const int version = -1);
	
    //获取节点/节点值
	int zkGetChildren(const string& path, set<string>& node_list);
   	int zkGetNodeInfo(const string& path, string & info, struct Stat& stat);

   	//设置节点值
	int zkSetNodeInfo(const string& path, const string& value);
private:
    	static void zkInitWatcher(zhandle_t* zh, int type, int state, const char* path, void* watcherCtx);
private:
	zhandle_t* m_zk_handle;
	string m_host_list;
};

#endif