#pragma once

#include <limits>
#include <memory>
#include <algorithm>

template <class T, class C = std::less<T>, class A = std::allocator<T>>
class bst_in {
private:
    struct Node {
        T value;
        Node *left, *right, *prev;
        using allocator_type = A;
        Node(const T& val);
        void swap(Node& other);
    };
public:
    using key_type = T;
    typedef  T value_type;
    typedef typename A::size_type size_type;
    typedef typename A::difference_type difference_type;
    typedef  C key_compare;
    typedef  C value_compare;
    typedef  A allocator_type;
    typedef T& reference;
    typedef const T& const_reference;
    using AllocTraits = std::allocator_traits<A>;
    typedef typename AllocTraits::pointer pointer;
    typedef typename AllocTraits::const_pointer const_pointer;
    using NodeAlloc = typename AllocTraits::template rebind_alloc<Node>;
    using NodeAllocTraits = typename AllocTraits::template rebind_traits<Node>;

    typedef Node node_type;

    class iterator {
    public:
        Node *node_;
        Node *root_;    
        typedef typename A::difference_type difference_type;
        typedef  T value_type;
        typedef T& reference;
        typedef const typename std::allocator_traits<A>::pointer pointer;
        typedef std::bidirectional_iterator_tag iterator_category;
        iterator();
        iterator(const iterator&);

        iterator& operator=(const iterator&);
        bool operator==(const iterator&) const;
        bool operator!=(const iterator&) const;

        iterator& operator++();
        iterator& operator--();

        reference operator*() const;
        pointer operator->() const;
    };

    class const_iterator {
    public:
        Node *node_;
        Node *root_;
        typedef typename A::difference_type difference_type;
        typedef  T value_type;
        typedef T& reference;
        typedef const typename std::allocator_traits<A>::pointer pointer;
        typedef std::bidirectional_iterator_tag iterator_category;

        const_iterator ();
        const_iterator (const const_iterator&);
        const_iterator (const iterator&);

        const_iterator& operator=(const const_iterator&);
        bool operator==(const const_iterator&) const;
        bool operator!=(const const_iterator&) const;

        const_iterator& operator++();
        const_iterator& operator--();

        reference operator*() const;
        pointer operator->() const;
    };

    typedef typename std::reverse_iterator<iterator> reverse_iterator;
    typedef typename std::reverse_iterator<const_iterator> const_reverse_iterator;
    
    bst_in();
    bst_in(const bst_in&);
    bst_in& operator=(const bst_in&);
    ~bst_in();

    iterator begin();
    iterator end();
    const_iterator cbegin();
    const_iterator cend();
    reverse_iterator rbegin();
    const_reverse_iterator crbegin() const;
    reverse_iterator rend();
    const_reverse_iterator crend() const;

    bool operator==(const bst_in&);
    bool operator!=(const bst_in&);
    void swap(bst_in&);
    size_type max_size();
    bool empty();
    allocator_type get_allocator() const;
    size_t size() const;
    void clear();
    std::pair<iterator, bool> insert(const value_type&);
    std::pair<iterator, bool> insert(node_type&);
    iterator erase(iterator);
    size_type erase(const T&);

    node_type& extract(iterator);
    node_type& extract(const T&);
    template< class C2 >
    void merge(bst_in<T, C2, A>&);

    size_type count(const T&) const;
    iterator find( const T& );
    const_iterator find( const T& ) const;
    bool contains( const T& ) const;

    iterator lower_bound( const T& );
    const_iterator lower_bound( const T& ) const;
    iterator upper_bound( const T& );
    const_iterator upper_bound( const T& ) const;


    std::pair<iterator, iterator> equal_range( const T& );
    std::pair<const_iterator, const_iterator> equal_range( const T& ) const;

private:
    Node* root_;
    size_t size_;
    NodeAlloc alloc_;

    void node_dfs_destructor(Node *);
};

template <class T, class C = std::less<T>, class A = std::allocator<T>>
void swap(bst_in<T,C,A>&, bst_in<T,C,A>&);


template<typename T, typename C, typename A>
bst_in<T, C, A>::Node::Node(const T& val) {
    value = val;
    left = nullptr;
    right = nullptr;
    prev = nullptr;
}

