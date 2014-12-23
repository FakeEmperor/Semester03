using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
/***
 * @file Tree.cs
 * @author  FakeEmperor <fake_empire@mail.ru>
 * @version 1.0
 * @link http://github.com/fakeemperor/Semester-03/
 *
 * @section DESCRIPTION
 *
 * The TreeNode storaging class is defined and implemented here.
 * It's an import from C++ class TreeNode
 * @see http://github.com/fakeemperor/TMP-LAB04/
 *
*/

namespace Utils
{

    /**
    *	Enumeration that is used to define walk method for some functions in <I>TreeNode</I> class.
    **/
    public enum WalkMode
    {
        /**
        *	Prefix order of walking the tree
        *	(this node, left subtrees, right subtrees)
        */
        WALK_PRE_ORDER,
        /**
        *	Postfix order of walking the tree
        *	(left subtrees, right subtrees, this node)
        */
        WALK_POST_ORDER,
        /**
        *	Symmetrical order of walking the tree
        *	(left subtrees, this node, right subtrees)
        */
        WALK_SYM_ORDER
    }

    public delegate string ToStringDelegate<T>(T value);
    /***
    *	@brief Graph type storaging class.
    *	No specific access model is present
    */
    public class TreeNode<T>
    {
        #region Private variables

        /**
        *	Value on that node
        */
        T _value;
        /**
        *	Pasrent node of this node (can be <B>nullptr</B> if this node is a root node)
        */
        TreeNode<T> parent;
        /**
        *	Child nodes of this node (can be <B>nullptr</B> if this node has no child nodes)
        */
        List<TreeNode<T>> children;

        #endregion
        #region Protected and Private methods

        /**
        *	Represents node's value as string.
        *	If there is preconfigured function for representation its value, this function will be called.
        *	Otherwise, a standard output will present.
        *	@return String representation of the value.
        */
        protected string valToStr()
        {
            if (TreeNode<T>.ToStringFunction != null)
                return TreeNode<T>.ToStringFunction(this._value);
            StringBuilder ss = new StringBuilder();
            ss.AppendFormat("::object[{0} bytes]:{1}", System.Runtime.InteropServices.Marshal.SizeOf(this._value), this._value.ToString());
            return ss.ToString();
        }
        /**
        *	Prefix-order walk of the tree.
        *	@param[in,out] index Index of a node function needs to return. Also, in case of <I>out of range</I> error
        *						 this index will indicate the difference between maximum index in that tree and the defined one.
        *	@param[in,out] str String, that will hold all the elements visited in prefix order.
        *	@return Node by the specified index or <B>nullptr</B>, if out of range.
        **/
        TreeNode<T> goWalk_pre(ref int index, string str = null)
        {
            TreeNode<T> res = null;
            if (str != null)
                str += valToStr() + ";";
            if (index == 0)
                return this;
            else if (this.children.Count > 0)
            {
                int i, s;
                s = index > children.Count ? children.Count : index;
                for (i = 0; i < s && index != 0; ++i)
                {
                    --index;
                    res = this.children[i].goWalk_pre(ref index, str);
                    if (res != null)
                        break;
                }
            }
            return res;
        }
        /**
        *	Postfix-order walk of the tree.
        *	@param[in,out] index Index of a node function needs to return. Also, in case of <I>out of range</I> error
        *						 this index will indicate the difference between maximum index in that tree and the defined one.
        *	@param[in,out] str String, that will hold all the elements visited in postfix order.
        *	@return Node by the specified index or <B>nullptr</B>, if out of range.
        */
        TreeNode<T> goWalk_post(ref int index, string str = null)
        {
            TreeNode<T> res = null;
            if (this.children != null && index != 0)
            {
                int i, s;
                s = index > children.Count ? children.Count : index;
                for (i = 0; i < s && index != 0; ++i)
                {
                    --index;
                    res = this.children[i].goWalk_post(ref index, str);
                    if (res != null)
                        break;
                }
            }
            if (str != null)
                str += valToStr() + ";";
            if (index == 0)
                res = this;
            return res;
        }
        /**
        *	Symmetrical-order walk of the tree.
        *	@param[in,out] index Index of a node function needs to return. Also, in case of <I>out of range</I> error
        *						 this index will indicate the difference between maximum index in that tree and the defined one.
        *	@param[in,out] str String, that will hold all the elements visited in symmetrical order.
        *	@return Node by the specified index or <B>nullptr</B>, if out of range.
        */
        TreeNode<T> goWalk_sym(ref int index, string str = null)
        {
            TreeNode<T> res = null;
            int i, s;
            s = children.Count;
            for (i = 0; i < s / 2 + s % 2 && index > 0; ++i)
            {
                --index;
                res = this.children[(int)i].goWalk_sym(ref index, str);
                if (res != null)
                    break;
            }
            if (str != null)
                str += valToStr() + ";";
            if (index == 0)
                res = this;

            for (i = s / 2 + s % 2; i < s && index != 0; ++i)
            {
                --index;
                res = this.children[(int)i].goWalk_sym(ref index, str);
                if (res != null)
                    break;
            }
            return res;
        }
        
