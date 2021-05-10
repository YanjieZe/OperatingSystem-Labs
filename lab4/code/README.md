Completing this project will require writing the following C files:

schedule_fcfs.c
schedule_sjf.c
schedule_rr.c
schedule_priority.c
schedule_priority_rr.c

The supporting files invoke the appropriate scheduling algorithm. 

For example, to build the FCFS scheduler, enter

make fcfs

which builds the fcfs executable file.

# Work Flow
driver.c: read file and begin schedule(**schedulers.h and .c**)
CPU.c & cpu.h : **void run(Task *task, int slice)** to run the task and print

# log
- [x] FCFS
- [x] SJF
- [x] RR
- [x] Priority
- [x] Priority RR

# Test Command

./fcfs schedule.txt

./sjf schedule.txt

./rr schedule.txt

./priority schedule.txt

./priority_rr schedule.txt

./priority_rr rr-schedule.txt