template<typename T, typename C, typename A>
void bst_in<T, C, A>::Node::swap(Node& other) {
    if (this == &other) return;
    std::swap(value, other.value);
    std::swap(left, other.left);
    std::swap(right, other.right);
    std::swap(prev, other.prev);
}

template<typename T, typename C, typename A>
bst_in<T, C, A>::iterator::iterator(): node_(nullptr), root_(nullptr) {};

template<typename T, typename C, typename A>
bst_in<T, C, A>::iterator::iterator(const iterator& other): node_(other.node_), root_(other.root_) {};

template<typename T, typename C, typename A>
typename bst_in<T, C, A>::iterator& bst_in<T, C, A>::iterator::operator=(const iterator& other) {
    node_ = other.node_;
    return *this;
}

template<typename T, typename C, typename A>
bool bst_in<T, C, A>::iterator::operator==(const iterator& other) const {
    return node_ == other.node_;
}

template<typename T, typename C, typename A>
bool bst_in<T, C, A>::iterator::operator!=(const iterator& other) const {
    return node_ != other.node_;
}

template<typename T, typename C, typename A>
typename bst_in<T,C,A>::iterator& bst_in<T,C,A>::iterator::operator++() {
    if (!node_) return *this;

    if (node_->right) {
        node_ = node_->right;
        while (node_->left) node_ = node_->left;
        return *this;
    }

    while (node_->prev && (node_->prev->left != node_)) node_ = node_->prev;
    node_ = node_->prev;

    return *this;
}

template<typename T, typename C, typename A>
typename bst_in<T,C,A>::iterator& bst_in<T,C,A>::iterator::operator--() {
    if (!node_) {
        node_ = root_;
        while (node_->right) node_ = node_->right;
        return *this;
    }

    if (node_->left) {
        node_ = node_->left;
        while (node_->right) node_ = node_->right;
        return *this;
    }

    while (node_->prev && (node_->prev->right != node_)) node_ = node_->prev;
    node_ = node_->prev;

    return *this;
}

template<typename T, typename C, typename A>
typename bst_in<T,C,A>::iterator::reference bst_in<T,C,A>::iterator::operator*() const {
    return node_->value;
}

template<typename T, typename C, typename A>
typename bst_in<T,C,A>::iterator::pointer bst_in<T,C,A>::iterator::operator->() const {
    return &(node_->value);
}

//const iterator
template<typename T, typename C, typename A>
bst_in<T, C, A>::const_iterator::const_iterator(): node_(nullptr), root_(nullptr) {};

template<typename T, typename C, typename A>
bst_in<T, C, A>::const_iterator::const_iterator(const const_iterator& other): node_(other.node_), root_(other.root_) {};

template<typename T, typename C, typename A>
bst_in<T, C, A>::const_iterator::const_iterator(const iterator& other): node_(other.node_), root_(other.root_) {};

template<typename T, typename C, typename A>
typename bst_in<T, C, A>::const_iterator& bst_in<T, C, A>::const_iterator::operator=(const const_iterator& other) {
    node_ = other.node_;
    return *this;
}

template<typename T, typename C, typename A>
bool bst_in<T, C, A>::const_iterator::operator==(const const_iterator& other) const {
    return node_ == other.node_;
}

template<typename T, typename C, typename A>
bool bst_in<T, C, A>::const_iterator::operator!=(const const_iterator& other) const {
    return node_ != other.node_;
}

template<typename T, typename C, typename A>
typename bst_in<T,C,A>::const_iterator& bst_in<T,C,A>::const_iterator::operator++() {
    if (!node_) return *this;

    if (node_->right) {
        node_ = node_->right;
        while (node_->left) node_ = node_->left;
        return *this;
    }

    while (node_->prev && (node_->prev->left != node_)) node_ = node_->prev;
    node_ = node_->prev;

    return *this;
}

template<typename T, typename C, typename A>
typename bst_in<T,C,A>::const_iterator& bst_in<T,C,A>::const_iterator::operator--() {
    if (!node_) {
        node_ = root_;
        while (node_->right) node_ = node_->right;
        return *this;
    }

    if (node_->left) {
        node_ = node_->left;
        while (node_->right) node_ = node_->right;
        return *this;
    }

    while (node_->prev && (node_->prev->right != node_)) node_ = node_->prev;
    node_ = node_->prev;

    return *this;
}

