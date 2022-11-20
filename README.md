<div align="center">

# Os Programming Project

![C](https://img.icons8.com/color/48/null/c-programming.png)
![LINUX](https://img.icons8.com/color/48/null/linux--v1.png)
</div>

#### A PROGRAM DEVELOPED IN PARTIAL FULFILMENT OF MODULE EE 6206: OPERATING SYSTEM PROGRAMMING OFFERED BY [DEPARTMENT OF ELECTRICAL AND ELECTRONICS ENGINEERING, UNIVERSITY OF RUHUNA](http://eie.eng.ruh.ac.lk/).
#
#### The Program is supposed to insert randomly generated marks into a file and let the user, priveiliges to Insert,Read,Update and Delete written records. The program also supposed to calculate the Min,Max, Average marks of the students and number of students 17.5% .

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