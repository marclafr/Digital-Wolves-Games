#ifndef __c2List_H__
#define __c2List_H__

#include "c2Defs.h"

/**
* Contains items from double linked list
*/
template<class tdata>
struct c2List_item
{
	tdata                 data;
	c2List_item<tdata>*   next;
	c2List_item<tdata>*   prev;

	inline c2List_item(const tdata& _data)
	{
		data = _data;
		next = prev = NULL;
	}

	~c2List_item()
	{}
};

/**
* Manages a double linked list
*/
template<class tdata>
class c2List
{

public:

	c2List_item<tdata>*   start;
	c2List_item<tdata>*   end;

private:

	unsigned int  size;

public:

	/**
	* Constructor
	*/
	inline c2List()
	{
		start = end = NULL;
		size = 0;
	}

	/**
	* Destructor
	*/
	~c2List()
	{
		clear();
	}

	/**
	* Get Size
	*/
	unsigned int count() const
	{
		return size;
	}

	/**
	* Add new item
	*/
	c2List_item<tdata>* add(const tdata& item)
	{
		c2List_item<tdata>*   p_data_item;
		p_data_item = new c2List_item < tdata >(item);

		if(start == NULL)
		{
			start = end = p_data_item;
		}
		else
		{
			p_data_item->prev = end;
			end->next = p_data_item;
			end = p_data_item;
		}

		++size;
		return(p_data_item);
	}

	/**
	* Deletes an item from the list
	*/
	bool del(c2List_item<tdata>* item)
	{
		if(item == NULL)
		{
			return (false);
		}

		// Now reconstruct the list
		if(item->prev != NULL)
		{
			item->prev->next = item->next;

			if(item->next != NULL)
			{
				item->next->prev = item->prev;
			}
			else
			{
				end = item->prev;
			}
		}
		else
		{
			if(item->next)
			{
				item->next->prev = NULL;
				start = item->next;
			}
			else
			{
				start = end = NULL;
			}
		}

		RELEASE(item);
		--size;
		return(true);
	}

	/**
	* Destroy and free all mem
	*/
	void clear()
	{
		c2List_item<tdata>*   p_data;
		c2List_item<tdata>*   p_next;
		p_data = start;

		while(p_data != NULL)
		{
			p_next = p_data->next;
			RELEASE(p_data);
			p_data = p_next;
		}

		start = end = NULL;
		size = 0;
	}

	/**
	* read / write operator access directly to a position in the list
	*/
	tdata& operator  [](const unsigned int index)
	{
		long                  pos;
		c2List_item<tdata>*   p_item;
		pos = 0;
		p_item = start;

		while(p_item != NULL)
		{
			if(pos == index)
			{
				break;
			}

			++pos;
			p_item = p_item->next;
		}

		return(p_item->data);
	}

	/**
	* const read operator access directly to a position in the list
	*/
	const tdata& operator  [](const unsigned int index) const
	{
		long                  pos;
		c2List_item<tdata>*   p_item;
		pos = 0;
		p_item = start;

		while(p_item != NULL)
		{
			if(pos == index)
			{
				break;
			}

			++pos;
			p_item = p_item->next;
		}

		ASSERT(p_item);

		return(p_item->data);
	}

	/**
	* const read operator access directly to a position in the list
	*/
	const c2List<tdata>& operator +=(const c2List<tdata>& other_list)
	{
		c2List_item<tdata>*   p_item = other_list.start;

		while(p_item != NULL)
		{
			add(p_item->data);
			p_item = p_item->next;
		}

		return(*this);
	}

	/**
	* const access to a node in a position in the list
	*/
	const c2List_item<tdata>* At(unsigned int index) const
	{
		long                  pos = 0;
		c2List_item<tdata>*   p_item = start;

		while(p_item != NULL)
		{
			if(pos++ == index)
				break;

			p_item = p_item->next;
		}

		return p_item;
	}

	/**
	* access to a node in a position in the list
	*/
	c2List_item<tdata>* At(unsigned int index)
	{
		long                  pos = 0;
		c2List_item<tdata>*   p_item = start;

		while(p_item != NULL)
		{
			if(pos++ == index)
				break;

			p_item = p_item->next;
		}

		return p_item;
	}

	// Sort
	int BubbleSort()
	{
		int ret = 0;
		bool swapped = true;

		while(swapped)
		{
			swapped = false;
			c2List_item<tdata>* tmp = start;

			while(tmp != NULL && tmp->next != NULL)
			{
				++ret;
				if(tmp->data > tmp->next->data)
				{
					SWAP(tmp->data, tmp->next->data);
					swapped = true;
				}

				tmp = tmp->next;
			}
		}

		return ret;
	}

	/**
	* returns the first apperance of data as index (-1 if not found)
	*/
	int find(const tdata& data)
	{
		c2List_item<tdata>* tmp = start;
		int index = 0;

		while(tmp != NULL)
		{
			if(tmp->data == data)
				return(index);

			++index;
			tmp = tmp->next;
		}
		return (-1);
	}

	void InsertAfter(uint position, const c2List<tdata>& list)
	{
		c2List_item<tdata>* p_my_list = At(position);
		c2List_item<tdata>* p_other_list = list.start;

		while(p_other_list != NULL)
		{
			c2List_item<tdata>* p_new_item = new c2List_item<tdata>(p_other_list->data);

			p_new_item->next = (p_my_list) ? p_my_list->next : NULL;

			if(p_new_item->next != NULL)
				p_new_item->next->prev = p_new_item;
			else
				end = p_new_item;

			p_new_item->prev = p_my_list;
				
			if(p_new_item->prev != NULL)
				p_new_item->prev->next = p_new_item;
			else
				start = p_new_item;

			p_my_list = p_new_item;
			p_other_list = p_other_list->next;
		}
	}
};

#endif /*__c2List_H__*/