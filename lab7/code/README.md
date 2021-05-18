# Contiguous Memory Allocator
2021.4.17 Yanjie Ze
# Command Order
RQ (request), RL (release), C(compact), STAT (status report), and X (exit).


# Use
> ./allocator 123456
> 
> RQ P1 100 B
> 
> RL P1
> 
> STAT
> 
> X