template<typename T, typename C, typename A>
typename bst_in<T,C,A>::const_iterator::reference bst_in<T,C,A>::const_iterator::operator*() const {
    return node_->value;
}
template<typename T, typename C, typename A>
typename bst_in<T,C,A>::const_iterator::pointer bst_in<T,C,A>::const_iterator::operator->() const {
    return &(node_->value);
}

template<typename T, typename C, typename A>
bst_in<T,C,A>::bst_in(): root_(nullptr), size_(0), alloc_() {}

template<typename T, typename C, typename A>
bst_in<T,C,A>::bst_in(const bst_in& other) {
    root_ = other.root_;
    size_ = other.size_;
    alloc_ = other.alloc_;
}

template<class T, class C, class A>
bst_in<T, C, A>& bst_in<T, C, A>::operator=(const bst_in<T, C, A> &other) {
    if (this == &other) return *this;
    root_ = other.root_;
    size_ = other.size_;
    alloc_ = other.alloc_;
}

template<typename T, typename C, typename A>
void bst_in<T, C, A>::node_dfs_destructor(Node *node) {
    if (!node) return;
    node_dfs_destructor(node->left);
    node_dfs_destructor(node->right);
    NodeAllocTraits::destroy(alloc_, node);
    NodeAllocTraits::deallocate(alloc_, node, 1);
}

template<class T, class C, class A>
bst_in<T,C,A>::~bst_in() {
    node_dfs_destructor(root_);
}

template<class T, class C, class A>
typename bst_in<T,C,A>::iterator bst_in<T,C,A>::begin() {
    iterator res; res.node_ = root_; res.root_ = root_;
    while (res.node_ && res.node_->left) res.node_ = res.node_->left;
    return res;
}

template<class T, class C, class A>
typename bst_in<T,C,A>::iterator bst_in<T,C,A>::end() {
    iterator res; res.root_ = root_;
    return res;
}

template<class T, class C, class A>
typename bst_in<T,C,A>::const_iterator bst_in<T,C,A>::cbegin() {
    const_iterator res; res.node_ = root_; res.root_ = root_;
    while (res.node_ && res.node_->left) res.node_ = res.node_->left;
    return res;
}

template<class T, class C, class A>
typename bst_in<T,C,A>::const_iterator bst_in<T,C,A>::cend() {
    const_iterator res; res.root_ = root_;
    return res;
}

template<class T, class C, class A>
typename bst_in<T,C,A>::reverse_iterator bst_in<T,C,A>::rbegin() {
    reverse_iterator res(end());
    return res;
}

template<class T, class C, class A>
typename bst_in<T,C,A>::const_reverse_iterator bst_in<T,C,A>::crbegin() const {
    const_reverse_iterator res(end());
    return res;
}

template<class T, class C, class A>
typename bst_in<T,C,A>::reverse_iterator bst_in<T,C,A>::rend() {
    reverse_iterator res(begin());
    return res;
}

template<class T, class C, class A>
typename bst_in<T,C,A>::const_reverse_iterator bst_in<T,C,A>::crend() const {
    const_reverse_iterator res(begin());
    return res;
}

template<class T, class C, class A>
bool bst_in<T,C,A>::operator==(const bst_in& other) {
    return other.root_ == root_;
}

template<class T, class C, class A>
bool bst_in<T,C,A>::operator!=(const bst_in& other) {
    return other.root_ != root_;
}

template<class T, class C, class A>
void bst_in<T,C,A>::swap(bst_in& other) {
    if (this == &other) return;
    std::swap(alloc_, other.alloc_);
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
}

template<class T, class C, class A>
typename bst_in<T,C,A>::size_type bst_in<T,C,A>::max_size() {return std::numeric_limits<difference_type>::max();}

template<class T, class C, class A>
bool bst_in<T,C,A>::empty() {return size_ == 0;}

template<class T, class C, class A>
typename bst_in<T,C,A>::allocator_type bst_in<T,C,A>::get_allocator() const {
    return alloc_;
}

template<class T, class C, class A>
size_t bst_in<T,C,A>::size() const {return size_;}

template<class T, class C, class A>
void bst_in<T,C,A>::clear() {
    node_dfs_destructor(root_);
    size_ = 0;
    root_ = nullptr;
}

