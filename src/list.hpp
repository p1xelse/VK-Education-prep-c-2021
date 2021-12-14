#pragma once  // NOLINT
#include <iterator>
#include <limits>

namespace task {

template <class T>
class list {
 public:
    class const_iterator;
    struct node {
        friend class list<T>;
        node(const T& value);
        T data;
        node* prev;
        node* next;
    };
    class iterator {
     public:
        friend class list<T>;
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::bidirectional_iterator_tag;

        iterator();
        iterator(node* ptr);
        iterator(const iterator&);
        iterator& operator=(const iterator&);

        iterator& operator++();
        iterator operator++(int);
        reference operator*() const;
        pointer operator->() const;
        iterator& operator--();
        iterator operator--(int);

        bool operator==(iterator other) const;
        bool operator!=(iterator other) const;

        // Your code goes here?..

     private:
        node* cur;
    };

    class const_iterator {
     public:
        friend class list<T>;
        using difference_type = ptrdiff_t;
        using value_type = const T;
        using pointer = const T*;
        using reference = const T&;
        using iterator_category = std::bidirectional_iterator_tag;

        const_iterator();
        const_iterator(node* ptr);
        const_iterator(const const_iterator&);
        const_iterator(const iterator&);
        const_iterator& operator=(const const_iterator&);

        const_iterator& operator++();
        const_iterator operator++(int);
        reference operator*() const;
        pointer operator->() const;
        const_iterator& operator--();
        const_iterator operator--(int);

        bool operator==(const_iterator other) const;
        bool operator!=(const_iterator other) const;

        // Your code goes here?..

     private:
        node* cur;
    };

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    list();
    list(size_t count, const T& value);
    explicit list(size_t count);
    ~list();

    list(const list& other);
    list& operator=(const list& other);

    T& front();
    const T& front() const;

    T& back();
    const T& back() const;

    iterator begin() const;
    iterator end() const;

    const_iterator cbegin() const;
    const_iterator cend() const;

    reverse_iterator rbegin() const;
    reverse_iterator rend() const;

    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;

    bool empty() const;
    size_t size() const;
    size_t max_size() const;
    void clear();

    const_iterator insert(const_iterator pos, const T& value);
    const_iterator insert(const_iterator pos, size_t count, const T& value);

    const_iterator erase(const_iterator pos);
    const_iterator erase(const_iterator first, const_iterator last);

    void push_back(const T& value);
    void pop_back();

    void push_front(const T& value);
    void pop_front();

    void resize(size_t count);
    void swap(list& other);

    void sorted_insert(node* elem);
    void merge(list& other);
    void splice(const_iterator pos, list& other);
    void remove(const T& value);
    void reverse();
    void unique();
    void sort();

    node* last_ptr;
    node* first_ptr;
    size_t size_l;

