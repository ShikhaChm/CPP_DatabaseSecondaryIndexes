Please read the file to know how to run the .cpp files in this project.

File dataGen.cpp
======================
The file dataGen generates a database file everytime we execute it passing right command line arguments.This file creates a folder names 'indexes' inside which there will be 9 index folders named index1, index2 till index9(as our database file consists of 9 coulmns).

./dataGen <number of records you want to generate in databse file>  <path where the file is to be saved, give any folder name For ex: 'ds50'>

Usage Example:-
	./dataGen 50 ../data/ds50/


File createIndexes.cpp
======================
To create and save indexes on the columns of the database file.This file creates a folder names 'indexes' inside which there will be 9 index folders named index1, index2 till index9(as our database file consists of 9 coulmns). Inside each index folder there will be bit vectors saved as .bin files for each value of the corresponding column. Inside 'index1' folder there will be '1.bin' & '2.bin' files and a map file. In map file you can see which .bin file represents which value. Example, Index1 folder will have 1.bin - bit map of value'Male' and 2.bin -bit map of 'Female', In Map file you can see 
1, Male
2, Female

Usage Example:-
	./createIndexes ../data/ds50/


File binTreesIndex.cpp
======================
To execute file binTreesIndex we need to pass 2 command line arguments as follows:
./bintreesIndex <datapath of the folder> <tree type number 1 ,2 or 3>
Here tree type number means:
1- Optimal tree structure  (that we didn't want)
2- Optimal tree structure (as per the paper)
3- Sub-Optimal tree structure 

Usage Example:-
       ./binTreesIndex ../data/ds50 1 
or     ./binTreesIndex ../data/ds50 2
or     ./binTreesIndex ../data/ds50 3




createAndSaveTreeIndexes.cpp:
============================
Creates and saves tree indexes on the disk. It takes as commnad line arguments dataPath where the dataFile.csv is located and treetype.

Usage Example:-
	createAndSaveTreeIndexes.cpp ../data/dataset500k/ 2 
First argument->
	< ../data/dataset500k/ > ' dataset500k' is a folder name which has the datafile.csv , you can give the path of the file in which you want to create indexes.

Second argument->
	< 1 or 2 or 3 > - Tree type. {We only need 2,3 as 2 makes optimal tree structure and 3 makes sub optimal tree structure}

It calls the function 'writeToXML' from header file binaryTrees.h to write the tree structure in xml file. After we run this file it creates index columns in the directory path we provided while executing. ex: It will create a folder indexType3 in case we pass 3 for tree type 3, indexType2 for tree type 2.
nside of these folders there will be index generated for different numeric columns like index5,index6,index7,index8 as in the createAndSaveTreeIndexes.cpp we have given 5,6,7,8 on line number 51.	


readTreeIndexes.cpp:
====================

It calls the function 'writeToXML' from header file binaryTrees.h to write the tree structure in xml file. After we run this file it creates index columns in the directory path we provided while executing. ex: index5 for index created on column 5.

Usage Example:-
	readTreeIndexes.cpp ../data/dataset500k/ 2 

First argument->        
        < ../data/dataset500k/ > ' dataset500k' is a folder name which has the datafile.csv , you can give the path of the file in which you want to create indexes.

Second argument->
        < 1 or 2 or 3 > - Tree type. {We only need 2,3 as 2 makes optimal tree structure and 3 makes sub optimal tree structure}





For checking performance we execute the following files:
=======================================================
1- bytesRead.cpp : Shows the byte counts read from the disk by the tree files

  Usage Example:-
	./bytesRead


2- spaceCount.cpp : Shows roughly the space occupied by optimal,sub-optimal & B+ trees.  



Header files:
=============
1- binaryTree.h



2- binaryTrees.h

Has functions that creates three different type of trees 
treeType 1- Some tree structure (This was not needed in the project)
treeType 2- Optimal tree structure.
treeType 3- Sub-Optimal tree structure.

In command line argument we have to pass 1,2,3 for generating the respective functions to create a tree out of treeTypes 1,2,3.


3- utils.h

Has some important functions that are being used in many other files like trim , split, getRand etc.




