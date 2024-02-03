#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <bitset>
#include <unordered_map>
using namespace std;


class skillTree;
class resourceTree;
class taskTree;
class AllocationTree;

class skillTree
{
	class skillNode
	{
		int level; // it will be used as integer value in node to sort skill tree
		string name;
		skillNode* left;
		skillNode* right;
		friend class skillTree;
	public:
		skillNode()
		{
			left = nullptr;
			right = nullptr;
		}
	};
	skillNode* root;
	skillNode* InsertNode(skillNode* root, skillNode* newNode)
	{
		if (!root)
		{
			root = newNode;
			return root;
		}

		queue<skillNode*> nodesQ;
		nodesQ.push(root);

		while (!nodesQ.empty())
		{
			skillNode* node = nodesQ.front();
			nodesQ.pop();

			if (node->left)
			{
				nodesQ.push(node->left);
			}
			else
			{
				node->left = newNode;
				return root;
			}

			if (node->right)
			{
				nodesQ.push(node->right);
			}
			else
			{
				node->right = newNode;
				return root;
			}
		}
	}
	void PrintTree(skillNode* node)
	{
		if (!node)
			return;

		PrintTree(node->left);
		cout << "(" << node->name << " " << node->level << ") ";
		PrintTree(node->right);
	}
	void InsertMinHeap(vector<skillNode*>& heap, skillNode* node)
	{
		heap.push_back(node);
		int i = heap.size() - 1;
		while (i > 0 && heap[i]->level < heap[(i - 1) / 2]->level)
		{
			swap(heap[i], heap[(i - 1) / 2]);
			i = (i - 1) / 2;
		}
	}
	void InsertMaxHeap(vector<skillNode*>& heap, skillNode* node)
	{
		heap.push_back(node);
		int i = heap.size() - 1;
		while (i > 0 && heap[i]->level > heap[(i - 1) / 2]->level)
		{
			swap(heap[i], heap[(i - 1) / 2]);
			i = (i - 1) / 2;
		}
	}
	void CreateHeap(skillNode* node, int l, vector<skillNode*>& heap)
	{
		if (!node)
			return;

		CreateHeap(node->left, l, heap);
		if (l == 3)
		{
			InsertMaxHeap(heap, node);

		}
		else
		{
			InsertMinHeap(heap, node);
		}
		CreateHeap(node->right, l, heap);

	}
	void CompareNode(skillNode* node, skillNode* toCompare, bool& found, int strategy)	//traverses resource tree and compares skillNode of skillTree of task tree
	{
		if (!node)
			return;

		CompareNode(node->left, toCompare, found, strategy);
		if (strategy == 1)
		{
			if (node->name == toCompare->name && node->level == toCompare->level)
				found = true;
		}
		else
		{
			if (node->name == toCompare->name)
				found = true;
		}
		CompareNode(node->right, toCompare, found, strategy);
	}
	bool CompareSkillNode(skillNode* node, int strategy)	//make heap of skill tree of resource tree. this has skill tree of resource tree
	{
		if (strategy == 1)
		{
			if (node->level == 3 || node->level == 1)
			{
				vector<skillNode*> heap;
				CreateHeap(root, node->level, heap);
				bool matched = false;
				for (int i = 0; i < heap.size() && !matched; i++)
				{
					if (heap[i]->name == node->name && heap[i]->level == node->level)
					{
						matched = true;
					}
				}
				return matched;
			}
			else
			{
				bool matched = false;
				CompareNode(root, node, matched, strategy);
				return matched;
			}
		}
		else
		{
			bool matched = false;
			CompareNode(root, node, matched, strategy);
			return matched;
		}
	}
	void CompareSkillTree(skillNode* node, skillTree* skTree, bool& found, int strategy)	//left has skill tree of taskTree and right has skillTree of resourceTree
	{
		if (!node)
			return;

		CompareSkillTree(node->left, skTree, found, strategy);
		found = found && skTree->CompareSkillNode(node, strategy);
		CompareSkillTree(node->right, skTree, found, strategy);
	}
public:
	skillTree()
	{
		root = nullptr;
	}
	~skillTree()
	{

	}
	void Print()
	{
		PrintTree(root);
	}
	void Insert(skillNode* node)
	{
		root = InsertNode(root, node);
	}
	void ReadTree(fstream& fin)
	{
		int l;
		string n;
		char c;
		fin.get(c);
		while (c != '\n' && !fin.eof())
		{
			if (c != ':')
			{
				n = n + c;
			}
			else
			{
				fin.get(c);
				if (c >= '0' && c <= '9')
				{
					l = c - 48;
				}
				else
				{
					if (c == 'e')
						l = 3;
					else if (c == 'i')
						l = 2;
					else
						l = 1;
				}
				skillNode* newNode = new skillNode;
				newNode->level = l;
				newNode->name = n;
				Insert(newNode);
				fin.get(c);
				n.clear();
			}
			if (c != '\n')
				fin.get(c);
		}
	}
	bool Compare(skillTree* skTree, int strategy)
	{
		bool found = true;
		CompareSkillTree(root, skTree, found, strategy);
		return found;
	}
};


