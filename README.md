**NOTE :** *This repo , is a part of the 3rd Programming assignment that we had to implement for the course of Operating Systems - Fall 2020.*


<p align="center"> 
 <h1> Concurrency Runtime Project </h1> 
</p> 

In this repository I have implemented 2 processes that are running concurrent .
The concept is that we have one Chef and 3 saladmakers to make a number of Salads that the user needs.
The 3 saladmakers are working "in parallel".

To complete a salad we need 3 ingredients : 
1. tomato
2. green pepper
3. onion

Chef has these 3 main ingredients in one cart and each Saladmaker has only one  .

Chef each time picks 2 ingredients and have to find in witch saladmaker have to pass them.
Then notifies the corresponding saladmaker and saladmaker has to recieve the ingredients and start making salad.
After chef passes the ingredients to a saladmaker,continues with picking next 2 ingredients and passing them to the corresponding saladmaker.


I have also a shared segment with the semaphores needed to implement this project and other shared values.


