#include "cppgc.h"

AutoPtr::AutoPtr() {
	_ptr = nullptr;
	_count = (int *)malloc(sizeof(int));

	(*_count) = 1;
    _size = 0;
}

AutoPtr::AutoPtr(size_t size) {
    if(size == 0) {
        _ptr = nullptr;
    } else {
        _ptr = malloc(size);
        memset(_ptr, 0, size);
    }

	_count = (int *)malloc(sizeof(int));
	(*_count) = 1;
    _size = size;
}

AutoPtr::AutoPtr(void *p, size_t size) {
    if(size == 0) {
        _ptr = nullptr;
    } else {
        if(p == nullptr) {
            _ptr = nullptr;
        } else {
            _ptr = malloc(size+1);
            memset(_ptr, 0, size+1);
            memcpy(_ptr, p, size);
        }
    }

	_count = (int *)malloc(sizeof(int));
	(*_count) = 1;
    _size = size;

}

AutoPtr::AutoPtr(const AutoPtr &p) {
	if(this == &p) {
		return;
	}

	_ptr = p._ptr;
	_count = p._count;
	(*_count) ++;
    _size = p._size;
}

void AutoPtr::operator=(const AutoPtr &p) {
	if (&p == this) {
		return;
		//return *this;
	}

	Delete();

	_ptr = p._ptr;
	_count = p._count;

	(*_count) ++;
    _size = p._size;

	//return *this;
}

int AutoPtr::Count(){
	return (*_count);
}

void AutoPtr::Delete(){
	if (_count == nullptr) {
		free(_ptr);
		_ptr = nullptr;
        _size = 0;
		return;
	}


	if((*_count) > 0){
		(*_count) --;
	}

	if(0 == ((*_count))) {
        free(_count);
        if(_ptr != NULL) {
            free(_ptr);
        }
	}

	_count = nullptr;
	_ptr = nullptr;
    _size = 0;
}

AutoPtr::~AutoPtr(){
	Delete();
}

void *AutoPtr::Ptr(){
	return _ptr;
}

AutoPtr* AutoPtr::operator &(){
	return nullptr;
}

void *AutoPtr::operator new(size_t size) throw() {
	throw "Can not create AutoPtr on heap";
	return nullptr;
}

void AutoPtr::operator delete(void *){

}

std::string AutoPtr::ToString() {
    if (_ptr == nullptr || _size == 0) {
        return "";
    }
    std::string data = std::string((char*)_ptr, _size);
    return hexencoder::EncodeToHexString(data);
}

AutoPtr AutoPtr::FromString(std::string &s) {
    if(s.size() == 0) {
        return AutoPtr();
    }
    std::string p = hexencoder::DecodeHexString(s);
    return AutoPtr((void*)(p.data()), s.size()/2);
}

AutoPtrList::AutoPtrList(){
	_head = nullptr;
	_list.clear();
}

AutoPtrList::~AutoPtrList(){
	_head = nullptr;
	_list.clear();
}

std::string AutoPtrList::ToString() {
    neb::CJsonObject obj;
    neb::CJsonObject arr("[]");
    bool find_head = false;
    for(int i = 0;i<_list.size();i++) {
        std::string s = _list[i].ToString();
        arr.Add(s);
        if(_head == _list[i].Ptr()) {
            obj.Add("head", i);
            find_head = true;
        }
    }

    if(!find_head || _head == nullptr) {
        obj.Add("head", -1);
    }

    if(arr.IsArray() && arr.GetArraySize() > 0) {
        obj.Add("list",arr);
    } else {
        arr = neb::CJsonObject("[]");
        obj.Add("list", arr);
    }

    return obj.ToString();
}

AutoPtrList AutoPtrList::FromString(std::string &s) {
    neb::CJsonObject obj(s);

    neb::CJsonObject arr;
    AutoPtrList auto_ptr_list;

    obj.Get("list",arr);

    std::string tmp;
    int head = -1;
    obj.Get("head", head);

    auto_ptr_list._head = nullptr;

    if (arr.IsArray() && arr.GetArraySize() > 0) {
        int count = arr.GetArraySize();

        for(int i = 0;i<count;i++) {
            arr.Get(i, tmp);
            AutoPtr p = AutoPtr::FromString(tmp);
            if(i == head) {
                auto_ptr_list.SetHead(p);
            } else {
                auto_ptr_list.Add(p);
            }
        }
    }

    if (head < 0) {
        auto_ptr_list._head = nullptr;
    }

    return auto_ptr_list;
}

void *AutoPtrList::Add(AutoPtr &p) {
	_list.push_back(p);
	return p.Ptr();
}

void *AutoPtrList::Add(size_t size) {
	AutoPtr p(size);
	return Add(p);
}

void *AutoPtrList::SetHead(AutoPtr &p) {
	_head = p.Ptr();
	Add(p);
	return _head;
}

void *AutoPtrList::SetHead(size_t size) {
	AutoPtr p(size);
	return SetHead(p);
}

void *AutoPtrList::Ptr(int index) {
    if (index >= _list.size()) {
        return nullptr;
    }

    if (index < 0) {
        return nullptr;
    }

    return _list[index].Ptr();
}

void *AutoPtrList::Head(){
	return _head;
}

void *AutoPtrList::operator &(){
	return nullptr;
}

void *AutoPtrList::operator new(size_t size) throw(){
	throw "Can not create AutoPtrList on heap";
	return nullptr;
}

void AutoPtrList::operator delete(void *) {

}

AutoPtrList::AutoPtrList(const AutoPtrList &l){
	_list.clear();
	_head = l._head;
	_list = l._list;
}

void AutoPtrList::operator =(const AutoPtrList &l) {
	if(&l == this) {
		return;
	}

	_list.clear();
	_head = l._head;
	_list = l._list;
}

