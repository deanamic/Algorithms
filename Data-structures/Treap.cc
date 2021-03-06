#include <bits/stdc++.h>
using namespace std;

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator(seed);
uniform_int_distribution<int> distribution(0,1000000);
string ans = "";

struct Node {
	Node *left, *right, *parent;
	string word;
	int order, priority, count;

	int chars;

	//constructor
	Node (int x, string s) : word(s), order(x), priority(distribution(generator)), count(1) {
		left = right = parent = nullptr;
		chars = s.size();
	}


};

//returns the cardinality of a Node

inline int numWords(Node* N) {
	return (N ? N->count : 0);
}

inline int numChars(Node* N) {
	return (N ? N->chars : 0);
}

//Makes P the parent of N
inline void SetParent (Node* N, Node* P) {
	if(N) N->parent = P;
}

//Updates a node which has a modified son
//it re-sets the cardinality and adjusts the parents

void Update(Node* N) {
	if (!N) return;
	N->count = 1 + numWords(N->left) + numWords(N->right);
	N->chars = N->word.size() + numChars(N->left) + numChars(N->right);
	SetParent(N->left, N);
	SetParent(N->right, N);
}

//make the higher priority node on top, Merge recursively
//N1 seems to go to the left by default
Node* Merge(Node* N1, Node* N2) {
	if (!N1) return N2;
	if (!N2) return N1;
	if (N1->priority >= N2->priority) {
		N1->right = Merge(N1->right, N2);
		Update(N1);
		return N1;
	} else {
		N2->left = Merge(N1, N2->left);
		Update(N2);
		return N2;
	}
}

// Splits the Tree into trees with order > x and order <= x
// first is x <= x, second > x
// You go Splitting  recursively and reattaching edges to the
// First node you encounter of the same type.
pair<Node*, Node*> Split_by_Order(Node* N, int x) {
	if (!N) return {nullptr, nullptr};
	if (N->order < x) {
		auto p = Split_by_Order(N->right, x);
		N->right = p.first;
		Update(N);
		SetParent(N, nullptr);
		return {N, p.second};
	} else {
		auto p = Split_by_Order(N->left, x);
		N->left = p.second;
		Update(N);
		SetParent(N, nullptr);
		return {p.first, N};
	}
}


//Same as above, it simply gets the first x elements.
pair<Node*, Node*> Split_by_Cardinality(Node* N, int x) {
	if (!N) return {nullptr, nullptr};
	if (numWords(N->left) + 1 <= x) {
		auto p = Split_by_Cardinality(N->right, x - numWords(N->left) - 1);
		N->right = p.first;
		Update(N);
		SetParent(N, nullptr);
		return {N, p.second};
	} else {
		auto p = Split_by_Cardinality(N->left, x);
		N->left = p.second;
		Update(N);
		SetParent(N, nullptr);
		return {p.first, N};
	}
}

//Inserts N2 into N1
//It divides N1 into 2 Treaps, and merges with the Treap of
//lesser order, then reattaches the other Treap
Node * Insert(Node* N1, Node* N2) {
	auto p = Split_by_Cardinality(N1, N2->order);
	N1 = Merge(p.first, N2);
	N1 = Merge(N1, p.second);
	return N1;
}

/*
//Returns the number of nodes such that order < x
int Count(Node* N, int x) {
	if(!N) return 0;
	if(N->order <= x) return 1 + Cardinality(N->left) + Count(N->right, x);
	return Count(N->left,x);
}


//Updates a Node
//Strips the 2 sons of N and merges them into one treap
//saves the parent of N and completely resets N(except for priority)
//Reattaches the splitted sons into the parent, updates until we reach root
//if N was the root we make the merged sons the Root
//Now we insert the updated N
Node* UpdateNode(Node* &R, Node* N, int nx) {
	SetParent(N->left, nullptr);
	SetParent(N->right, nullptr);
	auto m = Merge(N->left, N->right);
	auto p = N->parent;

	N->left = N->right = N->parent = nullptr;
	N->order = nx;
	Update(N);
	if(p) {
		p->left == N ? p->left = m : p->right = m;
		if(m) m->parent = p;
		while(p) {
			Update(p);
			p = p->parent;
		}
	} else {
		R = m;
	}
	return Insert(R,N);
}

//Deletes a node
//Same as above but we don't reinsert
Node* Delete(Node* R, Node* N) {
	SetParent(N->left, nullptr);
	SetParent(N->right, nullptr);
	auto m = Merge(N->left, N->right);
	auto p = N->parent;

	if(p) {
		p->left == N ? p->left = m : p->right = m;
		if(m) m->parent = p;
		while(p) {
			Update(p);
			p = p->parent;
		}
	} else {
		R = m;
	}
}
*/

char ith(Node* N, int x) {
	if (numChars(N->left) - 1 >= x) {
		return ith(N->left, x);
	}
	if((int)(numChars(N->left) - 1 + N->word.size()) >= x) {
		return N->word[x - numChars(N->left)];
	}
	return ith(N->right, x - numChars(N->left) - N->word.size());
}


int main() {
	Node* Root = nullptr;
	char c;
	while(cin >> c) {
		if(c == 'I') {
			string s;
			int p;
			cin >> s >> p;
			Node* n = new Node(p, s);
			Root = Insert(Root, n);
		}
		if(c == 'C') {
			int x;
			cin >> x;
			cout << ith(Root, x);
		}
		if(c == 'E') return cout << endl, 0;
		//
	}
}
