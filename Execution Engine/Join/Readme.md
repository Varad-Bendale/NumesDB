# JOIN Clause 

Join clause is most probably the clause which needs the most optimization, as it deals with millions of tables being joined with each other again and again. So, we pretty much need to be very clever and try to optimize it at the `ON` condition of the joins. `CROSS JOIN` is a different thing, but the rest of the stuff is pretty much covered by the `ON` condition, and we can optimize a lot at that part.

So, let's start with the first point itself, taking everything from the parser to a special data structure created for it. The structure goes something like this: the `FROM` condition for each of the columns is made, and if a join occurs, we can get it as a proper structure since the structure is already present. One issue that currently persists is that it kind of works with only one join. When multiple joins are involved, it breaks, so that is something that needs to be worked on.

The structure basically contains the `FROM` name, the `JOIN` name, and the `ON` condition. The `ON` condition can further contain the left-side stuff and the right-side stuff. So, all of this is already handled by the `from_parser_to_struct` kind of structure. The structure itself remains the same for getting things from the parser to it, so I am not elaborating much on that here. If interested, you can read about it from the very first conversion in the main engine's documentation.

Similarly, the `if` in the raw function solver, `join_func`, remains pretty much the same as well, except for the data structure changing.

Now, let's consider the case where the `ON` condition consists of an `=` condition, something like `a.id = b.id`. Here, the best thing we can do is hash out the columns based on one thing, and then whichever of them matches, we perform the join operation on it. Pretty much, that is the thinking behind this approach.

This is covered in the function `join_equal_clause`. Here, first all the hash bins are created, which contain all the information like the actual value, the primary key of both the tables which are joining with each other, the number of entries, the row number, the table name, and other required information.

Now, why the primary key? The main thinking behind this was that we get the columns which are similar and the compare values of them, but to actually get to those values and move our cursor to them, we need to use the seek functions in the bytecodes. For that, the tree needs to be sorted, and trees are sorted according to the primary keys. So, the involvement of the primary keys becomes kind of important here.

Coming back to the main function which handles this equal clause, we pretty much need to get all the tables which are involved in the conditions, initialize the cursor, and move the cursor according to the condition that is present. There is also one special data structure created in `select_info`, which contains all the information about the number of tables, all the hash-related information, and other required details.

We then call these functions and make sure that whatever columns and tables are present need to be called only once. This is where the hash is used. In the main function, we take out the tables which need to be joined and take the primary keys of those tables. Then, if the tree is extending either to the left or right of the root, we take that information and put it into the hash. We use the bytecode for this operation. The same thing is done with the column name, and the extra column is found.

Coming to the bytecodes, this is where all of these things are actually done. In the hashing part, we pretty much put all the required details into the blob and then pass it. Here, the number of hash bins is calculated using a special formula. Then the primary keys of both sides, the row number, and the table name of the compare value are picked from the required data. The hash cursor is then given in the bytecode and the operation is performed.

Once we get the data, the whole blob is opened and we call the function `hash_bin_which`. This function simply assigns the hash bin for the data regardless of the data type it contains. This is how the whole hash table is created for each of the required things present on each side of the `=` condition.


