# 高可用的rpc demo

>使用grpc框架提供一个ping接口, 借助zookeeper实现服务注册和发现.



## demo目前已初步完成的状态

基于grpc的ping服务端可以简单部署服务, 即将多个服务, 注册到zookeeper框架中,

基于grpc的ping客户端可以通过zookeeper服务发现,  获取ping服务节点(ip:port)信息,  然后连接节点(ip:port), 从而获取ping结果.



## zookeeper服务端

目前可以集群部署, 官方介绍至少部署3个节点(最好奇数个节点部署, 为了选取过半作用)



## zookpeeer客户端

也就是server ping ,  client ping程序 .  链接zookeeper_mt多线程库.

# server ping

* 首先连接到zookeeper服务端
* 将其**节点信息ZkHandle**注册到zookeeper中,eg: /ping/host1, /ping/host2....
* 开启ping 服务



## client ping

* 首先连接到zookeeper服务端

* 获取zookeeper服务发现, 获取节点/ping中包含的子节点信息, 从而提取出子节点**ZkHandle**

* 通过子节点信息**ZkHandle**(目前默认选择第一个), 连接ping服务器

* 访问ping服务器, 从而调用ping接口.

  

### ZkHandle节点信息

```cpp
	std::string m_ip;
    std::string m_port;
    std::string m_zk_path;
    std::string m_is_ephemeral;
    std::string m_is_sequence;
```

* m_ip: 表示ping服务器监听的ip

* m_port: 表示ping服务器监听的port

* m_zk_path: 表示注册到zookeeper的路径, 

  对于ping的服务, 我们将其注册到/ping的子路径, 这样我们可以通过监听/ping节点,从而发现其他ping服务的节点信息

* m_is_ephemeral: 注册到zookeeper中的节点, 是否是临时节点, 对于临时节点, 程序关闭的时候, zookeeper会自动将其节点移除(这正是我们该服务想要的效果).  另外一点,  对于临时节点, 无法在其节点下面创建孩子节点.

  因此我们创建永久节点/ping,  创建临时节点/ping/host1, /ping/host2,

* m_is_sequence: 注册到zookeeper中的节点,使创建的节点,有递增的序号, 比如/ping/host0001, /ping/host002.

 

**程序读取节点信息**

比如ping server简单读取配置文件,文件内容如下.

```
IP[127.0.0.1]PORT[12345]ZK_PATH[/ping/host1]IS_EPHEMERAL[1]IS_SEQUENCE[0]
```

转为**ZkHandle**节点信息

```cpp
m_ip:127.0.0.1
m_port:12345
m_zk_path:/ping/host1
m_is_ephemeral:1
m_is_sequence:0
```



# 后续可以完善......



## server ping

* 可以增加一个检测server ping的程序, 当server ping退出的时候, 自动重启server ping. 确保server ping一直提供服务
* 如果在高并发访问server接口,  如何去实现或者更好完善.  ( 后续了解grpc机制)



## client ping

* 维护节点信息**ZkHandle队列**, 类似生产者消费者.  程序可以从**ZkHandle队列**列表中获取节点去连接.  当程序发现**ZkHandle队列**没有节点的时候等待,    程序启动另外一个线程定时从zookeeper中发现服务新增的节点. 保存到**ZkHandle队列**,  并且唤醒等待一端.
* 对于获取的节点列表, 应该在客户端通过使用负载均衡算法,去连接server ping.(根据官方例子**[load_balancing](https://github.com/grpc/grpc/tree/master/examples/cpp/load_balancing)**,尝试过, 暂时不起作用)



## 文件配置信息获取

为了方便阅读, 可以将配置信息改成其他格式, eg, json.



## .......



# 程序构建

* 首先确保已经安装grpc, zookeeper库,已将相关依赖.
* 直接在根目录执行**make**即可

之后可执行文件保存在**当前工程目录下/bin**

