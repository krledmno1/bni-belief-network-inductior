#ifndef HEAPHPP
#define HEAPHPP 

template <class T>
class Heap
{
private:
	T* array;
	int lenght;
	int noOfElements;
public:
	Heap();
	Heap(int);
	bool isEmpty();
	bool isFull();
	void insert(T&);
	T deleteRoot();
	T checkRoot();

	
	~Heap();
};
template <class T>
Heap<T>::Heap()
{
	lenght=0;
	array=NULL;
	noOfElements=0;
}
template <class T>
Heap<T>::Heap(int a)
{
	lenght=a;
	array=new T[lenght];
	noOfElements=0;
}
template <class T>
bool Heap<T>::isEmpty()
{
	return noOfElements?false:true;
}
template <class T>
bool Heap<T>::isFull()
{
	return (noOfElements==lenght)?true:false;
}
template <class T>
void Heap<T>::insert(T& a)
{
	if(isFull())
		throw "Overflow exception";
	int i=noOfElements;	
	noOfElements++;
	while(i>0&&array[(i-1)/2]>(*a))
	{
		array[i]=array[(i-1)/2];
		i=(i-1)/2;
	}
	array[i]=a;
}
template <class T>
T Heap<T>::deleteRoot()
{
	if(isEmpty())
		throw "Underflow exception!";
	T ret=array[0],last=array[noOfElements];
	noOfElements--;
	array[0]=array[noOfElements];
	int i=0;
	while(2*i+1<noOfElements)
	{
		int child=2*i+1;
		if(child+1<noOfElements)
		{
			if(array[child]>*array[child+1])
				child++;
		}
		if(array[i]<=*array[child])
			break;
	
		
		T pom = array[i];
		array[i] = array[child];
		array[child] = pom;
		i=child;
	}
	return ret;
}

template <class T>
T Heap<T>::checkRoot()
{
	if(!this->isEmpty())
		return this->array[0];
	else return NULL;
}

template <class T>
Heap<T>::~Heap()
{
	delete [] array;
}

#endif