template<class T, class C, class A>
std::pair<typename bst_in<T,C,A>::iterator, bool> bst_in<T,C,A>::insert(const value_type& value) {
    iterator v;
    v.node_ = root_;
    iterator prev;
    while (v.node_ && ((*v) != value)) {
        prev = v;
        if (*v > value) {
            v.node_ = v.node_->left;
        } else {
            v.node_ = v.node_->right;
        }
    }

    if (v.node_) return {v, false};
    else {
        size_++;
        Node *node = NodeAllocTraits::allocate(alloc_, 1);
        NodeAllocTraits::construct(alloc_, node, value);
        if (size_ == 1) root_ = node;
        v.node_ = node;
        node->prev = prev.node_;
        if (prev.node_ && (*prev < value)) prev.node_->right = node;
        else if (prev.node_) prev.node_->left = node;
        return {v, true};
    }
}

template<class T, class C, class A>
std::pair<typename bst_in<T,C,A>::iterator, bool> bst_in<T,C,A>::insert(node_type& node) {
    iterator v;
    v.node_ = root_;
    iterator prev;
    while (v.node_ && ((*v) != node.value)) {
        prev = v;
        if (*v > node.value) {
            v.node_ = v.node_->left;
        } else {
            v.node_ = v.node_->right;
        }
    }

    if (v.node_) return {v, false};
    else {
        size_++;
        if (size_ == 1) root_ = &node;
        v.node_ = &node;
        node.prev = prev.node_;
        if (prev.node_ && (*prev < node.value)) prev.node_->right = &node;
        else if (prev.node_) prev.node_->left = &node;
        return {v, true};
    }
}

template<class T, class C, class A>
typename bst_in<T,C,A>::iterator bst_in<T,C,A>::erase(iterator pos) {
    if (!pos.node_) return pos;
    size_--;
    if (size_ == 0) root_ = nullptr;

    if (pos.node_->right) {
        iterator next(pos);
        ++next;
        std::swap(pos.node_->value, next.node_->value);
        if (next.node_->right) {
            next.node_->right->prev = next.node_->prev;
            if (next.node_->prev->left == next.node_) {
               next.node_->prev->left = next.node_->right; 
            } else {
                next.node_->prev->right = next.node_->right;
            }
        } else {
            if (next.node_->prev->left == next.node_) {
               next.node_->prev->left = nullptr; 
            } else {
                next.node_->prev->right = nullptr;
            }
        }
        NodeAllocTraits::destroy(alloc_, next.node_);
        NodeAllocTraits::deallocate(alloc_, next.node_, 1);
        return pos;
    } else if (pos.node_->left) {
        iterator next(pos); ++next;
        if (root_ == pos.node_) root_ = pos.node_->left;
        if (pos.node_->prev) {
            if (pos.node_->prev->left == pos.node_) pos.node_->prev->left = pos.node_->left;
            else pos.node_->prev->right = pos.node_->left;
        }
        pos.node_->left->prev = pos.node_->prev;
        NodeAllocTraits::destroy(alloc_, pos.node_);
        NodeAllocTraits::deallocate(alloc_, pos.node_, 1);
        return next;
    } else {
        iterator next; next.node_ = pos.node_; ++next;
        if (root_ == pos.node_) root_ = nullptr;
        NodeAllocTraits::destroy(alloc_, pos.node_);
        NodeAllocTraits::deallocate(alloc_, pos.node_, 1);
        return next;
    }
}

template<class T, class C, class A>
typename bst_in<T,C,A>::size_type bst_in<T,C,A>::erase(const T& key) {

    iterator pos;
    pos.node_ = root_; pos.root_ = root_;

    while (pos.node_ && (*pos != key)) {
        if (*pos > key) pos.node_ = pos.node_->left;
        else pos.node_ = pos.node_->right;
    }

    if (!pos.node_) return 0;
    size_--;
    if (size_ == 0) root_ = nullptr;

    if (pos.node_->right) {
        iterator next(pos);
        ++next;
        std::swap(pos.node_->value, next.node_->value);
        if (next.node_->right) {
            next.node_->right->prev = next.node_->prev;
            if (next.node_->prev->left == next.node_) {
               next.node_->prev->left = next.node_->right; 
            } else {
                next.node_->prev->right = next.node_->right;
            }
        } else {
            if (next.node_->prev->left == next.node_) {
               next.node_->prev->left = nullptr; 
            } else {
                next.node_->prev->right = nullptr;
            }
        }
        NodeAllocTraits::destroy(alloc_, next.node_);
        NodeAllocTraits::deallocate(alloc_, next.node_, 1);
    } else if (pos.node_->left) {
        iterator next(pos); ++next;
        if (root_ == pos.node_) root_ = pos.node_->left;
        if (pos.node_->prev) {
            if (pos.node_->prev->left == pos.node_) pos.node_->prev->left = pos.node_->left;
            else pos.node_->prev->right = pos.node_->left;
        }
        pos.node_->left->prev = pos.node_->prev;
        NodeAllocTraits::destroy(alloc_, pos.node_);
        NodeAllocTraits::deallocate(alloc_, pos.node_, 1);
    } else {
        iterator next; next.node_ = pos.node_; ++next;
        if (root_ == pos.node_) root_ = nullptr;
        NodeAllocTraits::destroy(alloc_, pos.node_);
        NodeAllocTraits::deallocate(alloc_, pos.node_, 1);
    }
    return 1;
}

