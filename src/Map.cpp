#include <iostream>
#include <fstream>
#include <queue>

template <class Key, class Value>
class Node
{
public:
	Key data;
	Value value;
	int height;
	Node<Key, Value>* left, * right, * parent;
	Node(Key& data, Value& value)
	{
		this->data = data;
		this->value = value;
		height = 1;
		left = nullptr;
		right = nullptr;
		parent = nullptr;
	}
};

template <class Key, class Value, class Compare = std::less <Key> >
class Map
{
protected:

	Compare cmp; //COMPARATOR
	int height(Node<Key, Value>* x)
	{
		if (x == nullptr)
			return 0;
		else return x->height;
	}
	void preordine(Node<Key, Value>* nod)
	{
		std::cout << "\n" << nod->data << " " << "fb = "
			<< height(nod->right) - height(nod->left);
		if (nod->left != nullptr)
			preordine(nod->left);
		if (nod->right != nullptr)
			preordine(nod->right);
	}
	void inordine(Node<Key, Value>* nod)
	{
		if (nod->left != nullptr)
			inordine(nod->left);
		std::cout << "\n" << nod->data << " " << "fb = " << height(nod->right) - height(nod->left);
		if (nod->right != nullptr)
			inordine(nod->right);
	}
	void postordine(Node<Key, Value>* nod)
	{
		if (nod->left != nullptr)
			postordine(nod->left);
		if (nod->right != nullptr)
			postordine(nod->right);
		std::cout << "\n" << nod->data << " " << "fb = " << height(nod->right) - height(nod->left);
	}
	void niveluri()
	{
		std::queue< Node <Key, Value>* > q;
		q.push(root);
		while (!q.empty())
		{
			std::cout << "\n" << q.front()->data << " " << "fb = " << height(q.front()->right) - height(q.front()->left);
			if (q.front()->left != nullptr)
				q.push(q.front()->left);
			if (q.front()->right != nullptr)
				q.push(q.front()->right);
			q.pop();
		}
	}
	void transplant(Node<Key, Value>* x, Node<Key, Value>* y)
	{
		if (x->parent == nullptr)
			root = y;
		else
		{
			if (x->parent->left == x)
				x->parent->left = y;
			else x->parent->right = y;
		}
		if (y != nullptr)
		{
			y->parent = x->parent;
			y->height = std::max(height(y->left), height(y->right)) + 1; //test
		}
	}
	void parcurgere_clear(Node<Key, Value>* nod)
	{
		if (nod->left != nullptr)
			parcurgere_clear(nod->left);
		if (nod->right != nullptr)
			parcurgere_clear(nod->right);
		erase_node(nod);
	}
public:

	Node<Key, Value>* root;
	Map()
	{
		root = nullptr;
	}

