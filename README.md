# WebNote
一个C++实现的在线便签的后台程序。

综合使用C++11和Boost::Asio实现HTTP服务器，数据库使用Redis（没有实现同步到磁盘数据库）。

`http_base.hpp`实现了基本的HTTP服务器。

使用Boost::Asio异步实现TCP通讯。接收请求后会解析HTTP头（包括Cookie）。

然后根据正则匹配URL，结合请求类型调用处理函数。

处理函数统一在运行时通过HandlerRegister添加。

使用hiredis访问数据库，通过单例模式封装调用。

实现了Session用于存储当前登录用户的信息。会话信息存储在LRU缓存中。
