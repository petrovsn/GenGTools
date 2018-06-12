#include <Node.h>


Node::Node()
{
	this->ID = -1;
	this->str = "";
	this->End = false;
	this->NaNbp = false;
}

Node::Node(int id)
{
	this->ID = id;
	this->str = "";
	this->End = false;
	this->NaNbp = false;
}

Node::Node(string str, int id)
{
	this->str = str;
	this->ID = id;
	this->End = false;
	if (str[0] == 'n') this->NaNbp = true;
	else this->NaNbp = false;
}

const Node Node::operator = (Node& n)
{
	this->ID = n.ID;
	this->str = n.str;

	this->B = n.B;
	this->Next = n.Next;
	this->Prev = n.Prev;

	return *this;
}

Node Node::Split(int pos, int id)
{
	string tmp = this->str.substr(pos, str.length() - pos);
	this->str = this->str.substr(0, pos);

	Node n_pr(tmp, id);

	(n_pr.Prev).insert(pair<int, Node*>(this->ID, this));



	for (auto t1 : this->Next)
	{
		t1.second->Prev.erase(this->ID);
		t1.second->Prev.insert(pair<int, Node*>(id, &n_pr));
		n_pr.Next.insert(t1);
	}



	Next.clear();
	Next.insert(pair<int, Node*>(id, &n_pr));

	return n_pr;
}

void Node::Split(int pos, Node* n_pr)
{
	string tmp = this->str.substr(pos, str.length() - pos);
	this->str = this->str.substr(0, pos);

	n_pr->str = tmp;
	n_pr->NaNbp = this->NaNbp;

	(n_pr->Prev).insert(pair<int, Node*>(this->ID, this));

	for (auto t1 : this->Next)
	{
		t1.second->Prev.erase(this->ID);
		t1.second->Prev.insert(pair<int, Node*>(n_pr->ID, n_pr));
		n_pr->Next.insert(t1);
	}

	Next.clear();
	Next.insert(pair<int, Node*>(n_pr->ID, n_pr));



	for (auto t2 : this->B)
	{
		n_pr->B.insert(t2);
		B[t2.first] = n_pr->ID;
	}

}

void Node::Link(Node *N2)
{
	(this->Next).insert(pair<int, Node*>(N2->ID, N2));
	(N2->Prev).insert(pair<int, Node*>(this->ID, this));
}