FTMALLOC = ../libft_malloc.so

sys:
	gcc test0.c -o t0
	gcc test1.c -o t1
	gcc test2.c -o t2
	gcc test3.c -o t3
	gcc test4.c -o t4
	gcc test5.c -o t5
	gcc test6.c -o t6

my:
	gcc test0.c $(FTMALLOC) -I ../inc -o t0
	gcc test1.c $(FTMALLOC) -I ../inc -o t1
	gcc test2.c $(FTMALLOC) -I ../inc -o t2
	gcc test3.c $(FTMALLOC) -I ../inc -o t3
	gcc test4.c $(FTMALLOC) -I ../inc -o t4
	gcc test5.c $(FTMALLOC) -I ../inc -o t5
	gcc test6_2.c $(FTMALLOC) -I ../inc/ -o t6

clean:
	rm t0
	rm t1
	rm t2
	rm t3
	rm t4
	rm t5
	rm t6