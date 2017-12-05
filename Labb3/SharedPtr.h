#pragma once

using namespace std;


class RC
{
private:
	int count; // Reference count

public:
	void AddRef()
	{
		// Increment the reference count
		count++;
	}

	int Release()
	{
		// Decrement the reference count and
		// return the reference count.
		return --count;
	}

	int GetCount() {
		return count;
	}
};

template<typename C> 
class SharedPtr {
	RC* reference;
	
public:
	C* ptr;

	SharedPtr() : ptr(0) {
		reference = new RC();
		reference->AddRef();
	}

	SharedPtr(C* obj) : ptr(obj) {
		reference = new RC();
		reference->AddRef();
	}

	SharedPtr(nullptr_t) : ptr(nullptr), reference(nullptr) {
		
	}

	SharedPtr(const SharedPtr<C>& other) : ptr(other.ptr), reference(other.reference) {
		//Copy constructor
		//Copy data and reference pointer
		//increment reference count
		reference->AddRef();
	}

	SharedPtr(SharedPtr&& other) : ptr(other.ptr), reference(other.reference) {
		//Move constructor
		other.ptr = nullptr;
		other.reference = nullptr;
	}

	~SharedPtr() {
		//Destructor
		//Decrement the reference count
		//if reference count is 0 delete
		if (reference == nullptr) {
			delete ptr;
			delete reference;
		} else if (reference->Release() == 0) {
			delete ptr;
			delete reference;
		}
	}

	SharedPtr<C>& operator=(const SharedPtr<C>& other) {
		//Assignment operator
		if (this != &other) {
			//Decrement the old reference count
			//If reference becomes zero delete the old data
			if (reference->Release() == 0) {
				delete ptr;
				delete reference;
			}

			//Copy the data and reference pointer
			//and increment the reference count
			ptr = other.ptr;
			reference = other.reference;
			reference->AddRef();
		}
		return *this;
	}

	bool operator==(SharedPtr<C>& compare) const {
		C* rhs = compare.get();
		C* lhs = this->get();
		return lhs->value == rhs->value;
	}

	bool operator==(nullptr_t) {
		if (this->ptr == nullptr) {
			return true;
		}
		return false;
	}

	bool operator < (const SharedPtr<C>& compare) {
		C* rhs = compare.get();
		C* lhs = this->get();
		if (lhs != nullptr && rhs != nullptr) {
			return lhs->value < rhs->value;
		}
		else if (lhs == nullptr) {
			return true;
		}
		return false;
	}

	operator bool() {
		if (ptr != nullptr) {
			return true;
		}
		return false;
	}

	//TODO:FIXA ??
	/*bool operator <(nullptr_t) {

	}*/

	C* operator->() {
		return ptr;
	}

	C& operator*() {
		return *ptr;
	}

	C* get() const {
		return ptr;
	}

	bool unique() {
		if (reference->GetCount() == 1) {
			return true;
		}
		return false;
	}

	void reset() {
		ptr = nullptr;
	}

};
