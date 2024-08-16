#include <iostream>
#include <assert.h>
using namespace std;

template<class T>
struct AVLTreeNode
{
	AVLTreeNode(const T& data = T())
		: _pLeft(nullptr)
		, _pRight(nullptr)
		, _pParent(nullptr)
		, _data(data)
		, _bf(0)
	{}

	AVLTreeNode<T>* _pLeft;
	AVLTreeNode<T>* _pRight;
	AVLTreeNode<T>* _pParent;
	T _data;
	int _bf;   // 节点的平衡因子
};


 //AVL: 二叉搜索树 + 平衡因子的限制
template<class T>
class AVLTree
{
	typedef AVLTreeNode<T> Node;
public:
	AVLTree()
		: _pRoot(nullptr)
	{}

	 //在AVL树中插入值为data的节点
	bool Insert(const T& data)
	{
		if (_pRoot == nullptr)
		{
			_pRoot = new Node(data);
			return true;
		}
		//_pRoot不为空
		Node* cur = _pRoot, *parent = nullptr;
		while (cur)
		{
			if (cur->_data == data)//重复data
			{
				return false;
			}
			//data不重复
			if (data > cur->_data)//cur右走
			{
				parent = cur;
				cur = cur->_pRight;
			}
			else//cur往左走
			{
				parent = cur;
				cur = cur->_pLeft;
			}
		}
		cur = new Node(data);
		if (data < parent->_data)//cur 在 parent 左
		{
			parent->_pLeft = cur;
			cur->_pParent = parent;
		}
		else//cur在 右
		{
			parent->_pRight = cur;
			cur->_pParent = parent;
		}
		//更新整个树的_bf
		while (parent)//
		{
			//更新parent_bf
			//若cur不是新增节点, cur->-bf在上次更新中一定从0到-1/1，即cur高度加1, 所以更新parent
			if (cur == parent->_pLeft)
			{
				parent->_bf--;
			}
			else
			{
				parent->_bf++;
			}
			//停止更新
			if (parent->_bf == 0)//更新后的parent->_bf  == 0， 原来为1， -1 , 更新前后高度不变， 停止向上更新
			{
				break;
			}
			//向上更新
			else if (parent->_bf == 1 || parent->_bf == -1)//parent->_bf 原来为0, 更新后高度增加， 需要向上更新
			{
				cur = parent;
				parent = cur->_pParent;
			}
			//旋转
			else if (parent->_bf == 2 || parent->_bf == -2)//cur肯定不是新增 是更新上来的, 且cur->_bf==1，parent->_bf已经失去平衡， 需要旋转
			{//parent 失衡一定是cur导致的， 即cur一定在parent偏高的那一颗树上
				if (parent->_bf == 2)//parent 右高
				{
					if (cur->_bf == 1)//cur parent 同号， 单旋转即可
					{
						RotateL(parent);//左旋
					}
					else if (cur->_bf == -1) //cur parent异号， 需要双旋
					{
						RotateRL(parent);//先右后左旋
					}
					else
					{//cur->_bf 不为+-1， 出现错误
						assert(false);
					}
				}
				else//parent左高
				{
					if (cur->_bf == -1)//cur parent 同号， 单旋转即可
					{
						RotateR(parent);
					}
					else if (cur->_bf == 1) //cur parent异号， 需要双旋
					{
						RotateLR(parent);//先右后左旋
					}
					else
					{//cur->_bf 不为+-1， 出现错误
						assert(false);
					}
				}
				break;//旋转后抵消了cur造成的高度的增加， 即新增cur前和新增cur但旋转parent后， 以parent为根的树高度不变， 所以停止向上更新
			}
			else//parent->_bf 高度差大于2， 出现逻辑错误
			{
				assert(false);
			}
		}
	}

	// AVL树的验证
	bool IsAVLTree()
	{
		return _IsAVLTree(_pRoot);
	}

	// 根据AVL树的概念验证pRoot是否为有效的AVL树
	bool _IsAVLTree(Node* pRoot)
	{
		return true;
	}
	size_t _Height(Node* pRoot)
	{
		return 0;
	}
	// 右单旋
	void RotateR(Node* parent)
	{
		Node* pparent = parent->_pParent;
		Node* subL = parent->_pLeft;
		Node* subLR = subL->_pRight;
		parent->_pLeft = subLR; if (subLR) { subLR->_pParent = parent; }
		parent->_pParent = subL; subL->_pRight = parent;
		subL->_pParent = pparent;
		if (pparent != nullptr)
		{
			pparent->_pLeft == parent ? pparent->_pLeft = subL : pparent->_pRight = subL;
		}
		else//parent为根节点时， 不做pparent&subL处理， 更新_pRoot
		{
			_pRoot = subL;
		}
		//更新平衡因子
		parent->_bf = subL->_bf = 0;
	}
	// 左单旋
	void RotateL(Node* parent)
	{
		Node* pparent = parent->_pParent;
		Node* subR = parent->_pRight;
		Node* subRL = subR->_pLeft;
		parent->_pRight = subRL; if (subRL) { subRL->_pParent = parent;}
		parent->_pParent = subR; subR->_pLeft = parent;
		subR->_pParent = pparent;
		if (pparent != nullptr)
		{
			pparent->_pLeft == parent ? pparent->_pLeft = subR : pparent->_pRight = subR;
		}
		else//parent为根节点时， 不做pparent&subL处理， 更新_pRoot
		{
			_pRoot = subR;
		}
		//更新平衡因子
		parent->_bf = subR->_bf = 0;
	}
	// 右左双旋
	void RotateRL(Node* parent)
	{
		Node* subR = parent->_pRight;
		Node* subRL = subR->_pLeft;
		int bf = subRL->_bf;
		RotateR(parent->_pRight);
		RotateRL(parent);
		//更新平衡因子
		if (bf == 0)
		{
			subRL->_bf = parent->_bf = subR->_bf = 0;
		}
		else if (bf == 1)
		{
			subRL->_bf = 0;
			parent->_bf = -1;
			subR->_bf = 0;
		}
		else if (bf == -1)
		{
			subRL->_bf = 0;
			parent->_bf = 0;
			subR->_bf = 1;
		}
		else assert(false);
	}
	// 左右双旋
	void RotateLR(Node* parent)
	{
		Node* subL = parent->_pLeft;
		Node* subLR = subL->_pRight;
		int bf = subLR->_bf; //至于为什么看subLR的_bf， 可看抽象图理解
		RotateL(parent->_pLeft);
		RotateR(parent);
		//更新平衡因子
		if (bf == 0)
		{
			parent->_bf = subL->_bf = subLR->_bf = 0;
		}
		else if (bf == 1)
		{
			parent->_bf = 0;
			subL->_bf = -1;
			subLR->_bf = 0;
		}
		else if (bf == -1)
		{
			parent->_bf = 1;
			subL->_bf = 0;
			subLR->_bf = 0;
		}
		else
		{
			assert(false);
		}
	}
	void Inorder(Node* root)
	{
		if (!root) return;
		Inorder(root->_pLeft);
		cout << root->_data << " ";
		Inorder(root->_pRight);
	}
	void _Inorder()
	{
		Inorder(_pRoot);
		cout << endl;
	}
private:
	Node* _pRoot;
};


void TestAVL()
{
	cout << "hello vs2022" << endl;
	AVLTree<int> avl;
	avl.Insert(5);
	avl.Insert(2);
	avl.Insert(6);
	avl.Insert(4);
	avl.Insert(11);
	avl._Inorder();
}

int main()
{
	TestAVL();
	return 0;
}