        #endregion
        #region Public properties and Indexation operators
        /**
        *	A function which makes a string representation of holded value.
        *	If this function is a <B>nullptr</B>,
        *	default substitution string will be passed to the <B>TreeNode::toString()</B> method instead.
        */
        public static ToStringDelegate<T> ToStringFunction;
        /**
        *	@brief Counts number of direct children in this TreeNode.
        *	@return Number of storaged elements.
        */
        public int Length
        {
            get
            {
                return children.Count;
            }
        }
        /**
        *	@brief Counts number of <B>ALL</B> elements in the tree.
        *	@return Number of storaged elements.
        */
        public int TreeLength
        {
            get
            {
                int i, len = 1;
                for (i = 0; i < children.Count; i++)
                    len += this.children[i].TreeLength;
                return len;
            }
        }
        public TreeNode<T> this[int index]
        {
            get
            {
                int ind = index;
                TreeNode<T> res = goWalk_pre(ref index);
                if (res == null)
                    throw new ArgumentException("index", "Index is probably out of range. Walking Tree method had returned null value.");
                return res;
            }

        }
        public T Value
        {
            get { return this._value; }
        }
        public TreeNode<T> Parent
        {
            get { return this.parent; }
            set
            {
                if (value != this.parent)
                {
                    if (this.parent != null)
                        this.parent.RemoveChild(this.parent.IndexOf(this));
                    value.AddChild(this);
                }
            }
        }
        #endregion
        #region Cast operators

        public static implicit operator T(TreeNode<T> treenode)
        {
            return treenode._value;
        }

        #endregion
        #region Constructors and Destructors

        public TreeNode()
        {
            this._value = default(T);
            this.parent = null;
            this.children = new List<TreeNode<T>>();
        }

        public TreeNode(T val, TreeNode<T> parent = null, TreeNode<T>[] children = null, int sz = 0)
            : this()
        {
            this._value = val;
            if (parent != null)
                parent.AddChild(this);
            if (children != null)
                this.AddChildren(children);
        }

        #endregion
        #region Public methods

        public int AddChild(TreeNode<T> nd)
        {
            this.children.Add(nd);
            if (nd.parent != null)
                nd.parent.RemoveChild(nd.parent.IndexOf(nd));
            nd.parent = this;
            return children.Count;
        }
        public int AddChild(T v)
        {
            TreeNode<T> tree = new TreeNode<T>(v);
            tree.parent = this;
            this.children.Add(tree);
            return children.Count;
        }

        public int AddChildren(TreeNode<T>[] nds)
        {
            int num = nds.Length;
            this.children.AddRange(nds);
            for (int i = 0; i < num; i++)
            {
                var child = this.children[(int)(children.Count - num + i)];
                if(child.parent!=null)
                    child.parent.RemoveChild(child.parent.IndexOf(child));
                child.parent = this;
            }
                
            return children.Count;
        }

        public TreeNode<T> GetChild(int idx)
        {
            if (idx >= children.Count)
                return null;
            return this.children[(int)idx];
        }
        
        public TreeNode<T> RemoveChild(int idx)
        {
            TreeNode<T> res;
            if (idx > children.Count)
                return null;
            res = this.children[idx];
            res.parent = null;
            this.children.RemoveAt(idx);
            return res;
        }

        public void Clear()
        {
            children.Clear();
            if (this.parent != null)
                this.parent.RemoveChild(this.parent.IndexOf(this));
        }
        ~TreeNode()
        {
            this.Clear();
        }
        /**
        *	@brief Searches an index of an element by its node.
        *	@param[in] tn Pointer to that node.
        *	@return Index of an element, or <B>-1</B> if not found
        */
        public int IndexOf(T value, bool search_all = false)
        {
            return IndexOf(new TreeNode<T>(value), search_all);
        }
        private int _indexOf(TreeNode<T> tn, bool search_all, ref int offset)
        {
            int index = -1, i,s;
            
            if (tn != null)
            {
                //need to CHANGE (ineffective way)
                if (search_all)
                {
                    s = this.TreeLength;
                    for (i = 1; i < s && index == -1; ++i)
                        if (this[i]._value.Equals(tn._value))
                            index = i;

                }
                else
                {
                    s = children.Count;
                    for (i = 0; i < s && index == -1; ++i)
                        if (this.children[i]._value.Equals(tn._value))
                            index = i;
                }
            }
            return index;
        }
        public int IndexOf(TreeNode<T> tn, bool search_all = false)
        {
            int offset = 0;
            if (this._value.Equals(tn._value))
                return 0;
            int index = _indexOf(tn, search_all, ref offset);
            if (index >= 0)
                return index + 1 + offset;
            else
                return -1;
        }   
        /**
        *	@brief Represents the current storaging node as string value.
        *	@param[in] level Level of the current node for representation.
        *	@return Returns built std::string.
        */
        public override string ToString()
        {
            return this.ToString(0);
        }
        
        public string ToString(int level)
        {
            StringBuilder ss = new StringBuilder();
            int i, lvl = level, s = this.children.Count;
            for (i = 0; i < lvl; i++)
                ss.Append(" ");
            if (lvl > 0)
                ss.Append("|-");

            ss.AppendFormat("<{0}>", this.valToStr());
            ss.AppendLine();
            ++lvl;
            for (i = 0; i < s; i++)
                ss.Append(this.children[i].ToString(lvl));
            return ss.ToString();
        }
        /**
        *	Walks the tree and returns the string, containing all elements aligned in specified order.
        *	@param[in] wm Walk order mode
        *	@return Returns built std::string.
        */
        public string WalkTree(WalkMode wm = WalkMode.WALK_PRE_ORDER)
        {
            string str = "";
            int num = -1;
            switch (wm)
            {
                case WalkMode.WALK_PRE_ORDER:
                    goWalk_pre(ref num, str);
                    break;
                case WalkMode.WALK_POST_ORDER:
                    goWalk_post(ref num, str);
                    break;
                case WalkMode.WALK_SYM_ORDER:
                    goWalk_sym(ref num, str);
                    break;
            }
            return str;
        }
        #endregion
    }
}
