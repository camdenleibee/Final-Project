// include/util/min_heap.h
//
// YOU IMPLEMENT (this whole file; it is a template, so the code lives in the
// header). Spec §4.1.
//
// A binary MIN-heap stored in an array (a std::vector), plus top_k(), which
// uses a size-k min-heap to pick the k largest items in O(n log k).
//
// Rules for this file:
//   * Write sift-up and sift-down yourself.
//   * Do NOT use std::priority_queue, std::make_heap / push_heap / pop_heap /
//     sort_heap, std::sort / stable_sort / partial_sort / nth_element, or
//     std::set / multiset / map / multimap (full list: spec §7.3).
//   * std::vector is fine as the storage array; std::swap, std::reverse and
//     std::pair are fine too.
//
// `Less` is a "less-than" function object, exactly like the one std::sort
// takes: less(a, b) is true when a should come before b. The heap keeps the
// SMALLEST element (by less) at the top.

#pragma once
#include <algorithm>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename T, typename Less = std::less<T>>
class MinHeap {
public:
    explicit MinHeap(Less less = Less()) : less_(std::move(less)) {}

    // Adds value. O(log n).
    void push(T value) {
        data_.push_back(value);                 // Appends value
        std::size_t val = data_.size() - 1;     // Gets index
        data_.sift_up(val);                           // Percolate up to sort
    }

    // The smallest element. Throws std::out_of_range if empty. O(1).
    const T& top() const {
        // Error if Heap is empty 
        if(data_.empty()) throw std::out_of_range("MinHeap::empty Heap");
        else{
            std::size_t val = 0;
            return data_[val];
        }
        
    }

    // Removes and returns the smallest element. Throws std::out_of_range if
    // empty. O(log n).
    T pop() {
      
        if (data_.empty()) throw std::out_of_range("MinHeap::pop called on an empty heap");     // Empty Heap no root node

        T result = std::move(data_[0]);
        std::size_t val = data_.size() - 1; 
        if (data_.empty()) {
            return result;                      // Smallest element is the root and now sifting needs to be done    
        }
        data_[0] = data_[val];
        val = 0;                  // Move last element into root 
        data_.sift_down(val);
        
    }

    std::size_t size() const noexcept { return data_.size(); }
    bool        empty() const noexcept { return data_.empty(); }
    void        clear() noexcept { data_.clear(); }

private:
    // =====  HELPERS AND MEMBERS ====

    // Helper function that Sifts/Percolates Up 
    void sift_up(std::size_t i){
        if (i == 0) return;                         // If index is the root return immediately
        while (i > 0){                              // Percolate up until we reach 0 
            std::size_t parentIndex = (i - 1) / 2;            
            if (less_(data_[i], data_[parentIndex])){        // Check if the value is less then its parent
                std::swap(data_[parentIndex],data_[i]);      // If it is, swap the contents 
                i = parentIndex;                             // Change index i 
            }
            else return;    // Return if parent is smaller then or equal to value                             
        }
    }

    // Helper function that Sifts/Percolates Down 
    void sift_down(std::size_t i){
        while ( i < data_.size()){
            std::size_t left = (2 * i) + 1;    // Left Child 
            std::size_t right = (2 * i) + 2;   // Right Child 

            // Assume candidate to swap with is the left child  
            std::size_t candidate = left;
           
            // Check if left child exists
            if(left >= data_.size()) return;        // If it doesn't exist return immediately
        
            // Check if right child exists and is less then left child 
            if ((right < data_.size()) && (less_(data_[right],data_[left]))) candidate = right; // Make right child the candidate
            // Check if the candidate is less then the parent
            if(less_(data_[candidate],data_[i])){
                std::swap(data_[candidate],data_[i]);   // Swap if it is less
                i = candidate;                          // Update index so the siftdown can continue
            }
            else return;
        }
    }
    
    std::vector<T> data_;
    Less           less_;
};

// Returns the k LARGEST items of `items` (by `less`), largest first.
//   * k == 0 or items empty -> empty result
//   * k >= items.size()     -> every item, largest first
// Must keep a MinHeap of at most k items (the k largest seen so far, with
// the smallest of them on top), so it runs in O(n log k) time and O(k) extra
// space. Items that compare equal may come out in any order.
template <typename T, typename Less = std::less<T>>
std::vector<T> top_k(const std::vector<T>& items, std::size_t k, Less less = Less()) {
    // TODO: keep a MinHeap<T, Less> of at most k items. For each item: if
    // the heap has fewer than k items, push it; otherwise, if the heap's top
    // is less than the item, pop the top and push the item. At the end, pop
    // everything (smallest first) and return it reversed.
    //
    // Clues:
    //  * k == 0: return {} BEFORE the loop (top() on an empty heap throws).
    //  * Build the heap WITH the comparator:  MinHeap<T, Less> heap(less);
    //    Writing  MinHeap<T, Less> heap;  fails for lambdas ("use of deleted
    //    function ... lambda") because a lambda can't be default-constructed.
    //  * "top is less than the item" means  less(heap.top(), item),  not
    //    heap.top() < item.  Read less(a, b) as "a is WORSE than b": the heap
    //    keeps the k best items with the worst of them on top.
    //  * std::reverse (from <algorithm>) flips a vector.
    if (k == 0) return{};
    MinHeap<T, Less> best_items(less);
    std::vector<T> results(0);

    for (std::size_t i = 0; i < items.size(); i++){
        
        T val = items[i];                // Get the first value on the items list

        // If there are open slots in the new list, push it on
        if (best_items.size() < k){
            best_items.push(val);
        }
        // If the list is full compare the first value to the new value being entered
        else if (less(best_items.top(), val)){
            best_items.pop();
            best_items.push(val);       // Put the higher value onto the heap
        }
    // Take the items from the heap and store them in the vector results  
    while(!best_items.empty()){
        T i = best_items.pop();
        results.push_back(i);
    }
    // Reverse the vector so the values are descending greatest -> least 
    std::reverse(results.begin(), results.end());
    return reverse;
}
}

