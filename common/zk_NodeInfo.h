#ifndef _ZK_NODEINFO_H_
#define _ZK_NODEINFO_H_

#include<string>
#include <sstream>

using std::string;
using std::stringstream;


class ZkNodeInfo{

public:
	ZkNodeInfo(){
		Reset();
	}
	//直接使用字符串构造
	ZkNodeInfo(const string& info){
		auto get_val = [info](string key)->string
        {
            int idx;
            string key1 = key + "[";
            string key2 = "]";
            idx = info.find(key1) + key1.size();
            return info.substr(idx, info.find_first_of(key2, idx) - idx);
        };
        m_ip = get_val("IP");
        m_port = get_val("PORT");
        m_zk_path = get_val("ZK_PATH");
        m_is_ephemeral = get_val("IS_EPHEMERAL");
        m_is_sequence = get_val("IS_SEQUENCE");
	}

	//将其对象转为字符串
	string ToString()
    {
        stringstream oss;
        oss.str("");
        auto set_val = [&oss](const string& key, const string& value)
        {
            oss << key << "[" << value << "] ";
        };

        set_val("IP", m_ip);
        set_val("PORT", m_port);
        set_val("ZK_PATH", m_zk_path);
        set_val("IS_EPHEMERAL", m_is_ephemeral);
        set_val("IS_SEQUENCE", m_is_sequence);

        return oss.str();
    }

    void Reset()
    {
        m_ip = "";
        m_port = "";
        m_zk_path = "";
        m_is_ephemeral = "";
        m_is_sequence = "";
    }


public:
	std::string m_ip;
    std::string m_port;
    std::string m_zk_path;
    std::string m_is_ephemeral;
    std::string m_is_sequence;
};



#endif