 private:
    size_t max_size_par = std::numeric_limits<size_t>::max();
    // Your code goes here...
};

template <class T>
list<T>::iterator::iterator() : cur{NULL} {}

template <class T>
list<T>::iterator::iterator(node* ptr) : cur{ptr} {}

template <class T>
list<T>::iterator::iterator(const iterator& other) : cur{other.cur} {}

template <class T>
typename list<T>::iterator& list<T>::iterator::operator=(
    const iterator& other) {
    cur = other.cur;
    return *this;
}

template <class T>
typename list<T>::iterator& list<T>::iterator::operator++() {
    cur = cur->next;
    return *this;
}

template <class T>
typename list<T>::iterator list<T>::iterator::operator++(int) {
    node* tmp = cur;
    iterator it(tmp);
    cur = cur->next;
    return it;
}

template <class T>
typename list<T>::iterator& list<T>::iterator::operator--() {
    cur = cur->prev;
    return *this;
}

template <class T>
typename list<T>::iterator list<T>::iterator::operator--(int) {
    node* tmp = cur;
    iterator it(tmp);
    cur = cur->prev;
    return it;
}

template <class T>
typename list<T>::iterator::reference list<T>::iterator::operator*() const {
    return cur->data;
}

template <class T>
typename list<T>::iterator::pointer list<T>::iterator::operator->() const {
    return &cur->data;
}

template <class T>
bool list<T>::iterator::operator==(iterator other) const {
    return other.cur == cur;
}

template <class T>
bool list<T>::iterator::operator!=(iterator other) const {
    return other.cur != cur;
}

template <class T>
list<T>::const_iterator::const_iterator() : cur{NULL} {}

template <class T>
list<T>::const_iterator::const_iterator(const iterator& other)
    : cur{other.cur} {}

template <class T>
list<T>::const_iterator::const_iterator(node* ptr) : cur{ptr} {}

template <class T>
list<T>::const_iterator::const_iterator(const const_iterator& other)
    : cur{other.cur} {}

template <class T>
typename list<T>::const_iterator& list<T>::const_iterator::operator=(
    const const_iterator& other) {
    cur = other.cur;
    return *this;
}

template <class T>
typename list<T>::const_iterator& list<T>::const_iterator::operator++() {
    cur = cur->next;
    return *this;
}

template <class T>
typename list<T>::const_iterator list<T>::const_iterator::operator++(int) {
    node* tmp = cur;
    const_iterator it(tmp);
    cur = cur->next;
    return it;
}

template <class T>
typename list<T>::const_iterator& list<T>::const_iterator::operator--() {
    cur = cur->prev;
    return *this;
}

template <class T>
typename list<T>::const_iterator list<T>::const_iterator::operator--(int) {
    node* tmp = cur;
    const_iterator it(tmp);
    cur = cur->prev;
    return it;
}

template <class T>
typename list<T>::const_iterator::reference list<T>::const_iterator::operator*()
    const {
    return cur->data;
}

template <class T>
typename list<T>::const_iterator::pointer list<T>::const_iterator::operator->()
    const {
    return &cur->data;
}

template <class T>
bool list<T>::const_iterator::operator==(const_iterator other) const {
    return other.cur == cur;
}

template <class T>
bool list<T>::const_iterator::operator!=(const_iterator other) const {
    return other.cur != cur;
}

template <class T>
typename list<T>::iterator list<T>::begin() const {
    if (first_ptr == NULL) return iterator();

    iterator it(first_ptr);
    return it;
}

template <class T>
typename list<T>::iterator list<T>::end() const {
    if (last_ptr == NULL) return iterator();

    iterator it(last_ptr->next);
    return it;
}

template <class T>
typename list<T>::const_iterator list<T>::cbegin() const {
    if (first_ptr == NULL) return iterator();

    const_iterator it(first_ptr);
    return it;
}

template <class T>
typename list<T>::reverse_iterator list<T>::rbegin() const {
    if (last_ptr == NULL) return reverse_iterator();
    reverse_iterator it(last_ptr->next);
    return it;
}

template <class T>
typename list<T>::const_reverse_iterator list<T>::crbegin() const {
    if (last_ptr == NULL) return const_reverse_iterator();

    const_reverse_iterator it(last_ptr->next);
    return it;
}

template <class T>
typename list<T>::const_iterator list<T>::cend() const {
    if (last_ptr == NULL) return const_iterator();

    const_iterator it(last_ptr->next);
    return it;
}

template <class T>
typename list<T>::reverse_iterator list<T>::rend() const {
    if (first_ptr == NULL) return reverse_iterator();

    reverse_iterator it(first_ptr);
    return it;
}

template <class T>
typename list<T>::const_reverse_iterator list<T>::crend() const {
    const_reverse_iterator it(first_ptr);
    return it;
}
template <class T>
typename list<T>::const_iterator list<T>::insert(const_iterator pos,
                                                 const T& value) {
    size_l++;
    node* new_node = new node(value);
    if (pos == cend()) {
        last_ptr = new_node;
    }
    if (pos == cbegin()) {
        new_node->prev = NULL;
        new_node->next = first_ptr;
        first_ptr->prev = new_node;
        first_ptr = new_node;
        return const_iterator(first_ptr);
    }

    new_node->next = pos.cur;
    new_node->prev = pos.cur->prev;

    pos.cur->prev->next = new_node;
    pos.cur->prev = new_node;

    return const_iterator(new_node);
}
template <class T>
typename list<T>::const_iterator list<T>::insert(const_iterator pos,
                                                 size_t count, const T& value) {
    if (count == 0) return const_iterator(pos.cur);
    typename list<T>::const_iterator it = insert(pos, value);
    for (size_t i = 0; i < count - 1; ++i) {
        insert(pos, value);
    }

    return it;
}
template <class T>
typename list<T>::const_iterator list<T>::erase(const_iterator pos) {
    if (size_l == 1) {
        pop_back();
        return iterator();
    }
    size_l--;
    if (pos == cbegin()) {
        first_ptr = pos.cur->next;
        delete pos.cur;
        first_ptr->prev = NULL;
        return const_iterator(first_ptr);
    }

    auto it_res = const_iterator(pos.cur->next);
    pos.cur->prev->next = pos.cur->next;
    pos.cur->next->prev = pos.cur->prev;
    if (pos == const_iterator(last_ptr)) last_ptr = last_ptr->prev;
    delete pos.cur;
    return it_res;
}
template <class T>
typename list<T>::const_iterator list<T>::erase(const_iterator first,
                                                const_iterator last) {
    auto it = const_iterator(last.cur->next);
    auto it_c = first;
    while ((it_c = erase(it_c)) != last) {}
    return it;
}

// template <class T>
// list<T>::node::node() {
//     data = T{};
//     prev = NULL;
//     next = NULL;
// }

template <class T>
list<T>::node::node(const T& value) {
    data = value;
    prev = NULL;
    next = NULL;
}

template <class T>
list<T>::list() {
    first_ptr = NULL;
    last_ptr = NULL;
    size_l = 0;
}

template <class T>
void list<T>::push_back(const T& value) {
    if (size_l == 0) {
        first_ptr = new node(value);
        node* cap = new node(T{});
        first_ptr->next = cap;
        cap->next = NULL;
        cap->prev = first_ptr;

        last_ptr = first_ptr;
    } else {
        node* new_node = new node(value);
        last_ptr->next->prev = new_node;
        new_node->next = last_ptr->next;
        new_node->prev = last_ptr;
        last_ptr->next = new_node;
        last_ptr = new_node;
    }
    size_l++;
}

template <class T>
void list<T>::push_front(const T& value) {
    if (size_l == 0) {
        first_ptr = new node(value);
        node* cap = new node(T{});
        first_ptr->next = cap;
        cap->next = NULL;
        cap->prev = first_ptr;
        last_ptr = first_ptr;
    } else {
        node* new_node = new node(value);
        first_ptr->prev = new_node;
        new_node->next = first_ptr;
        new_node->prev = NULL;
        first_ptr = new_node;
    }
    size_l++;
}

template <class T>
void list<T>::pop_front() {
    if (size_l == 0) {
    } else if (size_l == 1) {
        delete first_ptr->next;
        delete first_ptr;
        first_ptr = NULL;
        last_ptr = NULL;
    } else {
        first_ptr = first_ptr->next;
        delete first_ptr->prev;
        first_ptr->prev = NULL;
    }
    size_l--;
}

template <class T>
void list<T>::pop_back() {
    if (size_l == 0) {
    } else if (size_l == 1) {
        delete first_ptr->next;
        delete first_ptr;
        first_ptr = NULL;
        last_ptr = NULL;
    } else {
        node* tmp = last_ptr->prev;
        last_ptr->prev->next = last_ptr->next;
        last_ptr->next->prev = last_ptr->prev;
        delete last_ptr;
        last_ptr = tmp;
    }
    size_l--;
}

template <class T>
list<T>::list(size_t count, const T& value) : list() {
    for (size_t i = 0; i < count; i++) {
        push_back(value);
    }
}

template <class T>
T& list<T>::front() {
    return first_ptr->data;
}

template <class T>
const T& list<T>::front() const {
    return first_ptr->data;
}

template <class T>
T& list<T>::back() {
    return last_ptr->data;
}

template <class T>
const T& list<T>::back() const {
    return last_ptr->data;
}

template <class T>
list<T>::~list() {
    node* cur = first_ptr;
    node* next;
    for (; cur; cur = next) {
        next = cur->next;
        delete cur;
    }
}

template <class T>
list<T>::list(size_t count) : list() {
    for (size_t i = 0; i < count; i++) {
        push_back(T{});
    }
}

template <class T>
list<T>::list(const list& other) : list() {
    node* cur = other.first_ptr;
    for (; cur != other.last_ptr->next; cur = cur->next) {
        T value = cur->data;
        push_back(value);
    }
}

template <class T>
list<T>& list<T>::operator=(const list& other) {
    clear();
    size_l = 0;
    auto it = other.begin();
    while (it != other.end()) {
        push_back(*it);
        ++it;
    }
    return *this;
}

template <class T>
bool list<T>::empty() const {
    return size_l == 0;
}
template <class T>
size_t list<T>::max_size() const {
    return max_size_par;
}

template <class T>
size_t list<T>::size() const {
    return size_l;
}

template <class T>
void list<T>::clear() {
    while (size_l) pop_front();
}
template <class T>
void list<T>::resize(size_t count) {
    if (count == 0)
        clear();
    //    std::cout << size_l;
    size_t diff = count - size_l;
    if (count > size_l) {
        for (size_t i = 0; i < diff; ++i) push_back(T{});
    }
    if (count < size_l) {
        for (size_t i = 0; i < size_l - count; ++i) pop_back();
    }

    size_l = count;
}

template <class T>
void list<T>::swap(list& other) {
    node* f_tmp = first_ptr;
    node* l_tmp = last_ptr;
    size_t tmp_size = size_l;

    first_ptr = other.first_ptr;
    last_ptr = other.last_ptr;
    size_l = other.size_l;

    other.first_ptr = f_tmp;
    other.last_ptr = l_tmp;
    other.size_l = tmp_size;
}

template <class T>
void list<T>::sorted_insert(node* elem) {
    node* cur = first_ptr;
    if (cur == NULL) {
        first_ptr = last_ptr = elem;
        node *cap = new node(T{});
        cap->next = NULL;
        cap->prev = last_ptr;
        first_ptr->prev = NULL;
        first_ptr->next = cap;
        size_l++;
        return;
    }
    if (elem->data <= cur->data) {
        cur->prev = elem;
        elem->next = cur;
        elem->prev = NULL;
        first_ptr = elem;
        size_l++;
        return;
    }

    for (; cur != last_ptr->next; cur = cur->next) {
        if (cur->next == last_ptr->next) {
            elem->next = cur->next;
            elem->prev = cur;
            cur->next->prev = elem;
            cur->next = elem;
            last_ptr = elem;
            //            std::cout << "hello" << std::endl;
            size_l++;
            return;
        }

        if (cur->next->data >= elem->data) {
            elem->next = cur->next;
            elem->prev = cur->next->prev;
            cur->next->prev = elem;
            cur->next = elem;
            size_l++;
            return;
        }
    }
}

template <class T>
void list<T>::merge(list& other) {
    if (this == &other) return;
    node* next;
    delete other.last_ptr->next;
    other.last_ptr->next = NULL;
    for (node* cur = other.first_ptr; cur; cur = next) {
        next = cur->next;
        sorted_insert(cur);
    }
    other.first_ptr = NULL;
    other.last_ptr = NULL;
    other.size_l = 0;
}

template <class T>
void list<T>::splice(const_iterator pos, list& other) {
    size_l += other.size_l;
    if (pos == begin()) {
        delete other.last_ptr->next;
        first_ptr->prev = other.last_ptr;
        other.last_ptr->next = pos.cur;
        first_ptr = other.first_ptr;
    } else if (pos == end()) {
        delete last_ptr->next;
        last_ptr->next = other.first_ptr;
        other.first_ptr->prev = last_ptr;
        last_ptr = other.last_ptr;

    } else {
        pos.cur->prev->next = other.first_ptr;
        other.first_ptr->prev = pos.cur->prev;

        delete other.last_ptr->next;
        other.last_ptr->next = pos.cur;
        pos.cur->prev = other.last_ptr;
    }

    other.first_ptr = NULL;
    other.last_ptr = NULL;
    other.size_l = 0;
}
template <class T>
void list<T>::remove(const T& value) {
    T value_copy = value;
    auto it = cbegin();
    while (it != cend()) {
        if (*it == value_copy)
            it = erase(it);
        else
            ++it;
    }
}
template <class T>
void list<T>::reverse() {
    list<T> new_list;
    auto it = begin();
    while (it != end()) {
        new_list.push_front(it.cur->data);
        ++it;
    }
    swap(new_list);
    new_list.clear();
}

template <class T>
void list<T>::unique() {
    node* cur = first_ptr;
    while (cur->next) {
        if (cur->data == cur->next->data) {
            node* new_cur = cur->next;
            while (new_cur->next && (new_cur->data == new_cur->next->data)) {
                new_cur = new_cur->next;
            }
            auto it_beg = const_iterator(cur);
            auto it_end = const_iterator(new_cur);
            auto it_res = erase(it_beg, it_end);
            cur = it_res.cur;
        } else {
            cur = cur->next;
        }
    }
}

template <class T>
void list<T>::sort() {
    list<T> new_list;
    node* cur = first_ptr;
    if (cur == NULL) return;

    delete last_ptr->next;
    last_ptr->next = NULL;

    while (cur != NULL) {
        node* next = cur->next;
        new_list.sorted_insert(cur);
        cur = next;
    }

    swap(new_list);

    new_list.last_ptr = NULL;
    new_list.first_ptr = NULL;
    new_list.size_l = 0;
}

}  // namespace task
