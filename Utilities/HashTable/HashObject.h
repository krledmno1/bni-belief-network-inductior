#ifndef _HASHOBJECT_
#define _HASHOBJECT_
#include <stdio.h>

template <class T, class K>
class HashObject
{
public:
	T key;
	bool free;
	K* content;
	HashObject();
	HashObject(T);
	HashObject(T,K);
	HashObject<T,K>& operator=(HashObject<T,K>&);
	bool operator==(HashObject<T,K>&);
	void deleteContent();
	T getKey();
	K* getContent();
	bool isEqKey(T);
	~HashObject();
};
template <class T, class K>
HashObject<T,K>::HashObject()
{
	key=(T)0;
	free=true;
	content=NULL;
}
template <class T, class K>
HashObject<T,K>::HashObject(T a)
{
	free=true;
	content=NULL;
	key=a;
}
template <class T, class K>
HashObject<T,K>::HashObject(T a,K b)
{
	key=a;
	free=false;
	content=new K(b);
}
template <class T, class K>
HashObject<T,K>& HashObject<T,K>::operator =(HashObject<T,K>& obj)
{
	key=obj.key;
	free=obj.free;
	content=obj.content;
	return this;
}
template <class T, class K>
bool HashObject<T,K>::operator ==(HashObject<T,K>& obj)
{
	return *(content)==*(obj.content);
}
template <class T, class K>
void HashObject<T,K>::deleteContent()
{
	if(content)
	{
		delete content;
		content=NULL;
	}
}
template <class T, class K>
T HashObject<T,K>::getKey()
{
	return key;
}
template <class T, class K>
K* HashObject<T,K>::getContent()
{
	return content;
}
template <class T, class K>
bool HashObject<T,K>::isEqKey(T a)
{
	return key==a;
}
template <class T, class K>
HashObject<T,K>::~HashObject()
{
	if(content)
	{
		delete content;
		content=NULL;
	}
}
#endif
