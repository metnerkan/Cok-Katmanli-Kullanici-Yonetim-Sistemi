#include <iostream>

class LinkedList
{
public:
	LinkedList() = delete;
	LinkedList(int data)
	{
		this->data = data;
		this->next = NULL;
	}
	int data;
	LinkedList* next;
	void addTail(int data)
	{
		LinkedList* curr = this;
		LinkedList* wba = new LinkedList(data);
		if (!curr->next)
			curr->next = wba;
		else
		{
			while (curr->next)
				curr = curr->next;
			curr->next = wba;
		}
	}
	void printList()
	{
		LinkedList* curr = this;
		while (curr)
		{
			std::cout << curr->data << std::endl;
			curr = curr->next;
		}
	}
private:
};

int main()
{
	LinkedList* head = new LinkedList(9);

}