template<class T, class C, class A>
typename bst_in<T,C,A>::node_type& bst_in<T,C,A>::extract(iterator pos) {
    if (!pos.node_) return *(NodeAllocTraits::allocate(alloc_, 1));
    size_--;
    if (size_ == 0) root_ = nullptr;

    if (pos.node_->right) {
        iterator next(pos);
        ++next;
        std::swap(pos.node_->value, next.node_->value);
        if (next.node_->right) {
            next.node_->right->prev = next.node_->prev;
            if (next.node_->prev->left == next.node_) {
               next.node_->prev->left = next.node_->right; 
            } else {
                next.node_->prev->right = next.node_->right;
            }
        } else {
            if (next.node_->prev->left == next.node_) {
               next.node_->prev->left = nullptr; 
            } else {
                next.node_->prev->right = nullptr;
            }
        }
        return *next.node_;
    } else if (pos.node_->left) {
        iterator next(pos); ++next;
        if (root_ == pos.node_) root_ = pos.node_->left;
        if (pos.node_->prev) {
            if (pos.node_->prev->left == pos.node_) pos.node_->prev->left = pos.node_->left;
            else pos.node_->prev->right = pos.node_->left;
        }
        pos.node_->left->prev = pos.node_->prev;
        return *pos.node_;
    } else {
        iterator next; next.node_ = pos.node_; ++next;
        if (root_ == pos.node_) root_ = nullptr;
        return *pos.node_;
    }
}

template<class T, class C, class A>
typename bst_in<T,C,A>::node_type& bst_in<T,C,A>::extract(const T& key) {

    iterator pos;
    pos.node_ = root_; pos.root_ = root_;

    while (pos.node_ && (*pos != key)) {
        if (*pos > key) pos.node_ = pos.node_->left;
        else pos.node_ = pos.node_->right;
    }

    if (!pos.node_) return *(NodeAllocTraits::allocate(alloc_, 1));
    size_--;
    if (size_ == 0) root_ = nullptr;

    if (pos.node_->right) {
        iterator next(pos);
        ++next;
        std::swap(pos.node_->value, next.node_->value);
        if (next.node_->right) {
            next.node_->right->prev = next.node_->prev;
            if (next.node_->prev->left == next.node_) {
               next.node_->prev->left = next.node_->right; 
            } else {
                next.node_->prev->right = next.node_->right;
            }
        } else {
            if (next.node_->prev->left == next.node_) {
               next.node_->prev->left = nullptr; 
            } else {
                next.node_->prev->right = nullptr;
            }
        }
        return next.node_;
    } else if (pos.node_->left) {
        iterator next(pos); ++next;
        if (root_ == pos.node_) root_ = pos.node_->left;
        if (pos.node_->prev) {
            if (pos.node_->prev->left == pos.node_) pos.node_->prev->left = pos.node_->left;
            else pos.node_->prev->right = pos.node_->left;
        }
        pos.node_->left->prev = pos.node_->prev;
        return pos.node_;
    } else {
        iterator next; next.node_ = pos.node_; ++next;
        if (root_ == pos.node_) root_ = nullptr;
        return pos.node_;
    }
}

template<class T, class C, class A>
template< class C2 >
void bst_in<T,C,A>::merge(bst_in<T,C2,A>& source) {
    while (source.size()) {
        insert(source.extract(source.begin()));
    }
}

