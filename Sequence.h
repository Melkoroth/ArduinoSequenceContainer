// #############################################################################
// #
// # Name       : sequence.h
// # Version    : 1.1
// # Author     : Juan L. Perez Diez <ender.vs.melkor at gmail>
// # Date       : 30.10.2016
// 
// # Description: Implements a class for using sequences of items. 
//	 Arduino compatible.
// 
// #  This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// #############################################################################

#ifndef __SEQUENCE_H
#define __SEQUENCE_H

template <class TElem>
class CSequence;

// *********************************************
// Definition of the class that stores a node of the dynamic sequence
//
// [TElem]->
// *********************************************
template <class TElem>
class CNodeSequence {
	private:
		//Atributes
		TElem _elem;
		CNodeSequence<TElem>* _next;
		//Constructors
		CNodeSequence();
		CNodeSequence(const TElem&, CNodeSequence<TElem>* = 0);
	public:	
		//Complexity of these is O(1)
		const TElem& elem() const;
		//Pointer to next node
		CNodeSequence<TElem> *next() const;
		//Best m8's
		friend class CSequence<TElem>;
};

// *********************************************
// Implements a dynamic sequence
// Iteration is made by moving a "Point of Interest" forward
//
//	[Ghost]->[TElem0]->[TElem1]->[TElem2]->[TElem3]
//	 |_first			|_prec	  |-PoI
//
// _first is a "phantom node" so we avoid special cases when the sequence is empty
// _prec points to the object BEFORE the point of interest so we get 0(1) on basic functions
// *********************************************
template <class TElem>
class CSequence {
	public:
		CSequence();
		CSequence(const CSequence<TElem>&);
		~CSequence();
		CSequence &operator=(const CSequence<TElem>&);
		//Adds element before the point of interest.
		// O(TElem::TElem(TElem&)), O(1) using standard vartypes
		void add(const TElem&);
		//Deletes the element where the point of interest is. 
		//The point of interest is moved to the next element
		//O(TElem::~TElem()), O(1) using standard vartypes
		void del();
		//Returns the element where the point of interest is. O(1)
		const TElem& actual() const;
		//Forwards the point of interest one place. O(1)
		void forward();
		//Makes the point of interest be the first element. O(1)
		void restart();
		//True if the point of interest is at the end of the sequence. O(1)
		bool isEnd() const;
		//True if sequence is empty. O(1)
		bool isEmpty() const;
		//Returns number of elements. O(1)
		int numElems() const; 
		//Reverses sequence. Leaves POI as is
		void reverse();
		//Leaves POI where found or at end
		bool find(const TElem&);
		//Removes a given element
		bool remove(const TElem&);
		//Returns elem at given index
		const TElem& getByPos(const int pos) const;
		void sortInPlace();

	private:
		CNodeSequence<TElem> *_first, *_prec;
		void copy(const CSequence<TElem>&);
		void free();
		int _numElems; 
};

// *********************************************
// Implementation of CNodeSequence
// *********************************************
template <class TElem>
inline CNodeSequence<TElem>::CNodeSequence() : _next(0) {};

template <class TElem>
inline CNodeSequence<TElem>::CNodeSequence(const TElem& elem,CNodeSequence<TElem>* next) :
        _elem(elem), _next(next) {};

template <class TElem>
inline const TElem& CNodeSequence<TElem>::elem() const {
	return _elem;
}
template <class TElem>
inline CNodeSequence<TElem>* CNodeSequence<TElem>::next() const {
	return _next;
}
// *********************************************
// Implementation of CSequence
// *********************************************
template <class TElem>
inline CSequence<TElem>::CSequence() {
	_first = _prec = new CNodeSequence<TElem>();
	_numElems = 0;
}

template <class TElem>
inline CSequence<TElem>::CSequence(const CSequence<TElem>& other) {
	copy(other);
}

template <class TElem>
inline CSequence<TElem>::~CSequence( ){
	free();
	_numElems = 0;
}

template <class TElem>
inline CSequence<TElem>& CSequence<TElem>::operator=(const CSequence<TElem>& other) {
	if( this != &other ) {
		free();
		copy(other);
	}
	return *this;
}

template <class TElem>
void CSequence<TElem>::add(const TElem& elem) {
	CNodeSequence<TElem>* nuevo = new CNodeSequence<TElem>(elem, _prec->next());
	_prec->_next = nuevo;
	_prec = nuevo;
	_numElems++;
}

template <class TElem>
void CSequence<TElem>::del() {
	CNodeSequence<TElem>* act = _prec->next();
	_prec->_next = act->next(); 
	delete act;
	_numElems--;
}

template <class TElem>
inline const TElem& CSequence<TElem>::actual() const {
	return _prec->next()->elem();
}

template <class TElem>
inline void CSequence<TElem>::forward() {
	_prec = _prec->next();
}

template <class TElem>
inline void CSequence<TElem>::restart() {
	_prec = _first;
}

template <class TElem>
inline bool CSequence<TElem>::isEnd() const {
	return _prec->next() == 0;
}

template <class TElem>
inline bool CSequence<TElem>::isEmpty() const {
	return _first->next() == 0;
}

template <class TElem>
inline int CSequence<TElem>::numElems() const{	
	return _numElems;
}

template <class TElem>
void CSequence<TElem>::free() {
	while (_first != 0){
		_prec = _first;
		_first = _first->next();
		delete _prec;
	}
	_numElems = 0;
}

template <class TElem>
void CSequence<TElem>::copy(const CSequence<TElem>& sequence) {
	CNodeSequence<TElem> *antcopy, *actcopy, *act;
	act = sequence._first;
	actcopy = _prec = _first = new CNodeSequence<TElem>();
	while (act->next() != 0) {
		act = act->next();
		antcopy = actcopy;
		actcopy = new CNodeSequence<TElem>(act->elem(),0);
		if (sequence._prec == act) { _prec = actcopy; }
		antcopy->_next = actcopy;
	}
	_numElems = sequence._numElems;
}

//Reverses sequence. Does not modify point of interest
template <class TElem>
void CSequence<TElem>::reverse() {
	if (!isEmpty()) {
		CNodeSequence<TElem> *prev, *act, *next;
		prev = 0;
		act = _first->next();
		next = act->next();
		while(next != 0) {
			act->_next = prev;
			prev = act;
			act = next;
			next = act->next();
		}
		act->_next = prev;
		_first->_next = act;
	}
}

//Returns element at given sequence position [0-numElems]
template <class TElem>
const TElem& CSequence<TElem>::getByPos(const int pos) const {
	if (pos <= _numElems-1) {
		CNodeSequence<TElem> *act = _first->next();	
		for (int i = 0; i < pos; i++) {
			act = act->next();
		}
		return act->elem();
	}
	return 0;
}

//If found returns true & leaves the point of interest at item.
//If not returns false & at end otherwise. O(n)
template <class TElem>
bool CSequence<TElem>::find(const TElem& elem) {
	restart();
	while (!isEnd()) {
		if (actual() == elem)
			return true;
		else 
			forward();
	}
	return false;
}

//True if it finds and removes given element. O(n)
//Leaves point of interest where element got removed or at end if not present
template <class TElem>
bool CSequence<TElem>::remove(const TElem& elem) {
	restart();
	while (!isEnd()) {
		if (actual() == elem) {
			del();
			return true;
		} else 
			forward();
	}
	return false;
}

#endif
