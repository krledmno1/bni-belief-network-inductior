#ifndef _HASHTABLE_
#define _HASHTABLE_


#include <stdio.h>
#include <iostream>



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










template <class T, class K>
class HashTable
{
	private:
		HashObject<T,K>* array;
		int lenght;
		int count;
	public:
		HashTable(int);
		void insert(K);
		void withdraw(K);
		int h(K);
		int g(int);
		int find(K);
		void print();
		~HashTable();
};
template <class T, class K>
HashTable<T,K>::HashTable(int a)
{
	lenght=a;
	count=0;
	array=new HashObject<T,K>[lenght];
}
template <class T, class K>
int HashTable<T,K>::h(K a)
{
	K tmp1=a%100;
	K tmp3=a/100;
	K tmp2=tmp3%1000;
	tmp3=tmp3/1000;
	T tmp4=(T)(tmp1+tmp2+tmp3)%lenght;
	return (int)tmp4;
}
template <class T, class K>
int HashTable<T,K>::g(int key)
{
	return (key+3)%lenght;
}
template <class T, class K>
void HashTable<T,K>::insert(K s)
{
	if(count==lenght)
		throw "Hash table is full";
	int k=h(s);
	int probe=k;
	while(!array[probe].free)
	{
		probe=g(probe);
	}
	array[probe].free=false;
	array[probe].content=new K(s);
	array[probe].key=(T)k;
	count++;
}
template <class T, class K>
int HashTable<T,K>::find(K a)
{
	if(count==0)
		throw "Hash table is empty";
	int hash=h(a);
	int n=1;
	while(n!=lenght)
	{
		if(array[hash].content==NULL)
		{
			hash=g(hash);
			n++;
		}
		else 
			if(*(array[hash].content)==a)
				break;
				else 
				{
					hash=g(hash);
					n++;
				}
	}
	if(n!=lenght)
		return hash;
	else return -1;
}
template <class T, class K>
void HashTable<T,K>::withdraw(K a)
{
	if(count==0)
		throw "Hash table is empty";
	int hash=find(a);
	if(hash!=-1)
	{
		array[hash].deleteContent();
		array[hash].content=NULL;
		array[hash].key=(T)0;
		array[hash].free=true;
		count--;
	}

}


template <class T, class K>
HashTable<T,K>::~HashTable()
{
	for(int i=0;i<lenght;i++)
		if(array[i].content!=NULL)
			array[i].deleteContent();
	delete [] array;
}



#endif