class resourceTree
{
	class resourceNode
	{
		int id; // it will be used as integer value in node to sort resource tree
		string name;
		skillTree skTree;
		resourceNode* left;
		resourceNode* right;
		friend class resourceTree;
	public:
		resourceNode()
		{
			left = nullptr;
			right = nullptr;
		}
	};
	resourceNode* resourceRoot;
	resourceNode* InsertNode(resourceNode* root, resourceNode* newNode)
	{
		if (!root)
		{
			root = newNode;
			return root;
		}

		queue<resourceNode*> nodesQ;
		nodesQ.push(root);

		while (!nodesQ.empty())
		{
			resourceNode* node = nodesQ.front();
			nodesQ.pop();

			if (node->left)
			{
				nodesQ.push(node->left);
			}
			else
			{
				node->left = newNode;
				return root;
			}

			if (node->right)
			{
				nodesQ.push(node->right);
			}
			else
			{
				node->right = newNode;
				return root;
			}
		}
	}
	void PrintTree(resourceNode* node)
	{
		if (!node)
			return;

		PrintTree(node->left);
		cout << "(" << node->name << " " << node->id << ": ";
		node->skTree.Print();
		cout << ")\n";
		PrintTree(node->right);
	}
	void CompareSkillTree(resourceNode* node, skillTree* skTree, AllocationTree* allocTree, string skillName, int idNumber, int strategy);
public:
	resourceTree()
	{
		resourceRoot = nullptr;
	}
	~resourceTree()
	{

	}
	void Print()
	{
		PrintTree(resourceRoot);
	}
	void Insert(resourceNode* node)
	{
		resourceRoot = InsertNode(resourceRoot, node);
	}
	void ReadTree(fstream& fin)
	{
		int ID = 1;
		string n;
		char c;
		while (!fin.eof())
		{
			fin.get(c);
			if (c == '|')
			{
				resourceNode* newNode = new resourceNode;
				newNode->id = ID;
				newNode->name = n;
				newNode->skTree.ReadTree(fin);
				Insert(newNode);
				ID++;
				n.clear();
			}
			else
			{
				n = n + c;
			}
		}
	}
	void CompareSkills(skillTree* skTree, AllocationTree* allocTree, string skillName, int idNumber, int strategy)
	{
		CompareSkillTree(resourceRoot, skTree, allocTree, skillName, idNumber, strategy);
	}
};

class taskNode
{
	int id; // it will be used as integer value in node to sort task tree
	string name;
	skillTree skTree;
	taskNode* left;
	taskNode* right;
	friend class taskTree;
public:
	taskNode()
	{
		left = nullptr;
		right = nullptr;
	}
	~taskNode()
	{

	}
};

