# Default to debugging flags. Set to -O3 if you want optimisations
# (either change it here, or type 'make CFLAGS=-O3' in the shell).
CFLAGS?=-g -Wall -Wno-unused-value

all: train reaction

train: train-runner.c train.c pintos_thread.h
	$(CC) $(CFLAGS) -o train train-runner.c -lpthread 

reaction: reaction-runner.c reaction.c pintos_thread.h
	$(CC) $(CFLAGS) -o reaction reaction-runner.c -lpthread 

run: train reaction
	./train
	./train
	./train
	./reaction 0
	./reaction 0
	./reaction 20
	./reaction 20
	./reaction 40
	./reaction 40
	./reaction 60
	./reaction 60
	./reaction 80
	./reaction 80
	./reaction 100
	./reaction 100

submit:
	cp -f train.c reaction.c thread_lab
	submit334 thread_lab thread_lab

clean:
	$(RM) train reaction
	$(RM) -r *.dSYM
