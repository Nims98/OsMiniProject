<div align="center">

# Os Programming Project

![C](https://img.icons8.com/color/48/null/c-programming.png)
![LINUX](https://img.icons8.com/color/48/null/linux--v1.png)
</div>

#### A Program developed for partial fulfilment of module EE 6206: Operating System Programming offered by [Department of Electrical & Information Engineering, University of Ruhuna](http://eie.eng.ruh.ac.lk/).
#

<p>

The Program is supposed to insert randomly generated marks into a file and let the user, priveiliges to Insert,Read,Update and Delete written records. The program also supposed to calculate the Min,Max, Average marks of the students and number of students below 17.5% using 4 forked processes with Shared Memory as inter process communication method.

</p>
<p>

* Assignment 1 Marks 15%
* Assignment 2 Marks 15%
* Project Marks 20%
* Final Exam Marks 50%

</p>

#
## Random Marks Generator
```
$ ./Part_A
```
## Insert
```
 $ ./Part_A -i <INDEX> <ASSIGNMENT 1 MARKS> <ASSIGNMENT 2 MARKS> <PROJECT> <FINAL EXAM>
 ```
## Read
```
 $ ./Part_A -r
 ```
## Update
```
 $ ./Part_A -u <INDEX> <ASSIGNMENT 1 MARKS> <ASSIGNMENT 2 MARKS> <PROJECT> <FINAL EXAM>
 ```
If you do not wish to update an certain mark keep the relevant argument as "-".
## Delete
```
 $ ./Part_A -d <INDEX>
 ```