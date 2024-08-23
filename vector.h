#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <stdexcept>

template <typename T>
class Vector{
	
	public:
		class ConstIterator;
  		class Iterator; 
		using value_type = T; //T -> template 
		using size_type = std::size_t;
 		using difference_type = std::ptrdiff_t;
 	 	using reference = value_type&;
  		using const_reference = const value_type&;
  		using pointer = value_type*;
  		using const_pointer = const value_type*;
  		using iterator = Vector::Iterator;
  		using const_iterator = Vector::ConstIterator;
		
	private:
		static constexpr std::size_t min_sz = 10; //Min size Vector = 10
		std::size_t max_sz; //vectorsize
		std::size_t sz; //Number elements
		pointer values; //pointer to stored chars
		
		void realloc(std::size_t new_sz) {
			pointer new_values{new value_type[new_sz]}; //new memoryspace with size new_sz
			for(std::size_t i{0}; i < sz; ++i){
				new_values[i] = values[i]; //copy all elements into new space
			}
			delete[] values; //delete old space and variables
			values = new_values;
			max_sz = new_sz;
		}
		
		std::size_t grow() { //function to refactor memory size
			return max_sz*2;
		}
	
	public:
		Vector() : max_sz{min_sz}, sz{0}, values{new value_type[min_sz]} {} //default constructor: def size = 10
		
		Vector(std::size_t max_sz) : max_sz{max_sz < min_sz ? min_sz:max_sz}, sz{0}, values{new value_type[this->max_sz]} {} //Constructor with size as parameter: If max_sz > min_sz = size = max_sz, 0 elements, new memory area size max_szConstructor with size as parameter: If max_sz > min_sz = size = max_sz, 0 elements, new memory area size max_sz
		
		Vector(std::initializer_list<value_type> ilist) : Vector(ilist.size()) { //2. Constructor is called with max_sz = ilist.size(), then copied elements individually into memory area with for loop
			for(const auto& val : ilist){
				values[sz++] = val;
			}
		}
		
		Vector(const Vector& src) : Vector(src.sz) { //Copy constructor save elements of src in this->values
			for(; sz < src.sz; ++sz) {
				values[sz] = src.values[sz];
			}
		}
		
		~Vector(){ //Delete  space
			delete[] values;
		}
		
		Vector& operator=(Vector src) { 
			std::swap(max_sz, src.max_sz); 
			std::swap(sz, src.sz);
			std::swap(values, src.values);
			return *this; //swap all values

		}
		
		void push_back(const_reference val) {
			if(sz >= max_sz){ //if elements at the end, expand vector size = realloc
				realloc(grow()); 
			}
			values[sz++] = val; //then values ​​at sz = val and increase sz
		}
		
		reference operator[](std::size_t index) {
			if(index >= sz){ //if you want to access an element that does not exist (index outside the memory area)
				throw std::runtime_error{"Index out of bounds"}; 
			}
			return values[index];
		}
		
		const_reference operator[](std::size_t index) const {
			if(index >= sz){
				throw std::runtime_error{"Index out of bounds"};
			}
			return values[index];
		}
		
		std::size_t size() const{ //size of vector = number of elements
			return sz;
		}
		
		bool empty() const { //checks whether vector is empty
			if(sz == 0) {
				return true; 
			}
			return false;   //return sz == 0 ? true:false;
		}
		
		void clear() {
			pointer new_values{new value_type[min_sz]}; //new memory area with minimum size
			delete[] values; //delete old memory area and change instance variables
			values = new_values;
			sz = 0;
		}
		
		void reserve(std::size_t n) { //Expand memory area to desired value
			if(max_sz < n){ //n must be > max_sz (and of course min_sz)!!!
				realloc(n);
			}
		}
		
		void shrink_to_fit() { //Reduce the memory area to the desired value so that the last element is at the end of the vector
			if(sz < min_sz){ //if number of elements < 10, then vector is only reduced to size 10 (because it has to be at least that big)
				realloc(min_sz);
			}
			else{
				realloc(sz);
			}
		}
		
		void pop_back() {
			if(sz == 0) { //if there are no elements, you cannot delete them
				throw std::runtime_error{"No deletable element found"};
			}
			std::size_t temp;
			if(sz > min_sz){
				temp = sz;
			}
			else {
				temp = min_sz;
			}
			pointer new_values{new value_type[temp]};
			for(std::size_t i{0}; i < sz-1; ++i){ //copy all elements except the last one
				new_values[i] = values[i];
			}
			delete[] values;
			values = new_values;
			sz -= 1; //Sz -1 because an element was deleted
		}
		
		std::size_t capacity() const { //Returns vector memory size
			return max_sz;
		}
		
		
		std::ostream& print(std::ostream& o) const {
    			o << "[";
   			bool first{true};
    			for (std::size_t i = 0; i < sz; ++i) {
        			if (first) {
            		o << values[i];
            		first = false;
        			} else {
            		o << ", " << values[i];
        			}
    			}
    			o << "]";
    		return o;
		}
		
	  iterator begin() {
			if(sz == 0) {
				return end();
			}
			return Iterator {values, values +sz}; //returns an iterator that points to the first element and can go up to a maximum of values ​​+ sz
		}
		
		iterator end() {
			return Iterator{values + sz, values +sz}; //points to the end (behind the last element) of the vector (not the end of the memory area)
		}
		
		const_iterator begin() const{ //Analogous to iterator methods
			if(sz == 0) {
				return end();
			}
			return ConstIterator{values, values + sz};
		}
		
