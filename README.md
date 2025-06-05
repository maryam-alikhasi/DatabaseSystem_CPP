# Simple B-Tree Database System

This project is a simplified simulation of a database management system implemented in C++. It utilizes a **B-Tree** data structure to store and manage indexed data efficiently. The system supports basic table creation, record insertion, deletion, and search functionalities using three types of indexes: incremental, unique, and non-unique.

---

## Features

- Create multiple tables with a specified schema and index type
- Support for different index modes:
  - Incremental (auto-increment primary key)
  - Unique indexes
  - Non-unique indexes
- Insert new records into any table
- Remove a record using its index
- Search for a record by its index
- Print all records in a table in sorted order
- Command-line menu interface

---

## Data Structure

This system uses a **B-Tree** as the underlying data structure for each table to ensure efficient insertion, deletion, and search operations. The B-Tree maintains sorted data and allows logarithmic time complexity for basic operations.

---

## Implementation Details

- `Row<T>`: Template class to represent a row in a table, where `T` is the data type.
- `BTreeNode<T>`: Template class implementing nodes of the B-Tree.
- `BTree<T>`: Template class for the full B-Tree, managing insert, delete, and search operations.
- `tables<T>`: High-level abstraction representing a table with a B-Tree as its index.

---

## How to Use

Compile the program using a C++ compiler (e.g., `g++`)

You will be guided through a menu to:

* Create a table
* Insert or delete data
* Search and print records

---

## Sample Menu Interface

```
1:Create a new table
2:Exite

Enter the name of the table and its data type
table employees was created

What are table indexes based on?
1:incremental(main index)
2:Unique indexes
3:Non-unique indexes
```

---

## Example Use Case

You can create a table named `employees` with 3 fields (e.g., name, role, department) and use either a unique employee ID or an auto-incremented index to manage entries efficiently.

---

## Concepts Practiced

* Data structure design (B-Tree)
* Template programming in C++
* Basic database management concepts
* Command-line interface development
