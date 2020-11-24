
#include"zk_handle.h"


const int kMaxBufferLen = 4096;

ZkHandle::ZkHandle()
	:m_zk_handle(nullptr){

}

ZkHandle::~ZkHandle(){
	zkClose();
}

int ZkHandle::zkInit(const std::string& host_list,const int time_out){

    m_zk_handle = zookeeper_init(host_list.c_str(), &zkInitWatcher, time_out, 0, nullptr, 0);
    if (nullptr == m_zk_handle)
    {
        printf("ZkHandle::zkInit: connect to zk fail.\n");
        return -1;
    }
    return 0;
}
int ZkHandle::zkClose(){
    if (m_zk_handle)
    {
        zookeeper_close(m_zk_handle);
        m_zk_handle = nullptr;
        printf("ZkHandle::ZkClose: zk close.\n");
    }
}

void ZkHandle::zkInitWatcher(zhandle_t* zh, int type, int state, const char* path, void* watcherCtx){

  	//if(3 == state){
		//连接成功	
	//}
    printf("ZkHandle::ZkInitWatchar: [type=%d] [state=%d] [path=%s] [watcher_ctx=%p]\n", type, state, path, watcherCtx);
}


int ZkHandle::zkExists(const string& path, struct Stat & stat){
	int ret_code = zoo_exists(m_zk_handle, path.c_str(), 0, &stat);
    printf("ZkHandle::zkExists: [ret=%d]\n", ret_code);
    if(ZOK == ret_code)
    {
        printf("ZkHandle::zkExists: [path=%s] [czxid=%ld] [mzxid=%ld] [version=%d] [cversion=%d] [child_num=%d]\n",
            path.c_str(), stat.czxid, stat.mzxid, stat.version, stat.cversion, stat.numChildren);
    }
    return ret_code;
}
int ZkHandle::zkCreateNode(const string& path, const string& value,bool is_ephemeral, bool is_sequential, string& raw_node_name){
	int ret_code = 0;
    char tmp_name[kMaxBufferLen];
    //默认零时节点
    int flag = 0;
    //判断是否零时节点(零时节点，当客户端断开的时候， 节点会自动删除)
    if(is_ephemeral)
    	 flag |= ZOO_EPHEMERAL;
    //判断序列化节点, 有对节点添加后缀  eg/test ->  /test000001
    if (is_sequential)
    {
        flag |= ZOO_SEQUENCE;
    }
    printf("ZkHandle::zkCreateNode create node [path=%s] [value=%s]\n", path.c_str(), value.c_str());
    
    ret_code = zoo_create(m_zk_handle, path.c_str(), value.c_str(), value.size(), &ZOO_OPEN_ACL_UNSAFE, flag, tmp_name, kMaxBufferLen);
    if (ZOK != ret_code)
    {
        printf("ZkHandle::zkCreateNode create node fail. ret=%d\n", ret_code);
    }
    else
    {
        printf("ZkHandle::zkCreateNode create node succ! path=%s\n", tmp_name);
    }

    raw_node_name = tmp_name;
    return ret_code;
}
int ZkHandle::zkDeleteNode(const string& path, const int version){
	int ret_code = zoo_delete(m_zk_handle, path.c_str(), version);
	if(ret_code == 0){
		 printf("ZkHandle::zkDeleteNode delete node succ path=%s version=%d ret=%d\n", path.c_str(), version, ret_code);
	}else{
		 printf("ZkHandle::zkCreateNode delete node fail path=%s, ret = %d\n", path.c_str(),ret_code);
	}
    return ret_code;
}

int ZkHandle::zkGetChildren(const string& path, set<string>& node_list){

 	int ret_code = 0;
    struct String_vector children_list;

    printf("ZkHandle::zkGetChildren get children for path=%s\n", path.c_str());
    //获取节点对应的所有字节点
    ret_code = zoo_get_children(m_zk_handle, path.c_str(), 0, &children_list);

    if (ZOK != ret_code)
    {
        printf("CZkHandle::ZkGetChildren get children fail. ret=%d\n", ret_code);
        return ret_code;
    }
    
    printf("ZkHandle::zkGetChildrenn get children succ. children_num=%d\n", children_list.count);

    //将获取的节点保存到node_list中
    for (unsigned int children_idx = 0; children_idx < children_list.count; ++children_idx)
    {
        printf("ZkHandle::zkGetChildren children_idx=%u, children_name=%s\n", children_idx, children_list.data[children_idx]);
        node_list.insert(children_list.data[children_idx]);
    }

    deallocate_String_vector(&children_list);
    return ret_code;
}
int ZkHandle::zkGetNodeInfo(const string& path, string & info, struct Stat& stat){

	int ret_code = 0;
    char buffer[kMaxBufferLen];
    int buffer_len = kMaxBufferLen;

    printf("ZkHandle::zkGetChildren get node info for path=%s\n", path.c_str());
    //buffer保存节点返回的数据
    ret_code = zoo_get(m_zk_handle, path.c_str(), 0, buffer, &buffer_len, &stat);

    if (ZOK != ret_code)
    {
        printf("CZkHandle::ZkGetNodeInfo get node info for path=%s fail. ret=%d\n", path.c_str(), ret_code);
        return ret_code;
    }

    buffer[buffer_len] = 0;
    printf("CZkHandle::ZkGetNodeInfo get node info for path=%s succ. buffer=%s\n", path.c_str(), buffer);
    printf("CZkHandle::ZkGetNodeInfo: [path=%s] [czxid=%ld] [mzxid=%ld] [version=%d] [cversion=%d] [child_num=%d]\n",
        path.c_str(), stat.czxid, stat.mzxid, stat.version, stat.cversion, stat.numChildren);

    info = buffer;

    return ret_code;
}

int ZkHandle::zkSetNodeInfo(const string& path, const string& value){
	printf("ZkHandle::zkSetNodeInfo set node info. path=%s value=%s\n", path.c_str(), value.c_str());
    int ret_code = zoo_set(m_zk_handle, path.c_str(), value.c_str(), value.size(), -1);
    if (ZOK != ret_code)
    {
        printf("ZkHandle::zkSetNodeInfo set node fail.");
    }
    return ret_code;
}