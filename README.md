# ArduinoSequenceContainer
Arduino compatible implementation of a sequence container, similar to vector of STL

## Method list:

```c++
template <class TElem>
//Adds element before the point of interest.
void add(const TElem&);
//Deletes the element where the point of interest is. 
//The point of interest is moved to the next element
void del();
//Returns the element where the point of interest is.
const TElem& actual() const;
//Forwards the point of interest one place.
void forward();
//Makes the point of interest be the first element.
void restart();
//True if the point of interest is at the end of the sequence.
bool isEnd() const;
//True if sequence is empty.
bool isEmpty() const;
//Returns number of elements.
int numElems() const; 
//Reverses sequence. Leaves POI as is
void reverse();
//Leaves POI where found or at end
bool find(const TElem&);
//Removes a given element
bool remove(const TElem&);
//Returns elem at given index
const TElem& getByPos(const int pos) const;

```

### Usage:

```c++
//Declare
CSequence<type> name;
//Using object methods
name.method(typeVar);
//Iterate
if (!name.isEmpty()) {
	for (name.restart(); !name.isEnd(); name.forward()) {
    	//Do something
  	}
}
```

See example folder for in-depth usage
