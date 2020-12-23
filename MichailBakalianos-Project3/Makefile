# PATH FOR 
INCLUDE = include
MODULES = modules
LOGS = logs

# LOG FILES
MULTI_LOG = logfile.txt
LOG_1 = $(LOGS)/saladmaker1.txt
LOG_2 = $(LOGS)/saladmaker2.txt
LOG_3 = $(LOGS)/saladmaker3.txt

# ARGUMENTS 
CHEF_ARGS = -n 16 -m 2.34
SALADMAKER_1_ARGS = -t1 3.25 -t2 6.83 -s 1700269
SALADMAKER_2_ARGS = -t1 1.00 -t2 5.00 -s 1700269
SALADMAKER_3_ARGS = -t1 6.25 -t2 8.13 -s 1700269

# compiler
CC = gcc 


# Compile options. 
CFLAGS = -Wall -g -I$(INCLUDE)
LDFLAGS = -lm -pthread

# .o files
OBJS1 = src/chef.o  $(MODULES)/chef_functions.o $(MODULES)/common_functions.o $(MODULES)/shared_segment_functions.o 
OBJS2 = src/saladmaker.o $(MODULES)/saladmaker_functions.o $(MODULES)/common_functions.o 

# Τhe executable programs
EXEC1 = chef
EXEC2 = saladmaker

$(EXEC1): $(OBJS1)
	$(CC) $(OBJS1) -o $(EXEC1) $(LDFLAGS)


$(EXEC2): $(OBJS2)
	$(CC) $(OBJS2) -o $(EXEC2) $(LDFLAGS)

all :  $(EXEC1) $(EXEC2)

# Run targets
run_chef: $(EXEC1)
	./$(EXEC1) $(CHEF_ARGS)

run_saladmaker_1: $(EXEC2)
	./$(EXEC2) $(SALADMAKER_1_ARGS)

run_saladmaker_2: $(EXEC2)
	./$(EXEC2) $(SALADMAKER_2_ARGS)

run_saladmaker_3: $(EXEC2)
	./$(EXEC2) $(SALADMAKER_3_ARGS)

# Run using valgrind debugger
valgrind_chef: $(EXEC1)
	valgrind ./$(EXEC1) $(CHEF_ARGS)

valgrind_saladmaker_1: $(EXEC2)
	valgrind ./$(EXEC2) $(SALADMAKER_1_ARGS)

valgrind_saladmaker_2: $(EXEC2)
	valgrind ./$(EXEC2) $(SALADMAKER_2_ARGS)

valgrind_saladmaker_3: $(EXEC2)
	valgrind ./$(EXEC2) $(SALADMAKER_3_ARGS)


# Clean 
clean:
	rm -f $(OBJS1) $(EXEC1) $(OBJS2) $(EXEC2) $(MULTI_LOG) $(LOG_1) $(LOG_2) $(LOG_3) 