class taskTree
{
	taskNode* taskRoot;
	taskNode* InsertNode(taskNode* root, taskNode* newNode)
	{
		if (!root)
		{
			root = newNode;
			return root;
		}

		queue<taskNode*> nodesQ;
		nodesQ.push(root);

		while (!nodesQ.empty())
		{
			taskNode* node = nodesQ.front();
			nodesQ.pop();

			if (node->left)
			{
				nodesQ.push(node->left);
			}
			else
			{
				node->left = newNode;
				return root;
			}

			if (node->right)
			{
				nodesQ.push(node->right);
			}
			else
			{
				node->right = newNode;
				return root;
			}
		}
	}
	void PrintTree(taskNode* node)
	{
		if (!node)
			return;

		PrintTree(node->left);
		cout << "(" << node->name << node->id << ": ";
		node->skTree.Print();
		cout << ")\n";
		PrintTree(node->right);
	}
	void CompareResources(taskNode* node, resourceTree* resTree, AllocationTree* allocTree, int strategy)
	{
		if (!node)
			return;

		CompareResources(node->left, resTree, allocTree, strategy);
		resTree->CompareSkills(&node->skTree, allocTree, node->name, node->id, strategy);
		CompareResources(node->right, resTree, allocTree, strategy);
	}
public:
	taskTree()
	{
		taskRoot = nullptr;
	}
	~taskTree()
	{

	}
	void Print()
	{
		PrintTree(taskRoot);
	}
	void Insert(taskNode* node)
	{
		taskRoot = InsertNode(taskRoot, node);
	}
	void ReadTree(fstream& fin)
	{
		int ID = 1;
		string n;
		char c;
		while (!fin.eof())
		{
			fin.get(c);
			if (c == '|')
			{
				taskNode* newNode = new taskNode;
				newNode->id = ID;
				newNode->name = n;
				newNode->skTree.ReadTree(fin);
				Insert(newNode);
				ID++;
				n.clear();
			}
			else
			{
				n = n + c;
			}
		}
	}
	void Compare(resourceTree* resTree, AllocationTree* allocTree, int strategy)
	{
		CompareResources(taskRoot, resTree, allocTree, strategy);
	}
};

