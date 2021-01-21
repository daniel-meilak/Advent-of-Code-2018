## Day 14

Had to use lists as iterators to vectors can be invalidated after resizing the vector.

Can remedy this by using reserve to allocate enough storage for scores string (vector<char>). This way iterators can be used and access times should be faster.