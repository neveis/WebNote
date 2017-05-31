#pragma once
#ifndef _LRUCACHE_HPP
#define _LRUCACHE_HPP

#include <map>

/*
* file name: LRUCache.h
* desp: 使用STL实现一个LRU模板类
* Impl:结合list和map
* list用于对最近使用进行排列，需要自己实现，因为不能通过遍历来寻找
* map用于查找，更适合的是使用哈希表
* 1.创建 设置容量，有默认值
*	1）构造函数
*	1）单独一个设置函数
* 2.添加数据
* 3.获取数据
* 4.销毁
*
* 列表的操作：
* 1.创建节点
* 2.销毁节点
* 3.从列表中移除一个节点
* 4.插入节点到表头(因为是用于LRU，只会插入到表头)
* 5.更新节点(将节点换到表头)
*/
template <typename KeyType,typename ValueType>
struct Entry
{
	KeyType key;
	ValueType value;
	Entry<KeyType, ValueType> *prev;
	Entry<KeyType, ValueType> *next;
	Entry() :prev(nullptr), next(nullptr) {};
};

template <typename KeyType, typename ValueType>
class LRUCache {

public:
	LRUCache(int capacity) :_capacity(capacity),_size(0) {};
	LRUCache() :_capacity(20), _size(0) {};
	void set(KeyType const& key, ValueType const& value);
	ValueType* get(KeyType const& key);
	void erase(KeyType const& key);
	bool isExist(KeyType const& key);
	~LRUCache();
	int size();
	virtual void print();

private:
	std::map<KeyType, Entry<KeyType,ValueType>*> _map;
	int _capacity;
	int _size;
	Entry<KeyType, ValueType> *_listHead;
	Entry<KeyType, ValueType> *_listTail;
	//禁用拷贝和赋值
	LRUCache(const LRUCache& l) {};
	LRUCache& operator =(const LRUCache& l) {};
	Entry<KeyType, ValueType> * newCacheEntry(KeyType const& key, ValueType const& value);
	void freeCacheEntry(Entry<KeyType, ValueType> * entry);
	Entry<KeyType, ValueType>* insertToHead(Entry<KeyType, ValueType> * entry);
	void removeFromList(Entry<KeyType, ValueType> * entry);
	void updateList(Entry<KeyType, ValueType> * entry);

};

template<typename KeyType, typename ValueType>
void LRUCache<KeyType, ValueType>::set(KeyType const& key, ValueType const& value)
{
	//1.检查是否在缓存中
	if (_map.find(key) != _map.end()) {
		//2.1.存在则更新value，并移动到列表头
		Entry<KeyType, ValueType> *temp = _map[key];
		temp->value = value;
		updateList(temp);
	}
	else {
		//2.2.不存在，从头插入到列表中
		Entry<KeyType, ValueType> *entry = newCacheEntry(key, value);
		Entry<KeyType, ValueType> *removeEntry = insertToHead(entry);

		//3.2.判断是否有节点被移除，有则释放内存
		if (removeEntry != nullptr) {
			_map.erase(removeEntry->key);
			freeCacheEntry(removeEntry);
		}

		//4.2.加入到map中
		_map[key] = entry;

	}

}

template<typename KeyType, typename ValueType>
ValueType* LRUCache<KeyType, ValueType>::get(KeyType const& key)
{
	if (_map.find(key) == _map.end()) {
		//printf("get key failed\n");
		return nullptr;
	}
	//更新到表头
	updateList(_map[key]);
	return &(_map[key]->value);
}

template<typename KeyType, typename ValueType>
void LRUCache<KeyType, ValueType>::erase(KeyType const & key)
{
	auto it = _map.find(key);
	if (it != _map.end()) {
		Entry<KeyType, ValueType> *temp = it->second;
		_map.erase(key);
		removeFromList(temp);
		freeCacheEntry(temp);
	}
}

template<typename KeyType, typename ValueType>
bool LRUCache<KeyType, ValueType>::isExist(KeyType const & key)
{
	auto it = _map.find(key);
	if (it != _map.end()) {
		return true;
	}
	else {
		return false;
	}
}

template<typename KeyType, typename ValueType>
LRUCache<KeyType, ValueType>::~LRUCache()
{
	for (auto it = _map.begin(); it != _map.end(); ++it) {
		freeCacheEntry(it->second);
	}
}

template<typename KeyType, typename ValueType>
int LRUCache<KeyType, ValueType>::size()
{
	return _size;
}

template<typename KeyType, typename ValueType>
void LRUCache<KeyType, ValueType>::print()
{

}

/*
* 创建一个列表节点存储key和value
*/
template<typename KeyType, typename ValueType>
Entry<KeyType, ValueType> * LRUCache<KeyType, ValueType>::newCacheEntry(KeyType const& key, ValueType const& value)
{
	Entry<KeyType, ValueType> *entry = new Entry<KeyType, ValueType>();
	if (entry == nullptr) {
		printf("create entry failed\n");
		return nullptr;
	}
	entry->key = key;
	entry->value = value;

	return entry;
}

template<typename KeyType, typename ValueType>
void LRUCache<KeyType, ValueType>::freeCacheEntry(Entry<KeyType, ValueType>* entry)
{
	if (entry != nullptr)
	{
		delete entry;
	}
}

template<typename KeyType, typename ValueType>
Entry<KeyType, ValueType>* LRUCache<KeyType, ValueType>::insertToHead(Entry<KeyType, ValueType>* entry)
{
	Entry<KeyType, ValueType>* removeEntry = nullptr;
	//1.检查缓存是否满
	if (_size == _capacity)
	{
		//2.1满则删除最后一个节点
		removeEntry = _listTail;
		removeFromList(removeEntry);
	}
	//3.添加新节点到表头
	if (_listHead == nullptr && _listTail == nullptr)
	{
		//3.1如果当前列表是空的
		_listHead = _listTail = entry;
	}
	else
	{
		//3.2当前列表不空
		entry->next = _listHead;
		entry->prev = nullptr;
		_listHead = entry;
		entry->next->prev = entry;
	}

	//4.节点数加一
	_size++;
	return removeEntry;
}

template<typename KeyType, typename ValueType>
void LRUCache<KeyType, ValueType>::removeFromList(Entry<KeyType, ValueType>* entry)
{
	//1.列表是否为空
	if (_size <= 0) {
		printf("the list is empty\n");
		return;
	}

	//2.列表只有一个节点
	if (_listHead == entry && _listTail == entry)
	{
		_listHead = _listTail = nullptr;
	}
	//3.节点为头节点
	else if (_listHead == entry)
	{
		_listHead = entry->next;
		entry->next->prev = nullptr;
	}
	//4.节点为尾节点
	else if (_listTail == entry)
	{
		_listTail = entry->prev;
		entry->prev->next = nullptr;
	}
	//5.一般情况
	else
	{
		entry->prev->next = entry->next;
		entry->next->prev = entry->prev;
	}

	//6.节点数减一
	_size--;
}

template<typename KeyType, typename ValueType>
void LRUCache<KeyType, ValueType>::updateList(Entry<KeyType, ValueType>* entry)
{
	removeFromList(entry);
	insertToHead(entry);
}

#endif // !_LRUCACHE_HPP