class AllocationTree
{
	class AllocationNode
	{
		int id;
		string name;
		class allocResTree
		{
			class allocatedResources
			{
				int id;
				string name;
				allocatedResources* left;
				allocatedResources* right;
				friend class allocResTree;
			public:
				allocatedResources()
				{
					left = nullptr;
					right = nullptr;
				}
				~allocatedResources()
				{

				}
			};
			allocatedResources* allocResRoot;
			friend class AllocationNode;
			allocatedResources* InsertNode(allocatedResources* n, string nestedName, int nestedID)
			{
				if (!n)
				{
					allocatedResources* newNode = new allocatedResources;
					newNode->id = nestedID;
					newNode->name = nestedName;
					n = newNode;
					return n;
				}

				queue<allocatedResources*> nodesQ;
				nodesQ.push(n);

				while (!nodesQ.empty())
				{
					allocatedResources* node = nodesQ.front();
					nodesQ.pop();

					if (node->left)
					{
						nodesQ.push(node->left);
					}
					else
					{
						allocatedResources* newNode = new allocatedResources;
						newNode->id = nestedID;
						newNode->name = nestedName;
						node->left = newNode;
						return n;
					}

					if (node->right)
					{
						nodesQ.push(node->right);
					}
					else
					{
						allocatedResources* newNode = new allocatedResources;
						newNode->id = nestedID;
						newNode->name = nestedName;
						node->right = newNode;
						return n;
					}
				}
			}
			void PrintTree(allocatedResources* node)
			{
				if (!node)
					return;

				PrintTree(node->left);
				cout << "(" << node->name << " " << node->id << ") ";
				PrintTree(node->right);
			}
			string charToBin(char c)
			{
				int ascii = static_cast<int>(c);
				bitset<8> bin(ascii);
				return bin.to_string();
			}
			void stringToBin(allocatedResources* node)
			{
				string binName;
				for (int i = 0; i < node->name.size(); i++)
				{
					binName = binName + charToBin(node->name[i]);
				}
				node->name = binName;
			}
			void convertToBinary(allocatedResources* node)
			{
				if (!node)
					return;

				convertToBinary(node->left);
				stringToBin(node);
				convertToBinary(node->right);
			}
			void writeBinary(fstream& wrt, string n)
			{

				for (int i = 0; i < n.size(); i++)
				{
					wrt << n[i];
				}
				wrt << '\n';
			}
			void writeDecodedToFile(allocatedResources* node, fstream& wrt)
			{
				if (!node)
					return;

				writeDecodedToFile(node->left, wrt);
				writeBinary(wrt, node->name);
				writeDecodedToFile(node->right, wrt);
			}
		public:
			allocResTree()
			{
				allocResRoot = nullptr;
			}
			~allocResTree()
			{

			}
			void Insert(string nestedName, int nestedID)
			{
				allocResRoot = InsertNode(allocResRoot, nestedName, nestedID);
			}
			void Print()
			{
				PrintTree(allocResRoot);
			}
			void toBinary()
			{
				convertToBinary(allocResRoot);
			}
			void WriteDecoded(fstream& wrt)
			{
				writeDecodedToFile(allocResRoot, wrt);
			}
		};
		AllocationNode* left;
		AllocationNode* right;
		allocResTree allocResources;
		friend class AllocationTree;
	public:
		AllocationNode()
		{
			left = nullptr;
			right = nullptr;
		}
	};
	AllocationNode* allocationRoot;
	resourceTree ResourceTree;
	taskTree TaskTree;
	int strategy;
	AllocationNode* InsertNode(AllocationNode* root, AllocationNode* newNode)
	{
		if (!root)
		{
			root = newNode;
			return root;
		}

		queue<AllocationNode*> nodesQ;
		nodesQ.push(root);

		while (!nodesQ.empty())
		{
			AllocationNode* node = nodesQ.front();
			nodesQ.pop();

			if (node->left)
			{
				nodesQ.push(node->left);
			}
			else
			{
				node->left = newNode;
				return root;
			}

			if (node->right)
			{
				nodesQ.push(node->right);
			}
			else
			{
				node->right = newNode;
				return root;
			}
		}
	}
	void InsertInExisting(AllocationNode* node, string n, int ID, string nestedN, int nestedID, bool& inserted)
	{
		if (!node)
			return;

		InsertInExisting(node->left, n, ID, nestedN, nestedID, inserted);
		if (node->id == ID && node->name == n && !inserted)
		{
			node->allocResources.Insert(nestedN, nestedID);
			inserted = true;
		}
		InsertInExisting(node->right, n, ID, nestedN, nestedID, inserted);
	}
	void PrintTree(AllocationNode* node)
	{
		if (!node)
			return;

		PrintTree(node->left);
		cout << "(" << node->name << node->id;
		node->allocResources.Print();
		cout << ")\n";
		PrintTree(node->right);
	}
	void convertToBin(AllocationNode* node)
	{
		if (!node)
			return;

		convertToBin(node->left);
		node->allocResources.toBinary();
		convertToBin(node->right);
	}
	void writeDecoded(AllocationNode* node, fstream& wrt)
	{
		if (!node)
			return;

		writeDecoded(node->left, wrt);
		node->allocResources.WriteDecoded(wrt);
		writeDecoded(node->right, wrt);

	}
public:
	AllocationTree()
	{
		allocationRoot = nullptr;
		strategy = 2;
	}
	void InitializeResources()
	{
		fstream fin;
		fin.open("applicants.txt", ios::in);
		ResourceTree.ReadTree(fin);
		fin.close();
	}
	void InitializeTasks()
	{
		fstream fin;
		fin.open("tasks.txt", ios::in);
		TaskTree.ReadTree(fin);
		fin.close();
	}
	void insert(string n, int ID, string nestedN, int nestedID)
	{
		bool inserted = false;
		InsertInExisting(allocationRoot, n, ID, nestedN, nestedID, inserted);

		if (!inserted)
		{
			AllocationNode* newNode = new AllocationNode;
			newNode->id = ID;
			newNode->name = n;
			newNode->allocResources.Insert(nestedN, nestedID);
			allocationRoot = InsertNode(allocationRoot, newNode);
		}
	}
	void BuildAllocationTree()
	{
		TaskTree.Compare(&ResourceTree, this, strategy);
	}
	void Initialize()
	{
		InitializeResources();
		InitializeTasks();
	}
	void PrintTrees()
	{
		cout << "Resource Tree:\n";
		ResourceTree.Print();
		cout << "\n\nTask Tree:\n";
		TaskTree.Print();

	}
	void Print()
	{
		cout << "Allocation tree:\n";
		PrintTree(allocationRoot);
	}
	void toBinary()
	{
		convertToBin(allocationRoot);
	}
	void WriteDecoded()
	{
		fstream wrt;
		wrt.open("decoded.txt", ios::out);
		writeDecoded(allocationRoot, wrt);
		wrt.close();
	}
};

