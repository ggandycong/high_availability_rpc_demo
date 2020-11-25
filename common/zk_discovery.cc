
#include"zk_discovery.h"

void ZKDiscovery::reset(){
	m_path_to_nodes.clear();
}

int ZKDiscovery::zkDoDiscovery(){
	reset();
	printf("ZKDiscovery::zkDoDiscovery: %s\n",m_path.c_str());
	if(m_zkHandle){
		int ret_code = 0;
		set<string> node_list;
		ret_code = m_zkHandle->zkGetChildren(m_path,node_list);
		if(ret_code != ZOK){
			return ret_code;
		}
		for (const auto& node_path : node_list){
			//完整的节点路径
			std::string full_node_path = m_path;
			if(m_path != "/")
				full_node_path += "/";
			full_node_path += node_path;
			//假如节点已经存在，直接跳过
			if(m_path_to_nodes.find(full_node_path) != m_path_to_nodes.end())
				continue;
			struct Stat stat;
			string node_info;
			ret_code = m_zkHandle->zkGetNodeInfo(full_node_path.c_str(), node_info,stat);
			if(ret_code == ZOK && !node_info.empty()){
				ZkNodeInfo zkNode(node_info);
				m_path_to_nodes[full_node_path] = zkNode;
				printf("ZKDiscovery::zkDoDiscovery, successfully to add node: %s\n",zkNode.ToString().c_str());
			}
		}
		return m_path_to_nodes.empty() ? -1 : 0;
	}
	return -1;
}
