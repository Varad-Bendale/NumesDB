Moving forwad for th compile select this is the main function where the whole compilation and the execution of all the queries 
and functions are pretty much called here so yeah i had kind of had 2 versions of it the first one was kind of made thinking 
there would be no join function and yeah these son of a bitch have so much of power that it liertally needed me to make a freaking
new compilation thing itself so yeah pretty lets go thorugh the structure at first see the very earlier this function is called
after the from and the join functions are completly done now at this point my join just supports the two table join only so yeah
entire of the strucutre is kind of dependent on that part and yeah so pretty much we start with the registers from both the tables
the columns are gotten as the cursor is moved before it  and the tables names and then the thing is the groupby kind of changes 
a lot of things here as well so things are parted here as well so like the liek suppose the orderby comes in the play then it 
have two very different roles liek the groupby only the main things which need to be printed only those are sorted and in rest 
everything is sorted so pretty much need to like make a segregation over there then we kind of enter in the the stuff now we 
pretty much need to make the ready the sorter if the orderby exists else just moving forward with the where clause like if the
condition exists then like just move forward else just jump forward well here the thing is i had my brain stuck for a while so
see liek the eq_op is used if the registers match it jumps to some of the steps else it dont so pretty much like right before 
the funciton is ending we would make it jump there if the condition is not fullfilled  anyways moving forward see if the operator 
is not there we kind of directly need to get the stuff so pretty much liek the registers which we got earlier which are passed
just check whicvh table they belong to and then like just get the stuff in the register now here it can be the columns of the
some table or it can be just normal values like string or integer something like that so pretty much need to like take care of 
all of it and if the operator is presen tyou pretty much need to like put it in the function which would solve everything up 
lets quickly go to the func which solves the whole expressions comes in so first we pretty much catch what the operator is and
which operator to put in the bytecode and execute them now we kind of multiple instances in here like the first one like if the
left and the right are null so pretty much if the operator is not the aggregate function or isnull is there the extra col and the
normal col is going be present so we pretty much take them operate and return the value and as the aggregate functions are kinf
of handled in another function ill come on it later on so no worry about it so yeah comming on the part two here like the one 
of them is not null and one is null so its like where the one of the thing is the just the column but the next one is the
operator and the euqation kind of gets repepated so recursion in it and if both of them are present then simply just do the
recursion easy as that okay so pretty much as soon as we get the aggregate function in the thing we just initialise it and 
then put them in the the things are put in the aggregate data structure and then liek we iterate till we dont reach the end 
now ofc there is the col name involved in it but there is a possiblity of having the aggregate function just on the equation
or something liek that 
lets hop to the main compilation file right so once we get the data if the orderby is not there we directly make the record
of the stuff else we need to get all the stuff all the equations and all the columns in the orderby so now i have designed 
it like take the stuff make the record of ti and then put them in the sorter and then right after this we put the where clasue
jump conditon if the thing is not matching just jump to this one 
