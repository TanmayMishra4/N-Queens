# General flags we'll always use (-Werror to prevent code generation if *any* errors)
GENFLAGS := -Wall -Wextra -pedantic -std=c99 -Wvla -Wfloat-equal -Werror -lm

# -O3 means optimise compiled code for execution speed
OPTIM := $(GENFLAGS) -O3

# Creates a much slower executable but filled with run-time error/bounds checking etc.
SANITIZE := $(GENFLAGS) -g3 -fsanitize=address -fsanitize=undefined# General flags we'll always use (-Werror to prevent code generation if *any* errors)
GENFLAGS := -Wall -Wextra -pedantic -std=c99 -Wvla -Wfloat-equal -Werror

# -O3 means optimise compiled code for execution speed
OPTIM := $(GENFLAGS) -O3

# Creates a much slower executable but filled with run-time error/bounds checking etc.
SANITIZE := $(GENFLAGS) -g3 -fsanitize=address -fsanitize=undefined

LIBS =  -lm `pkg-config sdl2 --libs`
DEVFLAGS=$(GENFLAGS) -fsanitize=address -fsanitize=undefined -g3
PRODFLAGS=$(BASEFLAGS) -O1 -Werror `pkg-config sdl2 --cflags`

# In the labs you'll probably use clang, but make sure to try gcc too
CC := clang

8q: 8q.c 8q.h
	$(CC) 8q.c $(OPTIM) -o 8q

8q_san: 8q.c 8q.h
	$(CC) 8q.c $(SANITIZE) -o 8q_san

extension: extension.c extension.h
	$(CC) extension.c neillsdl2.c -o extension $(PRODFLAGS) $(LIBS)


demo_neillsdl2: demo_neillsdl2.c neillsdl2.c neillsdl2.h
	$(CC) demo_neillsdl2.c neillsdl2.c -o demo_neillsdl2 $(PRODFLAGS) $(LIBS)

all: 8q 8q_san extension

run:
	./8q_san 4

clean:
	rm -f 8q 8q_san 8q.zip

# Crete a .zip file using ALL files in this directory
zip: 8q.c 8q.h
	zip -r 8q.zip 8q.c 8q.h # extension.txt extension.c Makefile 

# In the labs you'll probably use clang, but make sure to try gcc too
CC := clang

8q: 8q.c 8q.h
	$(CC) 8q.c $(OPTIM) -o 8q

8q_san: 8q.c 8q.h
	$(CC) 8q.c $(SANITIZE) -o 8q_san

all: 8q 8q_san

run:
	./8q_san 4

clean:
	rm -f 8q 8q_san 8q.zip

# Create a .zip file using ALL files in this directory
zip: 8q.c 8q.h extension.c extension.h extension.txt neillsdl2.c neillsdl2.h Makefile
	zip -r 8q.zip 8q.c 8q.h extension.h extension.txt extension.c Makefile neillsdl2.c neillsdl2.h
	
	