template<class T, class C, class A>
typename bst_in<T,C,A>::size_type bst_in<T,C,A>::count(const T& key) const {
    iterator pos;
    pos.node_ = root_; pos.root_ = root_;

    while (pos.node_ && (*pos != key)) {
        if (*pos > key) pos.node_ = pos.node_->left;
        else pos.node_ = pos.node_->right;
    }

    if (!pos.node_) return 0;
    return 1;
}

template<class T, class C, class A>
typename bst_in<T,C,A>::iterator bst_in<T,C,A>::find(const T& key) {
    iterator pos;
    pos.node_ = root_; pos.root_ = root_;

    while (pos.node_ && (*pos != key)) {
        if (*pos > key) pos.node_ = pos.node_->left;
        else pos.node_ = pos.node_->right;
    }

    if (!pos.node_) return end();
    return pos;
}

template<class T, class C, class A>
typename bst_in<T,C,A>::const_iterator bst_in<T,C,A>::find(const T& key) const {
    const_iterator pos;
    pos.node_ = root_; pos.root_ = root_;

    while (pos.node_ && (*pos != key)) {
        if (*pos > key) pos.node_ = pos.node_->left;
        else pos.node_ = pos.node_->right;
    }

    if (!pos.node_) return cend();
    return pos;
}

template <class T, class C, class A>
bool bst_in<T,C,A>::contains(const T& key) const {
    iterator pos;
    pos.node_ = root_; pos.root_ = root_;

    while (pos.node_ && (*pos != key)) {
        if (*pos > key) pos.node_ = pos.node_->left;
        else pos.node_ = pos.node_->right;
    }

    if (!pos.node_) return false;
    return true;
}

template <class T, class C, class A>
typename bst_in<T,C,A>::iterator bst_in<T,C,A>::lower_bound(const T& key) {
    iterator pos;
    pos.node_ = root_; pos.root_ = root_;

    while (pos.node_ && (*pos != key)) {
        if (*pos > key) {
            if (pos.node_->left) pos.node_ = pos.node_->left;
            else break;
        } else {
            if (pos.node_->right) pos.node_ = pos.node_->right;
            else break;
        }
    }
    
    if (!pos.node_) return end();
    while ((pos != end()) && (*pos < key)) ++pos;
    return pos;
}

template <class T, class C, class A>
typename bst_in<T,C,A>::const_iterator bst_in<T,C,A>::lower_bound(const T& key) const {
    const_iterator pos;
    pos.node_ = root_; pos.root_ = root_;

    while (pos.node_ && (*pos != key)) {
        if (*pos > key) {
            if (pos.node_->left) pos.node_ = pos.node_->left;
            else break;
        } else {
            if (pos.node_->right) pos.node_ = pos.node_->right;
            else break;
        }
    }
    
    if (!pos.node_) return cend();
    while ((pos != cend()) && (*pos < key)) ++pos;
    return pos;
}

template <class T, class C, class A>
typename bst_in<T,C,A>::iterator bst_in<T,C,A>::upper_bound(const T& key) {
    iterator pos = lower_bound(key);
    if ((pos == end()) || (*pos > key)) return pos;
    else return ++pos;
}

template <class T, class C, class A>
typename bst_in<T,C,A>::const_iterator bst_in<T,C,A>::upper_bound(const T& key) const {
    const_iterator pos = lower_bound(key);
    if ((pos == end()) || (*pos > key)) return pos;
    else return ++pos;
}

template <class T, class C, class A>
std::pair<typename bst_in<T,C,A>::iterator,typename bst_in<T,C,A>::iterator> bst_in<T,C,A>::equal_range(const T& key) {
    return {lower_bound(key), upper_bound(key)};
}

template <class T, class C, class A>
std::pair<typename bst_in<T,C,A>::const_iterator,typename bst_in<T,C,A>::const_iterator> bst_in<T,C,A>::equal_range(const T& key) const {
    return {lower_bound(key), upper_bound(key)};
}

template <class T, class C, class A>
void swap(bst_in<T,C,A>& lhs, bst_in<T,C,A>& rhs) {
    if (lhs != rhs) {
        std::swap(lhs.alloc_, rhs.alloc_);
        std::swap(lhs.root_, rhs.root_);
        std::swap(lhs.size_, rhs.size_);
    }
}