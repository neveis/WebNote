# WebNote
一个C++实现的在线便签的后台程序。

综合使用C++11和Boost::Asio实现HTTP服务器，数据库使用Redis（没有实现同步到磁盘数据库）。

`http_base.hpp`实现了基本的HTTP服务器。

使用Boost::Asio异步实现TCP通讯。接收请求后会解析HTTP头（包括Cookie）。

然后根据正则匹配URL，结合请求类型调用处理函数。

处理函数统一在运行时通过HandlerRegister添加。

使用hiredis访问数据库，通过单例模式封装调用。

实现了Session用于存储当前登录用户的信息。会话信息存储在LRU缓存中。

---

使用Vue和UIkit简单写了个前端演示，位于/dist/www文件夹中。
注：需要较高版本浏览器。

使用方法：

1.安装依赖

安装[Redis](https://redis.io/)

安装[Boost](http://www.boost.org/)以及[hiredis](https://github.com/redis/hiredis)

安装Cmake

2.编译
```
cmake .
make
```

3.运行Redis

设置起始id，可为任意非负整数：
```
set user:uid 1000
set note:nid 1000
```

4.运行
```
./dist/WebNote
```
默认端口为10080。

演示地址：neveis.com:10080
不一定在启动中。