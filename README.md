# The EXP programming language
Exp is a high-level, interpreted, just in time compiled programming language with a stable syntax. Created in 2021 for no particular purpose.

### Syntax description
#### Possible syntax tokens
* `$` - refer to an existing variable `$var_name`
* `&` - refer to a variable with a check for its existence (if it does not exist, then create it with the value of 0) `&new_var`
* `~` - assigns a number to the variable `&num ~ 123` or moving a value from one variable to another `&take ~ $get` in this case, the first variable remains selected (`$take`)
* `+` - increases the value of the selected variable by 1
* `-` - reduces the value of the selected variable by 1
* `?` - if the value of the variable is not zero, then it changes it to zero, else to one
* `<` - if the value of the variable is less than zero, then it changes it by one, else by zero
* `>` - if the value of the variable is more than zero, then it changes it by one, else by zero
* `:` - outputs the value of a variable as a number
* `;` - outputs the value of a variable as a char
* `.` - input the value of a variable as a number
* `,` - input the value of a variable as a char
* `{` `}` - the body of the loop is allocated, which will be executed as many times as the value in the selected variable, if the value of the variable is less than zero, then the loop is infinite
* `%` - if the selected variable is equal to 1, then we exit the loop or from the execution of the program
* `"` `"` - the one in the directory at the specified path is added to the executable files `libs/nums`
* `|` `|` - comments that are not taken into account when compiling

#### Variables, assigning, output
In the programming process, all actions are performed on variables. After the variable has been accessed, all further actions will be performed on it until you refer to another one. To optimize the process, there are two prefixes denoting access to the variable. `$` refers to an already created variable, and `&` is intended to create a variable. Before applying, it checks its existence and creates it if necessary. Initially, a variable named `df` was selected.

```
&var ~ 1 |all ok|
$n ~ $var |Error. The variable n does not exist|
&n ~ $var |all ok|
$n:; |print 1☺|
```

#### Cycles, exit

```
&col.
&fiv ~ 5
&spc ~ 32  |Space symbol|
&endl ~ 10 |New line symbol|
&itr ~ 33

&col {
	&fiv { &itr:;+ &spc;}
	&endl;
}
```
Console:
```
4
33! 34" 35# 36$ 37%
38& 39' 40( 41) 42*
43+ 44, 45- 46. 47/
480 491 502 513 524
```

##### While

```
&need ~ 7

&C-{
	&input.
	&need{&input-}
	&input?%
}
&b+;
```
Console:
```
5
3
9
7
☺
```

***more examples in repository...***