		const_iterator end() const{
			return ConstIterator {values + sz, values + sz};
		}
		
		iterator insert(const_iterator pos, const_reference val) {
  				auto diff = pos - begin();
  				if (diff < 0 || static_cast<size_type>(diff) > sz){
    					throw std::runtime_error("Iterator out of bounds");
    			}
  				size_type current{static_cast<size_type>(diff)};
  				if (sz >= max_sz){
    				reserve(max_sz * 2); 
    			}
  				for (auto i{sz}; i-- > current;){
    					values[i + 1] = values[i];
    			}
  				values[current] = val;
  				++sz;
  				return iterator{values + current, values +sz};
		}

		iterator erase(const_iterator pos) {
  			auto diff = pos - begin();
  			if (diff < 0 || static_cast<size_type>(diff) >= sz){
    			throw std::runtime_error("Iterator out of bounds");
    		}
  			size_type current{static_cast<size_type>(diff)};
  			for (auto i{current}; i < sz - 1; ++i){
    			values[i] = values[i + 1];
    		}
  			--sz;
  			return iterator{values + current, values + sz};
		}
		
		
			class Iterator {
   				 public:
   				 	using value_type = Vector::value_type;
      				using reference = Vector::reference;
      				using pointer = Vector::pointer;
      				using difference_type = Vector::difference_type;
    					using iterator_category = std::forward_iterator_tag;
    				
    				 private:
      				pointer ptr; //pointer to memory area
      				pointer end_ptr; //end of the pointer (no further than this memory area) for secure iterator, in order not to look outside the vector
      				
   				 public:
      				
      				Iterator () : ptr{nullptr}, end_ptr{nullptr} {}
      				//Iterator (pointer ptr) : ptr{ptr}, end_ptr{nullptr} {}
      				Iterator (pointer ptr, pointer end_ptr) : ptr{ptr}, end_ptr{end_ptr} {}
      				
      				reference operator*() const{
      					if (ptr == end_ptr) {
      						throw std::runtime_error("Dereferenzierung des End-Iterators");
      					}
      					return *ptr;
      				}
      				
      				pointer operator->() const{
      					if (ptr == end_ptr) {
      						throw std::runtime_error("Dereferenzierung des End-Iterators");
      					}
      					return ptr;
      				}
      				
      				bool operator==(const const_iterator& i) const{
      					ConstIterator temp{ptr, end_ptr}; //Convert this to ConstIterator to call the const_iterator method ==
      					if(temp == i){
      						return true;
      					}
      					return false;
      				}
      				
      				bool operator!=(const const_iterator& i) const{
      					ConstIterator temp{ptr, end_ptr}; //analogous to operator==
      					if(temp == i){
      						return false;
      					}
      					return true;
      				}
      				
      				Iterator& operator++() {
      					if(ptr != end_ptr){ //If ptr hasn't reached the end yet, it will be incremented, otherwise nothing will be done
      						++ptr;
      					}
      					return *this;
      				}
      				
      				Iterator operator++(int){ //Analogous to operator++, except that the old value of ptr is returned
      					Iterator old = *this;
      					if(ptr != end_ptr){
      						++ptr;
      					}
      					return old;
      				}
      				
      				operator const_iterator() const{
      					return ConstIterator{ptr, end_ptr};
      				}
      				
      				Vector::difference_type operator-(const Vector::Iterator& rop) {
  							return ptr - rop.ptr;
							}
      				      				
  			};
  			
  			class ConstIterator {
    				public:
      				using value_type = Vector::value_type;
      				using reference = Vector::const_reference;
      				using pointer = Vector::const_pointer;
      				using difference_type = Vector::difference_type;
      				using iterator_category = std::forward_iterator_tag;
    				
    				private:
      				pointer ptr;
      				pointer end_ptr;
      				
    				public:
    					ConstIterator() : ptr{nullptr}, end_ptr{nullptr} {}
      				//ConstIterator(pointer ptr) : ptr{ptr}, end_ptr{nullptr} {}
      				ConstIterator (pointer ptr, pointer end_ptr) : ptr{ptr}, end_ptr{end_ptr} {}
    					
      				reference operator*() const{
      					if (ptr == end_ptr) {
      						throw std::runtime_error("Dereferenzierung des End-Iterators");
      					}
      					return *ptr;
      				}
      				
      				pointer operator->() const{
      					if (ptr == end_ptr) {
      						throw std::runtime_error("Dereferenzierung des End-Iterators");
      					}
      					return ptr;
      				}
      				
      				bool operator==(const const_iterator& i) const{
      					if(ptr == i.ptr && end_ptr == i.end_ptr){ //Comparison of the two instance variables ptr and ptr_ende
      						return true;
      					}
      					return false;
      				}
      				
      				bool operator!=(const const_iterator& i) const{
      					if(ptr == i.ptr && end_ptr == i.end_ptr){ //analogous to operator==
      						return false;
      					}
      					return true;
      				}
      				
      				ConstIterator& operator++() {
      					if(ptr != end_ptr){ 
      						++ptr;
      					}
      					return *this;
      				}
      				
      				ConstIterator operator++(int){
      					ConstIterator old = *this;
      					if(ptr != end_ptr){
      						++ptr;
      					}
      					return old;
      				}
      				
      				
      				Vector::difference_type operator-(const Vector::ConstIterator& rop) {
  							return ptr - rop.ptr;
							}
  			};
			
		
};

template<typename T>
std::ostream& operator<<(std::ostream& o, const Vector<T>& v) { 
			return v.print(o);
		}

#endif
