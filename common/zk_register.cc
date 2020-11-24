
#include "zk_register.h"



int ZKRegister::zkDoRegister(){
	printf("ZKRegister::zkDoRegister: %s\n", m_zkNode.ToString().c_str());
	if(m_zkHandle){
		std::string raw_name;
		bool is_ephemeral = m_zkNode.m_is_ephemeral[0] == '1'? true : false;
		bool is_ssequence = m_zkNode.m_is_sequence[0] == '1'? true : false;
		int ret_code = m_zkHandle->zkCreateNode(m_zkNode.m_zk_path.c_str(), m_zkNode.ToString(), is_ephemeral,is_ssequence, raw_name);
		if(ret_code == ZOK){
			 printf("ZKRegister::zkDoRegister register succ!\n.");
			 return ret_code;
		}
		else if (ZNODEEXISTS == ret_code)
	    {
	        printf("ZKRegister::zkDoRegister register fail. someone has already register\n");
	        return ret_code;
	    }
	    else 
	    {
	        printf("ZKRegister::zkDoRegister register fail. ret=%d\n", ret_code);
	        return ret_code;
	    }
	}
	return -1;
}
int ZKRegister::zkUnDoRegister(){
	printf("ZKRegister::zkUnDoRegister: %s\n", m_zkNode.m_zk_path.c_str());
	if(m_zkHandle){
		struct Stat stat;
		//当节点路径存在,再将其删除
		if(m_zkHandle->zkExists(m_zkNode.m_zk_path.c_str(),stat) == ZOK){
			int ret_code = m_zkHandle->zkDeleteNode(m_zkNode.m_zk_path.c_str());
			if(ret_code == ZOK){
				printf("ZKRegister::zkUnDoRegister delete node succ %s\n", m_zkNode.m_zk_path.c_str());
				return ret_code;
			}
		}
	}
	return -1;
}

void ZKRegister::setZkNode(const ZkNodeInfo& zkNode){
	m_zkNode = zkNode;
}