void resourceTree::CompareSkillTree(resourceNode* node, skillTree* skTree, AllocationTree* allocTree, string skillName, int idNumber, int strategy)	//traversing resource tree
{
	if (!node)
		return;

	CompareSkillTree(node->left, skTree, allocTree, skillName, idNumber, strategy);
	if (skTree->Compare(&node->skTree, strategy))					//left has skill tree of task tree. right has skill tree of resourceTree
	{
		allocTree->insert(skillName, idNumber, node->name, node->id);
	}
	CompareSkillTree(node->right, skTree, allocTree, skillName, idNumber, strategy);
}

struct HuffmanNode
{
	string data;
	unsigned frequency;
	HuffmanNode* left, * right;

	HuffmanNode(string d, unsigned frequency) : data(d), frequency(frequency), left(nullptr), right(nullptr) {}
};


struct Compare
{
	bool operator()(HuffmanNode* left, HuffmanNode* right)
	{
		return left->frequency > right->frequency;
	}
};


HuffmanNode* buildHuffmanTree(const string& binaryText) {
	priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> minHeap;

	unordered_map<string, unsigned> frequencyMap;
	for (int i = 0; i < binaryText.length() - 7; i += 8) {
		string binarySeq = binaryText.substr(i, 8);
		frequencyMap[binarySeq]++;
	}

	for (auto& pair : frequencyMap) {
		minHeap.push(new HuffmanNode(pair.first, pair.second));
	}

	while (minHeap.size() != 1) {
		HuffmanNode* left = minHeap.top();
		minHeap.pop();

		HuffmanNode* right = minHeap.top();
		minHeap.pop();

		HuffmanNode* newNode = new HuffmanNode("$", left->frequency + right->frequency);
		newNode->left = left;
		newNode->right = right;

		minHeap.push(newNode);
	}

	return minHeap.top();
}

void generateHuffmanCodes(HuffmanNode* root, const string& code, unordered_map<string, string>& huffmanCodes)
{
	if (root)
	{
		if (!root->left && !root->right)
		{
			huffmanCodes[root->data] = code;
		}

		generateHuffmanCodes(root->left, code + "0", huffmanCodes);
		generateHuffmanCodes(root->right, code + "1", huffmanCodes);
	}
}

void compressFile(const string& inputFile, const string& compressedFile) {
	ifstream inFile(inputFile, ios::binary);
	ofstream outFile(compressedFile, ios::binary);

	string text;
	string encodedText;
	char c;
	while (!inFile.eof())
	{
		inFile.get(c);
		if (c == '\n' && text.size() > 0)
		{
			if (text[text.size() - 1] == '\r')
				text.erase(text.size() - 1);
			HuffmanNode* root = buildHuffmanTree(text);
			unordered_map<string, string> huffmanCodes;
			generateHuffmanCodes(root, "", huffmanCodes);


			for (size_t i = 0; i < text.length() - 7; i += 8) {
				string bits = text.substr(i, 8);
				encodedText += huffmanCodes[bits];
			}
			outFile << encodedText << endl;
			text.clear();
			encodedText.clear();
		}
		else
		{
			text = text + c;
		}
	}

	inFile.close();
	outFile.close();
}

int main()
{
	AllocationTree allocTree;
	allocTree.Initialize();
	allocTree.PrintTrees();
	allocTree.BuildAllocationTree();
	cout << endl;
	allocTree.Print();
	cout << endl;
	allocTree.toBinary();
	allocTree.Print();
	allocTree.WriteDecoded();
	compressFile("decoded.txt", "encoded.txt");
	return 0;
}
