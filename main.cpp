#include<iostream>
using namespace std;

bool visited = true;
template<typename T>
class Row
{
public:
    int index;
    T data[100];
    bool operator < (const Row<T>& other) 
    {
        return index < other.index;
    }
    bool operator > (const Row<T>& other) 
    {
        return index > other.index;
    }

    bool operator == (const Row<T>& other) 
    {
        return index == other.index;
    }
};
template<typename T>
class BTreeNode
{
public:
	Row<T> *keys; // An array of keys
	int t;	 // Minimum degree (defines the range for number of keys)
	BTreeNode **C; // An array of child pointers
	int n;	 // Current number of keys
	bool leaf; // Is true when node is leaf. Otherwise false
    BTreeNode(int t1, bool leaf1) 	// Copy the given minimum degree and leaf property
    {
        t = t1;
        leaf = leaf1;
        keys = new Row<T>[2*t-1];
        C = new BTreeNode *[2*t];
        n = 0;
    }
    void traverse(int NUM)
    {
        int i;
        for (i = 0; i < n; i++)
        {
            if (leaf == false)
                C[i]->traverse(NUM);
            for(int j=0 ; j<NUM ; j++)
            {
                cout << keys[i].data[j] << "|" ;
            }
            cout << keys[i].index << endl;
        }
        if (leaf == false)
            C[i]->traverse(NUM);
    }
    BTreeNode *search(int k)
    {
        int i = 0;
        while (i < n && k > keys[i].index)
            i += 1;
        if (keys[i].index == k)
        {
            BTreeNode<T> *s = new BTreeNode<T>(3, false);
            s->keys[0] = keys[i] ;
            return s;
        }
        if (leaf == true)
        {
            return NULL;
        }
        return C[i]->search(k);
    }
    int findKey(int k)
    {
        int idx=0;
        while (idx<n && keys[idx].index < k)
            ++idx;
        return idx;
    }
    void insertNonFull(Row<T> k , int NUM)
    {
        int i = n-1;
        if (leaf == true)
        {
            while (i >= 0 && keys[i].index > k.index)
            {
                keys[i+1].index = keys[i].index;
                for(int j=0 ; j<NUM ; j++)
                {
                    keys[i+1].data[j] = keys[i].data[j];       
                }
                i--;
            }
            keys[i+1].index = k.index;
            for(int j=0 ; j<NUM ; j++)
            {
                keys[i+1].data[j] = k.data[j];
            }
            n = n+1;
        }
        else // If this node is not leaf
        {
            while (i >= 0 && keys[i].index > k.index)
                i--;
            if (C[i+1]->n == 2*t-1)
            {
                splitChild(i+1, C[i+1] , NUM);
                if (keys[i+1].index < k.index)
                    i++;
            }
            C[i+1]->insertNonFull(k , NUM);
        }
    }
    void splitChild(int i, BTreeNode *y , int NUM)
    {
        BTreeNode *z = new BTreeNode(y->t, y->leaf);
        z->n = t - 1;
        for (int j = 0; j < t-1; j++)
        {
            z->keys[j].index = y->keys[j+t].index;
            for(int j=0 ; j<NUM ; j++)
            {
                z->keys[j].data[j] = y->keys[j+t].data[j];       
            }
        }
            
        if (y->leaf == false)
        {
            for (int j = 0; j < t; j++)
                z->C[j] = y->C[j+t];
        }
        y->n = t - 1;
        for (int j = n; j >= i+1; j--)
            C[j+1] = C[j];
        C[i+1] = z;
        for (int j = n-1; j >= i; j--)
        {
            keys[j+1].index = keys[j].index;
            for(int j=0 ; j<NUM ; j++)
            {
               keys[j+1].data[j] = keys[j].data[j];      
            }
        }
           
        keys[i].index = y->keys[t-1].index;
        for(int j=0 ; j<NUM ; j++)
        {
            keys[i].data[j] = y->keys[t-1].data[j];
        }
        n = n + 1;
    }
    void remove(int k , int NUM)
    {
        int idx = findKey(k);
        if (idx < n && keys[idx].index == k)
        {
            if (leaf)
                removeFromLeaf(idx , NUM);
            else
                removeFromNonLeaf(idx , NUM);
        }
        else
        {
            if (leaf)
            {
                visited = false ;
                return;
            }
            bool flag = ( (idx==n)? true : false );
            if (C[idx]->n < t)
                fill(idx , NUM);
            if (flag && idx > n)
                C[idx-1]->remove(k , NUM);
            else
                C[idx]->remove(k , NUM);
        }
        return;
    }
    void removeFromLeaf(int idx , int NUM)
    {
        for (int i=idx+1; i<n; ++i)
        {
            keys[i-1].index = keys[i].index;
            for(int j=0 ; j<NUM ; j++)
            {
                keys[i-1].data[j] = keys[i].data[j];
            }
        }   
        n--;
        return;
    }
    void removeFromNonLeaf(int idx , int NUM)
    {
        Row<T> k ;
        k.index = keys[idx].index;
        for(int j=0 ; j<NUM ; j++)
        {
            k.data[j] = keys[idx].data[j];
        }
        if (C[idx]->n >= t)
        {
            Row<T> pred = getPred(idx);
            keys[idx].index = pred.index;
            for(int j=0 ; j<NUM ; j++)
            {
                keys[idx].data[j] = pred.data[j];
            }

            C[idx]->remove(pred.index , NUM);
        }
        else if (C[idx+1]->n >= t)
        {
            Row<T> succ = getSucc(idx);
            keys[idx].index  = succ.index;
            for(int j=0 ; j<NUM ; j++)
            {
                keys[idx].data[j] = succ.data[j];
            }
            C[idx+1]->remove(succ.index , NUM);
        }
        else
        {
            merge(idx , NUM);
            C[idx]->remove(k.index , NUM);
        }
        return;
    }
    Row<T> getPred(int idx)
    {
        BTreeNode *cur=C[idx];
        while (!cur->leaf)
            cur = cur->C[cur->n];
        return cur->keys[cur->n-1];
    }
    Row<T> getSucc(int idx)
    {
        BTreeNode *cur = C[idx+1];
        while (!cur->leaf)
            cur = cur->C[0];
        return cur->keys[0];
    }
    void fill(int idx , int NUM)
    {
        if (idx!=0 && C[idx-1]->n>=t)
            borrowFromPrev(idx , NUM);

        else if (idx!=n && C[idx+1]->n>=t)
            borrowFromNext(idx , NUM);

        else
        {
            if (idx != n)
                merge(idx , NUM);
            else
                merge(idx-1 , NUM);
        }
        return;
    }
    void borrowFromPrev(int idx , int NUM)
    {

        BTreeNode *child=C[idx];
        BTreeNode *sibling=C[idx-1];
        for (int i=child->n-1; i>=0; --i)
        {
            child->keys[i+1].index = child->keys[i].index;
            for(int j=0 ; j<NUM ; j++)
            {
                child->keys[i+1].data[j] = child->keys[i].data[j];
            }
        }
            
        if (!child->leaf)
        {
            for(int i=child->n; i>=0; --i)
                child->C[i+1] = child->C[i];
        }
        child->keys[0].index = keys[idx-1].index;
        for(int j=0 ; j<NUM ; j++)
        {
            child->keys[0].data[j] = keys[idx-1].data[j];
        }
        if(!child->leaf)
            child->C[0] = sibling->C[sibling->n];
        keys[idx-1].index = sibling->keys[sibling->n-1].index;
        for(int j=0 ; j<NUM ; j++)
        {
            keys[idx-1].data[j] = sibling->keys[sibling->n-1].data[j];
        }

        child->n += 1;
        sibling->n -= 1;

        return;
    }
    void borrowFromNext(int idx , int NUM)
    {

        BTreeNode *child=C[idx];
        BTreeNode *sibling=C[idx+1];
        child->keys[(child->n)].index = keys[idx].index;
        for(int j=0 ; j<NUM ; j++)
        {
            child->keys[(child->n)].data[j] = keys[idx].data[j];
        }
        if (!(child->leaf))
            child->C[(child->n)+1] = sibling->C[0];
        keys[idx].index = sibling->keys[0].index;
        for(int j=0 ; j<NUM ; j++)
        {
            keys[idx].data[j] = sibling->keys[0].data[j];
        }

        for (int i=1; i<sibling->n; ++i)
            sibling->keys[i-1] = sibling->keys[i];

        if (!sibling->leaf)
        {
            for(int i=1; i<=sibling->n; ++i)
                sibling->C[i-1] = sibling->C[i];
        }
        child->n += 1;
        sibling->n -= 1;

        return;
    }
    void merge(int idx , int NUM)
    {
        BTreeNode *child = C[idx];
        BTreeNode *sibling = C[idx+1];

        child->keys[t-1].index = keys[idx].index;
        for(int j=0 ; j<NUM ; j++)
        {
            child->keys[t-1].data[j] = keys[idx].data[j];
        }
        for (int i=0; i<sibling->n; ++i)
        {
            child->keys[i+t].index = sibling->keys[i].index;
            for(int j=0 ; j<NUM ; j++)
            {
                child->keys[i+t].data[j] = sibling->keys[i].data[j];
            }
        }

        if (!child->leaf)
        {
            for(int i=0; i<=sibling->n; ++i)
                child->C[i+t] = sibling->C[i];
        }
        for (int i=idx+1; i<n; ++i)
        {
            keys[i-1].index = keys[i].index;
            for(int j=0 ; j<NUM ; j++)
            {
                keys[i-1].data[j] = keys[i].data[j];
            }

        }
        for (int i=idx+2; i<=n; ++i)
            C[i-1] = C[i];

        child->n += sibling->n+1;
        n--;
        delete(sibling);
        return;
    }
};
//********************************************************************************************************
template<typename T>
class BTree
{
public:
	BTreeNode<T> *root; // Pointer to root node
	int t = 3; // Minimum degree
	BTree() 
	{
		root = NULL;
	}
	void traverse(int NUM)
	{
		if (root != NULL) 
            root->traverse(NUM);
	}
	BTreeNode<T>* search(int k)	// function to search a key in this tree
	{
		return (root == NULL)? NULL : root->search(k);
	}
    void insert(Row<T> k , int NUM)
    {
        if (root == NULL)
        {
            root = new BTreeNode<T>(t, true);
            root->keys[0].index = k.index ; // Insert key
            for(int j=0 ; j<NUM ; j++)
            {
                root->keys[0].data[j] = k.data[j] ;
            }
            root->n = 1; // Update number of keys in root
        }
        else // If tree is not empty
        {
            if (root->n == 2*t-1)
            {
                BTreeNode<T> *s = new BTreeNode<T>(t, false);
                s->C[0] = root;
                s->splitChild(0, root , NUM);
                int i = 0;
                if (s->keys[0].index < k.index)
                    i++;
                s->C[i]->insertNonFull(k , NUM);
                root = s;
            }
            else // If root is not full, call insertNonFull for root
                root->insertNonFull(k , NUM);
        }
    }
    void remove(int k , int NUM)
    {
        if (!root)
        {
            cout << "The table is empty\n";
            visited = false;
            return;
        }
        root->remove(k , NUM);

        if (root->n==0)
        {
            BTreeNode<T> *tmp = root;
            if (root->leaf)
                root = NULL;
            else
                root = root->C[0];
            delete tmp;
        }
        return;
    }

};
//*********************************************************************************************
template<typename T>
class tables
{
public:
    BTree<T> trees; //Each table has a tree
    int type_index; //Index type: unique and non-unique and increment
    int NUM ; //The number of data in each line
    string name ; //name of table
    string type_table; // type of data in the table
    int incremental = 1; // Incremental index
};
void menu()
{
    cout << "Enter the desired option :" << endl ;
    cout << "1:Add a new index to the table " << endl ;
    cout << "2:Remove the desired index from the table" << endl;
    cout << "3:Information search by index " << endl ;
    cout << "4:Print the table " << endl ;
    cout << "5:Exite" << endl ;
}
int main()
{
	tables<string> t[50];
    Row<string> x;
    cout << "1:Create a new table" << endl ;
    cout << "2:Exite" << endl ;
    int main_order ;
    cin >> main_order ;
    string name ;
    string type;
    int i_th = 0;
    while (main_order != 2)
    {
        cout << "Enter the name of the table and its data type" << endl ;
        cin >> name >> type ;
        cout << "table " << name << " was created" << endl ;
        i_th ++; 
        t[i_th-1].name = name;
        t[i_th-1].type_table = type ;
        cout << "Enter the number of data in each row" << endl;
        cin >> t[i_th-1].NUM;
        cout << "What are table indexes based on?" << endl ;
        cout << "1:incremental(main index)" << endl ;
        cout << "2:Unique indexes" << endl;
        cout << "3:Non-unique indexes" << endl;
        cin >> t[i_th-1].type_index ;
        menu();
        int order ;
        cin >> order ;
        while(order != 5)
        {
            cout << "In which table should the operation be done? " << endl ;
            int number_table ;
            cin >> name ;
            int wrong ;
            for(int i=0 ; i<50 ; i++)
            {
                if (t[i].name == name)
                {
                    number_table = i ;
                    break;
                }
                wrong = i ;
            }
            if(wrong == 49)
            {
                cout << "This table is not available" << endl << "****************" << endl;
                wrong = 0 ;
            }
            else if(order == 1)
            {
                cout << "Please enter the data" << endl ;
                for(int i=0 ; i<t[number_table].NUM ; i++)
                {
                    cin >> x.data[i];
                }
                if (t[number_table].type_index == 1)
                {
                    x.index = t[number_table].incremental;
                    t[number_table].trees.insert(x ,t[number_table].NUM);
                    t[number_table].incremental++;
                }
                else if (t[number_table].type_index == 2)
                {
                    cout << "Please enter the index" << endl ;
                    int myindex;
                    cin >> myindex;
                    BTreeNode<string> * T = t[number_table].trees.search(myindex);
                    if(T == nullptr)
                    {
                        x.index = myindex;
                        t[number_table].trees.insert(x , t[number_table].NUM);
                    }
                }
                else if(t[number_table].type_index == 3)
                {
                    cout << "Please enter the index" << endl ;
                    int myindex;
                    cin >> myindex;
                    x.index = myindex;
                    t[number_table].trees.insert(x , t[number_table].NUM);
                }   
            }
            else if(order == 2)
            { 
                cout << "Please enter your desired index" << endl ;
                int rem_index ;
                cin >> rem_index;
                while (visited == true)
                {
                    t[number_table].trees.remove(rem_index, t[number_table].NUM); 
                }
                visited = true ; 
            }
            else if(order == 3)
            {
                cout << "Please enter your desired index" << endl ;
                int ser_index ;
                cin >> ser_index;
                BTreeNode<string> * T = t[number_table].trees.search(ser_index);
                if(T == nullptr)
                {
                    cout << "The desired index is not available in the table" << endl ;
                }
                else
                {
                    for(int i=0 ; i<t[number_table].NUM ; i++)
                    {
                        cout <<  T->keys[0].data[i] << "|" ;
                    }
                    cout << endl ;
                }
            }
            else if(order == 4)
            {
                cout << "name of table : " << t[number_table].name << "|" << "type of data in table : " << t[number_table].type_table << endl;
                t[number_table].trees.traverse(t[number_table].NUM);
            }
            menu();
            cin >> order ;
        }
        cout << "1:Create a new table" << endl ;
        cout << "2:Exite" << endl ;
        cin >> main_order ;
    }
	return 0;
}