	void insert(Key key, Value value)
	{
		if (find(key) == nullptr)
		{
			Node<Key, Value>* z = new Node<Key, Value>(key, value);
			Node<Key, Value>* y = nullptr;
			Node<Key, Value>* x = root;
			while (x != nullptr)
			{
				y = x;
				if (cmp(x->data, key)) //COMPARATOR
					x = x->right;
				else x = x->left;
			}
			z->parent = y;
			if (y == nullptr)
				root = z;
			else if (cmp(y->data, key)) //COMPARATOR
				y->right = z;
			else y->left = z;
			if (y != nullptr)
				insert_repair(y);
		}
	}
	void insert_repair(Node<Key, Value>* x)
	{
		x->height = std::max(height(x->right), height(x->left)) + 1;
		int fb = height(x->right) - height(x->left);
		while (fb != 0)
		{
			if (fb == 1 || fb == -1)
				x = x->parent;
			else if (fb == -2)
			{
				int fb_left = height(x->left->right) - height(x->left->left);
				if (fb_left == -1)
					rotate_right(x);
				else
				{
					rotate_left(x->left);
					rotate_right(x);
				}
			}
			else
			{
				int fb_right = height(x->right->right) - height(x->right->left);
				if (fb_right == 1)
					rotate_left(x);
				else
				{
					rotate_right(x->right);
					rotate_left(x);
				}
			}
			if (x == nullptr)
				break;
			x->height = std::max(height(x->right), height(x->left)) + 1;
			fb = height(x->right) - height(x->left);
		}
	}
	Node<Key, Value>* max(Node<Key, Value>* x)
	{
		while (x->right != nullptr)
			x = x->right;
		return x;
	}
	Node<Key, Value>* min(Node<Key, Value>* x)
	{
		while (x->left != nullptr)
			x = x->left;
		return x;
	}
	Node<Key, Value>* succesor(Node<Key, Value>* x)
	{
		if (x->right != nullptr)
			return min(x->right);
		Node<Key, Value>* y = x->parent;
		while (y != nullptr && x == y->right)
		{
			x = y;
			y = y->parent;
		}
		return y;
	}
	Node<Key, Value>* predecesor(Node<Key, Value>* x)
	{
		if (x->left != nullptr)
			return max(x->left);
		Node<Key, Value>* y = x->parent;
		while (y != nullptr && x == y->left)
		{
			x = y;
			y = y->parent;
		}
		return y;
	}
	Node<Key, Value>* find(Key key)
	{
		Node<Key, Value>* search = root;
		while (search != nullptr && search->data != key)
		{
			if (cmp(search->data, key)) //COMPARATOR
				search = search->right;
			else search = search->left;
		}
		return search;
	}
	void remove_key(Key key)
	{
		erase_node(find(key));
	}
	void erase_node(Node<Key, Value>* x)
	{
		if (x != nullptr)
		{
			if (root == x && x->left == nullptr && x->right == nullptr)
			{
				root = nullptr;
				delete x;
			}
			else if (x->height == 1 && root != x)
			{
				if (x->parent->left == x)
				{
					x->parent->left = nullptr;
					delete_repair(x->parent);
					delete x;
					x = nullptr;
				}
				else if (x->parent->right == x)
				{
					x->parent->right = nullptr;
					delete_repair(x->parent);
					delete x;
					x = nullptr;
				}
			}
			else
			{
				if (x->left != nullptr && x->right == nullptr)
				{
					transplant(x, x->left);
					delete x;
					x = nullptr;
				}
				else if (x->left == nullptr && x->right != nullptr)
				{
					transplant(x, x->right);
					delete x;
					x = nullptr;
				}
				else
				{
					Node<Key, Value>* y = succesor(x);
					Node<Key, Value>* parent_y = nullptr;
					if (y->parent != x)
					{
						parent_y = y->parent;
						transplant(y, y->right);
						y->right = x->right;
						if (x->right != nullptr)
							y->right->parent = y;
					}
					transplant(x, y);
					if (parent_y == nullptr)
						parent_y = y->parent;
					y->left = x->left;
					if (y->left != nullptr)
						y->left->parent = y;
					delete x;
					x = nullptr;
					if (parent_y != nullptr)
						delete_repair(parent_y);
				}
			}
		}
	}
	void delete_repair(Node<Key, Value>* x)
	{
		x->height = std::max(height(x->right), height(x->left)) + 1;
		int fb = height(x->right) - height(x->left);
		while (fb != -1 && fb != 1)
		{
			if (fb == 0)
				x = x->parent;
			if (fb == -2)
			{
				int fb_left = height(x->left->right) - height(x->left->left);
				if (fb_left == -1 || fb_left == 0)
				{
					rotate_right(x);
					if (height(x) - height(x->parent->left) == 0)
					{
						x = x->parent->parent;
					}
				}
				else
				{
					rotate_left(x->left);
					rotate_right(x);
					x = x->parent;
				}
			}
			else if (fb == 2)
			{
				int fb_right = height(x->right->right) - height(x->right->left);
				if (fb_right == 1 || fb_right == 0)
				{
					rotate_left(x);
					if (height(x->parent->right) - height(x) == 0)
					{
						x = x->parent->parent;
					}
				}
				else
				{
					rotate_right(x->right);
					rotate_left(x);
					x = x->parent;
				}
			}
			if (x == nullptr)
				break;
			x->height = std::max(height(x->right), height(x->left)) + 1;
			fb = height(x->right) - height(x->left);
		}
	}
	void rotate_left(Node<Key, Value>* x)
	{
		if (x->right != nullptr)
		{
			Node<Key, Value>* y = x->right;
			x->right = y->left;
			if (y->left != nullptr)
				y->left->parent = x;
			y->parent = x->parent;
			if (x->parent == nullptr)
				root = y;
			else if (x == x->parent->left)
				x->parent->left = y;
			else x->parent->right = y;
			y->left = x;
			x->parent = y;
			x->height = std::max(height(x->left), height(x->right)) + 1;
			y->height = std::max(height(y->left), height(y->right)) + 1;
		}
	}
	void rotate_right(Node<Key, Value>* x)
	{
		if (x->left != nullptr)
		{
			Node<Key, Value>* y = x->left;
			x->left = y->right;
			if (y->right != nullptr)
				y->right->parent = x;
			y->parent = x->parent;
			if (x->parent == nullptr)
				root = y;
			else if (x == x->parent->left)
				x->parent->left = y;
			else x->parent->right = y;
			y->right = x;
			x->parent = y;
			x->height = std::max(height(x->left), height(x->right)) + 1;
			y->height = std::max(height(y->left), height(y->right)) + 1;
		}
	}
	void print_tree(int opt)
	{
		if (opt == 1)
		{
			std::cout << "\nParcurgerea in preordine este: ";
			if (root != nullptr)
				preordine(root);
		}
		else if (opt == 2)
		{
			std::cout << "\nParcurgerea in inordine este: ";
			if (root != nullptr)
				inordine(root);
		}
		else if (opt == 3)
		{
			std::cout << "\nParcurgerea in postordine este: ";
			if (root != nullptr)
				postordine(root);
		}
		else
		{
			std::cout << "\nParcurgerea pe niveluri este: ";
			if (root != nullptr)
				niveluri();
		}
	}
	void clear()
	{
		while (!empty())
			parcurgere_clear(root);
	}
	bool empty()
	{
		if (root == nullptr)
			return 1;
		return 0;
	}
	void construct(std::vector <int> v)
	{
		for (int i = 0; i < v.size(); i++)
			insert(v[i], 1);
	}
	Map<Key, Value, Compare>& operator=(const Map<Key, Value, Compare>& another_map)
	{
		root = another_map.root;
		return *this;
	}
	int& operator[](Key key)
	{
		Node<Key, Value> *nod = find(key);
		if (nod != nullptr)
			nod->data = key;
		else
		{ 
			insert(key, 0);
			nod = find(key);
		}
		return nod->value;
	}
	class Iterator
	{
	private:
		Map <Key, Value, Compare>* map;
		Node<Key, Value>* nod;
		using pointer = Node<Key, Value>*;
		using reference = Node<Key, Value>&;
	public:
		Iterator(Map<Key, Value, Compare>* another_map)
		{
			map = another_map;
			nod = another_map->root;
		}
		Iterator(pointer ptr, Map<Key, Value, Compare> *another_map)
		{
			nod = ptr;
			map = another_map;
		}
		Iterator(const Iterator& it)
		{
			map = it.map;
			this->nod = it.nod;
		}
		Iterator& operator=(const Iterator& it)
		{
			map = it.map;
			nod = it.nod;
			return *this;
		}
		bool operator==(const Iterator& it) const
		{
			return (nod == it.nod);
		}
		bool operator!=(const Iterator& it) const
		{
			return (nod != it.nod);
		}
		Iterator& operator++()
		{
			nod = map->succesor(nod);
			return *this;
		}
		Iterator& operator++(int)
		{
			Iterator tmp(map->succesor(nod), map);
			nod = map->succesor(nod);
			return tmp;
		}
		Iterator& operator--()
		{
			nod = map->predecesor(nod);
			return *this;
		}
		Iterator& operator--(int)
		{
			Iterator tmp(map->predecesor(nod), map);
			nod = map->predecesor(nod);
			return tmp;
		}
		reference operator*()
		{
			return *nod;
		}
		pointer operator->()
		{
			return nod;
		}
		Iterator begin() const
		{
			Iterator it(map->root, map);
			if (it.nod != nullptr)
			it.nod = map->min(it.nod);
			return it;
		}
		Iterator end() const
		{
			Iterator it(map->root, map);
			if(it.nod != nullptr)
			it.nod = map->max(it.nod)->right;
			return it;
		}
		Iterator rbegin() const
		{
			Iterator it(map->root, map);
			if (it.nod != nullptr)
				it.nod = map->min(it.nod)->left;
			return it;
		}
		Iterator rend() const
		{
			Iterator it(map->root, map);
			if (it.nod != nullptr)
				it.nod = map->max(it.nod);
			return it;
		}
	};
};

int main()
{
	Map<std::string, int> map;
	std::ifstream in("input.txt");
	int n;
	in >> n;
	for (int i = 0; i < n; ++i)
	{ 
		std::string s;
		int val;
		in >> s >> val;
		map.insert(s,val);
	}
	Map<std::string, int>::Iterator it(&map);
	for (it = it.begin(); it != it.end(); ++it)
		std::cout << it->data << " " << it->value << '\n';
	std::cout << '\n';
	std::string s1, s2;
	int val1, val2;
	std::cin >> s1 >> val1;
	std::cin >> s2 >> val2;
	map[s1] = val1;
	map[s2] = val2;
	std::cout << '\n';

	for (it = it.begin(); it != it.end(); ++it)
		std::cout << it->data << " " << it->value << '\n';
	int valoare = map["ana"];
	std::cout << valoare << std::endl;
	std::cout << '\n';
	for(it = it.rend(); it != it.rbegin(); it--)
		std::cout << it->data << " " << it->value << '\n';
	return 0;
}